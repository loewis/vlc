/*****************************************************************************
 * converter.h: OpenGL internal header
 *****************************************************************************
 * Copyright (C) 2016 VLC authors and VideoLAN
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef VLC_OPENGL_CONVERTER_H
#define VLC_OPENGL_CONVERTER_H

#ifdef HAVE_LIBPLACEBO
#include <libplacebo/shaders.h>
#endif

#include "vout_helper.h"
#include <vlc_plugin.h>

#define VLCGL_PICTURE_MAX 128

#ifndef GL_TEXTURE_RECTANGLE
# define GL_TEXTURE_RECTANGLE 0x84F5
#endif

#ifndef APIENTRY
# define APIENTRY
#endif

/* Core OpenGL/OpenGLES functions: the following functions pointers typedefs
 * are not defined. */
#if !defined(_WIN32) /* Already defined on Win32 */
typedef void (*PFNGLACTIVETEXTUREPROC) (GLenum texture);
#endif
typedef GLenum (APIENTRY *PFNGLGETERRORPROC) (void);
typedef const GLubyte *(APIENTRY *PFNGLGETSTRINGPROC) (GLenum name);
typedef void (APIENTRY *PFNGLGETINTEGERVPROC) (GLenum pname, GLint *data);
typedef void (APIENTRY *PFNGLBINDTEXTUREPROC) (GLenum target, GLuint texture);
typedef void (APIENTRY *PFNGLTEXPARAMETERIPROC) (GLenum target, GLenum pname, GLint param);
typedef void (APIENTRY *PFNGLTEXPARAMETERFPROC) (GLenum target, GLenum pname, GLfloat param);
typedef void (APIENTRY *PFNGLGENTEXTURESPROC) (GLsizei n, GLuint *textures);
typedef void (APIENTRY *PFNGLDELETETEXTURESPROC) (GLsizei n, const GLuint *textures);
typedef void (APIENTRY *PFNGLTEXIMAGE2DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (APIENTRY *PFNGLTEXSUBIMAGE2DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
typedef void (APIENTRY *PFNGLGETTEXLEVELPARAMETERIVPROC) (GLenum target, GLint level, GLenum pname, GLint *params);
typedef void (APIENTRY *PFNGLPIXELSTOREIPROC) (GLenum pname, GLint param);
typedef void (APIENTRY *PFNGLBUFFERSTORAGEPROC) (GLenum target, GLsizeiptr size, const void *data, GLbitfield flags);

/* The following are defined in glext.h but not for GLES2 or on Apple systems */
#if defined(USE_OPENGL_ES2) || defined(__APPLE__)
#   define PFNGLGETPROGRAMIVPROC             typeof(glGetProgramiv)*
#   define PFNGLGETPROGRAMINFOLOGPROC        typeof(glGetProgramInfoLog)*
#   define PFNGLGETSHADERIVPROC              typeof(glGetShaderiv)*
#   define PFNGLGETSHADERINFOLOGPROC         typeof(glGetShaderInfoLog)*
#   define PFNGLGETUNIFORMLOCATIONPROC       typeof(glGetUniformLocation)*
#   define PFNGLGETATTRIBLOCATIONPROC        typeof(glGetAttribLocation)*
#   define PFNGLVERTEXATTRIBPOINTERPROC      typeof(glVertexAttribPointer)*
#   define PFNGLENABLEVERTEXATTRIBARRAYPROC  typeof(glEnableVertexAttribArray)*
#   define PFNGLUNIFORMMATRIX4FVPROC         typeof(glUniformMatrix4fv)*
#   define PFNGLUNIFORMMATRIX3FVPROC         typeof(glUniformMatrix3fv)*
#   define PFNGLUNIFORMMATRIX2FVPROC         typeof(glUniformMatrix2fv)*
#   define PFNGLUNIFORM4FVPROC               typeof(glUniform4fv)*
#   define PFNGLUNIFORM4FPROC                typeof(glUniform4f)*
#   define PFNGLUNIFORM3FPROC                typeof(glUniform3f)*
#   define PFNGLUNIFORM2FPROC                typeof(glUniform2f)*
#   define PFNGLUNIFORM1FPROC                typeof(glUniform1f)*
#   define PFNGLUNIFORM1IPROC                typeof(glUniform1i)*
#   define PFNGLCREATESHADERPROC             typeof(glCreateShader)*
#   define PFNGLSHADERSOURCEPROC             typeof(glShaderSource)*
#   define PFNGLCOMPILESHADERPROC            typeof(glCompileShader)*
#   define PFNGLDELETESHADERPROC             typeof(glDeleteShader)*
#   define PFNGLCREATEPROGRAMPROC            typeof(glCreateProgram)*
#   define PFNGLLINKPROGRAMPROC              typeof(glLinkProgram)*
#   define PFNGLUSEPROGRAMPROC               typeof(glUseProgram)*
#   define PFNGLDELETEPROGRAMPROC            typeof(glDeleteProgram)*
#   define PFNGLATTACHSHADERPROC             typeof(glAttachShader)*
#   define PFNGLGENBUFFERSPROC               typeof(glGenBuffers)*
#   define PFNGLBINDBUFFERPROC               typeof(glBindBuffer)*
#   define PFNGLBUFFERDATAPROC               typeof(glBufferData)*
#   define PFNGLBUFFERSUBDATAPROC            typeof(glBufferSubData)*
#   define PFNGLDELETEBUFFERSPROC            typeof(glDeleteBuffers)*
#if defined(__APPLE__)
#   import <CoreFoundation/CoreFoundation.h>
#endif
#endif

/* The following are defined in glext.h but doesn't exist in GLES2 */
#if defined(USE_OPENGL_ES2) || defined(__APPLE__)
typedef struct __GLsync *GLsync;
typedef uint64_t GLuint64;
typedef void *(APIENTRY *PFNGLMAPBUFFERRANGEPROC) (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef void (APIENTRY *PFNGLFLUSHMAPPEDBUFFERRANGEPROC) (GLenum target, GLintptr offset, GLsizeiptr length);
typedef GLboolean (APIENTRY *PFNGLUNMAPBUFFERPROC) (GLenum target);
typedef GLsync (APIENTRY *PFNGLFENCESYNCPROC) (GLenum condition, GLbitfield flags);
typedef void (APIENTRY *PFNGLDELETESYNCPROC) (GLsync sync);
typedef GLenum (APIENTRY *PFNGLCLIENTWAITSYNCPROC) (GLsync sync, GLbitfield flags, GLuint64 timeout);
#endif

/**
 * Structure containing function pointers to shaders commands
 */
typedef struct {
    /*
     * GL / GLES core functions
     */
    PFNGLGETERRORPROC       GetError;
    PFNGLGETSTRINGPROC      GetString;
    PFNGLGETINTEGERVPROC    GetIntegerv;
    PFNGLBINDTEXTUREPROC    BindTexture;
    PFNGLTEXPARAMETERIPROC  TexParameteri;
    PFNGLTEXPARAMETERFPROC  TexParameterf;
    PFNGLPIXELSTOREIPROC    PixelStorei;
    PFNGLGENTEXTURESPROC    GenTextures;
    PFNGLDELETETEXTURESPROC DeleteTextures;
    PFNGLTEXIMAGE2DPROC     TexImage2D;
    PFNGLTEXSUBIMAGE2DPROC  TexSubImage2D;

    /* GL only core functions: NULL for GLES2 */
    PFNGLGETTEXLEVELPARAMETERIVPROC GetTexLevelParameteriv; /* Can be NULL */

    /*
     * GL / GLES extensions
     */

    /* Shader commands */
    PFNGLCREATESHADERPROC   CreateShader;
    PFNGLSHADERSOURCEPROC   ShaderSource;
    PFNGLCOMPILESHADERPROC  CompileShader;
    PFNGLATTACHSHADERPROC   AttachShader;
    PFNGLDELETESHADERPROC   DeleteShader;

    /* Shader log commands */
    PFNGLGETPROGRAMIVPROC       GetProgramiv;
    PFNGLGETSHADERIVPROC        GetShaderiv;
    PFNGLGETPROGRAMINFOLOGPROC  GetProgramInfoLog;
    PFNGLGETSHADERINFOLOGPROC   GetShaderInfoLog;

    /* Shader variables commands */
    PFNGLGETUNIFORMLOCATIONPROC      GetUniformLocation;
    PFNGLGETATTRIBLOCATIONPROC       GetAttribLocation;
    PFNGLVERTEXATTRIBPOINTERPROC     VertexAttribPointer;
    PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray;
    PFNGLUNIFORMMATRIX4FVPROC        UniformMatrix4fv;
    PFNGLUNIFORMMATRIX3FVPROC        UniformMatrix3fv;
    PFNGLUNIFORMMATRIX2FVPROC        UniformMatrix2fv;
    PFNGLUNIFORM4FVPROC              Uniform4fv;
    PFNGLUNIFORM4FPROC               Uniform4f;
    PFNGLUNIFORM3FPROC               Uniform3f;
    PFNGLUNIFORM2FPROC               Uniform2f;
    PFNGLUNIFORM1FPROC               Uniform1f;
    PFNGLUNIFORM1IPROC               Uniform1i;

    /* Program commands */
    PFNGLCREATEPROGRAMPROC CreateProgram;
    PFNGLLINKPROGRAMPROC   LinkProgram;
    PFNGLUSEPROGRAMPROC    UseProgram;
    PFNGLDELETEPROGRAMPROC DeleteProgram;

    /* Texture commands */
    PFNGLACTIVETEXTUREPROC ActiveTexture;

    /* Buffers commands */
    PFNGLGENBUFFERSPROC    GenBuffers;
    PFNGLBINDBUFFERPROC    BindBuffer;
    PFNGLBUFFERDATAPROC    BufferData;
    PFNGLDELETEBUFFERSPROC DeleteBuffers;

    /* Commands used for PBO and/or Persistent mapping */
    PFNGLBUFFERSUBDATAPROC          BufferSubData; /* can be NULL */
    PFNGLBUFFERSTORAGEPROC          BufferStorage; /* can be NULL */
    PFNGLMAPBUFFERRANGEPROC         MapBufferRange; /* can be NULL */
    PFNGLFLUSHMAPPEDBUFFERRANGEPROC FlushMappedBufferRange; /* can be NULL */
    PFNGLUNMAPBUFFERPROC            UnmapBuffer; /* can be NULL */
    PFNGLFENCESYNCPROC              FenceSync; /* can be NULL */
    PFNGLDELETESYNCPROC             DeleteSync; /* can be NULL */
    PFNGLCLIENTWAITSYNCPROC         ClientWaitSync; /* can be NULL */
} opengl_vtable_t;

/*
 * Structure that is filled by "glhw converter" module probe function
 * The implementation should initialize every members of the struct that are
 * not set by the caller
 */
typedef struct opengl_tex_converter_t opengl_tex_converter_t;
struct opengl_tex_converter_t
{
    VLC_COMMON_MEMBERS

    module_t *p_module;

    /* Pointer to object gl, set by the caller */
    vlc_gl_t *gl;

#ifdef HAVE_LIBPLACEBO
    /* libplacebo context, created by the caller (optional) */
    struct pl_context *pl_ctx;
#endif

    /* Function pointers to OpenGL functions, set by the caller */
    const opengl_vtable_t *vt;

    /* True to dump shaders, set by the caller */
    bool b_dump_shaders;

    /* Function pointer to the shader init command, set by the caller, see
     * opengl_fragment_shader_init() documentation. */
    GLuint (*pf_fragment_shader_init)(opengl_tex_converter_t *, GLenum,
                                      vlc_fourcc_t, video_color_space_t);

    /* Available gl extensions (from GL_EXTENSIONS) */
    const char *glexts;

    /* True if the current API is OpenGL ES, set by the caller */
    bool is_gles;
    /* GLSL version, set by the caller. 100 for GLSL ES, 120 for desktop GLSL */
    unsigned glsl_version;
    /* Precision header, set by the caller. In OpenGLES, the fragment language
     * has no default precision qualifier for floating point types. */
    const char *glsl_precision_header;

    /* Can only be changed from the module open function */
    video_format_t fmt;

    /* Fragment shader, must be set from the module open function. It will be
     * deleted by the caller. */
    GLuint fshader;

    /* Number of textures, cannot be 0 */
    unsigned tex_count;

    /* Texture mapping (usually: GL_TEXTURE_2D), cannot be 0 */
    GLenum tex_target;

    /* Set to true if textures are generated from pf_update() */
    bool handle_texs_gen;

    struct opengl_tex_cfg {
        /* Texture scale factor, cannot be 0 */
        vlc_rational_t w;
        vlc_rational_t h;

        /* The following is used and filled by the opengl_fragment_shader_init
         * function. */
        GLint  internal;
        GLenum format;
        GLenum type;
    } texs[PICTURE_PLANE_MAX];

    /* The following is used and filled by the opengl_fragment_shader_init
     * function. */
    struct {
        GLint Texture[PICTURE_PLANE_MAX];
        GLint TexSize[PICTURE_PLANE_MAX]; /* for GL_TEXTURE_RECTANGLE */
        GLint Coefficients;
        GLint FillColor;
        GLint *pl_vars; /* for pl_sh_res */
    } uloc;
    bool yuv_color;
    GLfloat yuv_coefficients[16];

#ifdef HAVE_LIBPLACEBO
    struct pl_shader *pl_sh;
    const struct pl_shader_res *pl_sh_res;
#endif

    /* Private context */
    void *priv;

    /*
     * Callback to allocate data for bound textures
     *
     * This function pointer can be NULL. Software converters should call
     * glTexImage2D() to allocate textures data (it will be deallocated by the
     * caller when calling glDeleteTextures()). Won't be called if
     * handle_texs_gen is true.
     *
     * \param fc OpenGL tex converter
     * \param textures array of textures to bind (one per plane)
     * \param tex_width array of tex width (one per plane)
     * \param tex_height array of tex height (one per plane)
     * \return VLC_SUCCESS or a VLC error
     */
    int (*pf_allocate_textures)(const opengl_tex_converter_t *tc, GLuint *textures,
                                const GLsizei *tex_width, const GLsizei *tex_height);

    /*
     * Callback to allocate a picture pool
     *
     * This function pointer *can* be NULL. If NULL, A generic pool with
     * pictures allocated from the video_format_t will be used.
     *
     * \param fc OpenGL tex converter
     * \param requested_count number of pictures to allocate
     * \return the picture pool or NULL in case of error
     */
    picture_pool_t *(*pf_get_pool)(const opengl_tex_converter_t *fc,
                                   unsigned requested_count);

    /*
     * Callback to update a picture
     *
     * This function pointer cannot be NULL. The implementation should upload
     * every planes of the picture.
     *
     * \param fc OpenGL tex converter
     * \param textures array of textures to bind (one per plane)
     * \param tex_width array of tex width (one per plane)
     * \param tex_height array of tex height (one per plane)
     * \param pic picture to update
     * \param plane_offset offsets of each picture planes to read data from
     * (one per plane, can be NULL)
     * \return VLC_SUCCESS or a VLC error
     */
    int (*pf_update)(const opengl_tex_converter_t *fc, GLuint *textures,
                     const GLsizei *tex_width, const GLsizei *tex_height,
                     picture_t *pic, const size_t *plane_offset);

    /*
     * Callback to fetch locations of uniform or attributes variables
     *
     * This function pointer cannot be NULL. This callback is called one time
     * after init.
     *
     * \param fc OpenGL tex converter
     * \param program linked program that will be used by this tex converter
     * \return VLC_SUCCESS or a VLC error
     */
    int (*pf_fetch_locations)(opengl_tex_converter_t *fc, GLuint program);

    /*
     * Callback to prepare the fragment shader
     *
     * This function pointer cannot be NULL. This callback can be used to
     * specify values of uniform variables.
     *
     * \param fc OpenGL tex converter
     * \param tex_width array of tex width (one per plane)
     * \param tex_height array of tex height (one per plane)
     * \param alpha alpha value, used only for RGBA fragment shader
     */
    void (*pf_prepare_shader)(const opengl_tex_converter_t *fc,
                              const GLsizei *tex_width, const GLsizei *tex_height,
                              float alpha);
};

/*
 * Generate a fragment shader
 *
 * This utility function can be used by hw opengl tex converters that need a
 * generic fragment shader. It will compile a fragment shader generated from
 * the chroma and the tex target. This will initialize all elements of the
 * opengl_tex_converter_t struct except for priv, pf_allocate_texture,
 * pf_get_pool, pf_update
 *
 * \param tc OpenGL tex converter
 * \param tex_target GL_TEXTURE_2D or GL_TEXTURE_RECTANGLE
 * \param chroma chroma used to generate the fragment shader
 * \param if not COLOR_SPACE_UNDEF, YUV planes will be converted to RGB
 * according to the color space
 * \return the compiled fragment shader or 0 in case of error
 */
static inline GLuint
opengl_fragment_shader_init(opengl_tex_converter_t *tc, GLenum tex_target,
                            vlc_fourcc_t chroma, video_color_space_t yuv_space)
{
    return tc->pf_fragment_shader_init(tc, tex_target, chroma, yuv_space);
}

#endif /* include-guard */
