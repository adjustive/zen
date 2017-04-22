/* zen_gl.h - v0.42 - public domain opengl wrapper -https://github.com/ZenToad/zen
                                     no warranty implied; use at your own risk

   Do this:
      #define ZEN_GL_IMPLEMENTATION
   before you include this file in *one* C or C++ file to create the implementation.
   // i.e. it should look like this:
   #include ...
   #include ...
   #include ...
   #define ZEN_GL_IMPLEMENTATION
   #include "zen_gl.h"

	Very hacked up version of Ginger Bill's code: https://github.com/gingerBill/gb

	Full license at bottom of file.	

*/

#ifndef ZEN_GL_INCLUDE
#define ZEN_GL_INCLUDE

#ifndef ZGL_BS_MAX_VERTEX_COUNT
#define ZGL_BS_MAX_VERTEX_COUNT 64
#endif

#ifndef ZGL_BS_MAX_INDEX_COUNT
#define ZGL_BS_MAX_INDEX_COUNT 6
#endif

#ifndef ZGL_MAX_UNIFORM_COUNT
#define ZGL_MAX_UNIFORM_COUNT 32
#endif

#ifndef f32
#include <stdint.h>
typedef float f32;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint8_t u8;
typedef uint16_t u16;
typedef int32_t b32;
#endif


#ifdef __cplusplus
extern "C" {
#endif

#include "stb_truetype.h"


typedef enum ZGLShaderType {
	ZGL_VERTEX_SHADER,
	ZGL_FRAGMENT_SHADER,

	ZGL_SHADER_COUNT,
} ZGLShaderType;


typedef enum ZGLShaderError {
	ZGL_ERROR_NONE,
	ZGL_VERTEX_COMPILE_ERROR,
	ZGL_FRAGMENT_COMPILE_ERROR,
 	ZGL_LINKER_ERROR,

	ZGL_SHADER_ERROR_COUNT,
} ZGLShaderError;


typedef struct ZGLShader {
	u32 shaders[ZGL_SHADER_COUNT];
	u32 program;

	i32   uniform_locs[ZGL_MAX_UNIFORM_COUNT];
	char *uniform_names[ZGL_MAX_UNIFORM_COUNT];
	i32   uniform_count;

	char *files[ZGL_SHADER_COUNT];

	char base_name[64];
} ZGLShader;


typedef union ZGLColor {
	u32    rgba; // NOTE(tim): 0xaabbggrr in little endian
	struct { u8 r, g, b, a; };
	u8     e[4];
} ZGLColor;


typedef struct ZGLBasicVertex {
	f32 x, y;
	f32 u, v;
} ZGLBasicVertex;  


typedef struct ZGLTexture {
	i32 width, height, channel_count;
	u32 handle;
} ZGLTexture;

typedef struct ZGLFont {
	
	i32 atlas_width;
	i32 atlas_height;
	i32 char_start;
	i32 char_count;
	i32 type_flag;
	f32 size_in_pixels;

	stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
	ZGLTexture font_texture;

} ZGLFont;

typedef struct ZGLBasicState {

	ZGLBasicVertex vertices[ZGL_BS_MAX_VERTEX_COUNT];
	u16 indices[ZGL_BS_MAX_INDEX_COUNT];

	ZGLShader ortho_col_shader;	
	ZGLShader ortho_tex_shader;	
	ZGLShader ortho_font_shader;	
	u32 vao;
	u32 vbo;
	u32 ebo;

	f32 ortho_mat[16];
	i32 width;
	i32 height;

	u32 nearest_sampler;
	u32 linear_sampler;
	u32 mipmap_sampler;

	ZGLFont font;

} ZGLBasicState;


#ifdef ZEN_GL_STATIC
#define ZGLDEF static
#else 
#define ZGLDEF extern
#endif


ZGLDEF void zgl_bs_initialize(ZGLBasicState *bs, int window_width, int window_height);
ZGLDEF void zgl_bs_begin(ZGLBasicState *bs, i32 window_width, i32 window_height);
ZGLDEF void zgl_bs_end(ZGLBasicState *bs);
ZGLDEF void zgl_bs_draw_rect(ZGLBasicState *bs, f32 mv[16], f32 x, f32 y, f32 w, f32 h, ZGLColor col);
ZGLDEF void zgl_bs_draw_quad(ZGLBasicState *bs, f32 mv[16], f32 x0, f32 y0, f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, ZGLColor col);
ZGLDEF void zgl_bs_draw_circle(ZGLBasicState *bs, f32 mv[16], f32 cx, f32 cy, f32 r, ZGLColor color);
ZGLDEF void zgl_bs_draw_point(ZGLBasicState *bs, f32 mv[16], f32 x, f32 y, ZGLColor color);
ZGLDEF void zgl_bs_draw_line(ZGLBasicState *bs, f32 mv[16], f32 x0, f32 y0, f32 x1, f32 y1, ZGLColor color);
ZGLDEF void zgl_bs_fill_rect(ZGLBasicState *bs, f32 mv[16], f32 x, f32 y, f32 w, f32 h, ZGLColor col);
ZGLDEF void zgl_bs_fill_quad(ZGLBasicState *bs, f32 mv[16], f32 x0, f32 y0, f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, ZGLColor col);
ZGLDEF void zgl_bs_fill_circle(ZGLBasicState *bs, f32 mv[16], f32 cx, f32 cy, f32 r, ZGLColor color);

ZGLDEF  b32 zgl_load_texture2d_from_memory(ZGLTexture *tex, void const *data, i32 width, i32 height, i32 channel_count);
ZGLDEF  b32 zgl_load_texture2d_from_file(ZGLTexture *texture, b32 flip_vertically, char const *filename);
ZGLDEF void zgl_bs_draw_textured_rect(ZGLBasicState *bs, ZGLTexture *tex, f32 mv[16], f32 x, f32 y, f32 w, f32 h, b32 v_up);
ZGLDEF void zgl_bs_draw_textured_subrect( ZGLBasicState *bs, ZGLTexture *tex, f32 mv[16], f32 x, f32 y, f32 w, f32 h, f32 u0, f32 v0, f32 u1, f32 v1,b32 v_up);

ZGLDEF b32 zgl_load_font(ZGLBasicState *bs, const char *filename, f32 size_in_pixels);
ZGLDEF void zgl_draw_string(ZGLBasicState *bs, const char* text, f32 *x, f32 *y, ZGLColor color);


#ifdef __cplusplus
}
#endif


