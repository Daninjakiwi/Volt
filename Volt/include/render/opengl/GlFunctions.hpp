#pragma once

#ifdef BUILD_WINDOWS
#pragma once
#include <platform/windows/VoltWin.hpp>
#include <gl/GL.h>
#include <platform/windows/glext.h>
#include <platform/windows/wglext.h>

/*

Vertex Array functions

*/

typedef void (APIENTRYP PFNGLCREATEVERTEXARRAYSPROC)(GLsizei n, GLuint* buffers);
GLAPI PFNGLCREATEVERTEXARRAYSPROC volt_glCreateVertexArrays;
#define glCreateVertexArrays volt_glCreateVertexArrays

GLAPI PFNGLGENVERTEXARRAYSPROC volt_glGenVertexArrays;
#define glGenVertexArrays volt_glGenVertexArrays

typedef void (APIENTRYP PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n, const GLuint* arrays);
GLAPI PFNGLDELETEVERTEXARRAYSPROC volt_glDeleteVertexArrays;
#define glDeleteVertexArrays volt_glDeleteVertexArrays

typedef void (APIENTRYP PFNGLBINDVERTEXARRAYPROC)(GLuint array);
GLAPI PFNGLBINDVERTEXARRAYPROC volt_glBindVertexArray;
#define glBindVertexArray volt_glBindVertexArray

