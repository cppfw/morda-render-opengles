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

#include <morda/render/frame_buffer.hpp>
#include <morda/render/texture_2d.hpp>
#include <utki/config.hpp>

#if M_OS_NAME == M_OS_NAME_IOS
#	include <OpenGlES/ES2/glext.h>
#else
#	include <GLES2/gl2.h>
#endif

namespace morda::render_opengles {

class frame_buffer : public morda::frame_buffer
{
public:
	GLuint fbo;

	frame_buffer(const utki::shared_ref<morda::texture_2d>& color);

	frame_buffer(const frame_buffer&) = delete;
	frame_buffer& operator=(const frame_buffer&) = delete;

	frame_buffer(frame_buffer&&) = delete;
	frame_buffer& operator=(frame_buffer&&) = delete;

	~frame_buffer()override;

private:
};

} // namespace morda