#endif // ZEN_GL_INCLUDE


#ifdef ZEN_GL_IMPLEMENTATION

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"

#ifndef STB_TRUETYPE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION  
#endif
#include "stb_truetype.h"

#include <assert.h>


#define zgl_vert_ptr_aa(index, element_count, Type, var_name) \
    zgl_vert_ptr_aa_f32(index, element_count, Type, var_name)

#define zgl_vert_ptr_aa_f32(index, element_count, Type, var_name) do {  \
	glVertexAttribPointer(index,                                         \
	                      element_count,                                 \
	                      GL_FLOAT,                                      \
	                      false,                                         \
	                      zen_sizeof(Type),                              \
	                      (void const *)(offsetof(Type, var_name))); \
	glEnableVertexAttribArray(index);                                    \
} while (0)


static char zgl_err_buf[1024];


i32 const gbglTextureFormat[4] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };
i32 const zglInternalTextureFormat_8[4]  = { GL_R8,   GL_RG8,   GL_RGB8,	  GL_RGBA8   };


static ZGLShaderError zgl_create_shader(ZGLShader *shader, const char* vertex_shader, const char* fragment_shader) {

	ZGLShaderError status = ZGL_ERROR_NONE;

	shader->shaders[ZGL_VERTEX_SHADER] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader->shaders[ZGL_VERTEX_SHADER], 1, &vertex_shader, NULL);
	glCompileShader(shader->shaders[ZGL_VERTEX_SHADER]);

	GLint result = GL_FALSE;
	glGetShaderiv(shader->shaders[ZGL_VERTEX_SHADER], GL_COMPILE_STATUS, &result);
	if (!result) {
		GLint len = 0;
		glGetShaderiv(shader->shaders[ZGL_VERTEX_SHADER], GL_INFO_LOG_LENGTH, &len);
		if(len > 1023) len = 1023;
		glGetShaderInfoLog(shader->shaders[ZGL_VERTEX_SHADER], len, NULL, zgl_err_buf);
		zgl_err_buf[len] = '\0';
		printf("----- Shader compile error: %s:%d\n%s\n", __FILE__, __LINE__, zgl_err_buf);
		status = ZGL_VERTEX_COMPILE_ERROR;
	}

	shader->shaders[ZGL_FRAGMENT_SHADER] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader->shaders[ZGL_FRAGMENT_SHADER], 1, &fragment_shader, NULL);
	glCompileShader(shader->shaders[ZGL_FRAGMENT_SHADER]);

	result = GL_FALSE;
	glGetShaderiv(shader->shaders[ZGL_FRAGMENT_SHADER], GL_COMPILE_STATUS, &result);
	if (!result) {
		GLint len = 0;
		glGetShaderiv(shader->shaders[ZGL_FRAGMENT_SHADER], GL_INFO_LOG_LENGTH, &len);
		glGetShaderInfoLog(shader->shaders[ZGL_FRAGMENT_SHADER], len, NULL, zgl_err_buf);
		zgl_err_buf[len] = '\0';
		printf("----- Shader compile error: %s:%d\n%s\n", __FILE__, __LINE__, zgl_err_buf);
		status = ZGL_FRAGMENT_COMPILE_ERROR;
	}

	shader->program = glCreateProgram();
	glAttachShader(shader->program, shader->shaders[ZGL_VERTEX_SHADER]);
	glAttachShader(shader->program, shader->shaders[ZGL_FRAGMENT_SHADER]);
	glLinkProgram(shader->program);

	glGetProgramiv(shader->program, GL_LINK_STATUS, &result);
	if (!result) {
		GLint len = 0;
		glGetProgramiv(shader->program, GL_INFO_LOG_LENGTH, &len);
		if(len > 1023) len = 1023;
		glGetProgramInfoLog(shader->program, len, NULL, zgl_err_buf);
		zgl_err_buf[len] = '\0';
		printf("----- Program link error: %s:%d\n%s\n", __FILE__, __LINE__, zgl_err_buf);
		status = ZGL_LINKER_ERROR;
	}

	glDetachShader(shader->program, shader->shaders[ZGL_VERTEX_SHADER]);
	glDetachShader(shader->program, shader->shaders[ZGL_FRAGMENT_SHADER]);

	return status;

}

