


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
	//glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

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
	
	//std::vector<glm::vec3> structure;
	std::vector<unsigned int> indices;

	//genCube(structure, indices);
	genSolidDodecahedron(structure, indices);

	
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
	
	glGenBuffers(1, &indexId);
	CheckError("GenBuffers(1, indexId)");
	glEnableVertexAttribArray(1);
	CheckError("EnableVertexAttriArray(1)");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	CheckError("BindBuffer(indexId");

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	CheckError("BufferData(indices)");
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	CheckError("VertexAttribPointer(1, ");

	std::vector<glm::vec3> normals;
	calculateNormals(5, indices, structure, normals);

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
	camera.p_camera = glm::vec3(0.0f, 0.0f, -12.0f);
	camera.p_lookat = glm::vec3(0.0f, 0.0f, 0.0f);
	camera.v_up = glm::vec3(0.0f, 1.0f, 0.0f);

	//init view pyramid
	view.beta = 45;
	view.z_n = 0.1f;
	view.z_f = 100.0f;
	view.aspect = static_cast<float>(viewport_width / viewport_height);
	view.h = -2.0f * view.z_n * tan(view.beta / 2.0f);

	//light posiiton
	glm::vec3 light = camera.p_camera;
	lightId = glGetUniformLocation(programmID, "light");
	CheckError("GetUniformLocation(light");
	glUniform3fv(lightId, 1, glm::value_ptr(light));
	CheckError("Uniform1fv(lightId");

	//diffuse reflectance
	glm::vec4 f_r = glm::vec4(0.5f, 0.5f, 0.3f, 1.0f) / PI;
	GLint f_rId = glGetUniformLocation(programmID, "f_r");
	glUniform4fv(f_rId, 1, glm::value_ptr(f_r));

	//Calculate of matrices
	ProjectionMatrix = glm::perspective(view.beta, view.aspect, view.z_n, view.z_f);

	ViewMatrix =  glm::lookAt(
		camera.p_camera,
		camera.p_lookat,
		camera.v_up
		);

	ModelMatrix = glm::mat4(1.0f);

	//upload matrix to shader
	uniMVP = glGetUniformLocation(programmID, "MVP");
	glUniformMatrix4fv(uniMVP, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix*ViewMatrix*ModelMatrix));

	// Enable depth test
	//glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	//glDepthFunc(GL_LESS);
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
	
	//CubeAngle = RotateAngle * static_cast<float>(3.14159265358979323846 / 180);
	CubeAngle = -static_cast<float>(PI / 30) * ((float)(Now) / (CLOCKS_PER_SEC));
	//std::cout << RotateAngle << " " << CubeAngle << std::endl;
	LastTime = Now;

	ModelMatrix = { cos(CubeAngle), 0, -sin(CubeAngle), 0,
		0, 1, 0, 0,
		sin(CubeAngle), 0, cos(CubeAngle), 0,
		0, 0, 0, 1 };

	glUniformMatrix4fv(uniMVP, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix*ViewMatrix*ModelMatrix));

	glDrawElements(GL_TRIANGLES, structure.size()*sizeof(glm::vec3), GL_UNSIGNED_INT, (void*)0);
	
	
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	
	context.swapBuffers();
}

