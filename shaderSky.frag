#version 450 core

uniform vec3 horizonColor;
uniform vec3 zenithColor;

uniform vec3 colorSunFirstLayer;
uniform vec3 colorSunSecondLayer;
uniform float haloWidth;

uniform vec3 sunPos;
uniform float epsilon;
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

void main(void)
{
    float angle = frag.z / length(frag);
	
	float paramMix = abs(sin(angle));
	
	FragColor = vec4(mix(horizonColor, zenithColor, paramMix), opacity);
	
	float angleSun = acos(dot(normalize(sunPos), normalize(frag))) * 180 / PI;
	
	if(angleSun <= haloWidth)
	{
		drawSun(angleSun);
	}
}