static void zgl_bs_set_resolution(ZGLBasicState *bs, i32 window_width, i32 window_height) {

	f32 left   = -cast(f32)window_width / 2.0f;
	f32 right  = cast(f32)window_width / 2.0f;
	f32 bottom = -cast(f32)window_height / 2.0f;
	f32 top    = cast(f32)window_height / 2.0f;
	f32 znear  = 0.0f;
	f32 zfar   = 1.0f;

	bs->width  = window_width;
	bs->height = window_height;

	bs->ortho_mat[0] = 2.0f / (right - left);
	bs->ortho_mat[1] = 0.0f;
	bs->ortho_mat[2] = 0.0f;
	bs->ortho_mat[3] = 0.0f;

	bs->ortho_mat[4] = 0.0f;
	bs->ortho_mat[5] = 2.0f / (top - bottom);
	bs->ortho_mat[6] = 0.0f;
	bs->ortho_mat[7] = 0.0f;

	bs->ortho_mat[8]  =  0.0f;
	bs->ortho_mat[9]  =  0.0f;
	bs->ortho_mat[10] = -2.0f / (zfar - znear);
	bs->ortho_mat[11] =  0.0f;

	bs->ortho_mat[12] = -(right + left) / (right - left);
	bs->ortho_mat[13] = -(top + bottom) / (top - bottom);
	bs->ortho_mat[14] = -(zfar + znear) / (zfar - znear);
	bs->ortho_mat[15] = 1.0f;
}

ZGLDEF void zgl_bs_begin(ZGLBasicState *bs, i32 window_width, i32 window_height) {
	glBindVertexArray(bs->vao);
	glDisable(GL_SCISSOR_TEST);
	zgl_bs_set_resolution(bs, window_width, window_height);
}

ZGLDEF void zgl_bs_end(ZGLBasicState *bs) {
	glBindVertexArray(0);
}

static u32 zgl__make_buffer(isize size, void const *data, i32 target, i32 usage_hint) {
	u32 buffer_handle;
	glGenBuffers(1, &buffer_handle);
	glBindBuffer(target, buffer_handle);
	glBufferData(target, size, data, usage_hint);
	return buffer_handle;
}

// NOTE(tim): usage_hint == (GL_STATIC_DRAW, GL_STREAM_DRAW, GL_DYNAMIC_DRAW)
static u32 zgl_make_vbo(void const *data, isize size, i32 usage_hint) {
	return zgl__make_buffer(size, data, GL_ARRAY_BUFFER, usage_hint);
}

static u32 zgl_make_ebo(void const *data, isize size, i32 usage_hint) {
	return zgl__make_buffer(size, data, GL_ELEMENT_ARRAY_BUFFER, usage_hint);
}

static u32 zgl_make_sampler(u32 min_filter, u32 max_filter, u32 s_wrap, u32 t_wrap) {
	u32 samp;
	glGenSamplers(1, &samp);
	glSamplerParameteri(samp, GL_TEXTURE_MIN_FILTER, min_filter);
	glSamplerParameteri(samp, GL_TEXTURE_MAG_FILTER, max_filter);
	glSamplerParameteri(samp, GL_TEXTURE_WRAP_S,     s_wrap);
	glSamplerParameteri(samp, GL_TEXTURE_WRAP_T,     t_wrap);
	return samp;
}

static i32 zgl_get_uniform(ZGLShader *s, char const *name) {
	i32 i, loc = -1;
	for (i = 0; i < s->uniform_count; i++) {
		if (strcmp(s->uniform_names[i], name) == 0) {
			return s->uniform_locs[i];
		}
	}

	assert(s->uniform_count < ZGL_MAX_UNIFORM_COUNT);

	loc = glGetUniformLocation(s->program, name);

	usize len = strlen(name);
	char* uname = (char*)calloc(len+1, zen_sizeof(char));
	uname[len] = '\0';
	strcpy(uname, name);
	s->uniform_names[s->uniform_count] = uname;
	s->uniform_locs[s->uniform_count] = loc;
	s->uniform_count++;

	return loc;
}

