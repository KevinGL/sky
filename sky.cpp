#include <GL/glew.h>
#include "sky.h"

void Sky::Init(tm &dat, const float lat, glm::vec3 hColor, glm::vec3 zColor, const float spd)
{
    speed = spd;
    date = dat;
    latitude = lat;

    if(date.tm_mon <= 11)
    {
        date.tm_year = 71;
    }

    else
    if(date.tm_mon == 12)
    {
        if(date.tm_mday <= 20)
        {
            date.tm_year = 70;
        }
        else
        {
            date.tm_year = 70;
        }
    }

    horizonColor = glm::vec3(hColor.x/255, hColor.y/255, hColor.z/255);
    zenithColor = glm::vec3(zColor.x/255, zColor.y/255, zColor.z/255);

    shader = loadShader("../Mini-libs/sky/shaderSky.");
    shaderSun = loadShader("../Mini-libs/sky/shaderSun.");
    shaderStars = loadShader("../Mini-libs/sky/shaderStars.");

    InitVBO();

    chrono = SDL_GetTicks();
}

void Sky::InitVBO()
{
    float vertex[6 * 2 * 3 * 3]=
    {
        -SKY_SIZE, -SKY_SIZE, -SKY_SIZE,          //Bas
        -SKY_SIZE, SKY_SIZE, -SKY_SIZE,
        SKY_SIZE, -SKY_SIZE, -SKY_SIZE,

        SKY_SIZE, -SKY_SIZE, -SKY_SIZE,
        -SKY_SIZE, SKY_SIZE, -SKY_SIZE,
        SKY_SIZE, SKY_SIZE, -SKY_SIZE,

        //////////////

        -SKY_SIZE, -SKY_SIZE, SKY_SIZE,          //Haut
        -SKY_SIZE, SKY_SIZE, SKY_SIZE,
        SKY_SIZE, -SKY_SIZE, SKY_SIZE,

        SKY_SIZE, -SKY_SIZE, SKY_SIZE,
        -SKY_SIZE, SKY_SIZE, SKY_SIZE,
        SKY_SIZE, SKY_SIZE, SKY_SIZE,

        //////////////

        -SKY_SIZE, -SKY_SIZE, -SKY_SIZE,          //Gauche
        -SKY_SIZE, -SKY_SIZE, SKY_SIZE,
        -SKY_SIZE, SKY_SIZE, -SKY_SIZE,

        -SKY_SIZE, SKY_SIZE, -SKY_SIZE,
        -SKY_SIZE, -SKY_SIZE, SKY_SIZE,
        -SKY_SIZE, SKY_SIZE, SKY_SIZE,

        //////////////

        SKY_SIZE, -SKY_SIZE, -SKY_SIZE,          //Droite
        SKY_SIZE, -SKY_SIZE, SKY_SIZE,
        SKY_SIZE, SKY_SIZE, -SKY_SIZE,

        SKY_SIZE, SKY_SIZE, -SKY_SIZE,
        SKY_SIZE, -SKY_SIZE, SKY_SIZE,
        SKY_SIZE, SKY_SIZE, SKY_SIZE,

        //////////////

        -SKY_SIZE, -SKY_SIZE, -SKY_SIZE,          //Derri�re
        -SKY_SIZE, -SKY_SIZE, SKY_SIZE,
        SKY_SIZE, -SKY_SIZE, -SKY_SIZE,

        SKY_SIZE, -SKY_SIZE, -SKY_SIZE,
        -SKY_SIZE, -SKY_SIZE, SKY_SIZE,
        SKY_SIZE, -SKY_SIZE, SKY_SIZE,

        //////////////

        -SKY_SIZE, SKY_SIZE, -SKY_SIZE,          //Devant
        -SKY_SIZE, SKY_SIZE, SKY_SIZE,
        SKY_SIZE, SKY_SIZE, -SKY_SIZE,

        SKY_SIZE, SKY_SIZE, -SKY_SIZE,
        -SKY_SIZE, SKY_SIZE, SKY_SIZE,
        SKY_SIZE, SKY_SIZE, SKY_SIZE
    };

    glGenVertexArrays(1, &vbo.VAO);
    glGenBuffers(1, &vbo.VBO);

    glBindVertexArray(vbo.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, vbo.VBO);

    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * 3 * 3 * sizeof(float), vertex, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    //InitSun();
    InitStars();
}

