#pragma once

#include <utki/span.hpp>

#include <r4/vector.hpp>

#include <morda/render/vertex_buffer.hpp>

#include "opengl_buffer.hpp"

namespace morda{ namespace render_opengles{

class vertex_buffer : public morda::vertex_buffer, public opengl_buffer{
public:
	const GLint num_components;
	const GLenum type;
	
	vertex_buffer(utki::span<const r4::vector4<float>> vertices);
	
	vertex_buffer(utki::span<const r4::vector3<float>> vertices);
	
	vertex_buffer(utki::span<const r4::vector2<float>> vertices);
	
	vertex_buffer(utki::span<const float> vertices);
	
	vertex_buffer(const vertex_buffer&) = delete;
	vertex_buffer& operator=(const vertex_buffer&) = delete;

private:
	void init(GLsizeiptr size, const GLvoid* data);
};

}}
