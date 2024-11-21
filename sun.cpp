#include <GL/glew.h>
#include "sky.h"

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
