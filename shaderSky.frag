#version 450 core

uniform vec3 horizonColor;
uniform vec3 zenithColor;

//uniform double opacity;
uniform vec3 sunPos;
uniform float epsilon;
uniform float opacity;

in vec3 frag;

out vec4 FragColor;

float PI = 3.14159265359;

float largSun = 8.0;//2.0;

void drawSun(float angleSun)
{
	float coef = -1.0/largSun;
	
	float facteurMix = coef * angleSun+  1.0;
	
	vec3 color = mix(FragColor.rgb, vec3(1.0, 1.0, 0.702), facteurMix);
	
	FragColor = vec4(color, FragColor.a);
}

void main(void)
{
    float angle = frag.z/length(frag);
	
	float paramMix = abs(sin(angle));
	
	FragColor = vec4(mix(horizonColor, zenithColor, paramMix), opacity);
	
	float angleSun = acos(dot(normalize(sunPos), normalize(frag))) * 180/PI;
	
	if(angleSun <= largSun)
	{
		drawSun(angleSun);
	}
}
