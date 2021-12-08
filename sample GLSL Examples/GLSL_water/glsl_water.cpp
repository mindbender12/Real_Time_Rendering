#include <iostream>
#include <cmath>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>

#include "Shader.h"
#include "ShaderProgram.h"
#include "FrameTimer.h"
#include "VertexBuffer.h"
#include "MeshLoader.h"
#include "Light.h"
#include "Texture.h"
#include "FrameBuffer.h"

using std::cout;
using std::endl;

int main(int argc, char **argv)
{
	SDL_Surface *screen;
	SDL_Event event;
	const SDL_VideoInfo* info = NULL;
	int width = 1280;
	int height = 800;
	int bpp = 0;
	int flags = 0;

	bool quit = false;
	bool rotate = false, lmove = true;
	GLfloat rotcam = 0.0f, roty = 0.0f, lrot = 0.0f;

	const float waterHeight_step = 0.1f;
	const float default_waterHeight = 0.0f;
	float waterHeight = default_waterHeight;
	const float waveLen_step = 0.1f;
	const float default_waveLen = 1.0f;
	float waveLen = default_waveLen;
	const float waveHeight_step = 0.1f;
	const float default_waveHeight = 1.0f;
	float waveHeight = default_waveLen;
	int rendermode = 4;

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
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 0);
	
	flags = SDL_OPENGL;

	
// ----- Setting up the screen surface ---------------
	if ((screen = SDL_SetVideoMode(width, height, bpp, flags)) == 0) {
		fprintf(stderr, "Video mode set failed: %s\n", SDL_GetError());
        	exit(-1);
	}

	SDL_WM_SetCaption("GLSL_water by Encelo", NULL);


// ----- OpenGL init ---------------
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	light.CAttenuation(0.0);
	light.LAttenuation(0.1);
	light.QAttenuation(0.001);

	ShaderProgram nclip_sh("shaders/normalclip.vert", "shaders/parallaxclip.frag");
	ShaderProgram water_sh("shaders/water.vert", "shaders/water.frag");
	nclip_sh.Uniform("Tex0", 0);
	nclip_sh.Uniform("Tex1", 1);
	water_sh.Uniform("Tex0", 0);
	water_sh.Uniform("Tex1", 1);
	water_sh.Uniform("Tex2", 2);
	water_sh.Uniform("R0", powf(1.0 - 1.333333, 2.0)/powf(1.0 + 1.333333, 2.0));

	FrameBuffer refraction_fbo(width, height);
	refraction_fbo.AttachRender(GL_DEPTH_COMPONENT24);
    refraction_fbo.AttachTexture(GL_RGBA);
	FrameBuffer reflection_fbo(width, height);
	reflection_fbo.AttachRender(GL_DEPTH_COMPONENT24);
    reflection_fbo.AttachTexture(GL_RGBA);
	FrameBuffer::Unbind();

	Texture waterTex("textures/waterbump.jpg");
	Texture texr0("textures/bricks2_color.jpg");
	Texture texr1("textures/bricks2_parallax.png");

	MeshLoader ml;
	VertexBuffer room;
	ml.Load("models/uv_room.ply", &room);

	VertexBuffer quad;
	GLfloat qverts[] = {-1,-1, 0,  1,-1, 0,  1, 1, 0, -1, 1, 0};
	GLfloat qnorms[] = { 0, 0,-1,  0, 0,-1,  0, 0,-1,  0, 0,-1};
	GLfloat qcoords[] = {0, 0,  1, 0,  1, 1,  0, 1};
	GLuint qind[] = {0, 1, 2,  0, 2, 3};
	quad.SetVertices(4, qverts);
	quad.SetNormals(4, qnorms);
	quad.SetTexCoords(4, qcoords);
	quad.SetIndices(6, qind);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, float(width)/float(height), 0.1f, 100.0f);

	t.Reset();


// ----- Event cycle ---------------
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) { 
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
				case SDLK_q:
					quit = true;
					break;
				case SDLK_F1:
					SDL_WM_ToggleFullScreen(screen);
					break;
				case SDLK_SPACE:
					lmove = !lmove;
					break;
				case SDLK_r:
					rotate = !rotate;
					break;
				case SDLK_1:
					rendermode = 1;
					break;
				case SDLK_2:
					rendermode = 2;
					break;
				case SDLK_3:
					rendermode = 3;
					break;
				case SDLK_4:
					rendermode = 4;
					break;
				case SDLK_KP0:
					rotcam = roty = lrot = 0.0f;
					waterHeight = default_waterHeight;
					waveLen = default_waveLen;
					waveHeight = default_waveHeight;
					break;
				case SDLK_KP7:
					waterHeight += waterHeight_step;
					break;
				case SDLK_KP8:
					waterHeight -= waterHeight_step;
					break;
				case SDLK_KP9:
					waterHeight = default_waterHeight;
					break;
				case SDLK_KP4:
					waveLen += waveLen_step;
					break;
				case SDLK_KP5:
					if (waveLen - waveLen_step > 0.0f)
						waveLen -= waveLen_step;
					break;
				case SDLK_KP6:
					waveLen = default_waveLen;
					break;
				case SDLK_KP1:
					waveHeight += waveHeight_step;
					break;
				case SDLK_KP2:
					if (waveHeight - waveHeight_step > 0.0f)
						waveHeight -= waveHeight_step;
					break;
				case SDLK_KP3:
					waveHeight = default_waveHeight;
					break;
				default:
					break;
				}
			}
		}

		
