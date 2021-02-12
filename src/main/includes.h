/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This file has all the includes so the app works in different systems.
	It is a little bit low level so do not worry about the code.
*/

#ifndef INCLUDES_H
#define INCLUDES_H

//under windows we need this file to make opengl work
#ifdef WIN32 
	#include <windows.h>
	#pragma comment(lib, "SDL2.lib")
	#pragma comment(lib, "SDL2main.lib")
#endif

//remove warnings about unsafe functions
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#pragma warning(disable:4996)

//SDL

#ifndef APIENTRY
    #define APIENTRY
#endif

//GLUT
#ifdef __APPLE__
	#include <OpenGL/gl.h>
    #include <OpenGL/glext.h>
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_opengl.h>
    #include <OpenGL/glu.h>
    #include <GLUT/glut.h>
#else
	#include <SDL2/SDL.h>

	#define glActiveTexture glActiveTexture___

	#include <SDL2/SDL_opengl.h>

	#undef glActiveTexture

	#include <SDL2/SDL.h>
	#include <SDL2/SDL_opengl.h>
	#include <GL/glut.h>
	#include <GL/glu.h>
#endif

#include <iostream>
#include <cmath>

//OpenGL
 //including GLUT we include everything (opengl, glu and glut)

//used to access opengl extensions
//void* getGLProcAddress(const char*);
#define REGISTER_GLEXT(RET, FUNCNAME, ...) typedef RET (APIENTRY * FUNCNAME ## _func)(__VA_ARGS__); FUNCNAME ## _func FUNCNAME = NULL; 
#define IMPORT_GLEXT(FUNCNAME) FUNCNAME = (FUNCNAME ## _func) SDL_GL_GetProcAddress(#FUNCNAME); if (FUNCNAME == NULL) { std::cout << "ERROR: This Graphics card doesnt support " << #FUNCNAME << std::endl; }

#endif
