#version 450 core

uniform vec3 colorLayer;

uniform float radius;
//uniform float opacityGlobal;

in vec3 frag;

out vec4 FragColor;

float PI = 3.14159265359;

void main(void)
{
    vec3 center = vec3(0.0);
	float dist = sqrt(pow(frag.x - center.x, 2) + pow(frag.z - center.z, 2));
	
	float gamma = log(100.0) / (radius * radius);
	float opacity = exp(-gamma * dist * dist);
	
	FragColor = vec4(colorLayer, opacity);
}
