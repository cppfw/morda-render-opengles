#pragma once

#include <ruis/render/texture_depth.hpp>

#include "opengl_texture.hpp"

namespace ruis::render::opengles {

class texture_depth :
	public ruis::render::texture_depth, //
	public opengl_texture
{
public:
	texture_depth(r4::vector2<uint32_t> dims);

	texture_depth(const texture_depth&) = delete;
	texture_depth& operator=(const texture_depth&) = delete;

	texture_depth(texture_depth&&) = delete;
	texture_depth& operator=(texture_depth&&) = delete;

	~texture_depth() override = default;
};

} // namespace ruis::render::opengles
