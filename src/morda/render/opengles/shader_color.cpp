/*
morda-render-opengles - OpenGL ES GUI renderer

Copyright (C) 2012-2021  Ivan Gagis <igagis@gmail.com>

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

#include "shader_color.hpp"

using namespace morda::render_opengles;

shader_color::shader_color() :
		shader_base(
				R"qwertyuiop(
						#ifndef GL_ES
						#	define highp
						#	define mediump
						#	define lowp
						#endif

						attribute highp vec4 a0;

						uniform highp mat4 matrix;
						
						void main(void){
							gl_Position = matrix * a0;
						}
					)qwertyuiop",
				R"qwertyuiop(
						#ifndef GL_ES
						#	define highp
						#	define mediump
						#	define lowp
						#endif
		
						uniform lowp vec4 uniform_color;
		
						void main(void){
							gl_FragColor = uniform_color;
						}
				)qwertyuiop"
			)
{
	this->colorUniform = this->get_uniform("uniform_color");
}

void shader_color::render(const r4::matrix4<float>& m, const morda::vertex_array& va, r4::vector4<float> color)const{
	this->bind();
	
	this->set_uniform4f(this->colorUniform, color.x(), color.y(), color.z(), color.w());
	
	this->shader_base::render(m, va);
}
