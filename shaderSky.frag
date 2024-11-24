#version 450 core

uniform vec3 horizonColor;
uniform vec3 zenithColor;

uniform vec3 colorSunFirstLayer;
uniform vec3 colorSunSecondLayer;
uniform float haloWidth;

uniform vec3 sunPos;
uniform float opacity;

in vec3 frag;

out vec4 FragColor;

float PI = 3.14159265359;

void drawSun(float angleSun)
{
	float alpha = log(100.0) / (haloWidth * haloWidth);
	float factorMix = exp(-alpha * angleSun * angleSun);
	
	vec3 color = mix(FragColor.rgb, colorSunFirstLayer, factorMix);
	
	FragColor = vec4(color, FragColor.a);
	
	//////////////////////////////////////////////////////////////////////////////////
	
	alpha = log(100.0) / ((haloWidth/2) * (haloWidth/2));
	factorMix = exp(-alpha * angleSun * angleSun);
	
	color = mix(FragColor.rgb, colorSunSecondLayer, factorMix);
	
	FragColor = vec4(color, FragColor.a);
}

vec3 calculColorSunset()
{
	vec3 colorSunset = vec3(1.0, 127.0 / 255, 39.0 / 255);
	vec3 toSun = normalize(sunPos);
	vec3 toSunProj = vec3(sunPos.xy, 0.0);
	vec3 toFragProj = normalize(vec3(frag.xy, 0.0));
	
	float dotProd = dot(toSun, toSunProj);
	float dotLimit = 0.8;
	float coef = 1.0 / (1.0 - dotLimit);
	float ord = -coef * 1.0 + 1.0;
	
	float factor = coef * dotProd + ord;
	
	if(factor < 0.0)
	{
		factor = 0.0;
	}
	
	if(factor > 1.0)
	{
		factor = 1.0;
	}
	
	vec3 colorSwitchSunHeight = mix(horizonColor, colorSunset, factor);
	
	dotLimit = -1.0;
	coef = -1.0 / (1.0 - dotLimit);
	ord = -coef * 1.0 + 0.0;
	dotProd = dot(toFragProj, toSunProj);
	
	factor = coef * dotProd + ord;
	
	if(factor < 0.0)
	{
		factor = 0.0;
	}
	
	if(factor > 1.0)
	{
		factor = 1.0;
	}
	
	//return mix(horizonColor, colorSunset, factor);
	return mix(colorSwitchSunHeight, horizonColor, factor);
}

void main(void)
{
    float angle = frag.z / length(frag);
	
	float paramMix = abs(sin(angle));
	
	vec3 horizonColor2 = calculColorSunset();
	
	FragColor = vec4(mix(horizonColor2, zenithColor, paramMix), opacity);
	
	float angleSun = acos(dot(normalize(sunPos), normalize(frag))) * 180 / PI;
	
	if(angleSun <= haloWidth)
	{
		drawSun(angleSun);
	}
}
