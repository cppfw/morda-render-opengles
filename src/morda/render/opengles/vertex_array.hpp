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

#include <utki/config.hpp>

#include <morda/render/vertex_array.hpp>

#if M_OS_NAME == M_OS_NAME_IOS
#	include <OpenGlES/ES2/glext.h>
#else
#	include <GLES2/gl2.h>
#endif

namespace morda{ namespace render_opengles{

class vertex_array : public morda::vertex_array{
public:	
	vertex_array(
		std::vector<utki::shared_ref<const morda::vertex_buffer>>&& buffers, // TODO: pass by value
		const utki::shared_ref<const morda::index_buffer>& indices,
		mode rendering_mode
	);
	
	vertex_array(const vertex_array&) = delete;
	vertex_array& operator=(const vertex_array&) = delete;

private:

};

}}
