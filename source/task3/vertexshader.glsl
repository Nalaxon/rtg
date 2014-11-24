#version 440
layout(location=0) in vec3 in_Vertex;
layout(location=2) in vec3 in_normals;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform vec3 light;
uniform vec3 f_r;



out vec4 FragmentColor;
void main(void)
{
	vec3 l =  f_r * vec3(1.0f) * max(in_normals * (light - in_Vertex), 0);

	gl_Position = proj *view *model * vec4(in_Vertex, 1.0f);
	
    FragmentColor = vec4(l, 1.0f);
}