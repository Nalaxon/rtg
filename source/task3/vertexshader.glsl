#version 440
layout(location=0) in vec3 in_Vertex;

out vec4 FrontColor;
void main(void)
{
	gl_Position = vec4(in_Vertex, 1.0f);
    FrontColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}