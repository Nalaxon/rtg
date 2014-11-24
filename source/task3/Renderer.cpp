


#include "Renderer.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>

static float PI = 3.14159265358979323846f;

Renderer::Renderer(GL::platform::Window& window)
	: context(window)
{
	glClearColor(0.1f, 0.3f, 1.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	window.attach(this);
}

void Renderer::resize(int width, int height)
{
	viewport_width = width;
	viewport_height = height;
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
		delete[] * ShaderSource;
	*ShaderSource = 0;
	return 0;
}

void Renderer::createShader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	GLint result = GL_FALSE;
	//GLenum ErrorCheckValue;
	int infoLogLength;
	std::string vertexShaderSource, fragmentShaderSource;

	loadshader(vertexShaderPath, vertexShaderSource);
	loadshader(fragmentShaderPath, fragmentShaderSource);

	vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	CheckError("CreateShader(VERTEX)");
	fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	CheckError("CreateShader(FRAGMENT)");

	const char* const_vss = vertexShaderSource.c_str();
	const char* const_vfs = fragmentShaderSource.c_str();

	std::cout << "Compile Vertex Shader..." << std::endl;

	glShaderSource(vertexShaderObject, 1, &const_vss, NULL);
	glCompileShader(vertexShaderObject);


	// Check Vertex Shader
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE)
	{
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> VertexShaderErrorMessage(infoLogLength);
		glGetShaderInfoLog(vertexShaderObject, infoLogLength, NULL, &VertexShaderErrorMessage[0]);
		fprintf(
			stderr,
			"ERROR: Could not create the shaders: %s \n", &VertexShaderErrorMessage[0]
			);
		getchar();
		exit(-1);
	}

	std::cout << "Compile Fragment Shader..." << std::endl;

	glShaderSource(fragmentShaderObject, 1, &const_vfs, NULL);
	glCompileShader(fragmentShaderObject);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE)
	{
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> FragmentShaderErrorMessage(infoLogLength);
		glGetShaderInfoLog(fragmentShaderObject, infoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		fprintf(
			stderr,
			"ERROR: Could not create the shaders: %s \n", &FragmentShaderErrorMessage[0]
			);
		getchar();
		exit(-1);
	}

	std::cout << "Create Program..." << std::endl;

	programmID = glCreateProgram();
	CheckError("CreateProgramm()");

	glAttachShader(programmID, vertexShaderObject);
	CheckError("AttachShader(vertex)");
	glAttachShader(programmID, fragmentShaderObject);
	CheckError("AttachShader(fragment)");

	glLinkProgram(programmID);

	// Check the program
	glGetProgramiv(programmID, GL_LINK_STATUS, &result);
	if (result != GL_TRUE)
	{
		glGetProgramiv(programmID, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> ProgramErrorMessage(std::max(infoLogLength, int(1)));
		glGetProgramInfoLog(programmID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(
			stderr,
			"ERROR: Could not link programm: %s \n", &ProgramErrorMessage[0]
			);
		getchar();
		exit(-1);
	}

	glUseProgram(programmID);
	CheckError("UseProgramm()");

}

void Renderer::destroyShader(void)
{
	glUseProgram(0);
	CheckError("UserProgram(0)");
	glDetachShader(programmID, vertexShaderObject);
	CheckError("DetachShader(, vertexShader)");
	glDetachShader(programmID, fragmentShaderObject);
	CheckError("DetachShader(, framentShader)");
	glDeleteShader(fragmentShaderObject);
	CheckError("DeleteShader(fragmentShader)");
	glDeleteShader(vertexShaderObject);
	CheckError("DeleteShader(vertexShader)");
	glDeleteProgram(programmID);
	CheckError("DelteProgramm()");
}

void Renderer::createNaiveStructure(void)
{
	//VAO
	glGenVertexArrays(1, &vaoId);
	CheckError("GenVertexArrays(1, vaoId");
	glBindVertexArray(vaoId);
	CheckError("BindVertexArray(vaoId)");

	float a = (1 + std::sqrt(5.0f)) / 2;
	//dodecahedron
	
	std::vector<glm::vec3> structure;
		//Back
	structure.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));  //0
	structure.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));   //1
	structure.push_back(glm::vec3(1.0f, 1.0f, 1.0f));    //2
	structure.push_back(glm::vec3(1.0f, -1.0f, 1.0f));   //3

        //Buttom
	structure.push_back(glm::vec3(1.0f, -1.0f, 1.0f));   //4
	structure.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));  //5
	structure.push_back(glm::vec3(-1.0f, -1.0f, -1.0f)); //6
	structure.push_back(glm::vec3(1.0f, -1.0f, -1.0f));  //7
		
		//Left
	structure.push_back(glm::vec3(-1.0f, -1.0f, -1.0f)); //8
	structure.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));   //9
	structure.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));  //10
	structure.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));  //11

		//Right
	structure.push_back(glm::vec3(1.0f, -1.0f, 1.0f));   //12
	structure.push_back(glm::vec3(1.0f, 1.0f, -1.0f));   //13
	structure.push_back(glm::vec3(1.0f, 1.0f, 1.0f));    //14
	structure.push_back(glm::vec3(1.0f, -1.0f, -1.0f));  //15

		//Top
	structure.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));   //16
	structure.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));  //17
	structure.push_back(glm::vec3(1.0f, 1.0f, -1.0f));   //18
	structure.push_back(glm::vec3(1.0f, 1.0f, 1.0f));    //19

		//Front
	structure.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));  //20
	structure.push_back(glm::vec3(1.0f, 1.0f, -1.0f));   //21
	structure.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));  //22
	structure.push_back(glm::vec3(1.0f, -1.0f, -1.0f));  //23

		//basic quader
	/*	-1.0f, 1.0f, 1.0f,     //foreground
		-1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,     //background
		-1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,

		//edge fore and background
		0.0f, 1 / a, -a,
		0.0f, -1 / a, -a,
		0.0f, 1 / a, a,
		0.0f, -1 / a, a,

		//edges top buttom
		-1 / a, a, 0.0f,
		1 / a, a, 0.0f,
		-1 / a, -a, 0.0f,
		1 / a, -a, 0.0f,

		//edges left right
		-a, 0.0f, -1 / a,
		-a, 0.0f, 1 / a,
		a, 0.0f, -1 / a,
		a, 0.0f, 1 / a
		*/

	
	glGenBuffers(1, &vStructureId);
	CheckError("GenBuffers(1, StructureId)");
	glEnableVertexAttribArray(0);
	CheckError("EnableVertexAttriArray(0)");
	glBindBuffer(GL_ARRAY_BUFFER, vStructureId);
	CheckError("BindBuffer(Structre)");
	glBufferData(GL_ARRAY_BUFFER, structure.size() *sizeof(glm::vec3), &structure[0], GL_STATIC_DRAW);
	CheckError("Bufferdata()");
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	CheckError("VertexAttribPointer(0, ");

	const GLuint indices[] = {
		0, 2, 1, 0, 3, 2,  //back
		6, 4, 5, 6, 7, 4,  //button
		8, 9, 10, 8, 11, 9,  //left
		12, 13, 14, 12, 15, 13,  //right
		16, 18, 17, 16, 19, 18,  //top 
		23, 20, 21, 23, 22, 20  //front
	};
	
	glGenBuffers(1, &indexId);
	CheckError("GenBuffers(1, indexId)");
	glEnableVertexAttribArray(1);
	CheckError("EnableVertexAttriArray(1)");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	CheckError("BindBuffer(indexId");

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	CheckError("BufferData(indices)");
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	CheckError("VertexAttribPointer(1, ");

	std::vector<glm::vec3> normals;
	calculateNormals(sizeof(indices), indices, structure, normals);

    /*GLfloat normals[] = {
		0.0f, 0.0f, -1.0f,  //back
		0.0f, 0.0f, -1.0f,  //back
		0.0f, 0.0f, -1.0f,  //back
		0.0f, 0.0f, -1.0f,  //back
		0.0f, -1.0f, 0.0f, //down
		0.0f, -1.0f, 0.0f, //down
		0.0f, -1.0f, 0.0f, //down
		0.0f, -1.0f, 0.0f, //down
		-1.0f, 0.0f, 0.0f,  //left
		-1.0f, 0.0f, 0.0f,  //left
		-1.0f, 0.0f, 0.0f,  //left
		-1.0f, 0.0f, 0.0f,  //left
		1.0f, 0.0f, 0.0f,  //right
		1.0f, 0.0f, 0.0f,  //right
		1.0f, 0.0f, 0.0f,  //right
		1.0f, 0.0f, 0.0f,  //right
		0.0f, 1.0f, 0.0f,  //up
		0.0f, 1.0f, 0.0f,  //up
		0.0f, 1.0f, 0.0f,  //up
		0.0f, 1.0f, 0.0f,  //up
		0.0f, 0.0f, 1.0f,  //front
		0.0f, 0.0f, 1.0f,  //front
		0.0f, 0.0f, 1.0f,  //front
		0.0f, 0.0f, 1.0f,  //front
	};*/

	glGenBuffers(1, &normalId);
	CheckError("GenBuffers(1, normalId)");
    glEnableVertexAttribArray(2);
	CheckError("EnableVertexAttriArray(2)");
	glBindBuffer(GL_ARRAY_BUFFER, normalId);
	CheckError("BindBuffer(normalId)");
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3) , &normals[0], GL_STATIC_DRAW);
	CheckError("BufferData(indices)");
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	CheckError("VertexAttribPointer(2, ");
	
	//initial camera position and orientation
	camera.p_camera = glm::vec3(4.0f, 3.0f, 3.0f);
	camera.p_lookat = glm::vec3(0.0f, 0.0f, 0.0f);
	camera.v_up = glm::vec3(0.0f, 1.0f, 0.0f);

	//init view pyramid
	view.beta = 45;
	view.z_n = 0.1f;
	view.z_f = 100.0f;
	view.aspect = static_cast<float>(viewport_width / viewport_height);
	view.h = -2.0f * view.z_n * tan(view.beta / 2.0f);

	//light posiiton
	glm::vec3 light = glm::vec3(5.0f, 6.0f, 2.0f);
	lightId = glGetUniformLocation(programmID, "light");
	CheckError("GetUniformLocation(light");
	glUniform3fv(lightId, 1, glm::value_ptr(light));
	CheckError("Uniform1fv(lightId");

	//diffuse reflectance
	glm::vec3 f_r = glm::vec3(0.5f, 0.5f, 0.3f);
	GLint f_rId = glGetUniformLocation(programmID, "f_r");
	glUniform3fv(f_rId, 1, glm::value_ptr(f_r));

	//Calculate of matrices
	//ProjectionMatrix = {1/ (view.aspect * view.h), 0, 0, 0,
	//                             0, 1/view.h, 0, 0,
	//                             0, 0, - (view.z_f + view.z_n) / (view.z_f - view.z_n), -1,
	//                             0, 0, - (2 * view.z_f * view.z_n) / (view.z_f - view.z_n), 0};
	ProjectionMatrix = glm::perspective(view.beta, view.aspect, view.z_n, view.z_f);

	ViewMatrix =  glm::lookAt(
		camera.p_camera,
		camera.p_lookat,
		camera.v_up
		);

	ModelMatrix = glm::mat4(1.0f);

	//upload matrix to shader
	GLint uniTrans = glGetUniformLocation(programmID, "proj");
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

	GLint uniView = glGetUniformLocation(programmID, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(ViewMatrix));

	uniModel = glGetUniformLocation(programmID, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
}

