#include "include/main.h"

using namespace std;

bool InitGLSL()
{
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		return false;
	}

	char *glExtensions = (char*)glGetString(GL_EXTENSIONS);

	if (!strstr(glExtensions, "GL_ARB_shader_objects"))
	{
		MessageBox(g_WindowHandle, "GL_ARB_shader_objects extension not supported!", "Error", MB_OK);
		return false;
	}

	if (!strstr(glExtensions, "GL_ARB_shading_language_100"))
	{
		MessageBox(g_WindowHandle, "GL_ARB_shading_language_100 extension not supported!", "Error", MB_OK);
		return false;
	}

	return true;
}

const GLchar *Shader::readShader(const char *filename)
{
	FILE *infile;
	fopen_s(&infile, filename, "rb");
	if (infile == NULL)
	{
#ifdef _DEBUG
		std::cerr << "[error] Unable to open file '" << filename << "'" << std::endl;
#endif
		return NULL;
	}

	fseek(infile, 0, SEEK_END);
	int length = ftell(infile);
	fseek(infile, 0, SEEK_SET);

	GLchar *source = new GLchar[length + 1];
	fread(source, 1, length, infile);
	fclose(infile);
	source[length] = '\0';
	return const_cast<const GLchar*>(source);
}

void Shader::LoadShaders(ShaderInfo *shaders)
{
	int count = 0;
	if (shaders == NULL)
		return;

	m_Program = glCreateProgramObjectARB();
	ShaderInfo *entry = shaders;

	while (entry->type != GL_NONE)
	{
		GLuint shader = glCreateShaderObjectARB(entry->type);
		entry->shader = shader;

		const GLchar *source = readShader(entry->filename);
		if (source == NULL)
		{
			for (entry = shaders; entry->type != GL_NONE; ++entry)
			{
				glDeleteShader(entry->shader);
				entry->shader = 0;
				return;
			}
		}

		glShaderSourceARB(shader, 1, &source, NULL);
		delete[] source;

		glCompileShaderARB(shader);

		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (compiled != GL_TRUE)
		{
#ifdef _DEBUG
			GLsizei len;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

			GLchar *log = new GLchar[len + 1];
			glGetShaderInfoLog(shader, len, &len, log);
			std::cerr << "[error] Shader compilation failed: " << log << std::endl;
			delete[] log;
#endif
			return;
		}
		glAttachObjectARB(m_Program, shader);
		++entry;
		count++;
	}

	// We copy the shaders so we can free their memory at the end
	m_Shaders = new ShaderInfo[count + 1];
	int i;
	for (i = 0; i < count; i++)
	{
		m_Shaders[i].filename = shaders[i].filename;
		m_Shaders[i].shader = shaders[i].shader;
		m_Shaders[i].type = shaders[i].type;
	}
	m_Shaders[count].type = GL_NONE;
	m_Shaders[count].filename = NULL;

	glLinkProgramARB(m_Program);

	GLint linked;
	glGetProgramiv(m_Program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
#ifdef _DEBUG
		GLsizei len;
		glGetShaderiv(m_Program, GL_INFO_LOG_LENGTH, &len);

		GLchar *log = new GLchar[len + 1];
		glGetShaderInfoLog(m_Program, len, &len, log);
		std::cerr << "[error] Shader compilation failed: " << log << std::endl;
		delete[] log;
#endif
		for (entry = shaders; entry->type != GL_NONE; ++entry)
		{
			glDeleteShader(entry->shader);
			entry->shader = 0;
		}
		return;
	}

}

GLint Shader::GetVariable(std::string name)
{
	if (!m_Program)
		return -1;

	return glGetUniformLocationARB(m_Program, name.c_str());
}

void Shader::Release()
{
	ShaderInfo *entry = m_Shaders;
	for (entry = m_Shaders; entry->type != GL_NONE; ++entry)
	{
		glDetachObjectARB(m_Program, entry->shader);
		glDeleteObjectARB(entry->shader);
		entry->shader = 0;
	}

	if (m_Program)
	{
		glDeleteObjectARB(m_Program);
		m_Program = NULL;
	}
}