void Sky::Draw(glm::vec3 posCam, glm::mat4 model, glm::mat4 view, glm::mat4 proj)
{
    glDisable(GL_DEPTH_TEST);

    DrawStars(posCam, model, view, proj);

    glUseProgram(shader);

    glBindVertexArray(vbo.VAO);

    glUniform3f(glGetUniformLocation(shader, "posCam"), posCam.x, posCam.y, posCam.z);

    glUniform3f(glGetUniformLocation(shader, "horizonColor"), horizonColor.x, horizonColor.y, horizonColor.z);
    glUniform3f(glGetUniformLocation(shader, "zenithColor"), zenithColor.x, zenithColor.y, zenithColor.z);

    CalculSunPos();

    epsilon = fabs(sunHeightMin) / 10;
    const float theta = -4 * epsilon;

    float opacity;

	if(sunPos.z < theta)
	{
		opacity = 0.0f;
	}

	else
	if(sunPos.z >= theta && sunPos.z < 0.0f)
	{
		const float coef = 1.0f / fabs(theta);
		opacity = coef * sunPos.z + 1.0f;
	}

	else
	{
		opacity = 1.0f;
	}

	//////////////////////////////

	const glm::vec3 colorSunFirstLayer1 = glm::vec3(1.0f, 1.0f, 0.788f);
    const glm::vec3 colorSunSecondLayer1 = glm::vec3(1.0f);

    const glm::vec3 colorSunFirstLayer2 = glm::vec3(1.0f, 0.588f, 0.0627f);
    const glm::vec3 colorSunSecondLayer2 = glm::vec3(1.0f, 0.745f, 0.4196f);

    float haloWidth;
	const float haloWidthMin = 8.0f;
	const float haloWidthMax = 32.0f;
	const float epsilonHalo = 12 * epsilon;

    glm::vec3 colorSunFirstLayer, colorSunSecondLayer;

	if(sunPos.z < -epsilonHalo)
    {
        colorSunFirstLayer = colorSunFirstLayer2;
        colorSunSecondLayer = colorSunSecondLayer2;

        haloWidth = haloWidthMin;
    }

    else
	if(sunPos.z >= -epsilonHalo && sunPos.z < epsilonHalo)
    {
        const glm::vec3 coefFirstLayer = (colorSunFirstLayer1 - colorSunFirstLayer2) / (2 * epsilonHalo);
        const glm::vec3 coefSecondLayer = (colorSunSecondLayer1 - colorSunSecondLayer2) / (2 * epsilonHalo);

        const glm::vec3 ordFirstLayer = -coefFirstLayer * epsilonHalo + colorSunFirstLayer1;
        const glm::vec3 ordSecondLayer = -coefSecondLayer * epsilonHalo + colorSunSecondLayer1;

        colorSunFirstLayer = coefFirstLayer * sunPos.z + ordFirstLayer;
        colorSunSecondLayer = coefSecondLayer * sunPos.z + ordSecondLayer;

        ///////////////////////////////////////////

        const float coefWidth = (haloWidthMax - haloWidthMin) / (2 * epsilonHalo);
        const float ordWidth = -coefWidth * epsilonHalo + haloWidthMax;

        haloWidth = coefWidth *  sunPos.z + ordWidth;
    }

    else
    {
        colorSunFirstLayer = colorSunFirstLayer1;
        colorSunSecondLayer = colorSunSecondLayer1;

        haloWidth = haloWidthMax;
    }

	///////////////////////////////

	glUniform3f(glGetUniformLocation(shader, "colorSunFirstLayer"), colorSunFirstLayer.x, colorSunFirstLayer.y, colorSunFirstLayer.z);
    glUniform3f(glGetUniformLocation(shader, "colorSunSecondLayer"), colorSunSecondLayer.x, colorSunSecondLayer.y, colorSunSecondLayer.z);
    glUniform3f(glGetUniformLocation(shader, "sunPos"), sunPos.x, sunPos.y, sunPos.z);
    glUniform1f(glGetUniformLocation(shader, "haloWidth"), haloWidth);

	glUniform1f(glGetUniformLocation(shader, "opacity"), opacity);

    glUniformMatrix4fv(glGetUniformLocation(shader, "proj"), 1, false, glm::value_ptr(proj));
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, false, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));

    glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3 * 3);

    glBindVertexArray(0);

    //DrawSun(posCam, model, view, proj);

    glEnable(GL_DEPTH_TEST);
}

/*double Sky::SkyOpacity()
{
    tm sol;

    sol.tm_mday=21;
    sol.tm_mon=12;
    sol.tm_year=70;
    sol.tm_hour=0;
    sol.tm_min=0;
    sol.tm_sec=0;

    time_t now=mktime(&date);
    time_t solsticeDec=mktime(&sol);

    timestampYear=now-solsticeDec;

    //std::cout << timestampYear << std::endl;

    const float period=365.25f*24.0f*3600.0f;
    const float pulsation=(2*M_PI)/period;

    float amplitude=(12.0f/67.0f)*latitude;

    if(amplitude>12.0f)
        amplitude=12.0f;
    else
    if(amplitude<-12.0f)
        amplitude=-12.0f;

    const float average=12.0f;

    longDay=-amplitude*cos(pulsation*timestampYear)+average;

    const double sunshine=((24.0-longDay)/2)*3600000.0;
    const double sunset=(24.0-sunshine/3600000.0)*3600000.0;
    const double duration=DURATION_TRANS*3600000.0;

    hour=(date.tm_hour*3600.0+date.tm_min*60.0+date.tm_sec)*1000.0 + (SDL_GetTicks()-chrono)*speed;

    if(hour<0)
        hour+=24.0;
    else
    if(hour>24*3600000.0)
        hour-=24.0*3600000.0;

    //std::cout << hour << " " << sunshine << " " << sunset << std::endl;

    //std::cout << longDay << " " << hour/3600000.0 << " (" << sunshine/3600000.0 << " " << sunset/3600000.0 << ")" << std::endl;
    //std::cout << sunshine/3600000.0 << " " << sunset/3600000.0 << std::endl;

    double opacity;

    if(hour<sunshine)
        opacity=0.0;        //Avant lever du Soleil

    else
    if(hour>=sunshine && hour<sunshine+duration)
    {
        const double coef=1.0/duration;
        const double ord=-coef*sunshine+0.0;

        opacity=coef*hour+ord;           //Lever Soleil
    }

    else
    if(hour>=sunshine+duration && hour<sunset-duration)
        opacity=1.0;        //Jour

    else
    if(hour>=sunset-duration && hour<sunset)
    {
        const double coef=-1.0/duration;
        const double ord=-coef*sunset+0.0;

        opacity=coef*hour+ord;           //Coucher Soleil
    }

    else
    if(hour>=sunset)
        opacity=0.0;

    //std::cout << amplitude << " " << longDay << " " << hour/3600000.0 << " => " << opacity << std::endl;
    //std::cout << sunshine/3600000.0 << " " << sunset/3600000.0 << " " << hour/3600000.0 << " => " << opacity << std::endl;

    return opacity;
}*/
