/* GL3_parallax_DoF: Parallax mapping and DoF effect with a GL3 compliant class library
   Copyright (C) 2008 Angelo "Encelo" Theodorou

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#include <iostream>
#include <cmath>
#include <vector>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include "ViewMatrix.h"
#include "ProjMatrix.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "FrameTimer.h"
#include "VertexBuffer.h"
#include "Mesh.h"
#include "Light.h"
#include "Texture.h"
#include "FrameBuffer.h"

using std::cout;
using std::endl;
using std::vector;

int main(int argc, char **argv)
{
	SDL_Surface *screen;
	SDL_Event event;
	const SDL_VideoInfo* info = NULL;
	int width = 640;
	int height = 480;
	int bpp = 0;
	int flags = 0;

	bool quit = false;
	bool move = false, lmove = false, rotate = true;
	GLfloat movz = 0.0, lmov = 0.0, roty = 0.0;
	const float default_distance = 14.0f;
	const float default_range = 3.0f;
	float focalDistance = default_distance;
	float focalRange = default_range;

	ViewMatrix mv(4);
	Matrix n(5); // Normal matrix
	ProjMatrix p(2);
	FrameTimer t(5);
	Light light;

// ----- SDL init ---------------
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
		exit(-1);
	}

	atexit(SDL_Quit);
	
	info = SDL_GetVideoInfo();
	bpp = info->vfmt->BitsPerPixel;

	
// ----- OpenGL attribute setting via SDL ---------------
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	flags = SDL_OPENGL;

	
// ----- Setting up the screen surface ---------------
	if ((screen = SDL_SetVideoMode(width, height, bpp, flags)) == 0) {
		fprintf(stderr, "Video mode set failed: %s\n", SDL_GetError());
        	exit(-1);
	}

	SDL_WM_SetCaption("GL3_test by Encelo", NULL);


// ----- OpenGL init ---------------
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	light.CAttenuation(0.5);
	light.LAttenuation(0.02);
	light.QAttenuation(0.0);


	ShaderProgram first("shaders/first.vert", "shaders/first.frag");
	ShaderProgram second("shaders/second.vert", "shaders/second.frag");
	ShaderProgram third("shaders/third.vert", "shaders/third.frag");
	ShaderProgram fourth("shaders/fourth.vert", "shaders/fourth.frag");
	ShaderProgram fifth("shaders/second.vert", "shaders/fifth.frag");
	

	first.Uniform("Tex0", 0);
	first.Uniform("Tex1", 1);
	first.Uniform4v("LightSource.ambient", light.Ambient());
	first.Uniform4v("LightSource.diffuse", light.Diffuse());
	first.Uniform4v("LightSource.specular", light.Specular());
	first.Uniform("LightSource.constantAttenuation", light.CAttenuation());
	first.Uniform("LightSource.linearAttenuation", light.LAttenuation());
	first.Uniform("LightSource.quadraticAttenuation", light.QAttenuation());
	second.UniformMatrix4("ModelViewMatrix", mv.c_array());
	second.UniformMatrix4("ProjectionMatrix", p.c_array());
	third.Uniform("Width", width);
	third.UniformMatrix4("ModelViewMatrix", mv.c_array());
	third.UniformMatrix4("ProjectionMatrix", p.c_array());
	fourth.Uniform("Height", height);
	fourth.UniformMatrix4("ModelViewMatrix", mv.c_array());
	fourth.UniformMatrix4("ProjectionMatrix", p.c_array());
	fifth.Uniform("Tex0", 0);
	fifth.Uniform("Tex1", 1);
	fifth.UniformMatrix4("ModelViewMatrix", mv.c_array());
	fifth.UniformMatrix4("ProjectionMatrix", p.c_array());


	int i;
	FrameBuffer *temp;
	vector<FrameBuffer *> fbo;
	temp = new FrameBuffer(width, height);
	temp->AttachRender(GL_DEPTH_COMPONENT24);
	temp->AttachTexture(GL_RGBA16F_ARB);
	fbo.push_back(temp);
	for (i = 0; i < 3; i++) {
		temp = new FrameBuffer(int(width/2), int(height/2));
		temp->AttachTexture(GL_RGBA16F_ARB);
		fbo.push_back(temp);
	}
	FrameBuffer::Unbind();


	Texture texc0("textures/head_color.png", 0);
	Texture texc1("textures/head_normal.png", 1);
	Texture texr0("textures/bricks_color.jpg", 0);
	Texture texr1("textures/bricks_parallax.png", 1);

	Mesh head("models/head.ply");
	Mesh room("models/room.ply");

	VertexBuffer quad;
	GLfloat qverts[] = {-1,-1, 0,  1, -1, 0,  1, 1, 0,  -1, 1, 0};
	GLfloat qcoords[] = {0, 1,  1, 1,  1, 0,  0, 0};
	GLuint qind[] = {0, 1, 2,  0, 2, 3};
	quad.SetVertices(4, qverts);
	quad.SetTexCoords(4, qcoords);
	quad.SetIndices(6, qind);

	p.LoadIdentity();
	p.Perspective(45.0f, float(width)/float(height), 0.1f, 100.0f);
	t.Reset();


// ----- Event cycle ---------------
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				switch( event.key.keysym.sym ){
				case SDLK_ESCAPE:
				case SDLK_q:
					quit = true;
					break;
				case SDLK_F1:
					SDL_WM_ToggleFullScreen(screen);
					break;
				case SDLK_SPACE:
					move = !move;
					break;
				case SDLK_l:
					lmove = !lmove;
					break;
				case SDLK_r:
					rotate = !rotate;
					break;
				case SDLK_p:
					cout << "focalDistance = " << focalDistance << endl;
					cout << "focalRange = " << focalRange << endl;
					break;
				case SDLK_KP7:
					focalDistance -= 0.5f;
					break;
				case SDLK_KP8:
					focalDistance = default_distance;
					break;
				case SDLK_KP9:
					focalDistance += 0.5f;
					break;
				case SDLK_KP4:
					focalRange -= 0.5f;
					break;
				case SDLK_KP5:
					focalRange = default_range;
					break;
				case SDLK_KP6:
					focalRange += 0.5f;
					break;
				case SDLK_KP0:
					focalDistance = default_distance;
					focalRange = default_range;
					movz = 0.0;
					lmov = 0.0;
					roty = 0.0;
					break;
				case SDLK_1:
					focalDistance = 9.0;
					break;
				case SDLK_2:
					focalDistance = 12.5;
					break;
				case SDLK_3:
					focalDistance = 17.5;
					break;
				case SDLK_4:
					focalDistance = 21.0;
					break;
				case SDLK_5:
					focalDistance = 25.0;
					break;
				}
			}
		}

		
// ----- Blitting on the screen ---------------
		t.AddFrame();
		if (move) {
			movz += 0.0025 * t.GetInterval();
		}
		if (lmove) {
			lmov += 0.0025 * t.GetInterval();
		}
		if (rotate) {
			roty += 0.1 * t.GetInterval();
		}


		// First Pass: scene rendering
		fbo[0]->BindIn();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		first.Use();
		first.Uniform("focalDistance", focalDistance);
		first.Uniform("focalRange", focalRange);

		light.Position(6 * sinf(lmov), 0.0, 4.0);
		first.Uniform4v("LightSource.position", light.Position());
		mv.LoadIdentity();
		mv.Translate(0.0, -2.0, -10.0);
		mv.Rotate(15.0, 1.0, 0.0, 0.0);

		texr0.Bind(0);
		texr1.Bind(1);
		mv.Push();
		mv.Translate(0.0, 6.0f, -10.0);
		mv.Scale(10.0, 10.0, 10.0);
		mv.Rotate(180.0, 0.0, 0.0f, 1.0);
		n.Load(mv.c_array()); n.Invert(); n.Transpose();
		first.UniformMatrix4("ModelViewMatrix", mv.c_array());
		first.UniformMatrix4("NormalMatrix", n.c_array());
		first.UniformMatrix4("ProjectionMatrix", p.c_array());
		room.Draw();
		mv.Pop();

		texc0.Bind(0);
		texc1.Bind(1);

		for (i = 0; i < 5; i ++) {
			mv.Push();
			mv.Translate(0.0f, 0.0f, 3.0 * sinf(movz));
			mv.Translate(-3.0f, 0.0f, 0.0);
			mv.Translate(i * 2.0f, 0.0f, i * -4.0f);
			mv.Rotate(roty, 0.0, 1.0, 0.0);
			n.Load(mv.c_array()); n.Invert(); n.Transpose();
			first.UniformMatrix4("ModelViewMatrix", mv.c_array());
			first.UniformMatrix4("NormalMatrix", n.c_array());
			first.UniformMatrix4("ProjectionMatrix", p.c_array());
			head.Draw();
			mv.Pop();
		}

		p.Push();
		p.LoadIdentity();
		mv.LoadIdentity();
	
		glActiveTexture(GL_TEXTURE0);
		// Second pass: downsampling
		fbo[1]->BindIn();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fbo[0]->BindTex();
		second.Use();
		glViewport(0, 0, width/2, height/2);
		quad.DrawElements();

		// Third pass: Gaussian filtering along the X axis
		fbo[2]->BindIn();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fbo[1]->BindTex();
		third.Use();
		quad.DrawElements();

		// Fourth pass: Gaussian filtering along the Y axis
		fbo[3]->BindIn();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fbo[2]->BindTex();
		fourth.Use();
		quad.DrawElements();

		// Fifth pass: final compositing
		FrameBuffer::Unbind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		fbo[0]->BindTex();
		glActiveTexture(GL_TEXTURE1);
		fbo[3]->BindTex();
		fifth.Use();
		glViewport(0, 0, width, height);
		quad.DrawElements();

		p.Pop();


		SDL_GL_SwapBuffers();		
//		SDL_Delay(20);  // Decomment this if you want 1/50th screen update
	}

// ----- Quitting ---------------
	vector<FrameBuffer *>::const_iterator cii;
	for(cii = fbo.begin(); cii != fbo.end(); cii++) {
		delete *cii;
	}

	SDL_Quit();
	return 0;	
}
