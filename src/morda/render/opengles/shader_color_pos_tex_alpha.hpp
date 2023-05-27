#pragma once

#include <morda/render/coloring_texturing_shader.hpp>

#include "shader_base.hpp"

namespace morda{ namespace render_opengles{

class shader_color_pos_tex_alpha :
		public morda::coloring_texturing_shader,
		public shader_base
{
	GLint colorUniform;
public:
	shader_color_pos_tex_alpha();
	
	shader_color_pos_tex_alpha(const shader_color_pos_tex_alpha&) = delete;
	shader_color_pos_tex_alpha& operator=(const shader_color_pos_tex_alpha&) = delete;
	
	void render(const r4::matrix4<float>& m, const morda::vertex_array& va, r4::vector4<float> color, const morda::texture_2d& tex)const override;
};

}}