static void zgl_use_shader(ZGLShader *s) { glUseProgram(s ? s->program : 0); }

static void zgl_set_uniform_mat4_count(ZGLShader *s, char const *name, f32 const *m, isize count) {
	glUniformMatrix4fv(zgl_get_uniform(s, name), count, false, m);
}

static void zgl_set_uniform_mat4(ZGLShader *s, char const *name, f32 const *m) {
	zgl_set_uniform_mat4_count(s, name, m, 1);
}

static void zgl_set_uniform_vec4(ZGLShader *s, char const *name, f32 const *v) {
	glUniform4fv(zgl_get_uniform(s, name), 1, v);
}

static void zgl_set_uniform_colour(ZGLShader *s, char const *name, ZGLColor col) {
	f32 v[4];
	v[0] = col.r / 255.0f;
	v[1] = col.g / 255.0f;
	v[2] = col.b / 255.0f;
	v[3] = col.a / 255.0f;
	zgl_set_uniform_vec4(s, name, v);
}

static void zgl__buffer_copy(u32 buffer_handle, i32 target, void const *data, isize size, isize offset) {
	glBindBuffer(target, buffer_handle);
	glBufferSubData(target, offset, size, data);
}

static void zgl_vbo_copy(u32 vbo_handle, void *const data, isize size, isize offset) {
	zgl__buffer_copy(vbo_handle, GL_ARRAY_BUFFER, data, size, offset);
}

static void zgl__bs_setup_ortho_colour_state(ZGLBasicState *bs, isize vertex_count, ZGLColor col) {

	zgl_use_shader(&bs->ortho_col_shader);
	zgl_set_uniform_mat4(&bs->ortho_col_shader, "u_ortho_mat", bs->ortho_mat);
	zgl_set_uniform_colour(&bs->ortho_col_shader, "u_colour", col);

	zgl_vbo_copy(bs->vbo, bs->vertices, vertex_count*zen_sizeof(bs->vertices[0]), 0);
	zgl_vert_ptr_aa(0, 2, ZGLBasicVertex, x);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bs->ebo);

}

static void zgl_bind_texture2d(ZGLTexture const *t, u32 position, u32 sampler) {

	if (position > 31) {
		position = 31;
		fprintf(stderr, "Textures can only bound to position [0 ... 31]\n");
		fprintf(stderr, "Will bind to position [31]\n");
	}

	glActiveTexture(GL_TEXTURE0 + position);
	glBindTexture(GL_TEXTURE_2D, t ? t->handle : 0);
	glBindSampler(position, sampler);
}

b32 zgl_load_texture2d_from_memory(ZGLTexture *tex, void const *data, i32 width, i32 height, i32 channel_count) {

	b32 result = true;

	memset(tex, 0, zen_sizeof(ZGLTexture));

	tex->width = width;
	tex->height = height;
	tex->channel_count = channel_count;

	glGenTextures(1, &tex->handle);
	glBindTexture(GL_TEXTURE_2D, tex->handle);

	assert(GL_MAX_TEXTURE_SIZE > width);
	assert(GL_MAX_TEXTURE_SIZE > height);

	glTexImage2D(GL_TEXTURE_2D, 0,
	             zglInternalTextureFormat_8[channel_count-1],
	             width, height, 0,
	             gbglTextureFormat[channel_count-1],
	             GL_UNSIGNED_BYTE, data);

   glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFinish();


	return result;
}

b32 zgl_load_texture2d_from_file(ZGLTexture *texture, b32 flip_vertically, char const *filename) {

	b32 result;
	u8 *data;
	int width, height, comp;

	stbi_set_flip_vertically_on_load(flip_vertically);
	data = stbi_load(filename, &width, &height, &comp, 4);
	if (data == NULL) {
		fprintf(stderr, "Failed to load image: %s\n", filename);
		result = false;
	} else {
		result = zgl_load_texture2d_from_memory(texture, data, width, height, comp);
		stbi_image_free(data);
	}
	return result;
}

ZGLDEF void zgl_bs_draw_circle(ZGLBasicState *bs, f32 mv[16], f32 cx, f32 cy, f32 r, ZGLColor color) {

	i32 n = ZGL_BS_MAX_VERTEX_COUNT;
	f32 dr = 2.0f * M_PI / n;
	for (i32 i = 0; i < n; ++i) {
		f32 x = cx + r * cosf(i * dr);
		f32 y = cy + r * sinf(i * dr);
		bs->vertices[i].x = x;
		bs->vertices[i].y = y;
	}

	zgl__bs_setup_ortho_colour_state(bs, ZGL_BS_MAX_VERTEX_COUNT, color);
	zgl_set_uniform_mat4(&bs->ortho_col_shader, "u_model_view_mat", mv);

	glDrawArrays(GL_LINE_LOOP, 0, ZGL_BS_MAX_VERTEX_COUNT);
}

