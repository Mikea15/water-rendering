#ifndef _SHADER_H
#define _SHADER_H

#include "main.h"

class Shader
{
public:
	Shader() {}
	~Shader() { Release(); }

	typedef struct
	{
		GLenum type;
		const char *filename;
		GLuint shader;
	} ShaderInfo;

	//************************************
	// Method:    LoadShaders
	// FullName:  Shader::LoadShaders
	// Access:    public 
	// Returns:   void
	// Qualifier: Loads and initializes the shaders specified. Remember to
	// always delimit the list with GL_NONE
	// Parameter: ShaderInfo * shaders
	//************************************
	void LoadShaders(ShaderInfo *shaders);

	//************************************
	// Method:    GetVariable
	// FullName:  Shader::GetVariable
	// Access:    public 
	// Returns:   GLint
	// Qualifier: Gets the uniform variable
	// Parameter: std::string name
	//************************************
	GLint GetVariable(std::string name);

	//************************************
	// Method:    Release
	// FullName:  Shader::Release
	// Access:    public 
	// Returns:   void
	// Qualifier: Frees our shaders
	//************************************
	void Release();

	//************************************
	// Method:    TurnOn
	// FullName:  Shader::TurnOn
	// Access:    public 
	// Returns:   void
	// Qualifier: Turn on the shaders
	//************************************
	void TurnOn()
	{
		glUseProgramObjectARB(m_Program);
	}

	//************************************
	// Method:    TurnOff
	// FullName:  Shader::TurnOff
	// Access:    public 
	// Returns:   void
	// Qualifier: Turns off the shaders
	//************************************
	void TurnOff()
	{
		glUseProgramObjectARB(0);
	}

	GLuint GetProgram()
	{
		return m_Program;
	}

protected:
	//************************************
	// Method:    readShader
	// FullName:  Shader::readShader
	// Access:    protected 
	// Returns:   const GLchar *
	// Qualifier: Reads the source file for the shader
	// Parameter: const char * filename
	//************************************
	const GLchar *readShader(const char *filename);

private:
	ShaderInfo *m_Shaders;
	GLuint m_Program;
};

bool InitGLSL();

#endif