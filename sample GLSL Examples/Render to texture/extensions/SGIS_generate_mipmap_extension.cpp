//////////////////////////////////////////////////////////////////////////////////////////
//	SGIS_generate_mipmap_extension.cpp
//	SGIS_generate_mipmap extension setup
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
#include "SGIS_generate_mipmap_extension.h"

extern LOG errorLog;

bool SGIS_generate_mipmap_supported=false;

bool SetUpSGIS_generate_mipmap()
{
	//Check for support
	char * extensionString=(char *)glGetString(GL_EXTENSIONS);
	char * extensionName="GL_SGIS_generate_mipmap";

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
			SGIS_generate_mipmap_supported=true;
		}

		//if not, move on
		extensionString+=distanceToSpace+1;
	}

	if(!SGIS_generate_mipmap_supported)
	{
		errorLog.OutputError("SGIS_generate_mipmap unsupported!");
		return false;
	}

	errorLog.OutputSuccess("SGIS_generate_mipmap supported!");

	//get function pointers
	//none specified

	return true;
}