typedef void (APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
GLAPI PFNGLENABLEVERTEXATTRIBARRAYPROC volt_glEnableVertexAttribArray;
#define glEnableVertexAttribArray volt_glEnableVertexAttribArray

typedef void (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
GLAPI PFNGLVERTEXATTRIBPOINTERPROC volt_glVertexAttribPointer;
#define glVertexAttribPointer volt_glVertexAttribPointer

/*

Framebuffer functions

*/

GLAPI PFNGLGENFRAMEBUFFERSPROC volt_glGenFramebuffers;
#define glGenFramebuffers volt_glGenFramebuffers

GLAPI PFNGLGENRENDERBUFFERSPROC volt_glGenRenderbuffers;
#define glGenRenderbuffers volt_glGenRenderbuffers

GLAPI PFNGLCREATEFRAMEBUFFERSPROC volt_glCreateFramebuffers;
#define glCreateFramebuffers volt_glCreateFramebuffers

GLAPI PFNGLDELETEFRAMEBUFFERSPROC volt_glDeleteFramebuffers;
#define glDeleteFramebuffers volt_glDeleteFramebuffers

GLAPI PFNGLBINDFRAMEBUFFERPROC volt_glBindFramebuffer;
#define glBindFramebuffer volt_glBindFramebuffer

GLAPI PFNGLFRAMEBUFFERTEXTURE2DPROC volt_glFramebufferTexture2D;
#define glFramebufferTexture2D volt_glFramebufferTexture2D

GLAPI PFNGLCREATERENDERBUFFERSPROC volt_glCreateRenderbuffers;
#define glCreateRenderbuffers volt_glCreateRenderbuffers

GLAPI PFNGLDELETEFRAMEBUFFERSPROC volt_glDeleteRenderbuffers;
#define glDeleteRenderbuffers volt_glDeleteRenderbuffers

GLAPI PFNGLBINDRENDERBUFFERPROC volt_glBindRenderbuffer;
#define glBindRenderbuffer volt_glBindRenderbuffer

GLAPI PFNGLRENDERBUFFERSTORAGEPROC volt_glRenderbufferStorage;
#define glRenderbufferStorage volt_glRenderbufferStorage

GLAPI PFNGLFRAMEBUFFERRENDERBUFFERPROC volt_glFramebufferRenderbuffer;
#define glFramebufferRenderbuffer volt_glFramebufferRenderbuffer

GLAPI PFNGLCHECKFRAMEBUFFERSTATUSPROC volt_glCheckFramebufferStatus;
#define glCheckFramebufferStatus volt_glCheckFramebufferStatus

/*

Buffer functions

*/

typedef void (APIENTRYP PFNGLCREATEBUFFERSPROC)(GLsizei n, GLuint* buffers);
GLAPI PFNGLCREATEBUFFERSPROC volt_glCreateBuffers;
#define glCreateBuffers volt_glCreateBuffers

typedef void (APIENTRYP PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint* buffers);
GLAPI PFNGLDELETEBUFFERSPROC volt_glDeleteBuffers;
#define glDeleteBuffers volt_glDeleteBuffers

typedef void (APIENTRYP PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
GLAPI PFNGLBINDBUFFERPROC volt_glBindBuffer;
#define glBindBuffer volt_glBindBuffer

GLAPI PFNGLGENBUFFERSPROC volt_glGenBuffers;
#define glGenBuffers volt_glGenBuffers

/*

Buffer data functions

*/

typedef void (APIENTRYP PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
GLAPI PFNGLBUFFERDATAPROC volt_glBufferData;
#define glBufferData volt_glBufferData

typedef void (APIENTRYP PFNGLBUFFERSUBDATAPROC)(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data);
GLAPI PFNGLBUFFERSUBDATAPROC volt_glBufferSubData;
#define glBufferSubData volt_glBufferSubData

typedef void* (APIENTRYP PFNGLMAPBUFFERRANGEPROC)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
GLAPI PFNGLMAPBUFFERRANGEPROC volt_glMapBufferRange;
#define glMapBufferRange volt_glMapBufferRange

GLAPI PFNGLUNMAPBUFFERPROC volt_glUnmapBuffer;
#define glUnmapBuffer volt_glUnmapBuffer

GLAPI PFNGLMAPBUFFERPROC volt_glMapBuffer;
#define glMapBuffer volt_glMapBuffer

/*

Shader program functions

*/

typedef GLuint(APIENTRYP PFNGLCREATEPROGRAMPROC)(void);
GLAPI PFNGLCREATEPROGRAMPROC volt_glCreateProgram;
#define glCreateProgram volt_glCreateProgram

typedef void (APIENTRYP PFNGLDELETEPROGRAMPROC)(GLuint program);
GLAPI PFNGLDELETEPROGRAMPROC volt_glDeleteProgram;
#define glDeleteProgram volt_glDeleteProgram

typedef void (APIENTRYP PFNGLUSEPROGRAMPROC) (GLuint program);
GLAPI PFNGLUSEPROGRAMPROC volt_glUseProgram;
#define glUseProgram volt_glUseProgram

typedef void (APIENTRYP PFNGLLINKPROGRAMPROC)(GLuint program);
GLAPI PFNGLLINKPROGRAMPROC volt_glLinkProgram;
#define glLinkProgram volt_glLinkProgram

typedef void (APIENTRYP PFNGLVALIDATEPROGRAMPROC)(GLuint program);
GLAPI PFNGLVALIDATEPROGRAMPROC volt_glValidateProgram;
#define glValidateProgram volt_glValidateProgram

typedef void (APIENTRYP PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
GLAPI PFNGLATTACHSHADERPROC volt_glAttachShader;
#define glAttachShader volt_glAttachShader

/*

Shader functions

*/

typedef GLuint(APIENTRYP PFNGLCREATESHADERPROC)(GLenum type);
GLAPI PFNGLCREATESHADERPROC volt_glCreateShader;
#define glCreateShader volt_glCreateShader

typedef void (APIENTRYP PFNGLDELETESHADERPROC)(GLuint shader);
GLAPI PFNGLDELETESHADERPROC volt_glDeleteShader;
#define glDeleteShader volt_glDeleteShader

typedef void (APIENTRYP PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
GLAPI PFNGLSHADERSOURCEPROC volt_glShaderSource;
#define glShaderSource volt_glShaderSource

typedef void (APIENTRYP PFNGLCOMPILESHADERPROC)(GLuint shader);
GLAPI PFNGLCOMPILESHADERPROC volt_glCompileShader;
#define glCompileShader volt_glCompileShader

typedef void (APIENTRYP PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint* params);
GLAPI PFNGLGETSHADERIVPROC volt_glGetShaderiv;
#define glGetShaderiv volt_glGetShaderiv

/*

Shader uniform functions

*/

typedef GLint(APIENTRYP PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const GLchar* name);
GLAPI PFNGLGETUNIFORMLOCATIONPROC volt_glGetUniformLocation;
#define glGetUniformLocation volt_glGetUniformLocation

typedef void (APIENTRYP PFNGLACTIVETEXTUREPROC)(GLenum texture);
GLAPI PFNGLACTIVETEXTUREPROC volt_glActiveTexture;
#define glActiveTexture volt_glActiveTexture

typedef void (APIENTRYP PFNGLUNIFORM1IPROC)(GLint location, GLint v0);
GLAPI PFNGLUNIFORM1IPROC volt_glUniform1i;
#define glUniform1i volt_glUniform1i

typedef void (APIENTRYP PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
GLAPI PFNGLUNIFORM1FPROC volt_glUniform1f;
#define glUniform1f volt_glUniform1f

typedef void (APIENTRYP PFNGLUNIFORM3FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
GLAPI PFNGLUNIFORM3FPROC volt_glUniform3f;
#define glUniform3f volt_glUniform3f

typedef void (APIENTRYP PFNGLUNIFORM4FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
GLAPI PFNGLUNIFORM4FPROC volt_glUniform4f;
#define glUniform4f volt_glUniform4f

typedef void (APIENTRYP PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GLAPI PFNGLUNIFORMMATRIX4FVPROC volt_glUniformMatrix4fv;
#define glUniformMatrix4fv volt_glUniformMatrix4fv

typedef void (APIENTRYP PFNGLGENERATEMIPMAPPROC)(GLenum target);
GLAPI PFNGLGENERATEMIPMAPPROC volt_glGenerateMipmap;
#define glGenerateMipmap volt_glGenerateMipmap
#endif

namespace volt {
	void loadGlFunctions();
}
