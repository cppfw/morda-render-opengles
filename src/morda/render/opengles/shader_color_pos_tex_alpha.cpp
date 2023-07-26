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

#include "shader_color_pos_tex_alpha.hpp"

#include "texture_2d.hpp"

using namespace morda::render_opengles;

shader_color_pos_tex_alpha::shader_color_pos_tex_alpha() :
	shader_base(
		R"qwertyuiop(
						attribute highp vec4 a0;
						attribute highp vec2 a1;

						uniform highp mat4 matrix;
						
						varying highp vec2 tc0;

						void main(void){
							gl_Position = matrix * a0;
							tc0 = a1;
						}
					)qwertyuiop",
		R"qwertyuiop(
						uniform sampler2D texture0;
		
						uniform highp vec4 uniform_color;
		
						varying highp vec2 tc0;
		
						void main(void){
							gl_FragColor = vec4(
                                uniform_color.x,
                                uniform_color.y,
                                uniform_color.z,
                                uniform_color.w * texture2D(texture0, tc0).x
                            );
						}
					)qwertyuiop"
	),
	color_uniform(this->get_uniform("uniform_color"))
{}

void shader_color_pos_tex_alpha::render(
	const r4::matrix4<float>& m,
	const morda::vertex_array& va,
	r4::vector4<float> color,
	const morda::texture_2d& tex
) const
{
	ASSERT(dynamic_cast<const texture_2d*>(&tex))
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
	static_cast<const texture_2d&>(tex).bind(0);
	this->bind();

	this->set_uniform4f(this->color_uniform, color.x(), color.y(), color.z(), color.w());

	this->shader_base::render(m, va);
}
