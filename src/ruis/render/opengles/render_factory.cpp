/*
ruis-render-opengles - OpenGL ES GUI renderer

Copyright (C) 2012-2024  Ivan Gagis <igagis@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/* ================ LICENSE END ================ */

#include "render_factory.hpp"

#include <utki/config.hpp>

#include "shaders/shader_color.hpp"
#include "shaders/shader_color_pos_lum.hpp"
#include "shaders/shader_color_pos_tex.hpp"
#include "shaders/shader_color_pos_tex_alpha.hpp"
#include "shaders/shader_pos_clr.hpp"
#include "shaders/shader_pos_tex.hpp"

#include "frame_buffer.hpp"
#include "index_buffer.hpp"
#include "texture_2d.hpp"
#include "util.hpp"
#include "vertex_array.hpp"
#include "vertex_buffer.hpp"

#if CFG_OS_NAME == CFG_OS_NAME_IOS
#	include <OpenGlES/ES2/glext.h>
#else
#	include <GLES2/gl2.h>
#endif

using namespace ruis::render::opengles;

utki::shared_ref<ruis::render::texture_2d> render_factory::create_texture_2d(
	rasterimage::format format,
	rasterimage::dimensioned::dimensions_type dims,
	texture_2d_parameters params
)
{
	return this->create_texture_2d_internal(format, dims, nullptr, std::move(params));
}

utki::shared_ref<ruis::render::texture_2d> render_factory::create_texture_2d(
	const rasterimage::image_variant& imvar,
	texture_2d_parameters params
)
{
	auto imvar_copy = imvar;
	return this->create_texture_2d(std::move(imvar_copy), std::move(params));
}

utki::shared_ref<ruis::render::texture_2d> render_factory::create_texture_2d(
	rasterimage::image_variant&& imvar,
	texture_2d_parameters params
)
{
	return std::visit(
		[this, &imvar, &params](auto&& im) -> utki::shared_ref<ruis::render::texture_2d> {
			if constexpr (sizeof(im.pixels().front().front()) != 1) {
				throw std::logic_error(
					"render_factory::create_texture_2d(): "
					"non-8bit images are not supported"
				);
			} else {
				im.span().flip_vertical();
				auto data = im.pixels();
				return this->create_texture_2d_internal(
					imvar.get_format(),
					im.dims(),
					utki::make_span(data.front().data(), data.size_bytes()),
					std::move(params)
				);
			}
		},
		imvar.variant
	);
}

utki::shared_ref<ruis::render::texture_2d> render_factory::create_texture_2d_internal(
	rasterimage::format type,
	rasterimage::dimensioned::dimensions_type dims,
	utki::span<const uint8_t> data,
	texture_2d_parameters params
)
{
	ASSERT(data.size() % rasterimage::to_num_channels(type) == 0)
	ASSERT(data.size() % dims.x() == 0)
	ASSERT(data.size() == 0 || data.size() / rasterimage::to_num_channels(type) / dims.x() == dims.y())

	auto ret = utki::make_shared<texture_2d>(dims.to<float>());

	// TODO: save previous bind and restore it after?
	ret.get().bind(0);

	GLint internal_format = [&type]() {
		switch (type) {
			default:
				ASSERT(false)
			case rasterimage::format::grey:
				return GL_LUMINANCE;
			case rasterimage::format::greya:
				return GL_LUMINANCE_ALPHA;
			case rasterimage::format::rgb:
				return GL_RGB;
			case rasterimage::format::rgba:
				return GL_RGBA;
		}
	}();

	// we will be passing pixels to OpenGL which are 1-byte aligned
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	assert_opengl_no_error();

	glTexImage2D(
		GL_TEXTURE_2D,
		0, // 0th level, no mipmaps
		internal_format, // internal format
		GLsizei(dims.x()),
		GLsizei(dims.y()),
		0, // border, should be 0!
		internal_format, // format of the texel data
		GL_UNSIGNED_BYTE,
		data.size() == 0 ? nullptr : data.data()
	);
	assert_opengl_no_error();

	if (!data.empty() && params.mipmap != texture_2d::mipmap::none) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	auto to_gl_filter = [](texture_2d::filter f) {
		switch (f) {
			case texture_2d::filter::nearest:
				return GL_NEAREST;
			case texture_2d::filter::linear:
				return GL_LINEAR;
		}
		return GL_NEAREST;
	};

	GLint mag_filter = to_gl_filter(params.mag_filter);

	GLint min_filter = [&]() {
		switch (params.mipmap) {
			case texture_2d::mipmap::none:
				return to_gl_filter(params.min_filter);
			case texture_2d::mipmap::nearest:
				switch (params.min_filter) {
					case texture_2d::filter::nearest:
						return GL_NEAREST_MIPMAP_NEAREST;
					case texture_2d::filter::linear:
						return GL_LINEAR_MIPMAP_NEAREST;
				}
				break;
			case texture_2d::mipmap::linear:
				switch (params.min_filter) {
					case texture_2d::filter::nearest:
						return GL_NEAREST_MIPMAP_LINEAR;
					case texture_2d::filter::linear:
						return GL_LINEAR_MIPMAP_LINEAR;
				}
				break;
		}
		return GL_NEAREST;
	}();

	// It is necessary to set filter parameters for every texture. Otherwise it may not work.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	assert_opengl_no_error();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	assert_opengl_no_error();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	assert_opengl_no_error();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	assert_opengl_no_error();

	return ret;
}

