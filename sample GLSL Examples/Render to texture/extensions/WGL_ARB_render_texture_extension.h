//////////////////////////////////////////////////////////////////////////////////////////
//	WGL_ARB_render_texture_extension.h
//	Extension setup header
//	Downloaded from: www.paulsprojects.net
//	Created:	26th August 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef WGL_ARB_RENDER_TEXTURE_EXTENSION_H
#define WGL_ARB_RENDER_TEXTURE_EXTENSION_H

bool SetUpWGL_ARB_render_texture(const char * wglExtensions);
extern bool WGL_ARB_render_texture_supported;

extern PFNWGLBINDTEXIMAGEARBPROC			wglBindTexImageARB;
extern PFNWGLRELEASETEXIMAGEARBPROC			wglReleaseTexImageARB;
extern PFNWGLSETPBUFFERATTRIBARBPROC		wglSetPbufferAttribARB;

#endif	// WGL_ARB_RENDER_TEXTURE_EXTENSION_H