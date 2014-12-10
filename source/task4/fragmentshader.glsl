#version 440
in vec4 FragmentNormal;
in vec4 pos;
in vec2 FragUV;

uniform vec4 f_r;
uniform vec3 light;
uniform sampler2D tex;

out vec4 Color;

void main(void)

{
  
    vec4 l =  vec4(1.0f) * max(dot(FragmentNormal, (vec4(light, 1.0f) - pos)), 0);

    Color.rgb = texture(tex, FragUV).rgb;
}