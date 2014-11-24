#version 440
layout(location=0) in vec3 in_Vertex;
layout(location=2) in vec3 in_normals;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec4 FragmentColor;
void main(void)
{
	gl_Position = proj *view *model * vec4(in_Vertex, 1.0f);
    FragmentColor = vec4(in_normals, 1.0f);
}