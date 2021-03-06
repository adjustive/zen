/* zen_glfw.h - v0.42 - GLFW wrapper -https://github.com/ZenToad/zen

   Do this:
      #define ZEN_GLFW_IMPLEMENTATION
   before you include this file in *one* C or C++ file to create the implementation.
   // i.e. it should look like this:
   #include ...
   #include ...
   #include ...
   #define ZEN_GLFW_IMPLEMENTATION
   #include "zen_glfw.h"

   zlib license:
	 Full license at bottom of file.

*/

#ifndef __ZEN_GLFW_H__
#define __ZEN_GLFW_H__


#ifdef ZEN_GLFW_STATIC
#define ZGLFWDEF static
#else
#define ZGLFWDEF extern
#endif


#ifdef __cplusplus
extern "C" {
#endif


#if defined(ZEN_LIB_DEV)
#include "GLFW/glfw3.h"
#if defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"
#endif
#endif


#define ZGLFW_KEY_DOWN(glfw,key) glfw->keys[GLFW_KEY_##key]
#define ZGLFW_KEY_DOWN_ONCE(glfw,key) glfw->keys[GLFW_KEY_##key] == 1


typedef struct ZGLFW {

   const char *window_title;

   int major_version;
   int minor_version;
   int window_cursor_mode;
   int window_background;
   int should_close;

	int window_width;
	int window_height;
	float window_widthf;
	float window_heightf;

	int display_width;
	int display_height;
	float display_widthf;
	float display_heightf;

	double current_time;
	double last_time;
	double delta_time;
	double total_time;
	int total_frames;

	float mouse_x;
	float mouse_y;
	float mouse_dx;
	float mouse_dy;
	float last_mouse_x;
	float last_mouse_y;
	float mouse_scroll;
	int mouse_button[3];

   GLFWwindow* window;

   void (*error_callback)(int, const char*);
   void (*key_callback)(GLFWwindow*, int, int, int, int);
   void (*window_size_callback)(GLFWwindow*, int, int);
	void (*character_callback)(GLFWwindow*, unsigned int);
	void (*mouse_button_callback)(GLFWwindow*, int, int, int);
	void (*scroll_callback)(GLFWwindow*, double, double);

	int keys[GLFW_KEY_LAST];

} ZGLFW;


ZGLFWDEF ZGLFW *zglfw_create(const char *title, int width, int height, int major = 4, int minor = 4);
ZGLFWDEF void zglfw_init(ZGLFW *glfw);
ZGLFWDEF void zglfw_show_window(ZGLFW *glfw);
ZGLFWDEF int zglfw_is_running(ZGLFW *glfw);
ZGLFWDEF void zglfw_begin(ZGLFW *glfw);
ZGLFWDEF void zglfw_end(ZGLFW *glfw);
ZGLFWDEF void zglfw_quit(ZGLFW *glfw);
ZGLFWDEF void zglfw_destroy(ZGLFW *glfw);


#ifdef __cplusplus
}
#endif



#endif //__ZEN_GLFW_H__



//------------------------------------------
// Implementation
//------------------------------------------
//
#if defined(ZEN_GLFW_IMPLEMENTATION) || defined(ZEN_LIB_DEV)


#if defined(ZEN_LIB_DEV)
#include <stdio.h>
#include "zen_lib/zen.h"
#endif


typedef void* (* GLADloadproc)(const char *name);
int gladLoadGLLoader(GLADloadproc load);


static void zglfw_default_error_callback(int, const char *description) {
   fprintf(stderr, "Error: %s\n", description);
}


static void zglfw_default_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	ZGLFW *glfw = (ZGLFW *)glfwGetWindowUserPointer(window);
	if (glfw) {
		if (action) {
			glfw->keys[key]++;
		} else {
			glfw->keys[key] = 0;
		}
	}
}


static void zglfw_default_window_size_callback(GLFWwindow*, int width, int height) {
	glViewport(0, 0, width, height);
}


static void zglfw_default_character_callback(GLFWwindow*, unsigned int character) {
	zout("Types: %c", character);
}


static void zglfw_default_mouse_button_callback(GLFWwindow *window, int button, int action, int) {
	ZGLFW *glfw = (ZGLFW *)glfwGetWindowUserPointer(window);
	if (glfw) {
		if (action == GLFW_PRESS)
			glfw->mouse_button[button] = 1;
		else
			glfw->mouse_button[button] = 0;
	}
}


static void zglfw_default_scroll_callback(GLFWwindow *window, double /*xoffset*/, double yoffset) {
	ZGLFW *glfw = (ZGLFW *)glfwGetWindowUserPointer(window);
	if (glfw) {
		glfw->mouse_scroll += yoffset;
	}
}


ZGLFWDEF ZGLFW *zglfw_create(const char *title, int width, int height, int major, int minor) {


	ZGLFW *glfw = ZEN_CALLOC(ZGLFW, 1);
	GB_ASSERT_NOT_NULL(glfw);

	glfw->window_title = title;
	glfw->major_version = major;
	glfw->minor_version = minor;
	glfw->window_width = width;
	glfw->window_height = height;
	glfw->window_background = 0xFF333333;
	glfw->window_cursor_mode = GLFW_CURSOR_NORMAL;

	return glfw;

}