ZGLDEF void zgl_bs_fill_circle(ZGLBasicState *bs, f32 mv[16], f32 cx, f32 cy, f32 r, ZGLColor color) {

	i32 n = ZGL_BS_MAX_VERTEX_COUNT;
	f32 dr = 2.0f * M_PI / (n - 2);
	bs->vertices[0].x = cx;
	bs->vertices[0].y = cy;
	for (i32 i = 1; i < n; ++i) {
		f32 x = cx + r * cosf(i * dr);
		f32 y = cy + r * sinf(i * dr);
		bs->vertices[i].x = x;
		bs->vertices[i].y = y;
	}

	zgl__bs_setup_ortho_colour_state(bs, ZGL_BS_MAX_VERTEX_COUNT, color);
	zgl_set_uniform_mat4(&bs->ortho_col_shader, "u_model_view_mat", mv);

	glDrawArrays(GL_TRIANGLE_FAN, 0, ZGL_BS_MAX_VERTEX_COUNT);
}

ZGLDEF void zgl_bs_draw_point(ZGLBasicState *bs, f32 mv[16], f32 x, f32 y, ZGLColor color) {

	bs->vertices[0].x = x;
	bs->vertices[0].y = y;

	zgl__bs_setup_ortho_colour_state(bs, ZGL_BS_MAX_VERTEX_COUNT, color);
	zgl_set_uniform_mat4(&bs->ortho_col_shader, "u_model_view_mat", mv);

	glDrawArrays(GL_POINTS, 0, 1);
}

ZGLDEF void zgl_bs_draw_line(ZGLBasicState *bs, f32 mv[16], f32 x0, f32 y0, f32 x1, f32 y1, ZGLColor color) {

	bs->vertices[0].x = x0;
	bs->vertices[0].y = y0;
	bs->vertices[1].x = x1;
	bs->vertices[1].y = y1;

	zgl__bs_setup_ortho_colour_state(bs, ZGL_BS_MAX_VERTEX_COUNT, color);
	zgl_set_uniform_mat4(&bs->ortho_col_shader, "u_model_view_mat", mv);

	glDrawArrays(GL_LINES, 0, 2);
}

ZGLDEF void zgl_bs_draw_quad(ZGLBasicState *bs,
		f32 mv[16],
		f32 x0, f32 y0,
		f32 x1, f32 y1,
		f32 x2, f32 y2,
		f32 x3, f32 y3,
		ZGLColor col) {

	bs->vertices[0].x = x0;
	bs->vertices[0].y = y0;

	bs->vertices[1].x = x1;
	bs->vertices[1].y = y1;

	bs->vertices[2].x = x2;
	bs->vertices[2].y = y2;

	bs->vertices[3].x = x3;
	bs->vertices[3].y = y3;

	zgl__bs_setup_ortho_colour_state(bs, 4, col);
	zgl_set_uniform_mat4(&bs->ortho_col_shader, "u_model_view_mat", mv);

	glDrawElements(GL_LINE_STRIP, 6, GL_UNSIGNED_SHORT, NULL);

}

ZGLDEF void zgl_bs_draw_rect(ZGLBasicState *bs, f32 mv[16], f32 x, f32 y, f32 w, f32 h, ZGLColor col) {
	zgl_bs_draw_quad(bs, mv, x, y, x+w, y, x+w, y+h, x, y+h, col);
}

ZGLDEF void zgl_bs_fill_quad(ZGLBasicState *bs,
		f32 mv[16],
		f32 x0, f32 y0,
		f32 x1, f32 y1,
		f32 x2, f32 y2,
		f32 x3, f32 y3,
		ZGLColor col) {

	bs->vertices[0].x = x0;
	bs->vertices[0].y = y0;

	bs->vertices[1].x = x1;
	bs->vertices[1].y = y1;

	bs->vertices[2].x = x2;
	bs->vertices[2].y = y2;

	bs->vertices[3].x = x3;
	bs->vertices[3].y = y3;

	zgl__bs_setup_ortho_colour_state(bs, 4, col);
	zgl_set_uniform_mat4(&bs->ortho_col_shader, "u_model_view_mat", mv);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);

}

ZGLDEF void zgl_bs_fill_rect(ZGLBasicState *bs, f32 mv[16], f32 x, f32 y, f32 w, f32 h, ZGLColor col) {
	zgl_bs_fill_quad(bs, mv, x, y, x+w, y, x+w, y+h, x, y+h, col);
}

