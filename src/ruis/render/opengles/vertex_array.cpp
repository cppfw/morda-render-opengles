/*
ruis-render-opengles - OpenGL ES GUI renderer

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

#include "vertex_array.hpp"

#include "index_buffer.hpp"
#include "util.hpp"
#include "vertex_buffer.hpp"

using namespace ruis::render_opengles;

vertex_array::vertex_array(
	buffers_type buffers,
	const utki::shared_ref<const ruis::index_buffer>& indices,
	mode rendering_mode
) :
	ruis::vertex_array(std::move(buffers), indices, rendering_mode)
{}
