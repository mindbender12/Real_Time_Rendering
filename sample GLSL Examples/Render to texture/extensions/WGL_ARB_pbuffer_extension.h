//////////////////////////////////////////////////////////////////////////////////////////
//	WGL_ARB_pbuffer_extension.h
//	Extension setup header
//	Downloaded from: www.paulsprojects.net
//	Created:	26th August 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef WGL_ARB_PBUFFER_EXTENSION_H
#define WGL_ARB_PBUFFER_EXTENSION_H

bool SetUpWGL_ARB_pbuffer(const char * wglExtensions);
extern bool WGL_ARB_pbuffer_supported;

extern PFNWGLCREATEPBUFFERARBPROC			wglCreatePbufferARB;
extern PFNWGLGETPBUFFERDCARBPROC			wglGetPbufferDCARB;
extern PFNWGLRELEASEPBUFFERDCARBPROC		wglReleasePbufferDCARB;
extern PFNWGLDESTROYPBUFFERARBPROC			wglDestroyPbufferARB;
extern PFNWGLQUERYPBUFFERARBPROC			wglQueryPbufferARB;

#endif	// WGL_ARB_PBUFFER_EXTENSION_H