ZGLDEF void zgl_bs_draw_textured_rect(ZGLBasicState *bs, ZGLTexture *tex, f32 mv[16], f32 x, f32 y, f32 w, f32 h, b32 v_up) {
	bs->vertices[0].x = x;
	bs->vertices[0].y = y;
	bs->vertices[0].u = 0.0f;
	bs->vertices[0].v = v_up ? 0.0f : 1.0f;

	bs->vertices[1].x = x + w;
	bs->vertices[1].y = y;
	bs->vertices[1].u = 1.0f;
	bs->vertices[1].v = v_up ? 0.0f : 1.0f;

	bs->vertices[2].x = x + w;
	bs->vertices[2].y = y + h;
	bs->vertices[2].u = 1.0f;
	bs->vertices[2].v = v_up ? 1.0f : 0.0f;

	bs->vertices[3].x = x;
	bs->vertices[3].y = y + h;
	bs->vertices[3].u = 0.0f;
	bs->vertices[3].v = v_up ? 1.0f : 0.0f;

	zgl_use_shader(&bs->ortho_tex_shader);
	zgl_set_uniform_mat4(&bs->ortho_tex_shader, "u_ortho_mat", bs->ortho_mat);
	zgl_set_uniform_mat4(&bs->ortho_tex_shader, "u_model_view_mat", mv);
	zgl_bind_texture2d(tex, 0, bs->mipmap_sampler);

	zgl_vbo_copy(bs->vbo, bs->vertices, 4*zen_sizeof(bs->vertices[0]), 0);

	zgl_vert_ptr_aa(0, 2, ZGLBasicVertex, x);
	zgl_vert_ptr_aa(1, 2, ZGLBasicVertex, u);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bs->ebo);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
}

ZGLDEF void zgl_bs_draw_textured_subrect(
		ZGLBasicState *bs, ZGLTexture *tex, f32 mv[16], 
		f32 x0, f32 y0, f32 x1, f32 y1, 
		f32 u0, f32 v0, f32 u1, f32 v1,b32 v_up) {

	bs->vertices[0].x = x0;
	bs->vertices[0].y = y0;
	bs->vertices[0].u = u0;
	bs->vertices[0].v = v_up ? v0 : v1;

	bs->vertices[1].x = x1;
	bs->vertices[1].y = y0;
	bs->vertices[1].u = u1;
	bs->vertices[1].v = v_up ? v0 : v1;

	bs->vertices[2].x = x1;
	bs->vertices[2].y = y1;
	bs->vertices[2].u = u1;
	bs->vertices[2].v = v_up ? v1 : v0;

	bs->vertices[3].x = x0;
	bs->vertices[3].y = y1;
	bs->vertices[3].u = u0;
	bs->vertices[3].v = v_up ? v1 : v0;

	zgl_use_shader(&bs->ortho_tex_shader);
	zgl_set_uniform_mat4(&bs->ortho_tex_shader, "u_ortho_mat", bs->ortho_mat);
	zgl_set_uniform_mat4(&bs->ortho_col_shader, "u_model_view_mat", mv);
	zgl_bind_texture2d(tex, 0, bs->mipmap_sampler);

	zgl_vbo_copy(bs->vbo, bs->vertices, 4*zen_sizeof(bs->vertices[0]), 0);

	zgl_vert_ptr_aa(0, 2, ZGLBasicVertex, x);
	zgl_vert_ptr_aa(1, 2, ZGLBasicVertex, u);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bs->ebo);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
}

static void zgl_bs_draw_font(ZGLBasicState *bs, stbtt_aligned_quad *q, b32 up){

	bs->vertices[0].x = q->x0;
	bs->vertices[0].y = q->y0;
	bs->vertices[0].u = q->s0;
	bs->vertices[0].v = up ? q->t0 : q->t1;

	bs->vertices[1].x = q->x1;
	bs->vertices[1].y = q->y0;
	bs->vertices[1].u = q->s1;
	bs->vertices[1].v = up ? q->t0 : q->t1;

	bs->vertices[2].x = q->x1;
	bs->vertices[2].y = q->y1;
	bs->vertices[2].u = q->s1;
	bs->vertices[2].v = up ? q->t1 : q->t0;

	bs->vertices[3].x = q->x0;
	bs->vertices[3].y = q->y1;
	bs->vertices[3].u = q->s0;
	bs->vertices[3].v = up ? q->t1 : q->t0;

	zgl_use_shader(&bs->ortho_font_shader);
	zgl_set_uniform_mat4(&bs->ortho_font_shader, "u_ortho_mat", bs->ortho_mat);
	zgl_bind_texture2d(&bs->font.font_texture, 0, bs->mipmap_sampler);

	zgl_vbo_copy(bs->vbo, bs->vertices, 4*zen_sizeof(bs->vertices[0]), 0);

	zgl_vert_ptr_aa(0, 2, ZGLBasicVertex, x);
	zgl_vert_ptr_aa(1, 2, ZGLBasicVertex, u);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bs->ebo);

	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
}

