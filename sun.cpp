#include <GL/glew.h>
#include "sky.h"

void Sky::InitSun()
{
    float vertices[3 * 6] =
    {
        -0.5f, 0.0f, -0.5f,
        -0.5f, 0.0f, 0.5f,
        0.5f, 0.0f, -0.5f,

        0.5f, 0.0f, -0.5f,
        -0.5f, 0.0f, 0.5f,
        0.5f, 0.0f, 0.5f
    };

    glGenVertexArrays(1, &sun.VAO);
    glGenBuffers(1, &sun.VBO);

    glBindVertexArray(sun.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, sun.VBO);

    glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sky::DrawSun(glm::vec3 posCam, glm::mat4 model, glm::mat4 view, glm::mat4 proj)
{
    float haloHeight = 2.0f;
    const float wMin = haloHeight;
    const float wMax = 4 * haloHeight;
    float haloWidth;
    const float coefSizeMin = 0.5f;
    float coefSize;
	/*const float opacityMin = 0.1f;
	float opacity;

	if(sunPos.z < 0.0f)
    {
        opacity = opacityMin;
    }
    else
    if(sunPos.z >= 0.0f && sunPos.z < epsilon)
    {
        const float coef = (1.0f - opacityMin) / epsilon;
        opacity = coef * sunPos.z + opacityMin;
    }
    else
    {
        opacity = 1.0f;
    }*/

    /////////////////////////////////////////

    const float theta = epsilon;

    if(sunPos.z <= -theta)
    {
        haloWidth = wMax;
        coefSize = coefSizeMin;
    }
    else
    if(sunPos.z > -theta && sunPos.z <= theta)
    {
        float coef = (wMin - wMax) / (2 * theta);
        float ord = -coef * theta + wMin;
        haloWidth = coef * sunPos.z + ord;

        coef = (1.0f - coefSizeMin) / (2 * theta);
        ord = -coef * theta + 1.0f;
        coefSize = coef * sunPos.z + ord;
    }
    else
    {
        haloWidth = wMin;
        coefSize = 1.0f;
    }

	const glm::vec3 colorSunFirstLayer1 = glm::vec3(1.0f, 1.0f, 0.788f);
    const glm::vec3 colorSunSecondLayer1 = glm::vec3(1.0f);

    const glm::vec3 colorSunFirstLayer2 = glm::vec3(1.0f, 0.0f, 0.0f);
    const glm::vec3 colorSunSecondLayer2 = glm::vec3(1.0f, 0.4902, 0.1529f);

    glm::vec3 colorSunFirstLayer, colorSunSecondLayer;

    if(sunPos.z <= -epsilon)
    {
        colorSunFirstLayer = colorSunFirstLayer2;
        colorSunSecondLayer = colorSunSecondLayer2;
    }
    else
    if(sunPos.z > -epsilon && sunPos.z <= epsilon)
    {
        glm::vec3 coef = (colorSunFirstLayer1 - colorSunFirstLayer2) / (2 * epsilon);
        glm::vec3 ord = -coef * epsilon + colorSunFirstLayer1;
        colorSunFirstLayer = coef * sunPos.z + ord;

        coef = (colorSunSecondLayer1 - colorSunSecondLayer2) / (2 * epsilon);
        ord = -coef * epsilon + colorSunSecondLayer1;
        colorSunSecondLayer = coef * sunPos.z + ord;
    }
    else
    {
        colorSunFirstLayer = colorSunFirstLayer1;
        colorSunSecondLayer = colorSunSecondLayer1;
    }

    const glm::vec3 vecProj = glm::normalize(glm::vec3(sunPos.x, sunPos.y, 0.0f));
    const glm::vec3 toSun = glm::normalize(glm::vec3(sunPos.x, sunPos.y, sunPos.z));
    float angleH = acos(glm::dot(vecProj, glm::vec3(0.0f, 1.0f, 0.0f))) * 180 / M_PI;
    float angleV = acos(glm::dot(vecProj, toSun)) * 180 / M_PI;

    if(sunPos.x > 0)
    {
        angleH *= -1;
    }

    if(sunPos.z < 0)
    {
        angleV *= -1;
    }

    //std::cout << angleV << std::endl;

    model = glm::translate(posCam.x, posCam.y, posCam.z);
    model *= glm::rotate(angleH, 0.0f, 0.0f, 1.0f);
    model *= glm::rotate(angleV, 1.0f, 0.0f, 0.0f);
    model *= glm::translate(0.0f, 2.0f, 0.0f);
    model *= glm::scale(coefSize * haloWidth, 1.0f, coefSize * haloHeight);

    glUseProgram(shaderSun);

    glBindVertexArray(sun.VAO);

    glUniformMatrix4fv(glGetUniformLocation(shaderSun, "proj"), 1, false, glm::value_ptr(proj));
    glUniformMatrix4fv(glGetUniformLocation(shaderSun, "model"), 1, false, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderSun, "view"), 1, false, glm::value_ptr(view));

    //glUniform1f(glGetUniformLocation(shaderSun, "opacityGlobal"), opacity);

	glUniform3f(glGetUniformLocation(shaderSun, "colorLayer"), colorSunFirstLayer.x, colorSunFirstLayer.y, colorSunFirstLayer.z);
	glUniform1f(glGetUniformLocation(shaderSun, "radius"), 0.5f);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glUniform3f(glGetUniformLocation(shaderSun, "colorLayer"), colorSunSecondLayer.x, colorSunSecondLayer.y, colorSunSecondLayer.z);
    glUniform1f(glGetUniformLocation(shaderSun, "radius"), 0.25f);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}

