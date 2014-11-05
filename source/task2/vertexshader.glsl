#version 330
layout(location=0) in vec3 in_Vertex;
layout(location=1) in vec4 in_Color;

out vec4 FrontColor;
void main()
{
	gl_Position = vec4(in_Vertex, 1.0f);
        FrontColor = in_Color;
}