utki::shared_ref<ruis::render::vertex_buffer> render_factory::create_vertex_buffer(
	utki::span<const r4::vector4<float>> vertices
)
{
	return utki::make_shared<vertex_buffer>(vertices);
}

utki::shared_ref<ruis::render::vertex_buffer> render_factory::create_vertex_buffer(
	utki::span<const r4::vector3<float>> vertices
)
{
	return utki::make_shared<vertex_buffer>(vertices);
}

utki::shared_ref<ruis::render::vertex_buffer> render_factory::create_vertex_buffer(
	utki::span<const r4::vector2<float>> vertices
)
{
	return utki::make_shared<vertex_buffer>(vertices);
}

utki::shared_ref<ruis::render::vertex_buffer> render_factory::create_vertex_buffer(utki::span<const float> vertices)
{
	return utki::make_shared<vertex_buffer>(vertices);
}

utki::shared_ref<ruis::render::vertex_array> render_factory::create_vertex_array(
	std::vector<utki::shared_ref<const ruis::render::vertex_buffer>> buffers,
	const utki::shared_ref<const ruis::render::index_buffer>& indices,
	ruis::render::vertex_array::mode rendering_mode
)
{
	return utki::make_shared<vertex_array>(std::move(buffers), indices, rendering_mode);
}

utki::shared_ref<ruis::render::index_buffer> render_factory::create_index_buffer(utki::span<const uint16_t> indices)
{
	return utki::make_shared<index_buffer>(indices);
}

std::unique_ptr<ruis::render::render_factory::shaders> render_factory::create_shaders()
{
	auto ret = std::make_unique<ruis::render::render_factory::shaders>();
	// NOLINTNEXTLINE(bugprone-unused-return-value, "false positive")
	ret->pos_tex = std::make_unique<shader_pos_tex>();
	// NOLINTNEXTLINE(bugprone-unused-return-value, "false positive")
	ret->color_pos = std::make_unique<shader_color>();
	// NOLINTNEXTLINE(bugprone-unused-return-value, "false positive")
	ret->pos_clr = std::make_unique<shader_pos_clr>();
	// NOLINTNEXTLINE(bugprone-unused-return-value, "false positive")
	ret->color_pos_tex = std::make_unique<shader_color_pos_tex>();
	// NOLINTNEXTLINE(bugprone-unused-return-value, "false positive")
	ret->color_pos_tex_alpha = std::make_unique<shader_color_pos_tex_alpha>();
	// NOLINTNEXTLINE(bugprone-unused-return-value, "false positive")
	ret->color_pos_lum = std::make_unique<shader_color_pos_lum>();
	return ret;
}

utki::shared_ref<ruis::render::frame_buffer> render_factory::create_framebuffer(
	const utki::shared_ref<ruis::render::texture_2d>& color
)
{
	return utki::make_shared<frame_buffer>(color);
}