void Renderer::calculateNormals(const unsigned int corners, const std::vector<unsigned int> indices, const std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals)
{
	static int offset = 3;
	unsigned int o = 0;
	glm::fvec3 v1, v2, v3, a, b, cross;

	normals.clear();
	//std::cout << "size of indices: " << ind_size << "sizeof(unsigned int) "  << sizeof(unsigned int) << " / " << sizeof(GLuint) <<  "  size of vertices: " << vertices.size() << std::endl;
	for (unsigned int i = 0; i < indices.size(); i += offset)
	{
		v1 = vertices[indices[i]];
		v2 = vertices[indices[i + 1]];
		v3 = vertices[indices[i + 2]];
		a = v2 - v1;
		b = v3 - v2;

		cross = glm::cross(a, b);
		cross = glm::normalize(cross);

		//std::cout << "v1(" << v1.x << "," << v1.y << "," << v1.z << ") / v2(";
		//std::cout << v2.x << "," << v2.y << "," << v2.z << ") / v3(";
		//std::cout << v3.x << "," << v3.y << "," << v3.z << ")" << std::endl;
		 
		//std::cout << "a(" << a.x << "," << a.y << "," << a.z << ") / b(" << b.x << "," << b.y << "," << b.z << ")" << std::endl;
		//std::cout << "X(" << cross.x << "," << cross.y << "," << cross.z << std::endl;

		normals.push_back(cross);  //overtake normalised norm
		
		if (++o % (corners - 2) == 0)  // each face need (triangles + 2 normals)
		{
			normals.push_back(cross);
			normals.push_back(cross);
		}
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

void Renderer::genCube(std::vector<glm::vec3>& structure, std::vector<unsigned int>& indices)
{
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


	//Index
	indices.push_back(0); indices.push_back(2); indices.push_back(1);
	indices.push_back(0); indices.push_back(3); indices.push_back(2);  //back
	indices.push_back(6); indices.push_back(4); indices.push_back(5);
	indices.push_back(6); indices.push_back(7); indices.push_back(4);  //button
	indices.push_back(8); indices.push_back(9); indices.push_back(10);
	indices.push_back(8); indices.push_back(11); indices.push_back(9);  //left
	indices.push_back(12); indices.push_back(13); indices.push_back(14);
	indices.push_back(12); indices.push_back(15); indices.push_back(13);  //right
	indices.push_back(16); indices.push_back(18); indices.push_back(17);
	indices.push_back(16); indices.push_back(19); indices.push_back(18);  //top 
	indices.push_back(23); indices.push_back(20); indices.push_back(21);
	indices.push_back(23); indices.push_back(22); indices.push_back(20);  //front
}

void genPentagonIndexing(std::vector<unsigned int> &indices, unsigned int v1, unsigned int v2, unsigned int v3, unsigned int v4, unsigned int v5)
{
	indices.push_back(v1); indices.push_back(v3); indices.push_back(v2);
	indices.push_back(v1); indices.push_back(v4); indices.push_back(v3);
	indices.push_back(v1); indices.push_back(v5); indices.push_back(v4);
	
	//indices.push_back(v1); indices.push_back(v2); indices.push_back(v3);
	//indices.push_back(v1); indices.push_back(v3); indices.push_back(v4);
	//indices.push_back(v1); indices.push_back(v4); indices.push_back(v5);
}

void Renderer::genSolidDodecahedron(std::vector<glm::vec3>& structure, std::vector<unsigned int>& indices)
{
	float a = (1 + std::sqrt(5.0f)) / 2;
	structure.clear();
	
	//front right 
	structure.push_back(glm::vec3(0.0f, 1.0f / a, -a));  //green front up
	structure.push_back(glm::vec3(0.0f, -1.0f / a, -a));  //green front down
	structure.push_back(glm::vec3(1.0f, -1.0f, -1.0f));   //orange front right down
	structure.push_back(glm::vec3(a, 0.0f, -1.0f / a));     //pink front right 
	structure.push_back(glm::vec3(1.0f, 1.0f, -1.0f));    //orange front right up
	
	//front left
	structure.push_back(glm::vec3(-a, 0.0f, -1.0f / a));  //pink front left
	structure.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));  //orange front left down
	structure.push_back(glm::vec3(0.0f, -1.0f / a, -a));  //green front down
	structure.push_back(glm::vec3(0.0f, 1.0f / a, -a));  //green front up
	structure.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));  //orange front left up
	
	//front up
	structure.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));  //orange front left up
	structure.push_back(glm::vec3(0.0f, 1.0f / a, -a));  //green front up
	structure.push_back(glm::vec3(1.0f, 1.0f, -1.0f));    //orange front right up
	structure.push_back(glm::vec3(1.0f / a, a, 0.0f));    //blue right up
	structure.push_back(glm::vec3(-1.0f / a, a, 0.0f));    //blue left up
	
	//front down
	structure.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));  //orange front left down
	structure.push_back(glm::vec3(-1.0f / a, -a, 0.0f));    //blue left down
	structure.push_back(glm::vec3(1.0f / a, -a, 0.0f));    //blue right down
	structure.push_back(glm::vec3(1.0f, -1.0f, -1.0f));   //orange front right down
	structure.push_back(glm::vec3(0.0f, -1.0f / a, -a));  //green front down
	
	//right up
	structure.push_back(glm::vec3(1.0f, 1.0f, -1.0f));    //orange front right up
	structure.push_back(glm::vec3(a, 0.0f, -1.0f / a));     //pink front right
	structure.push_back(glm::vec3(a, 0.0f, 1.0f / a));     //pink back right
	structure.push_back(glm::vec3(1.0f, 1.0f, 1.0f));    //orange back right up
	structure.push_back(glm::vec3(1.0f / a, a, 0.0f));    //blue right up
	
	//right down
	structure.push_back(glm::vec3(1.0f, -1.0f, -1.0f));   //orange front right down
	structure.push_back(glm::vec3(1.0f / a, -a, 0.0f));    //blue right down
	structure.push_back(glm::vec3(1.0f, -1.0f, 1.0f));   //orange back right down
	structure.push_back(glm::vec3(a, 0.0f, 1.0f / a));     //pink back right
	structure.push_back(glm::vec3(a, 0.0f, -1.0f / a));     //pink front right
	
	//back up
	structure.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));  //orange back left up
	structure.push_back(glm::vec3(-1.0f / a, a, 0.0f));    //blue left up
	structure.push_back(glm::vec3(1.0f / a, a, 0.0f));    //blue right up
	structure.push_back(glm::vec3(1.0f, 1.0f, 1.0f));    //orange back right up
	structure.push_back(glm::vec3(0.0f, 1.0f / a, a));  //green back up
	
	//back down
	structure.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));  //orange back left down
	structure.push_back(glm::vec3(-1.0f / a, -a, 0.0f));    //blue left down
	structure.push_back(glm::vec3(1.0f / a, -a, 0.0f));    //blue right down
	structure.push_back(glm::vec3(1.0f, -1.0f, 1.0f));   //orange back right down
	structure.push_back(glm::vec3(0.0f, -1.0f / a, a));  //green back down
	
	//back left
	structure.push_back(glm::vec3(-a, 0.0f, 1.0f / a));  //pink back left
	structure.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));  //orange back left down
	structure.push_back(glm::vec3(0.0f, -1.0f / a, a));  //green back down
	structure.push_back(glm::vec3(0.0f, 1.0f / a, a));  //green back up
	structure.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));  //orange back left up
	
	//back right
	structure.push_back(glm::vec3(0.0f, 1.0f / a, a));  //green back up
	structure.push_back(glm::vec3(0.0f, -1.0f / a, a));  //green back down
	structure.push_back(glm::vec3(1.0f, -1.0f, 1.0f));   //orange back right down
	structure.push_back(glm::vec3(a, 0.0f, 1.0f / a));     //pink back right 
	structure.push_back(glm::vec3(1.0f, 1.0f, 1.0f));    //orange back right up
    
	//left up
	structure.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));  //orange back left up
	structure.push_back(glm::vec3(-1.0f / a, a, 0.0f));    //blue left up
	structure.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));  //orange front left up
	structure.push_back(glm::vec3(-a, 0.0f, -1.0f / a));     //pink front left
	structure.push_back(glm::vec3(-a, 0.0f, 1.0f / a));     //pink back left
	
	//left down
	structure.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));  //orange back left down
	structure.push_back(glm::vec3(-a, 0.0f, 1.0f / a));     //pink back left
	structure.push_back(glm::vec3(-a, 0.0f, -1.0f / a));     //pink front left
	structure.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));  //orange front left down
	structure.push_back(glm::vec3(-1.0f / a, -a, 0.0f));    //blue left down
	
	//*****
	//index
	//*****
	//front left
	genPentagonIndexing(indices, 0, 1, 2, 3, 4);
	//front right
	genPentagonIndexing(indices, 5, 6, 7, 8, 9);
	//front up
	genPentagonIndexing(indices, 10, 11, 12, 13, 14);
	//front down
	genPentagonIndexing(indices, 15, 16, 17, 18, 19);
	//right up
	genPentagonIndexing(indices, 20, 21, 22, 23, 24);
	//right down
	genPentagonIndexing(indices, 25, 26, 27, 28, 29);
	//left up
	genPentagonIndexing(indices, 30, 31, 32, 33, 34);
	//left down
	genPentagonIndexing(indices, 35, 36, 37, 38, 39);
	//back right
	genPentagonIndexing(indices, 40, 41, 42, 43, 44);
	//back left
	genPentagonIndexing(indices, 45, 46, 47 ,48, 49);
	//back up
	genPentagonIndexing(indices, 50, 51, 52, 53, 54);
	//back down
	genPentagonIndexing(indices, 55, 56, 57, 58, 59);
	
}
