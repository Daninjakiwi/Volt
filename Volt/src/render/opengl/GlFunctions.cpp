#ifdef BUILD_WINDOWS

#include <render/opengl/GlFunctions.hpp>

/*

Vertex Array functions

*/

PFNGLCREATEVERTEXARRAYSPROC volt_glCreateVertexArrays = 0;
PFNGLDELETEVERTEXARRAYSPROC volt_glDeleteVertexArrays = 0;
PFNGLBINDVERTEXARRAYPROC volt_glBindVertexArray = 0;
PFNGLENABLEVERTEXATTRIBARRAYPROC volt_glEnableVertexAttribArray = 0;
PFNGLVERTEXATTRIBPOINTERPROC volt_glVertexAttribPointer = 0;
PFNGLGENVERTEXARRAYSPROC volt_glGenVertexArrays = 0;

/*

Frame buffer functions

*/

PFNGLGENFRAMEBUFFERSPROC volt_glGenFramebuffers = 0;
PFNGLGENRENDERBUFFERSPROC volt_glGenRenderbuffers = 0;

PFNGLCREATEFRAMEBUFFERSPROC volt_glCreateFramebuffers = 0;
PFNGLDELETEFRAMEBUFFERSPROC volt_glDeleteFramebuffers = 0;
PFNGLBINDFRAMEBUFFERPROC volt_glBindFramebuffer = 0;
PFNGLFRAMEBUFFERTEXTURE2DPROC volt_glFramebufferTexture2D = 0;
PFNGLCREATERENDERBUFFERSPROC volt_glCreateRenderbuffers = 0;
PFNGLDELETEFRAMEBUFFERSPROC volt_glDeleteRenderbuffers = 0;
PFNGLBINDRENDERBUFFERPROC volt_glBindRenderbuffer = 0;
PFNGLRENDERBUFFERSTORAGEPROC volt_glRenderbufferStorage = 0;
PFNGLFRAMEBUFFERRENDERBUFFERPROC volt_glFramebufferRenderbuffer = 0;
PFNGLCHECKFRAMEBUFFERSTATUSPROC volt_glCheckFramebufferStatus = 0;

/*

Buffer functions

*/

PFNGLCREATEBUFFERSPROC volt_glCreateBuffers = 0;
PFNGLDELETEBUFFERSPROC volt_glDeleteBuffers = 0;
PFNGLBINDBUFFERPROC volt_glBindBuffer = 0;
PFNGLGENBUFFERSPROC volt_glGenBuffers = 0;

/*

Buffer data functions

*/

PFNGLBUFFERDATAPROC volt_glBufferData = 0;
PFNGLBUFFERSUBDATAPROC volt_glBufferSubData = 0;
PFNGLMAPBUFFERRANGEPROC volt_glMapBufferRange = 0;
PFNGLUNMAPBUFFERPROC volt_glUnmapBuffer = 0;
PFNGLMAPBUFFERPROC volt_glMapBuffer = 0;

/*

Shader program functions

*/

PFNGLCREATEPROGRAMPROC volt_glCreateProgram = 0;
PFNGLDELETEPROGRAMPROC volt_glDeleteProgram = 0;
PFNGLUSEPROGRAMPROC volt_glUseProgram = 0;
PFNGLLINKPROGRAMPROC volt_glLinkProgram = 0;
PFNGLVALIDATEPROGRAMPROC volt_glValidateProgram = 0;
PFNGLATTACHSHADERPROC volt_glAttachShader = 0;

/*

Shader functions

*/

PFNGLCREATESHADERPROC volt_glCreateShader = 0;
PFNGLDELETESHADERPROC volt_glDeleteShader = 0;
PFNGLSHADERSOURCEPROC volt_glShaderSource = 0;
PFNGLCOMPILESHADERPROC volt_glCompileShader = 0;
PFNGLGETSHADERIVPROC volt_glGetShaderiv = 0;

/*

Shader uniform functions

*/

PFNGLGETUNIFORMLOCATIONPROC volt_glGetUniformLocation = 0;
PFNGLACTIVETEXTUREPROC volt_glActiveTexture = 0;
PFNGLUNIFORM1IPROC volt_glUniform1i = 0;
PFNGLUNIFORM1FPROC volt_glUniform1f = 0;
PFNGLUNIFORM3FPROC volt_glUniform3f = 0;
PFNGLUNIFORM4FPROC volt_glUniform4f = 0;
PFNGLUNIFORMMATRIX4FVPROC volt_glUniformMatrix4fv = 0;

PFNGLGENERATEMIPMAPPROC volt_glGenerateMipmap = 0;



namespace volt {
	void loadGlFunctions() {
		volt_glCreateVertexArrays = (PFNGLCREATEVERTEXARRAYSPROC)wglGetProcAddress("glCreateVertexArrays");
		volt_glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
		volt_glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
		volt_glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
		volt_glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
		volt_glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");

		volt_glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");
		volt_glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffers");
		volt_glCreateFramebuffers = (PFNGLCREATEFRAMEBUFFERSPROC)wglGetProcAddress("glCreateFramebuffers");
		volt_glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffers");
		volt_glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");
		volt_glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2D");
		volt_glCreateRenderbuffers = (PFNGLCREATERENDERBUFFERSPROC)wglGetProcAddress("glCreateRenderbuffers");
		volt_glDeleteRenderbuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteRenderbuffers");
		volt_glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbuffer");
		volt_glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorage");
		volt_glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");
		volt_glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress("glCheckFramebufferStatus");

		volt_glCreateBuffers = (PFNGLCREATEBUFFERSPROC)wglGetProcAddress("glCreateBuffers");
		volt_glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
		volt_glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
		volt_glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");

		volt_glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
		volt_glBufferSubData = (PFNGLBUFFERSUBDATAPROC)wglGetProcAddress("glBufferSubData");
		volt_glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC)wglGetProcAddress("glMapBufferRange");
		volt_glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)wglGetProcAddress("glUnmapBuffer");
		volt_glMapBuffer = (PFNGLMAPBUFFERPROC)wglGetProcAddress("glMapBuffer");

		volt_glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
		volt_glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
		volt_glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
		volt_glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
		volt_glValidateProgram = (PFNGLVALIDATEPROGRAMPROC)wglGetProcAddress("glValidateProgram");
		volt_glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");

		volt_glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
		volt_glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
		volt_glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
		volt_glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
		volt_glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");

		volt_glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
		volt_glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
		volt_glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
		volt_glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
		volt_glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
		volt_glUniform4f = (PFNGLUNIFORM4FPROC)wglGetProcAddress("glUniform4f");
		volt_glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
		volt_glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
	}
}


#endif