ZGLDEF b32 zgl_load_font(ZGLBasicState *bs, const char *filename, f32 size_in_pixels) {

	static unsigned char ttf_buffer[1<<20];
	static unsigned char temp_bitmap[512*512];
	FILE* file = fopen(filename, "rb");
	if(!file) {
		printf("error reading file\n");
		return false;
	}
   usize size = fread(ttf_buffer, 1, 1<<20, fopen(filename, "rb"));
	if(ferror(file) != 0) {
		printf("error reading file\n");
		return false;
	}
	ttf_buffer[size+1] = '\0';

	bs->font.atlas_width = 512;
	bs->font.atlas_height = 512;
	bs->font.char_start = 32;
	bs->font.char_count = 96;
	bs->font.type_flag = 1; // 1 = OpenGL
	bs->font.size_in_pixels = size_in_pixels;
   stbtt_BakeFontBitmap(ttf_buffer, 0, 
			bs->font.size_in_pixels, temp_bitmap, bs->font.atlas_width, bs->font.atlas_height, 
			bs->font.char_start, bs->font.char_count, bs->font.cdata);
	fclose(file);

	assert(zgl_load_texture2d_from_memory(&bs->font.font_texture, temp_bitmap, bs->font.atlas_width, bs->font.atlas_height, bs->font.type_flag));

	return true;

}

ZGLDEF void zgl_draw_string(ZGLBasicState *bs, const char* text, f32 *x, f32 *y, ZGLColor color) {

	zgl_use_shader(&bs->ortho_font_shader);
	stbtt_aligned_quad q;
	isize len = strlen(text);
	zgl_set_uniform_colour(&bs->ortho_font_shader, "u_color", color);
	for(isize i = 0; i < len; ++i) {
		stbtt_GetBakedQuad(
				bs->font.cdata, bs->font.atlas_width, bs->font.atlas_height, 
				text[i] - bs->font.char_start, x, y, &q, bs->font.type_flag
		);

		zgl_bs_draw_font(bs, &q, true);
	}

}

static void zgl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char *message, const void*userParam) {

	printf("ERROR: ");
	switch(source) {
		case GL_DEBUG_SOURCE_API: printf("GL_DEBUG_SOURCE_API"); break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: printf("GL_DEBUG_SOURCE_SHADER_COMPILER_ARB"); break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: printf("GL_DEBUG_SOURCE_WINDOW_SYSTEM"); break;
		case GL_DEBUG_SOURCE_THIRD_PARTY: printf("GL_DEBUG_SOURCE_THIRD_PARTY"); break;
		case GL_DEBUG_SOURCE_APPLICATION: printf("GL_DEBUG_SOURCE_APPLICATION"); break;
		case GL_DEBUG_SOURCE_OTHER: printf("GL_DEBUG_SOURCE_OTHER"); break;
		default: printf("UNKNOWN_DEBUG_SOURCE"); break;
	}

	printf(" TYPE: ");
	switch (type) {
		case GL_DEBUG_TYPE_ERROR: printf("GL_DEBUG_TYPE_ERROR"); break;
		case GL_DEBUG_TYPE_MARKER: printf("GL_DEBUG_TYPE_MARKER"); break;
		case GL_DEBUG_TYPE_OTHER: printf("GL_DEBUG_TYPE_OTHER"); break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: printf("GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR"); break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: printf("GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR"); break;
		case GL_DEBUG_TYPE_PERFORMANCE: printf("GL_DEBUG_TYPE_PERFORMANCE"); break;
		case GL_DEBUG_TYPE_PORTABILITY: printf("GL_DEBUG_TYPE_PORTABILITY"); break;
		case GL_DEBUG_TYPE_PUSH_GROUP: printf("GL_DEBUG_TYPE_PUSH_GROUP"); break;
		case GL_DEBUG_TYPE_POP_GROUP: printf("GL_DEBUG_TYPE_POP_GROUP"); break;
	}

	printf(" ID: %d", id);

	printf(" SEVERITY: ");
	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH: printf("GL_DEBUG_SEVERITY_HIGH"); break;
		case GL_DEBUG_SEVERITY_MEDIUM: printf("GL_DEBUG_SEVERITY_MEDIUM"); break;
		case GL_DEBUG_SEVERITY_LOW: printf("GL_DEBUG_SEVERITY_LOW"); break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: printf("GL_DEBUG_SEVERITY_NOTIFICATION"); break;
	}

	printf(" -> %s\n", message);

}

