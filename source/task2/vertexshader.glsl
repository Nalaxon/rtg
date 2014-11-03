layout(location=1) in vec4 in_Color;
void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
        gl_FrontColor = in_Color;
        gl_BackColor  = in_Color;
}