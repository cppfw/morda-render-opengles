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

#pragma once

#include <utki/config.hpp>
#include <utki/debug.hpp>
#include <utki/span.hpp>

#include <r4/matrix.hpp>

#include <vector>

#include <morda/render/vertex_array.hpp>

#if M_OS_NAME == M_OS_NAME_IOS
#	include <OpenGlES/ES2/glext.h>
#else
#	include <GLES2/gl2.h>
#endif

#include "util.hpp"

namespace morda{ namespace render_opengles{

struct shader_wrapper{
	GLuint s;
	shader_wrapper(const char* code, GLenum type);
	~shader_wrapper()noexcept{
		glDeleteShader(this->s);
	}
};

struct program_wrapper{
	shader_wrapper vertexShader;
	shader_wrapper fragmentShader;
	GLuint p;
	program_wrapper(const char* vertexShaderCode, const char* fragmentShaderCode);

	virtual ~program_wrapper()noexcept{
		glDeleteProgram(this->p);
	}
};

class shader_base{
	program_wrapper program;
	
	const GLint matrixUniform;
	
	static const shader_base* boundShader;
public:
	shader_base(const char* vertexShaderCode, const char* fragmentShaderCode);
	
	shader_base(const shader_base&) = delete;
	shader_base& operator=(const shader_base&) = delete;
	
	virtual ~shader_base()noexcept{}

protected:
	GLint get_uniform(const char* n);
	
	void bind()const{
		glUseProgram(program.p);
		assert_opengl_no_error();
		boundShader = this;
	}
	
	bool is_bound()const noexcept{
		return this == boundShader;
	}
	
	void set_uniform_matrix4f(GLint id, const r4::matrix4<float>& m)const{
		auto mm = m.tpos();
		glUniformMatrix4fv(
				id,
				1,
				// OpenGL ES2 doies not support transposing, see description of 'transpose' parameter
				// of glUniformMatrix4fv(): https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glUniform.xml
				GL_FALSE,
				reinterpret_cast<const GLfloat*>(&mm)
			);
		assert_opengl_no_error();
	}
	
	void set_uniform4f(GLint id, float x, float y, float z, float a)const{
		glUniform4f(id, x, y, z, a);
		assert_opengl_no_error();
	}
	
	void set_matrix(const r4::matrix4<float>& m)const{
		this->set_uniform_matrix4f(this->matrixUniform, m);
		assert_opengl_no_error();
	}
	
	static GLenum mode_map[];
	
	static GLenum mode_to_gl_mode(morda::vertex_array::mode mode){
		return mode_map[unsigned(mode)];
	}
	
	void render(const r4::matrix4<float>& m, const morda::vertex_array& va)const;
};

}}
