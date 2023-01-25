#include <GL/glew.h>
#include "sky.h"

void Sky::CalculSunPos()
{
    float angle0h,angle12h,angleN,angleS;

    const float colatitude=90.0f-latitude;

    const float period=365.25f*24.0f*3600.0f;
    const float pulsation=(2*M_PI)/period;

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

    const float alpha=-23.0f*cos(pulsation*timestampYear);

    if(latitude>=0.0f)
    {
        const float beta=90.0f-(alpha+colatitude);

        angle12h=90.0f-beta;

        const float gamma=90.0f-(colatitude-alpha);

        angle0h=-(90.0f-gamma);

        angleN=angle0h;
        angleS=angle12h;
    }

    else
    {
        const float beta=90.0f-((180.0f-colatitude)-alpha);

        angle12h=90.0f-beta;

        const float gamma=90.0f-((180.0f-colatitude)+alpha);

        angle0h=-(90.0f-gamma);

        angleN=angle12h;
        angleS=angle0h;
    }

    float orbitHeight,theta,delta;

    glm::vec4 pos=glm::vec4(0.0f,0.0f,0.0f,1.0f);

    if(angleS>=0 && angleN>=0)
    {
        const float lambda=(180.0f-angleN-angleS)/2;
        orbitHeight=fabs(RADIUS_ORBIT_SUN*cos(lambda*M_PI/180));
        theta=90.0f-angleN-lambda;
        delta=sqrt(pow(RADIUS_ORBIT_SUN,2)-pow(orbitHeight,2));
    }
    else
    if(angleS>=0 && angleN<0)
    {
        const float lambda=(180.0f-angleS+fabs(angleN))/2;
        orbitHeight=fabs(RADIUS_ORBIT_SUN*cos(lambda*M_PI/180));

        if(angleS<fabs(angleN))
            orbitHeight*=-1;

        const float epsilon=90.0f-angleS;
        theta=lambda-epsilon;
        delta=sqrt(pow(RADIUS_ORBIT_SUN,2)-pow(orbitHeight,2));
    }
    else
    if(angleS<0 && angleN>=0)
    {
        const float lambda=(180.0f-fabs(angleS)+angleN)/2;
        orbitHeight=-fabs(RADIUS_ORBIT_SUN*cos(lambda*M_PI/180));

        if(angleN>fabs(angleS))
            orbitHeight*=-1;

        const float epsilon=90.0f-fabs(angleS);
        theta=lambda-epsilon;
        delta=sqrt(pow(RADIUS_ORBIT_SUN,2)-pow(orbitHeight,2));
    }
    else
    if(angleS<0 && angleN<0)
    {
        const float lambda=(180.0f-fabs(angleS)-fabs(angleN))/2;
        orbitHeight=-fabs(RADIUS_ORBIT_SUN*cos(lambda*M_PI/180));

        theta=90.0f-fabs(angleS)-lambda;
        delta=sqrt(pow(RADIUS_ORBIT_SUN,2)-pow(orbitHeight,2));
    }

    hour=(date.tm_hour*3600.0+date.tm_min*60.0+date.tm_sec)*1000.0 + (SDL_GetTicks()-chrono)*speed;

    if(hour<0)
        hour+=24.0*3600000.0;
    else
    if(hour>24*3600000.0)
        hour-=24.0*3600000.0;

    const float hourHours=hour/3600000.0f;
    const float angleZ=(hourHours/24)*360;

    glm::mat4 rotTheta=glm::rotate(-theta,glm::vec3(1.0f,0.0f,0.0f));
    glm::mat4 transHeight=glm::translate(0.0f,0.0f,orbitHeight);
    glm::mat4 rotZ=glm::rotate(-angleZ,glm::vec3(0.0f,0.0f,1.0f));
    glm::mat4 transY=glm::translate(0.0f,delta,0.0f);

    pos=rotTheta*transHeight*rotZ*transY*pos;

    sunPos=pos;

    //std::cout << "Sud : " << angleS << " Nord : " << angleN << std::endl;

    //std::cout << hourHours << " " << angleS << " " << angleN << " => " << sunPos.x << " " << sunPos.y << " " << sunPos.z << std::endl;
}
