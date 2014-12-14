#version 440
in vec4 FragmentNormal;
in vec4 pos;

uniform vec4 f_r;
uniform vec3 light;

out vec4 Color;

void main(void)

{
  
   vec4 l =  vec4(1.0f) * max(dot(FragmentNormal, normalize(vec4(light, 1.0f) - pos)), 0.0f);

   Color =  f_r * l;
   Color.w = 1.0f;
}