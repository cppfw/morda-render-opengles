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

#include <utki/config.hpp>

#include "renderer.hpp"
#include "frame_buffer.hpp"
#include "util.hpp"

#if M_OS_NAME == M_OS_NAME_IOS
#	include <OpenGlES/ES2/glext.h>
#else
#	include <GLES2/gl2.h>
#endif

using namespace morda::render_opengles;

namespace{
unsigned getMaxTextureSize(){
	GLint val;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &val);
	assert_opengl_no_error();
	ASSERT(val > 0, [&](auto&o){o << "val = " << val;})
	return unsigned(val);
}
}

renderer::renderer(std::unique_ptr<render_factory> factory) :
		morda::renderer(
				std::move(factory),
				[](){
					renderer::params p;
					p.max_texture_size = getMaxTextureSize();
					return p;
				}()
			)
{}

void renderer::set_framebuffer_internal(morda::frame_buffer* fb){
	if(!this->defaultFramebufferInitialized){
		// On some platforms the default framebuffer is not 0, so because of this
		// check if default framebuffer value is saved or not every time some
		// framebuffer is going to be bound and save the value if needed.
		GLint old_fb;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fb);
		LOG([&](auto&o){o << "old_fb = " << old_fb << std::endl;})
		this->defaultFramebuffer = decltype(this->defaultFramebuffer)(old_fb);
		this->defaultFramebufferInitialized = true;
	}
	
	if(!fb){
		glBindFramebuffer(GL_FRAMEBUFFER, GLuint(this->defaultFramebuffer));
		assert_opengl_no_error();
		return;
	}
	
	ASSERT(dynamic_cast<frame_buffer*>(fb))
	auto& ogl2fb = static_cast<frame_buffer&>(*fb);
	
	glBindFramebuffer(GL_FRAMEBUFFER, ogl2fb.fbo);
	assert_opengl_no_error();
}

void renderer::clear_framebuffer(){
	glClearColor(0, 0, 0, 1);
	assert_opengl_no_error();
	glClear(GL_COLOR_BUFFER_BIT);
	assert_opengl_no_error();
	
	glClearDepthf(0);

	glClear(GL_DEPTH_BUFFER_BIT);
	assert_opengl_no_error();
	
	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);
	assert_opengl_no_error();
}

bool renderer::is_scissor_enabled()const{
	return glIsEnabled(GL_SCISSOR_TEST) ? true : false; // "? true : false" is to avoid warning under MSVC
}

void renderer::set_scissor_enabled(bool enabled){
	if(enabled){
		glEnable(GL_SCISSOR_TEST);
	}else{
		glDisable(GL_SCISSOR_TEST);
	}
}

r4::rectangle<int> renderer::get_scissor()const{
	GLint osb[4];
	glGetIntegerv(GL_SCISSOR_BOX, osb);
	return r4::rectangle<int>(osb[0], osb[1], osb[2], osb[3]);
}

void renderer::set_scissor(r4::rectangle<int> r){
	glScissor(r.p.x(), r.p.y(), r.d.x(), r.d.y());
	assert_opengl_no_error();
}

r4::rectangle<int> renderer::get_viewport()const{
	GLint vp[4];

	glGetIntegerv(GL_VIEWPORT, vp);
	
	return r4::rectangle<int>(vp[0], vp[1], vp[2], vp[3]);
}

void renderer::set_viewport(r4::rectangle<int> r){
	glViewport(r.p.x(), r.p.y(), r.d.x(), r.d.y());
	assert_opengl_no_error();
}

void renderer::set_blend_enabled(bool enable){
	if(enable){
		glEnable(GL_BLEND);
	}else{
		glDisable(GL_BLEND);
	}
}

namespace{

GLenum blendFunc[] = {
	GL_ZERO,
	GL_ONE,
	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_DST_COLOR,
	GL_ONE_MINUS_DST_COLOR,
	GL_SRC_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA,
	GL_CONSTANT_COLOR,
	GL_ONE_MINUS_CONSTANT_COLOR,
	GL_CONSTANT_ALPHA,
	GL_ONE_MINUS_CONSTANT_ALPHA,
	GL_SRC_ALPHA_SATURATE
};

}

void renderer::set_blend_func(blend_factor src_color, blend_factor dst_color, blend_factor src_alpha, blend_factor dst_alpha){
	glBlendFuncSeparate(
			blendFunc[unsigned(src_color)],
			blendFunc[unsigned(dst_color)],
			blendFunc[unsigned(src_alpha)],
			blendFunc[unsigned(dst_alpha)]
		);
}
