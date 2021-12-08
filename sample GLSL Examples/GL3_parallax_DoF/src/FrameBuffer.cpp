#include <stdexcept>
#include <vector>
#include <GL/gl.h>
#include <GL/glext.h>

#include "FrameBuffer.h"

using std::vector;
using std::domain_error;
using std::invalid_argument;
using std::out_of_range;

#include <iostream>
using std::cout;
using std::endl;


FrameBuffer::FrameBuffer(int width, int height) : _width(0), _height(0), _frame_id(0), _render_id(0), _buffers(0)
{
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &_max_color_attachments);
	_buffers = new GLenum[_max_color_attachments];

	glGenFramebuffersEXT(1, &_frame_id);
	_width = width;
	_height = height;
}


FrameBuffer::~FrameBuffer()
{
	GLuint tex_id;

	vector<GLenum>::const_iterator cii;
	for(cii = _tex_id.begin(); cii != _tex_id.end(); cii++) {
		tex_id == *cii;
		glDeleteTextures(1, &tex_id);
	}

	glDeleteRenderbuffersEXT(1, &_render_id);
	glDeleteFramebuffersEXT(1, &_frame_id);
	delete _buffers;
}


void FrameBuffer::AttachRender(GLenum iformat) throw (std::invalid_argument)
{
	GLenum attachment;

	// XXX: only depth buffer supported
	if (iformat == GL_DEPTH_COMPONENT24) {
		attachment = GL_DEPTH_ATTACHMENT_EXT;
	}
	else {
		throw invalid_argument("FrameBuffer::AttachRender - unrecognized internal format");
	}

	glGenRenderbuffersEXT(1, &_render_id);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _frame_id);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, _render_id);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, iformat, _width, _height);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, attachment, GL_RENDERBUFFER_EXT, _render_id);

	// XXX: Only one render buffer supported
}


void FrameBuffer::AttachTexture(GLint iformat) throw(out_of_range)
{
	GLenum format;
	GLenum type;
	GLenum attachment;
	GLuint tex_id;

	if (_tex_id.size() == _max_color_attachments) {
		throw out_of_range("FrameBuffer::AttachTexture - GL_MAX_COLOR_ATTACHMENTS exceded");
	}

	// XXX: only float type supported
	if (iformat == GL_RGB16F_ARB || iformat == GL_RGB32F_ARB) {
		format = GL_RGB;
		type = GL_FLOAT;
	}
	else if (iformat == GL_RGBA16F_ARB || iformat == GL_RGBA32F_ARB) {
		format = GL_RGBA;
		type = GL_FLOAT;
	}
	attachment = GL_COLOR_ATTACHMENT0_EXT + _tex_id.size();

	glGenTextures(1, &tex_id);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _frame_id);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glTexImage2D(GL_TEXTURE_2D, 0, iformat, _width, _height, 0, format, type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // XXX: useful?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // XXX: useful?
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_2D, tex_id, 0);

	_tex_id.push_back(tex_id);
	_buffers[_tex_id.size() - 1] = attachment;
}


void FrameBuffer::BindIn() throw(domain_error)
{
	if (_tex_id.size() == 0) {
		throw domain_error("FrameBuffer::BindIn - no textures to bind");
	}	

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _frame_id);
	if (_tex_id.size() == 1) {
		glDrawBuffer(_buffers[0]);
	}
	else {
		glDrawBuffers(_tex_id.size(), _buffers);
	}
}


void FrameBuffer::BindOut()
{
	int i;

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDrawBuffer(GL_BACK);
	for (i = 0; i < _tex_id.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, _tex_id[i]);
	}
}


void FrameBuffer::BindTex(int num) throw(out_of_range)
{
	if (num + 1 > _tex_id.size()) {
		throw out_of_range("FrameBuffer::BindTex - texture vector size exceded");
	}

	glBindTexture(GL_TEXTURE_2D, _tex_id[num]);
}


void FrameBuffer::Check()
{
	GLenum status;


	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _frame_id);
	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
		cout << "FBO Status error: " << status << endl;
		throw invalid_argument("FrameBuffer::Check - status error");
	}
}


void FrameBuffer::Unbind()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDrawBuffer(GL_BACK);
}
