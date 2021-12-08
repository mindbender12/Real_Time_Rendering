//////////////////////////////////////////////////////////////////////////////////////////
//	EXT_texture_filter_anisotropic_extension.cpp
//	EXT_texture_filter_anisotropic extension setup
//	Downloaded from: www.paulsprojects.net
//	Created:	10th September 2002
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
#include "EXT_texture_filter_anisotropic_extension.h"

extern LOG errorLog;

bool EXT_texture_filter_anisotropic_supported=false;

bool SetUpEXT_texture_filter_anisotropic()
{
	//Check for support
	char * extensionString=(char *)glGetString(GL_EXTENSIONS);
	char * extensionName="GL_EXT_texture_filter_anisotropic";

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
			EXT_texture_filter_anisotropic_supported=true;
		}

		//if not, move on
		extensionString+=distanceToSpace+1;
	}

	if(!EXT_texture_filter_anisotropic_supported)
	{
		errorLog.OutputError("EXT_texture_filter_anisotropic unsupported!");
		return false;
	}

	errorLog.OutputSuccess("EXT_texture_filter_anisotropic supported!");

	//get function pointers
	//none specified

	return true;
}
