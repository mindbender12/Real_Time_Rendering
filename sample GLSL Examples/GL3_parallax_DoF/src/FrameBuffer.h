#ifndef CLASS_FRAME_BUFFER
#define CLASS_FRAME_BUFFER

#include <stdexcept>
#include <vector>
#include <GL/gl.h>
#include <GL/glext.h>

/// Frame Buffer Object class
class FrameBuffer
{
	protected:
		int _width;
		int _height;
		GLuint _frame_id;
		GLuint _render_id;
		std::vector<GLuint> _tex_id;
		GLenum *_buffers; // glDrawBuffers() needs a C array

		int _max_color_attachments;
	public:
		/// Default Constructor
		FrameBuffer(int width, int height);
		/// Destructor
		~FrameBuffer();
		/// Attach a render target to the FBO
		void AttachRender(GLenum iformat) throw(std::invalid_argument);
		/// Attach a texture to the FBO
		void AttachTexture(GLint iformat) throw(std::out_of_range);
		/// Bind the FBO for reading
		void BindIn() throw(std::domain_error);
		/// Bind the FBO for writing
		void BindOut();
		/// Bind the specified textures to the context
		void BindTex(int num = 0) throw(std::out_of_range);
		/// Check OpenGL status of the FBO
		void Check();

		/// Disable rendering to FBO
		static void Unbind();
};

#endif
