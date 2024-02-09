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

#pragma once

#include <utki/config.hpp>
#include <utki/debug.hpp>

#if CFG_OS_NAME == CFG_OS_NAME_IOS
#	include <OpenGlES/ES2/glext.h>
#else
#	include <GLES2/gl2.h>
#endif

namespace ruis::render_opengles {

inline void assert_opengl_no_error()
{
#ifdef DEBUG
	GLenum error = glGetError();
	switch (error) {
		case GL_NO_ERROR:
			return;
		case GL_INVALID_ENUM:
			ASSERT(false, [](auto& o) {
				o << "OpenGL error: GL_INVALID_ENUM";
			})
			break;
		case GL_INVALID_VALUE:
			ASSERT(false, [](auto& o) {
				o << "OpenGL error: GL_INVALID_VALUE";
			})
			break;
		case GL_INVALID_OPERATION:
			ASSERT(false, [](auto& o) {
				o << "OpenGL error: GL_INVALID_OPERATION";
			})
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			ASSERT(false, [](auto& o) {
				o << "OpenGL error: GL_INVALID_FRAMEBUFFER_OPERATION";
			})
			break;
		case GL_OUT_OF_MEMORY:
			// TODO: throw exception
			ASSERT(false, [](auto& o) {
				o << "OpenGL error: GL_OUT_OF_MEMORY";
			})
			break;
		default:
			ASSERT(false, [&](auto& o) {
				o << "Unknown OpenGL error, code = " << int(error);
			})
			break;
	}
#endif
}

} // namespace ruis::render_opengles
