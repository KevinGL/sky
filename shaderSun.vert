#version 450 core

layout (location = 0) in vec3 vertex;

uniform mat4 proj;
uniform mat4 model;
uniform mat4 view;

uniform vec3 posCam;

out vec3 frag;

void main(void)
{
    vec4 vertex2 = vec4(vertex, 1.0);

    gl_Position = proj * view * model * vertex2;
	
	frag = vertex;
}
