#pragma once

#include <morda/render/render_factory.hpp>

namespace morda{ namespace render_opengles{

class render_factory : public morda::render_factory{
public:
	render_factory();
	
	render_factory(const render_factory&) = delete;
	render_factory& operator=(const render_factory&) = delete;
	
	virtual ~render_factory()noexcept;

	std::shared_ptr<morda::texture_2d> create_texture_2d(morda::texture_2d::type type, r4::vector2<unsigned> dims, utki::span<const uint8_t> data)override;

	std::shared_ptr<morda::vertex_buffer> create_vertex_buffer(utki::span<const r4::vector4<float>> vertices)override;
	std::shared_ptr<morda::vertex_buffer> create_vertex_buffer(utki::span<const r4::vector3<float>> vertices)override;
	std::shared_ptr<morda::vertex_buffer> create_vertex_buffer(utki::span<const r4::vector2<float>> vertices)override;
	std::shared_ptr<morda::vertex_buffer> create_vertex_buffer(utki::span<const float> vertices)override;

	std::shared_ptr<morda::index_buffer> create_index_buffer(utki::span<const uint16_t> indices)override;
	
	std::shared_ptr<morda::vertex_array> create_vertex_array(
			std::vector<std::shared_ptr<morda::vertex_buffer>>&& buffers,
			std::shared_ptr<morda::index_buffer> indices,
			morda::vertex_array::mode rendering_mode
		)override;

	std::unique_ptr<shaders> create_shaders()override;
	
	std::shared_ptr<morda::frame_buffer> create_framebuffer(std::shared_ptr<morda::texture_2d> color)override;
};

}}
