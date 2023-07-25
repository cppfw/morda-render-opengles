/*
morda-render-opengles - OpenGL ES GUI renderer

Copyright (C) 2012-2023  Ivan Gagis <igagis@gmail.com>

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

#pragma once

#include <morda/render/texturing_shader.hpp>

#include "shader_base.hpp"

namespace morda {
namespace render_opengles {

class shader_pos_tex : public morda::texturing_shader, public shader_base
{
	GLint textureUniform;

public:
	shader_pos_tex();

	void render(const r4::matrix4<float>& m, const morda::vertex_array& va, const morda::texture_2d& tex)
		const override;
};

} // namespace render_opengles
} // namespace morda
