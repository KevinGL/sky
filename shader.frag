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

void drawLayer1(float angleSun, float width)
{
	float alpha = log(100.0) / (width * width);
	float factorMix = exp(-alpha * angleSun * angleSun);
	
	vec3 color = mix(FragColor.rgb, colorSunFirstLayer, factorMix);
	
	FragColor = vec4(color, FragColor.a);
}

void drawLayer2(float angleSun, float width)
{
	float alpha = log(100.0) / ((width/2) * (width/2));
	float factorMix = exp(-alpha * angleSun * angleSun);
	
	vec3 color = mix(FragColor.rgb, colorSunSecondLayer, factorMix);
	
	FragColor = vec4(color, FragColor.a);
}

void drawSun(float angleSun)
{
	/*vec2 toSun = normalize(sunPos.xy);
	vec2 toFrag = normalize((frag - posCam).xy);
	float angleH = acos(dot(toSun, toFrag));// * 180 / PI;
	float a = haloWidth;
	float b = haloWidth / 2;
	//float angleV = b * sqrt(1.0 - pow(angleH / a, 2));		//Ellipse
	//float width = sqrt(pow(angleH, 2) + pow(angleV, 2));
	float width = (a * b) / sqrt(b * cos(angleH) * b * cos(angleH) + a * sin(angleH) * a * sin(angleH));
	
	drawLayer1(angleSun, width);
	drawLayer2(angleSun, width);*/
	
	drawLayer1(angleSun, haloWidth);
	drawLayer2(angleSun, haloWidth);
}

void main(void)
{
    float angle = frag.z / length(frag);
	
	float paramMix = abs(sin(angle));
	
	FragColor = vec4(mix(horizonColor, zenithColor, paramMix), opacity);
	
	float angleSun = acos(dot(normalize(sunPos), normalize(frag))) * 180 / PI;
	
	if(angleSun <= 2 * haloWidth)
	{
		//drawSun(angleSun);
	}
	
	//if(frag.y > 1.9)
	{
		float a = 2;
		float b = 1;
		vec3 center = vec3(0.0);
		float alpha = (frag.z - center.z) / (frag.x - center.x);
		float beta = -alpha * frag.x + frag.z;
		float A = b * b + alpha * alpha * a * a;
		float B = 2 * alpha * beta * a * a;
		float C = a * a * beta * beta - a * a * b * b;
		float delta = B * B - 4 * A * C;
		float x = (-B - sqrt(delta)) / (2 * A);
		float z = alpha * x + beta;
		float distMax = sqrt(pow(x - center.x, 2) + pow(z - center.z, 2));
		float dist = sqrt(pow(frag.x - center.x, 2) + pow(frag.z - center.z, 2));
		
		float gamma = log(100.0) / (distMax * distMax);
		float factorMix = exp(-gamma * dist * dist);
		
		vec3 color = mix(FragColor.rgb, colorSunFirstLayer, factorMix);
		
		FragColor = vec4(color, FragColor.a);
	}
}