void Renderer::destroyNaiveStructure(void)
{

}

void Renderer::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, viewport_width, viewport_height);

	float CubeAngle, delta;
	clock_t Now = clock();
	if (LastTime == 0)
		LastTime = Now;

	delta = float(Now - LastTime);
	RotateAngle += 45.0f * ((float)(Now) / (CLOCKS_PER_SEC));
	//RotateAngle += 45.0f * ((float)(Now - LastTime) / CLOCKS_PER_SEC);
	
	//CubeAngle = RotateAngle * static_cast<float>(3.14159265358979323846 / 180);
	CubeAngle = static_cast<float>(PI / 60) * ((float)(Now) / (CLOCKS_PER_SEC));
	//std::cout << RotateAngle << " " << CubeAngle << std::endl;
	LastTime = Now;

	ModelMatrix = { cos(CubeAngle), 0, -sin(CubeAngle), 0,
		0, 1, 0, 0,
		sin(CubeAngle), 0, cos(CubeAngle), 0,
		0, 0, 0, 1 };

	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
	
	
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	
	context.swapBuffers();
}


void Renderer::calculateNormals(const int ind_size, const GLuint* indices, const std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals)
{
	static int offset = 3;

	glm::vec3 v1, v2, v3, a, b, cross;
	std::cout << "size of indices: " << ind_size << "sizeof(unsigned int) " << sizeof(GLuint) <<  "  size of vertices: " << vertices.size() << std::endl;

	for (int i = 0; (i < ind_size/sizeof(GLuint)) ; i += offset)
	{
		std::cout << i << ": " << indices[i] << std::endl;
		v1 = vertices[indices[i]];
		v2 = vertices[indices[i+1]];
		v3 = vertices[indices[i+2]];
		a = v2 - v1;
		b = v3 - v2;

		cross = glm::cross(a, b);

		std::cout << "(" << v2.x << "/" << v2.y << "/" << v2.z << ") - (" << v1.x << "/" << v1.y << "/" << v1.z << ") = (";
		std::cout << a.x << "/" << a.y << "/" << a.z << ")" << std::endl;
		std::cout << "(" << v3.x << "/" << v3.y << "/" << v3.z << ") - (" << v2.x << "/" << v2.y << "/" << v2.z << ") = (";
		std::cout << b.x << "/" << b.y << "/" << b.z << ")" << std::endl;
		std::cout << "X= " << cross.x << "/" << cross.y << "/" << cross.z << std::endl;

 		normals.push_back(cross);
	}
}

void  Renderer::CheckError(const std::string funcName)
{
	GLenum ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		std::cerr << "ERROR: While function " << funcName << std::endl;
		getchar();
		exit(-1);
	}
}
