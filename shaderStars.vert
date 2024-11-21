#version 450 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in float opacity;
layout (location = 2) in float pulsation;

uniform mat4 proj;
uniform mat4 model;
uniform mat4 view;

uniform vec3 posCam;

out float opacityFrag;
out float pulsationFrag;

void main(void)
{
    vec4 vertex2 = vec4(vertex + posCam, 1.0);

    gl_Position = proj * view * model * vertex2;
	
	opacityFrag = opacity;
	pulsationFrag = pulsation;
}
