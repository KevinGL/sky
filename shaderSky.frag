#version 450 core

uniform vec3 horizonColor;
uniform vec3 zenithColor;

//uniform double opacity;
uniform vec3 sunPos;
uniform float epsilon;
uniform float heightSun;

in vec3 frag;

out vec4 FragColor;

float PI = 3.14159265359;

float largSun = 8.0;//2.0;

double calculOpacity()
{
	float opacity;
	
	if(heightSun >= 0.0)
	{
		opacity = 1.0;
	}
	
	else
	if(heightSun >= -epsilon && heightSun < 0.0)
	{
		float coef = 1.0 / epsilon;
		opacity = coef * heightSun + 1.0;
	}
	
	else
	{
		opacity = 0.0;
	}
	
	return opacity;
}

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
	
	FragColor = vec4(mix(horizonColor, zenithColor, paramMix), calculOpacity());
	
	float angleSun = acos(dot(normalize(sunPos), normalize(frag))) * 180/PI;
	
	if(angleSun <= largSun)
	{
		drawSun(angleSun);
	}
}
