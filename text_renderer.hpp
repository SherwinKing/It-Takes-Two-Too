#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

// #include <GL/glew.h>
// #include <GL/freeglut.h>

#include "GL.hpp"
#include "gl_compile_program.hpp"
#include "gl_errors.hpp"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// #include <ft2build.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <hb.h>
#include <hb-ft.h>

#include <iostream>



void init_text_rendering_gl_env();

/**
 * Render text using the currently loaded font and currently set font size.
 * Rendering starts at coordinates (x, y), z is always 0.
 * The pixel coordinates that the FreeType2 library uses are scaled by (sx, sy).
 * 
 * Based on a tutorial and its corresponding codes:
 * https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_01
 * https://gitlab.com/wikibooks-opengl/modern-tutorials/-/blob/master/text01_intro/text.cpp
 * Also used as reference: https://github.com/tangrams/harfbuzz-example/blob/master/src/hbshaper.h
 */
void render_text(const char *text, float x, float y, float sx, float sy, 
	glm::vec4 color  = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
	uint32_t font_size = 36);
