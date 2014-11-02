


#include "Renderer.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <GL\GL.h>

Renderer::Renderer(GL::platform::Window& window)
	: context(window)
{
        
	
	glClearColor(0.1f, 0.3f, 1.0f, 1.0f);

	window.attach(this);



}

void Renderer::resize(int width, int height)
{
	viewport_width = width;
	viewport_height = height;
}

void Renderer::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, viewport_width, viewport_height);

	
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDrawArrays(GL_TRIANGLES, 3, 3);
	glDrawArrays(GL_TRIANGLES, 6, 3);
	glDrawArrays(GL_TRIANGLES, 9, 3);
	glDrawArrays(GL_TRIANGLES, 12, 3);
	glDrawArrays(GL_TRIANGLES, 15, 3);
	glDrawArrays(GL_TRIANGLES, 18, 3);
	glDrawArrays(GL_TRIANGLES, 21, 3);
	glDrawArrays(GL_TRIANGLES, 24, 3);
	glDrawArrays(GL_TRIANGLES, 27, 3);
	glDrawArrays(GL_TRIANGLES, 30, 3);
	glDrawArrays(GL_TRIANGLES, 33, 3);
	context.swapBuffers();
}


void loadshader(const char* filename, std::string &ShaderSource)
{
    std::ifstream file;

    
	std::ifstream ShaderStream(filename, std::ios::in);
	if (ShaderStream.is_open())
	{
		std::string Line = "";
		while (std::getline(ShaderStream, Line))
			ShaderSource += "\n" + Line;
		ShaderStream.close();
	}

}

int unloadshader(GLubyte** ShaderSource)
{
    if (*ShaderSource != 0)
	delete[] *ShaderSource;
    *ShaderSource = 0;
	return 0;
}

void Renderer::createShader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    std::cout << "entered setUpShader()" << std::endl;
	GLint result = GL_FALSE;
	int infoLogLength;
	std::string vertexShaderSource, fragmentShaderSource;

	loadshader(vertexShaderPath, vertexShaderSource);
	loadshader(fragmentShaderPath, fragmentShaderSource);
	
	vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	const char* const_vss = vertexShaderSource.c_str();
	const char* const_vfs = fragmentShaderSource.c_str();

	glShaderSource(vertexShaderObject, 1, &const_vss, NULL);
	glCompileShader(vertexShaderObject);
	

	// Check Vertex Shader
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> VertexShaderErrorMessage(infoLogLength);
	glGetShaderInfoLog(vertexShaderObject, infoLogLength, NULL, &VertexShaderErrorMessage[0]);

	glShaderSource(fragmentShaderObject, 1, &const_vfs, NULL);
	glCompileShader(fragmentShaderObject);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> FragmentShaderErrorMessage(infoLogLength);
	glGetShaderInfoLog(fragmentShaderObject, infoLogLength, NULL, &FragmentShaderErrorMessage[0]);

	programmID = glCreateProgram();

	glAttachShader(programmID, vertexShaderObject);
	glAttachShader(programmID, fragmentShaderObject);

	glLinkProgram(programmID);

	// Check the program
	glGetProgramiv(programmID, GL_LINK_STATUS, &result);
	glGetProgramiv(programmID, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> ProgramErrorMessage(std::max(infoLogLength, int(1)));
	glGetProgramInfoLog(programmID, infoLogLength, NULL, &ProgramErrorMessage[0]);

	//glDeleteShader(vertexShaderObject);
	//glDeleteShader(fragmentShaderObject);

	glUseProgram(programmID);

	GLenum ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
			stderr,
			"ERROR: Could not create the shaders: \n"
			//gluErrorString(ErrorCheckValue)
			);
		exit(-1);
	}
}

void Renderer::destroyShader(void)
{
	GLenum ErrorCheckValue = glGetError();
	glUseProgram(0);
	glDetachShader(programmID, vertexShaderObject);
	glDetachShader(programmID, fragmentShaderObject);
	glDeleteShader(fragmentShaderObject);
	glDeleteShader(vertexShaderObject);
	glDeleteProgram(programmID);
	ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
			stderr,
			"ERROR: Could not destroy the shaders: \n"
			//gluErrorString(ErrorCheckValue)
			);
		exit(-1);
	}

}

void Renderer::createVertexBuffers(void)
{
	//VAO
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	//dodecagon
	GLfloat triangle[] = {
		0.6f, 0.0f, 0.0f,  //frist triangle, right x=0
		0.52f, 0.3f, 0.0f,
		0.0f, 0.0f, 0.0f,  
		0.52f, 0.3f, 0.0f,  //second
		0.3f, 0.52f, 0.0f,
		0.0f, 0.0f, 0.0f,  
		0.3f, 0.52f, 0.0f,  //thired
		0.0f, 0.6f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,    //fourth
		0.0f, 0.6f, 0.0f,
		-0.3f, 0.52f, 0.0f,
		0.0f, 0.0f, 0.0f,   //fived
		-0.3f, 0.52f, 0.0f,
		-0.52f, 0.3f, 0.0f,
		0.0f, 0.0f, 0.0f,  //sixed
		-0.52f, 0.3f, 0.0f,
		-0.6f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,  //seventh
		-0.6f, -0.f, 0.0f,
		-0.52f, -0.3f, 0.0f,
		0.0f, 0.0f, 0.0f,  // eighted
		-0.52, -0.3f, 0.0f,
		-0.3f, -0.52f, 0.0f,
		0.0f, 0.f, 0.0f,  //nined
		-0.3f, -0.52f, 0.0f,
		0.0f, -0.6f, 0.0f,
		0.3f, -0.52f, 0.0f,  // tenth
		0.0f, 0.0f, 0.0f,
		0.0f, -0.6f, 0.0f,
		0.52f, -0.3f, 0.0f,  //elefenth
		0.0f, 0.0f, 0.0f,
		0.3f, -0.52f, 0.0f,
		0.6f, 0.0f, 0.0f, //twelfth
		0.0f, 0.0f, 0.0f,
		0.52f, -0.3f, 0.0f,
	};
	
	glGenBuffers(1, &vTriangleId);
	glBindBuffer(GL_ARRAY_BUFFER, vTriangleId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLenum ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
			stderr,
			"ERROR: Could not create a VBO: \n"
			//glGetInfoErr(ErrorCheckValue)
			);
		exit(-1);
	}
}

void Renderer::destroyVertexBuffers(void)
{
	GLenum ErrorCheckValue = glGetError();
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vTriangleId);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vaoId);
	ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
			stderr,
			"ERROR: Could not destroy the VBO:  \n"
			//gluErrorString(ErrorCheckValue)
			);
		exit(-1);
	}
}


