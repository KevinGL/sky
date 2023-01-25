#include <SDL2/SDL.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "../glm/glm.hpp"
#include "../glm/gtx/transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
//#include <GL/glew.h>
#include <GL/gl.h>

#define SKY_SIZE 2.0f
#define RADIUS_ORBIT_SUN 1.0f
#define DURATION_TRANS 0.5f

struct Sky_VBO
{
    //std::vector<float> vertex;
    GLuint VBO;
    GLuint VAO;
    std::vector<GLuint> textures;
    unsigned int nbVertex;
};

class Sky
{
    private :

    GLuint shader;
    GLuint shaderStars;
    Sky_VBO vbo;
    Sky_VBO stars;
    glm::vec3 horizonColor;
    glm::vec3 zenithColor;
    float speed;
    unsigned int startHour;
    unsigned int chrono;
    tm date;
    float latitude;
    float longDay;
    glm::vec4 sunPos;
    double hour;
    unsigned int timestampYear;

    GLuint loadShader(const std::string path);
    char* readFileShader(const std::string &path);
    bool checkCompilShader(GLuint shader,const std::string &chemin);
    bool checkLink(GLuint programID);
    void InitVBO();
    void InitStars();
    void DrawStars(glm::vec3 posCam, glm::mat4 model, glm::mat4 view, glm::mat4 proj);
    //double SkyOpacity();
    void CalculSunPos();

    public :

    void Init(tm &dat,const float lat,glm::vec3 hColor,glm::vec3 zColor,const float spd=1.0f);
    void Draw(glm::vec3 posCam, glm::mat4 model, glm::mat4 view, glm::mat4 proj);
    glm::vec4 getSunPos()
    {
        return sunPos;
    }
};
