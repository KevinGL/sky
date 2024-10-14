#version 450 core

uniform vec3 horizonColor;
uniform vec3 zenithColor;

//uniform double opacity;
uniform vec3 sunPos;

in vec3 frag;

out vec4 FragColor;

float PI = 3.14159265359;

float largSun = 8.0;//2.0;

double calculOpacity()
{
	float ecartMax = 0.5;
	
	vec3 vecteurProjXY = vec3(sunPos.x, sunPos.y, 0.0);
	
	float anglePlanXY = acos(dot(normalize(vecteurProjXY), normalize(sunPos))) * 180/PI;
	
	if(sunPos.z < 0)
	{
		anglePlanXY *= -1;
	}
	
	float opacity;
	
	if(anglePlanXY < -ecartMax)
		opacity = 0.0;
	else
	if(anglePlanXY >= -ecartMax && anglePlanXY < ecartMax)
	{
		float coef = 1.0/(2 * ecartMax);
		
		opacity = coef * anglePlanXY + 0.5;
	}
	else
		opacity = 1.0;
	
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
