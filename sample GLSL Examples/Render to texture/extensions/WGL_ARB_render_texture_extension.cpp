//////////////////////////////////////////////////////////////////////////////////////////
//	WGL_ARB_render_texture_extension.cpp
//	WGL_ARB_render_texture extension setup
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
#include "WGL_ARB_render_texture_extension.h"

extern LOG errorLog;

bool WGL_ARB_render_texture_supported=false;

bool SetUpWGL_ARB_render_texture(const char * wglExtensions)
{

	//Check for support
	char * extensionString=(char *)wglExtensions;
	char * extensionName="WGL_ARB_render_texture";

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
			WGL_ARB_render_texture_supported=true;
		}

		//if not, move on
		extensionString+=distanceToSpace+1;
	}
	

	if(!WGL_ARB_render_texture_supported)
	{
		errorLog.OutputError("WGL_ARB_render_texture unsupported!");
		return false;
	}

	errorLog.OutputSuccess("WGL_ARB_render_texture supported!");

	//get function pointers
	wglBindTexImageARB			=	(PFNWGLBINDTEXIMAGEARBPROC)
									wglGetProcAddress("wglBindTexImageARB");
	wglReleaseTexImageARB		=	(PFNWGLRELEASETEXIMAGEARBPROC)
									wglGetProcAddress("wglReleaseTexImageARB");
	wglSetPbufferAttribARB		=	(PFNWGLSETPBUFFERATTRIBARBPROC)
									wglGetProcAddress("wglSetPbufferAttribARB");
	return true;
}

//function pointers
PFNWGLBINDTEXIMAGEARBPROC				wglBindTexImageARB				=NULL;
PFNWGLRELEASETEXIMAGEARBPROC			wglReleaseTexImageARB			=NULL;
PFNWGLSETPBUFFERATTRIBARBPROC			wglSetPbufferAttribARB			=NULL;
	