// ----- Blitting on the screen ---------------
		t.AddFrame();
		if (rotate) {
			rotcam += 0.001f * t.GetInterval();
		}
		if (lmove) {
			lrot += 0.0025f * t.GetInterval();
		}


		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(4 * sinf(lrot), -8.0f, -28.0f);
		glRotatef(45 + 45 * sinf(rotcam), 1.0f, 0.0f, 0.0f);
		light.Position(0.0f, 0.0f, 0.0f);


		// First Pass: Refraction Map
		if (rendermode == 1 || rendermode == 4) {
			FrameBuffer::Unbind();
			if (rendermode == 4)
				refraction_fbo.BindOutput();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			nclip_sh.Uniform("ClipPlane", 0.0f, -1.0f, 0.0f, -waterHeight);
			nclip_sh.Use();

			glLoadIdentity();
			glTranslatef(0.0f, -8.0, -28.0f);
			glRotatef(45 + 45 * sinf(rotcam), 1.0f, 0.0f, 0.0f);

			glActiveTexture(GL_TEXTURE0);
			texr0.Bind();
			glActiveTexture(GL_TEXTURE1);
			texr1.Bind();
			glPushMatrix();
			glScalef(10.0f, 10.0f, 10.0f);
			room.DrawElements();
			glPopMatrix();
		}

	
		//  Second Pass: Reflection Map
		if (rendermode == 2 || rendermode == 4) {
			FrameBuffer::Unbind();		
			if (rendermode == 4)
				reflection_fbo.BindOutput();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			nclip_sh.Uniform("ClipPlane", 0.0f, 1.0f, 0.0f, waterHeight);
			nclip_sh.Use();

			glLoadIdentity();
			glTranslatef(0.0f, -8.0f, -28.0f);
			glScalef(1.0f, -1.0f, 1.0f);
			glRotatef(45 + 45 * sinf(rotcam), -1.0f, 0.0f, 0.0f);

			glActiveTexture(GL_TEXTURE0);
			texr0.Bind();
			glActiveTexture(GL_TEXTURE1);
			texr1.Bind();
			glPushMatrix();
			glScalef(10.0f, 10.0f, 10.0f);
			glTranslatef(0.0f, 2.0f*waterHeight, 0.0f);
			room.DrawElements();
			glPopMatrix();
		}


		FrameBuffer::Unbind();
		// Third Pass: scene rendering
		if (rendermode == 3 || rendermode == 4) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			nclip_sh.Uniform("ClipPlane", 0.0f, 0.0f, 0.0f, 0.0f); // no clip
			nclip_sh.Use();

			glLoadIdentity();
			glTranslatef(0.0f, -8.0, -28.0f);
			glRotatef(45 + 45 * sinf(rotcam), 1.0f, 0.0f, 0.0f);

			glActiveTexture(GL_TEXTURE0);
			texr0.Bind();
			glActiveTexture(GL_TEXTURE1);
			texr1.Bind();
			glPushMatrix();
			glScalef(10.0f, 10.0f, 10.0f);
			room.DrawElements();
			glPopMatrix();
		}

		
		// Fourth Pass: water rendering
		if (rendermode == 4) {
			water_sh.Uniform("Time", float(t.GetNow()));
			water_sh.Uniform("WaveLen", waveLen);
			water_sh.Uniform("WaveHeight", waveHeight);
			water_sh.Use();
			glActiveTexture(GL_TEXTURE0);
			refraction_fbo.BindTex();
			glActiveTexture(GL_TEXTURE1);
			reflection_fbo.BindTex();
			glActiveTexture(GL_TEXTURE2);
			waterTex.Bind();

			glLoadIdentity();
			glTranslatef(0.0f, -8.0f, -28.0f);
			glRotatef(45 + 45 * sinf(rotcam), 1.0f, 0.0f, 0.0f);

			glPushMatrix();
			glScalef(10.0f, 10.0f, 10.0f);
			glTranslatef(0.0f, -waterHeight, -0.0f);
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			quad.DrawElements();
			glPopMatrix();
		}

		SDL_GL_SwapBuffers();		
	}

// ----- Quitting ---------------
	SDL_Quit();
	return 0;	
}
