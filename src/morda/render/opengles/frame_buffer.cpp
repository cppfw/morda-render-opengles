#include <utki/config.hpp>

#include "frame_buffer.hpp"
#include "texture_2d.hpp"

#include "util.hpp"

#if M_OS_NAME == M_OS_NAME_IOS
#	include <OpenGlES/ES2/glext.h>
#else
#	include <GLES2/gl2.h>
#endif

using namespace morda::render_opengles;

frame_buffer::frame_buffer(std::shared_ptr<morda::texture_2d> color) :
		morda::frame_buffer(std::move(color))
{
	glGenFramebuffers(1, &this->fbo);
	assert_opengl_no_error();
	
	GLint oldFb;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFb);
	
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	assert_opengl_no_error();
	
	ASSERT(dynamic_cast<texture_2d*>(this->color.operator->()))
	auto& tex = static_cast<texture_2d&>(*this->color);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.tex, 0);
	assert_opengl_no_error();
	
	//Check for completeness
#ifdef DEBUG
	{
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		assert_opengl_no_error();
		ASSERT(status == GL_FRAMEBUFFER_COMPLETE)
	}
#endif
	
	glBindFramebuffer(GL_FRAMEBUFFER, oldFb);
	assert_opengl_no_error();
}


frame_buffer::~frame_buffer()noexcept{
	glDeleteFramebuffers(1, &this->fbo);
	assert_opengl_no_error();
}
