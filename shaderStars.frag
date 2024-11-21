#version 450 core

uniform float chrono;

in float opacityFrag;
in float pulsationFrag;

out vec4 FragColor;

void main(void)
{
    float opacity = (opacityFrag / 4) * cos(pulsationFrag * chrono) + 3 * opacityFrag / 4;
	
	FragColor = vec4(1.0, 1.0, 1.0, opacity);
}
