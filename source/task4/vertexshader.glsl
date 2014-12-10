#version 440
layout(location=0) in vec3 in_Vertex;
layout(location=1) in vec3 in_normals;
layout(location=2) in vec2 in_uv;


uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec4 FragmentNormal;
out vec4 pos;
out vec2 FragUV;

void main(void)
{

	pos = proj *view *model * vec4(in_Vertex, 1.0f);
	gl_Position = pos;

	FragmentNormal = vec4(in_normals, 1.0f);

    FragUV = in_uv;
}