ZGLDEF void zgl_bs_initialize(ZGLBasicState *bs, int window_width, int window_height) {

	memset(bs, 0, zen_sizeof(ZGLBasicState));

	// turn on debugging
	glEnable(GL_DEBUG_OUTPUT); 
	glDebugMessageCallback(zgl_error_callback, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, GL_FALSE);

	zgl_bs_set_resolution(bs, window_width, window_height);
	glGenVertexArrays(1, &bs->vao);
	glBindVertexArray(bs->vao);

	bs->vbo = zgl_make_vbo(NULL, zen_sizeof(ZGLBasicVertex) * ZGL_BS_MAX_VERTEX_COUNT, GL_DYNAMIC_DRAW);

	for (int i = 0; i < ZGL_BS_MAX_INDEX_COUNT / 6; i++) {
		bs->indices[i*6 + 0] = i*4 + 0;
		bs->indices[i*6 + 1] = i*4 + 1;
		bs->indices[i*6 + 2] = i*4 + 2;
		bs->indices[i*6 + 3] = i*4 + 2;
		bs->indices[i*6 + 4] = i*4 + 3;
		bs->indices[i*6 + 5] = i*4 + 0;
	}
	bs->ebo = zgl_make_ebo(bs->indices, zen_sizeof(u16) * ZGL_BS_MAX_INDEX_COUNT, GL_DYNAMIC_DRAW);

	bs->nearest_sampler = zgl_make_sampler(GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	bs->linear_sampler  = zgl_make_sampler(GL_LINEAR,  GL_LINEAR,  GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	bs->mipmap_sampler  = zgl_make_sampler(GL_LINEAR_MIPMAP_LINEAR,  GL_LINEAR,  GL_REPEAT, GL_REPEAT);

	//TODO(tim): Need support for filled objects, line objects
	//TODO(tim): Need support for textured quads
	//TODO(tim): Need to get fonts working for real

	ZGLShaderError result = zgl_create_shader(&bs->ortho_col_shader, 
		"#version 420 core\n"
		"precision mediump float;"
		"layout (location = 0) in vec4 a_position;\n"
		"uniform mat4 u_ortho_mat;\n"
		"uniform mat4 u_model_view_mat;\n"
		"void main(void) {\n"
		"	gl_Position = u_ortho_mat * u_model_view_mat * a_position;\n"
		"}\n",

		"#version 420 core\n"
		"uniform vec4 u_colour;\n"
		"out vec4 o_colour;\n"
		"void main(void) {\n"
		"	o_colour = u_colour;\n"
		"}\n");

	if (result != ZGL_ERROR_NONE) {
		exit(EXIT_FAILURE);
	}

	result =	zgl_create_shader(&bs->ortho_tex_shader,
		"#version 420 core\n"
		"layout (location = 0) in vec4 a_position;\n"
		"layout (location = 1) in vec2 a_tex_coord;\n"
		"uniform mat4 u_ortho_mat;\n"
		"uniform mat4 u_model_view_mat;\n"
		"out vec2 v_tex_coord;\n"
		"void main(void) {\n"
		"	gl_Position = u_ortho_mat * u_model_view_mat * a_position;\n"
		"	v_tex_coord = a_tex_coord;\n"
		"}\n",

		"#version 420 core\n"
		"precision mediump float;"
		"in vec2 v_tex_coord;\n"
		"layout (binding = 0) uniform sampler2D u_tex;\n"
		"out vec4 o_colour;\n"
		"void main(void) {\n"
		"	o_colour = texture2D(u_tex, v_tex_coord);\n"
		"}\n"
	);

	result =	zgl_create_shader(&bs->ortho_font_shader,
		"#version 420 core\n"
		"layout (location = 0) in vec4 a_position;\n"
		"layout (location = 1) in vec2 a_tex_coord;\n"
		"uniform mat4 u_ortho_mat;\n"
		"out vec2 v_tex_coord;\n"
		"void main(void) {\n"
		"	gl_Position = u_ortho_mat * a_position;\n"
		"	v_tex_coord = a_tex_coord;\n"
		"}\n",

		"#version 420 core\n"
		"precision mediump float;"
		"in vec2 v_tex_coord;\n"
		"layout (binding = 0) uniform sampler2D u_tex;\n"
		"uniform vec4 u_color;\n"
		"out vec4 o_color;\n"
		"void main(void) {\n"
		"	o_color = u_color * texture2D(u_tex, v_tex_coord).r;\n"
		"}\n"
	);

	if (result != ZGL_ERROR_NONE) {
		exit(EXIT_FAILURE);
	}

}

#endif // ZEN_GL_IMPLEMENTATION

// Public Domain (www.unlicense.org)
// This is free and unencumbered software released into the public domain.
// Anyone is free to copy, modify, publish, use, compile, sell, or distribute this 
// software, either in source code form or as a compiled binary, for any purpose, 
// commercial or non-commercial, and by any means.
// In jurisdictions that recognize copyright laws, the author or authors of this 
// software dedicate any and all copyright interest in the software to the public 
// domain. We make this dedication for the benefit of the public at large and to 
// the detriment of our heirs and successors. We intend this dedication to be an 
// overt act of relinquishment in perpetuity of all present and future rights to 
// this software under copyright law.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
// AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN 
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