ZGLFWDEF void zglfw_destroy(ZGLFW *glfw) {
	free(glfw);
}


ZGLFWDEF void zglfw_init(ZGLFW *glfw) {

   if (glfw->error_callback) {
		glfwSetErrorCallback(glfw->error_callback);
   } else {
		glfwSetErrorCallback(zglfw_default_error_callback);
	}

	if (!glfwInit()) {
      exit(EXIT_FAILURE);
   }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glfw->major_version);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glfw->minor_version);
   glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	glfwWindowHint(GLFW_SAMPLES, 4);

	glfw->window = glfwCreateWindow(glfw->window_width, glfw->window_height, glfw->window_title, NULL, NULL);
	if (!glfw->window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetWindowUserPointer(glfw->window, glfw);

   if (glfw->key_callback) {
		glfwSetKeyCallback(glfw->window, glfw->key_callback);
   } else {
		glfwSetKeyCallback(glfw->window, zglfw_default_key_callback);
	}

   if (glfw->window_size_callback) {
		glfwSetWindowSizeCallback(glfw->window, glfw->window_size_callback);
   } else {
		glfwSetWindowSizeCallback(glfw->window, zglfw_default_window_size_callback);
	}

	if (glfw->character_callback) {
		glfwSetCharCallback(glfw->window, glfw->character_callback);
	} else {
		glfwSetCharCallback(glfw->window, zglfw_default_character_callback);
	}

	if (glfw->mouse_button_callback) {
		glfwSetMouseButtonCallback(glfw->window, glfw->mouse_button_callback);
	} else {
		glfwSetMouseButtonCallback(glfw->window, zglfw_default_mouse_button_callback);
	}

	if (glfw->scroll_callback) {
		glfwSetScrollCallback(glfw->window, glfw->scroll_callback);
	} else {
		glfwSetScrollCallback(glfw->window, zglfw_default_scroll_callback);
	}

	glfwSetInputMode(glfw->window, GLFW_CURSOR, glfw->window_cursor_mode);

	glfwMakeContextCurrent(glfw->window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	glfwSwapInterval(1);

	printf("OpenGL Loaded\n");
	printf("Vendor:   %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version:  %s\n", glGetString(GL_VERSION));

	glfw->current_time = glfwGetTime();
	glfw->last_time = glfw->current_time;
	glfw->delta_time = 0.0;
	glfw->total_time = 0.0;
	glfw->total_frames = 0;

}


ZGLFWDEF void zglfw_show_window(ZGLFW *glfw) {
	glfwShowWindow(glfw->window);
}


ZGLFWDEF int zglfw_is_running(ZGLFW *glfw) {
   int should_close = glfw->should_close || glfwWindowShouldClose(glfw->window);
   return !should_close;
}


ZGLFWDEF void zglfw_begin(ZGLFW *glfw) {

	glfw->last_time = glfw->current_time;
	glfw->current_time = glfwGetTime();
	glfw->delta_time = glfw->current_time - glfw->last_time;
	glfw->total_time += glfw->delta_time;
	glfw->total_frames++;

	double x, y;
	glfwGetCursorPos(glfw->window, &x, &y);

	glfw->last_mouse_x = glfw->mouse_x;
	glfw->last_mouse_y = glfw->mouse_y;
	glfw->mouse_x = cast(float)x;
	glfw->mouse_y = cast(float)y;
	glfw->mouse_dx = glfw->mouse_x - glfw->last_mouse_x;
	glfw->mouse_dy = glfw->mouse_y - glfw->last_mouse_y;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   int r = glfw->window_background & 0xFF;
   int g = (glfw->window_background >> 8) & 0xFF;
   int b = (glfw->window_background >> 16) & 0xFF;
   int a = (glfw->window_background >> 24) & 0xFF;
   glClearColor(r / 255.99f, g / 255.99f, b / 255.99f, a / 255.99f);

	glfwGetWindowSize(glfw->window, &glfw->window_width, &glfw->window_height);
	glfw->window_widthf = glfw->window_width;
	glfw->window_heightf = glfw->window_height;

   glfwGetFramebufferSize(glfw->window, &glfw->display_width, &glfw->display_height);
	glfw->display_widthf = glfw->display_width;
	glfw->display_heightf = glfw->display_height;

}


ZGLFWDEF void zglfw_end(ZGLFW *glfw) {

	for (int i = 32; i < GLFW_KEY_LAST; ++i) {
		if (glfw->keys[i]) {
			glfw->keys[i]++;
		}
	}

	for (int i = 0; i < 3; ++i) {
		if (glfw->mouse_button[i]) {
			glfw->mouse_button[i]++;
		}
	}

	glfw->mouse_scroll = 0.0f;
   glfwPollEvents();
   glfwSwapBuffers(glfw->window);
}


ZGLFWDEF void zglfw_quit(ZGLFW *glfw) {
	int ms = (int)(1.0e3 * glfw->total_time / glfw->total_frames);
	zout("\n%d (ms)\n", ms);
	glfwDestroyWindow(glfw->window);
	glfwTerminate();
}


#endif // ZEN_GLFW_IMPLEMENTATION

/*
  zlib license:

  Copyright (c) 2017 Timothy Wright https://github.com/ZenToad

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
