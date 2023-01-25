#include <GL/glew.h>
#include "sky.h"

void Sky::InitStars()
{
    std::vector<float> points;

    unsigned int nbStars=rand()%(1000-601)+600;

    points.resize(3*nbStars);

    for(unsigned int i=0;i<3*nbStars;i+=3)
    {
        const float angleH=rand()%360;
        const float angleV=rand()%180-90;

        points[i]=cos(angleV*M_PI/180)*cos(angleH*M_PI/180);
        points[i+1]=cos(angleV*M_PI/180)*sin(angleH*M_PI/180);
        points[i+2]=sin(angleV*M_PI/180);
    }

    glGenVertexArrays(1,&stars.VAO);
    glGenBuffers(1,&stars.VBO);

    glBindVertexArray(stars.VAO);
    glBindBuffer(GL_ARRAY_BUFFER,stars.VBO);

    glBufferData(GL_ARRAY_BUFFER,points.size()*sizeof(float),points.data(),GL_STREAM_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    stars.nbVertex=points.size()/3;
}

void Sky::DrawStars(glm::vec3 posCam, glm::mat4 model, glm::mat4 view, glm::mat4 proj)
{
    glUseProgram(shaderStars);

    glBindVertexArray(stars.VAO);

    glUniform3f(glGetUniformLocation(shaderStars,"posCam"), posCam.x, posCam.y, posCam.z);

    glUniformMatrix4fv(glGetUniformLocation(shaderStars,"proj"), 1, false, glm::value_ptr(proj));
    glUniformMatrix4fv(glGetUniformLocation(shaderStars,"model"), 1, false, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderStars,"view"), 1, false, glm::value_ptr(view));

    glDrawArrays(GL_POINTS,0,stars.nbVertex);

    glBindVertexArray(0);
}
