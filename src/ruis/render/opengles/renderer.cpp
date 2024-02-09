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

#include "renderer.hpp"

#include <utki/config.hpp>

#include "frame_buffer.hpp"
#include "util.hpp"

#if CFG_OS_NAME == CFG_OS_NAME_IOS
#	include <OpenGlES/ES2/glext.h>
#else
#	include <GLES2/gl2.h>
#endif

using namespace ruis::render_opengles;

renderer::renderer(std::unique_ptr<render_factory> factory) :
	ruis::renderer(std::move(factory), []() {
		renderer::params p;
		p.max_texture_size = []() {
			// the variable is initialized via output argument, so no need to initialize it here
			// NOLINTNEXTLINE(cppcoreguidelines-init-variables)
			GLint val;
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &val);
			assert_opengl_no_error();
			ASSERT(val > 0, [&](auto& o) {
				o << "val = " << val;
			})
			return unsigned(val);
		}();
		return p;
	}())
{
	glEnable(GL_CULL_FACE);
}

void renderer::set_framebuffer_internal(ruis::frame_buffer* fb)
{
	if (!this->default_framebuffer_initialized) {
		// On some platforms the default framebuffer is not 0, so because of this
		// check if default framebuffer value is saved or not every time some
		// framebuffer is going to be bound and save the value if needed.

		// the variable is initialized via output argument, so no need to initialize it here
		// NOLINTNEXTLINE(cppcoreguidelines-init-variables)
		GLint old_fb;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fb);
		LOG([&](auto& o) {
			o << "old_fb = " << old_fb << std::endl;
		})
		this->default_framebuffer = decltype(this->default_framebuffer)(old_fb);
		this->default_framebuffer_initialized = true;
	}

	if (!fb) {
		glBindFramebuffer(GL_FRAMEBUFFER, GLuint(this->default_framebuffer));
		assert_opengl_no_error();
		return;
	}

	ASSERT(dynamic_cast<frame_buffer*>(fb))
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
	auto& ogl2fb = static_cast<frame_buffer&>(*fb);

	glBindFramebuffer(GL_FRAMEBUFFER, ogl2fb.fbo);
	assert_opengl_no_error();
}

void renderer::clear_framebuffer()
{
	glClearColor(0, 0, 0, 1);
	assert_opengl_no_error();
	glClear(GL_COLOR_BUFFER_BIT);
	assert_opengl_no_error();

	glClearDepthf(0);

	glClear(GL_DEPTH_BUFFER_BIT);
	assert_opengl_no_error();

	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);
	assert_opengl_no_error();
}

bool renderer::is_scissor_enabled() const
{
	return glIsEnabled(GL_SCISSOR_TEST) ? true : false; // "? true : false" is to avoid warning under MSVC
}

void renderer::set_scissor_enabled(bool enabled)
{
	if (enabled) {
		glEnable(GL_SCISSOR_TEST);
	} else {
		glDisable(GL_SCISSOR_TEST);
	}
}

r4::rectangle<int> renderer::get_scissor() const
{
	// the variable is initialized via output argument, so no need to initialize it here
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
	std::array<GLint, 4> osb;
	glGetIntegerv(GL_SCISSOR_BOX, osb.data());
	return {osb[0], osb[1], osb[2], osb[3]};
}

void renderer::set_scissor(r4::rectangle<int> r)
{
	glScissor(r.p.x(), r.p.y(), r.d.x(), r.d.y());
	assert_opengl_no_error();
}

r4::rectangle<int> renderer::get_viewport() const
{
	// the variable is initialized via output argument, so no need to initialize it here
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
	std::array<GLint, 4> vp;

	glGetIntegerv(GL_VIEWPORT, vp.data());

	return {vp[0], vp[1], vp[2], vp[3]};
}

void renderer::set_viewport(r4::rectangle<int> r)
{
	glViewport(r.p.x(), r.p.y(), r.d.x(), r.d.y());
	assert_opengl_no_error();
}

void renderer::set_blend_enabled(bool enable)
{
	if (enable) {
		glEnable(GL_BLEND);
	} else {
		glDisable(GL_BLEND);
	}
}

namespace {

const std::array<GLenum, size_t(ruis::renderer::blend_factor::enum_size)> blend_func = {
	GL_ZERO,
	GL_ONE,
	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_DST_COLOR,
	GL_ONE_MINUS_DST_COLOR,
	GL_SRC_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA,
	GL_CONSTANT_COLOR,
	GL_ONE_MINUS_CONSTANT_COLOR,
	GL_CONSTANT_ALPHA,
	GL_ONE_MINUS_CONSTANT_ALPHA,
	GL_SRC_ALPHA_SATURATE
};

} // namespace

void renderer::set_blend_func(
	blend_factor src_color,
	blend_factor dst_color,
	blend_factor src_alpha,
	blend_factor dst_alpha
)
{
	glBlendFuncSeparate(
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
		blend_func[unsigned(src_color)],
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
		blend_func[unsigned(dst_color)],
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
		blend_func[unsigned(src_alpha)],
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
		blend_func[unsigned(dst_alpha)]
	);
}
