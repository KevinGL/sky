#include <GL/glew.h>
#include "sky.h"

void Sky::InitStars()
{
    stars.nbStars = rand() % (1000 - 600 + 1) + 600;

    for(unsigned int i = 0 ; i < stars.nbStars ; i++)
    {
        const float angleH = (float)(rand() % 360);
        const float angleV = (float)(rand() % 180 - 90);

        stars.vertices.push_back(cos(angleV * M_PI / 180) * cos(angleH * M_PI / 180));
        stars.vertices.push_back(cos(angleV * M_PI / 180) * sin(angleH * M_PI / 180));
        stars.vertices.push_back(sin(angleV * M_PI / 180));

        //////////////////////////////////////////

        stars.opacities.push_back((rand() % 101) / 100.0f);

        //////////////////////////////////////////

        if(rand() % 2 == 0)
        {
            const float frequency = (float)(rand() % 5 + 1);
            stars.pulsations.push_back(2 * M_PI * frequency);
        }

        else
        {
            stars.pulsations.push_back(0.0f);
        }
    }

    glGenVertexArrays(1, &stars.VAO);
    glGenBuffers(1, &stars.VBO);

    glBindVertexArray(stars.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, stars.VBO);

    const size_t totalSize = (stars.vertices.size() + stars.opacities.size() + stars.pulsations.size()) * sizeof(float);

    glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_STREAM_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, stars.vertices.size() * sizeof(float), stars.vertices.data());
    glBufferSubData(GL_ARRAY_BUFFER, stars.vertices.size() * sizeof(float), stars.opacities.size() * sizeof(float), stars.opacities.data());
    glBufferSubData(GL_ARRAY_BUFFER, stars.vertices.size() * sizeof(float) + stars.opacities.size() * sizeof(float), stars.pulsations.size() * sizeof(float), stars.pulsations.data());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)(stars.vertices.size() * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)(stars.vertices.size() * sizeof(float) + stars.opacities.size() * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

void Sky::DrawStars(glm::vec3 posCam, glm::mat4 model, glm::mat4 view, glm::mat4 proj)
{
    glUseProgram(shaderStars);

    glBindVertexArray(stars.VAO);

    glUniform3f(glGetUniformLocation(shaderStars, "posCam"), posCam.x, posCam.y, posCam.z);
    glUniform1f(glGetUniformLocation(shaderStars, "chrono"), (SDL_GetTicks() - chrono) / 1000.0f);

    glUniformMatrix4fv(glGetUniformLocation(shaderStars,"proj"), 1, false, glm::value_ptr(proj));
    glUniformMatrix4fv(glGetUniformLocation(shaderStars,"model"), 1, false, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderStars,"view"), 1, false, glm::value_ptr(view));

    glDrawArrays(GL_POINTS, 0, stars.nbStars);

    glBindVertexArray(0);
}