void Sky::CalculSunPos()
{
    const float cylinderRadius = 1.0f;
    const float obliquity = 23.0f;
    const float cylinderHeight = cylinderRadius * tan(obliquity * M_PI / 180);
    const float cylinderAngle = 90.0f - latitude;

    unsigned int daySinceDec21 = date.tm_mday + 11;
    unsigned int daysByMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    for(size_t i = 0 ; i < date.tm_mon ; i++)
    {
        daySinceDec21 += daysByMonth[i];
    }

    //std::cout << daySinceDec21 << std::endl;

    const float period = 365.25f;
    const float pulsation = (2 * M_PI) / period;
    const float circleHeight = -cylinderHeight * cos(pulsation * daySinceDec21);

    hour = (date.tm_hour * 3600.0 + date.tm_min * 60.0 + date.tm_sec) * 1000.0 + (SDL_GetTicks() - chrono) * speed;

    if(hour < 0)
    {
        hour += 24.0 * 3600000.0;
    }

    else
    if(hour > 24 * 3600000.0)
    {
        hour -= 24.0 * 3600000.0;
    }

    const float hourHours = hour / 3600000.0f;
    const float alpha = -(hourHours / 24) * 360;

    sunPos = glm::vec4(0.0f, cylinderRadius, 0.0f, 1.0f);
    glm::vec4 sunPosMax = glm::vec4(0.0f, cylinderRadius, 0.0f, 1.0f);
    glm::vec4 sunPosMin = glm::vec4(0.0f, cylinderRadius, 0.0f, 1.0f);

    const glm::mat4 cylinderIncl = glm::rotate(-cylinderAngle, 1.0f, 0.0f, 0.0f);
    const glm::mat4 circleUp = glm::translate(0.0f, 0.0f, circleHeight);
    const glm::mat4 circleRot = glm::rotate(alpha, 0.0f, 0.0f, 1.0f);

    glm::mat4 circleRotMidday = glm::rotate(180.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 circleRotMidnight = glm::rotate(0.0f, 0.0f, 0.0f, 1.0f);

    sunPos = circleRot * sunPos;
    sunPos = circleUp * sunPos;
    sunPos = cylinderIncl * sunPos;

    sunPosMax = circleRotMidday * sunPosMax;
    sunPosMax = circleUp * sunPosMax;
    sunPosMax = cylinderIncl * sunPosMax;

    sunPosMin = circleRotMidnight * sunPosMin;
    sunPosMin = circleUp * sunPosMin;
    sunPosMin = cylinderIncl * sunPosMin;

    sunHeightMin = sunPosMin.z;
    sunHeightMax = sunPosMax.z;

    //std::cout << hourHours << " => " << alpha << " => " << sunPos.x << " " << sunPos.y << " " << sunPos.z << std::endl;
    //printf("%f => %f (%f - %f)                \r", hourHours, sunPos.z, sunHeightMin, sunHeightMax);

    //std::cout << "Sud : " << angleS << " Nord : " << angleN << std::endl;

    //std::cout << hourHours << " " << angleS << " " << angleN << " => " << sunPos.x << " " << sunPos.y << " " << sunPos.z << std::endl;

    /*hour = (date.tm_hour * 3600.0 + date.tm_min * 60.0 + date.tm_sec) * 1000.0 + (SDL_GetTicks() - chrono) * speed;

    if(hour < 0)
    {
        hour += 24.0 * 3600000.0;
    }

    else
    if(hour > 24 * 3600000.0)
    {
        hour -= 24.0 * 3600000.0;
    }

    const float hourHours = hour / 3600000.0f;
    const float alpha = (hourHours / 24) * 360;

    heightSun = -declination * cos(alpha * M_PI / 180);

    sunPos.x = sin(alpha * M_PI / 180);
    sunPos.y = cos(alpha * M_PI / 180);
    sunPos.z = 0.0f;
    sunPos.w = 1.0f;

    //std::cout << hourHours << " => " << alpha << std::endl;

    glm::mat4 rotation = glm::rotate(-declination, 1.0f, 0.f, 0.0f);

    sunPos = rotation * sunPos;*/

    //std::cout << hourHours << " => " << heightSun << std::endl;
}
