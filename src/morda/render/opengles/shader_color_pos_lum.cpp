#include "shader_color_pos_lum.hpp"

using namespace morda::render_opengles;

shader_color_pos_lum::shader_color_pos_lum() :
		shader_base(
				R"qwertyuiop(
						#ifndef GL_ES
						#	define highp
						#	define mediump
						#	define lowp
						#endif

						attribute highp vec4 a0;
						attribute highp float a1;

						uniform highp mat4 matrix;

						varying highp float lum;

						void main(void){
							gl_Position = matrix * a0;
							lum = a1;
						}
					)qwertyuiop",
				R"qwertyuiop(
						#ifndef GL_ES
						#	define highp
						#	define mediump
						#	define lowp
						#endif
		
						uniform lowp vec4 uniformColor;
		
						varying highp float lum;
		
						void main(void){
							gl_FragColor = vec4(uniformColor.x, uniformColor.y, uniformColor.z, uniformColor.w * lum);
						}
					)qwertyuiop"
			)
{
	this->colorUniform = this->get_uniform("uniformColor");
}

void shader_color_pos_lum::render(const r4::matrix4<float>& m, const morda::vertex_array& va, r4::vector4<float> color)const {
	this->bind();
	
	this->set_uniform4f(this->colorUniform, color.x(), color.y(), color.z(), color.w());
	
	this->shader_base::render(m, va);
}
