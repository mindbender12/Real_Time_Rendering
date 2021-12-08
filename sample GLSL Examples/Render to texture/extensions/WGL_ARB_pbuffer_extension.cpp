//////////////////////////////////////////////////////////////////////////////////////////
//	WGL_ARB_pbuffer_extension.cpp
//	WGL_ARB_pbuffer extension setup
//	Downloaded from: www.paulsprojects.net
//	Created:	9th September 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <windows.h>
#include <GL\gl.h>
#include <GL\glext.h>
#include <GL\wglext.h>
#include "../LOG.h"
#include "WGL_ARB_pbuffer_extension.h"

extern LOG errorLog;

bool WGL_ARB_pbuffer_supported=false;

bool SetUpWGL_ARB_pbuffer(const char * wglExtensions)
{

	//Check for support
	char * extensionString=(char *)wglExtensions;
	char * extensionName="WGL_ARB_pbuffer";

	char * endOfString;									//store pointer to end of string
	unsigned int distanceToSpace;						//distance to next space

	endOfString=extensionString+strlen(extensionString);

	//loop through string
	while(extensionString<endOfString)
	{
		//find distance to next space
		distanceToSpace=strcspn(extensionString, " ");

		//see if we have found extensionName
		if((strlen(extensionName)==distanceToSpace) &&
			(strncmp(extensionName, extensionString, distanceToSpace)==0))
		{
			WGL_ARB_pbuffer_supported=true;
		}

		//if not, move on
		extensionString+=distanceToSpace+1;
	}
	

	if(!WGL_ARB_pbuffer_supported)
	{
		errorLog.OutputError("WGL_ARB_pbuffer unsupported!");
		return false;
	}

	errorLog.OutputSuccess("WGL_ARB_pbuffer supported!");

	//get function pointers
	wglCreatePbufferARB			=	(PFNWGLCREATEPBUFFERARBPROC)
									wglGetProcAddress("wglCreatePbufferARB");
	wglGetPbufferDCARB			=	(PFNWGLGETPBUFFERDCARBPROC)
									wglGetProcAddress("wglGetPbufferDCARB");
	wglReleasePbufferDCARB		=	(PFNWGLRELEASEPBUFFERDCARBPROC)
									wglGetProcAddress("wglReleasePbufferDCARB");
	wglDestroyPbufferARB		=	(PFNWGLDESTROYPBUFFERARBPROC)
									wglGetProcAddress("wglDestroyPbufferARB");
	wglQueryPbufferARB			=	(PFNWGLQUERYPBUFFERARBPROC)
									wglGetProcAddress("wglQueryPbufferARB");
	
	return true;
}

//function pointers
PFNWGLCREATEPBUFFERARBPROC				wglCreatePbufferARB				=NULL;
PFNWGLGETPBUFFERDCARBPROC				wglGetPbufferDCARB				=NULL;
PFNWGLRELEASEPBUFFERDCARBPROC			wglReleasePbufferDCARB			=NULL;
PFNWGLDESTROYPBUFFERARBPROC				wglDestroyPbufferARB			=NULL;
PFNWGLQUERYPBUFFERARBPROC				wglQueryPbufferARB				=NULL;
