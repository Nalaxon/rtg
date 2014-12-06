


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

void Renderer::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, viewport_width, viewport_height);

	
	//for (int i = 0; i < vertices.size(); i += 3)
	glDrawArrays(GL_TRIANGLES, 0, vertices.size()*3);

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

	vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	CheckError("CreateShader(VERTEX)");
	fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	CheckError("CreateShader(FRAGMENT)");

	const char* const_vss = vertexShaderSource.c_str();
	const char* const_vfs = fragmentShaderSource.c_str();

	std::cout << "Compile Vertex Shader..." << std::endl;

	glShaderSource(vertexShaderId, 1, &const_vss, NULL);
	glCompileShader(vertexShaderId);


	// Check Vertex Shader
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE)
	{
		glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> VertexShaderErrorMessage(infoLogLength);
		glGetShaderInfoLog(vertexShaderId, infoLogLength, NULL, &VertexShaderErrorMessage[0]);
		fprintf(
			stderr,
			"ERROR: Could not create the shaders: %s \n", &VertexShaderErrorMessage[0]
			);
		getchar();
		exit(-1);
	}

	std::cout << "Compile Fragment Shader..." << std::endl;

	glShaderSource(fragmentShaderId, 1, &const_vfs, NULL);
	glCompileShader(fragmentShaderId);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE)
	{
		glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> FragmentShaderErrorMessage(infoLogLength);
		glGetShaderInfoLog(fragmentShaderId, infoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		fprintf(
			stderr,
			"ERROR: Could not create the shaders: %s \n", &FragmentShaderErrorMessage[0]
			);
		getchar();
		exit(-1);
	}

	std::cout << "Create Program..." << std::endl;

	programmId = glCreateProgram();
	CheckError("CreateProgramm()");

	glAttachShader(programmId, vertexShaderId);
	CheckError("AttachShader(vertex)");
	glAttachShader(programmId, fragmentShaderId);
	CheckError("AttachShader(fragment)");

	glLinkProgram(programmId);

	// Check the program
	glGetProgramiv(programmId, GL_LINK_STATUS, &result);
	if (result != GL_TRUE)
	{
		glGetProgramiv(programmId, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> ProgramErrorMessage(std::max(infoLogLength, int(1)));
		glGetProgramInfoLog(programmId, infoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(
			stderr,
			"ERROR: Could not link programm: %s \n", &ProgramErrorMessage[0]
			);
		getchar();
		exit(-1);
	}

	glUseProgram(programmId);
	CheckError("UseProgramm()");

}

void Renderer::destroyShader(void)
{
	glUseProgram(0);
	CheckError("UserProgram(0)");
	glDetachShader(programmId, vertexShaderId);
	CheckError("DetachShader(, vertexShader)");
	glDetachShader(programmId, fragmentShaderId);
	CheckError("DetachShader(, framentShader)");
	glDeleteShader(fragmentShaderId);
	CheckError("DeleteShader(fragmentShader)");
	glDeleteShader(vertexShaderId);
	CheckError("DeleteShader(vertexShader)");
	glDeleteProgram(programmId);
	CheckError("DelteProgramm()");
}

void Renderer::createStructure(void)
{
	//VAO
	glGenVertexArrays(1, &vaoId);
	CheckError("GenVertexArrays(1, vaoId");
	glBindVertexArray(vaoId);
	CheckError("BindVertexArray(vaoId)");

	char* objpath = ".\\..\\..\\..\\..\\assets\\vader.obj";
	if (true != loadOBJ(objpath, vertices, uvs, normals))
	{
		std::cout << "Loading object failed! " << objpath << std::endl;
		getchar();
		exit(-1);
	}
		
	glGenBuffers(1, &structureVertexId);
	CheckError("genBuffer(VertexId");
	glEnableVertexAttribArray(0);
	CheckError("EnableVertexAttriArray(0)");
	glBindBuffer(GL_ARRAY_BUFFER, structureVertexId);
	CheckError("BindBuffer(structureVertexId");
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	CheckError("BufferData(vertices");
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	CheckError("VertexAttribPointer(0, ");
	//glDisableVertexAttribArray(0);

	glGenBuffers(1, &normalId);
	CheckError("genBuffers(normalId");
	glEnableVertexAttribArray(1);
	CheckError("EnableVertexAttriArray(1)");
	glBindBuffer(GL_ARRAY_BUFFER, normalId);
	CheckError("BindBuffer(normalId");
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	CheckError("BufferData(normals");
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	CheckError("VertexAttribPointer(1, ");
	//glDisableVertexAttribArray(1);

	//glGenBuffers(1, &structureUvId);
	//glBindBuffer(GL_ARRAY_BUFFER, structureUvId);
	//glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	//initial camera position and orientation
	camera.p_camera = glm::vec3(0.0f, 3.0f, 10.0f);
	camera.p_lookat = glm::vec3(0.0f, 0.0f, 0.0f);
	camera.v_up = glm::vec3(0.0f, 1.0f, 0.0f);

	//init view pyramid
	view.beta = 45;
	view.z_n = 0.1f;
	view.z_f = 100.0f;
	view.aspect = static_cast<float>(viewport_width / viewport_height);
	view.h = -2.0f * view.z_n * tan(view.beta / 2.0f);

	//light posiiton
	glm::vec3 light = glm::vec3(-3.0f, 5.0f, 0.0f);
	lightId = glGetUniformLocation(programmId, "light");
	CheckError("GetUniformLocation(light");
	glUniform3fv(lightId, 1, glm::value_ptr(light));
	CheckError("Uniform1fv(lightId");

	//diffuse reflectance
	glm::vec4 f_r = glm::vec4(0.5f, 0.5f, 0.3f, 1.0f) / PI;
	GLint f_rId = glGetUniformLocation(programmId, "f_r");
	glUniform4fv(f_rId, 1, glm::value_ptr(f_r));

	projectionMatrix = glm::perspective(view.beta, view.aspect, view.z_n, view.z_f);

	viewMatrix = glm::lookAt(
		camera.p_camera,
		camera.p_lookat,
		camera.v_up
		);

	modelMatrix = glm::mat4(1.0f);

	//upload matrix to shader
	GLint uniTrans = glGetUniformLocation(programmId, "proj");
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	GLint uniView = glGetUniformLocation(programmId, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	uniModel = glGetUniformLocation(programmId, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void Renderer::destroyStructure(void)
{

}

void Renderer::calculateNormals(const int ind_size, const GLuint* indices, const std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals)
{
	static int offset = 3;

	glm::fvec3 v1, v2, v3, a, b, cross;

	//std::cout << "size of indices: " << ind_size << "sizeof(unsigned int) " << sizeof(GLuint) <<  "  size of vertices: " << vertices.size() << std::endl;

	for (unsigned int i = 0; (i < ind_size / sizeof(GLuint)); i += offset)
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
		if (i % 2 == 0)
		{
			normals.push_back(cross);
			normals.push_back(cross);
		}

	}
}

bool Renderer::loadOBJ(
	const char * path,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
	)
{
	std::cout << "Loading OBJ file " << path << "..." << std::endl;

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(path, "r");
	if (file == NULL){
		std::cout << "Impossible to open the file ! Are you in the right path ?" << std::endl;
		getchar();
		return false;
	}

	while (1){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9){
				std::cout << "File can't be read by our simple parser :-( Try exporting with other options" << std::endl;
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i<vertexIndices.size(); i++){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		//std::cout << "vertex(" << vertex.x << "," << vertex.y << "," << vertex.z << ") / normal(";
		//std::cout << normal.x << "," << normal.y << "," << normal.z << ")" << std::endl;

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);

	}

	return true;
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
