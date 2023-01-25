#include <GL/glew.h>
#include "sky.h"

GLuint Sky::loadShader(const std::string path)
{
    std::string cheminVertexShader;
    std::string cheminGeometryShader;
    std::string cheminFragmentShader;
    char *vertexSource=nullptr;
    char *geometrySource=nullptr;
    char *fragmentSource=nullptr;
    GLuint vertexShader;
    GLuint geometryShader;
    GLuint fragmentShader;
    GLuint programID;
    int vertexSize;
    int fragmentSize;
    bool geoShader=false;

    if(path.rfind(".")==std::string::npos)
    {
        cheminVertexShader=path+".vert";
        cheminGeometryShader=path+".geom";
        cheminFragmentShader=path+".frag";
    }
    else
    {
        cheminVertexShader=path+"vert";
        cheminGeometryShader=path+"geom";
        cheminFragmentShader=path+"frag";
    }

    vertexSource=readFileShader(cheminVertexShader);
    if(vertexSource==nullptr)
    {
        std::cout << "Erreur chargement '" << cheminVertexShader << "', presser une touche pour quitter" << std::endl;
        getc(stdin);
        return 0;
    }

    geometrySource=readFileShader(cheminGeometryShader);
    if(geometrySource!=nullptr)
        geoShader=true;

    fragmentSource=readFileShader(cheminFragmentShader);
    if(fragmentSource==nullptr)
    {
        std::cout << "Erreur chargement '" << cheminFragmentShader << "', presser une touche pour quitter" << std::endl;
        getc(stdin);
        return 0;
    }

    vertexShader=glCreateShader(GL_VERTEX_SHADER);       //Création shaders
    if(geoShader)
        geometryShader=glCreateShader(GL_GEOMETRY_SHADER);
    fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);

    if(vertexShader==0)
    {
        std::cout << "Error creation vertex shader, press a key to quit" << std::endl;
        getc(stdin);
        return 0;
    }

    if(geoShader)
    {
        if(geometryShader==0)
        {
            std::cout << "Error creation geometry shader, press a key to quit" << std::endl;
            getc(stdin);
            return 0;
        }
    }

    if(fragmentShader==0)
    {
        std::cout << "Error creation fragment shader, press a key to quit" << std::endl;
        getc(stdin);
        return 0;
    }

    glShaderSource(vertexShader,1,(const GLchar**)&vertexSource,nullptr);
    if(geoShader)
        glShaderSource(geometryShader,1,(const GLchar**)&geometrySource,nullptr);
    glShaderSource(fragmentShader,1,(const GLchar**)&fragmentSource,nullptr);

    free(vertexSource);
    if(geoShader)
        free(geometrySource);
    free(fragmentSource);

    glCompileShader(vertexShader);
    if(geoShader)
        glCompileShader(geometryShader);
    glCompileShader(fragmentShader);

    if(!checkCompilShader(vertexShader,path))
        return 0;
    if(geoShader)
    {
        if(!checkCompilShader(geometryShader,path))
            return 0;
    }
    if(!checkCompilShader(fragmentShader,path))
        return 0;

    programID=glCreateProgram();
    if(programID==0)
    {
        printf("Error creation program, press key to quit");
        getc(stdin);
        return 0;
    }

    glAttachShader(programID,vertexShader);
    if(geoShader)
        glAttachShader(programID,geometryShader);
    glAttachShader(programID,fragmentShader);       //Liens shaders

    glLinkProgram(programID);
    if(!checkLink(programID))
        return 0;

    return programID;
}

char* Sky::readFileShader(const std::string &path)
{
    unsigned int tailleFichier;
    char *contenuFichier;

    std::ifstream fichier(path.c_str(),std::ios::binary);
    if(!fichier)
        return nullptr;

    fichier.seekg(0,fichier.end);
    tailleFichier=fichier.tellg();
    fichier.seekg(0,fichier.beg);

    contenuFichier=(char*)malloc(tailleFichier+1);

    fichier.read(contenuFichier,tailleFichier);

    contenuFichier[tailleFichier]='\0';

    fichier.close();

    return contenuFichier;
}

bool Sky::checkCompilShader(GLuint shader,const std::string &chemin)
{
    GLint statut;
    char *log;
    int tailleLog;
    char chemin2[chemin.size()];

    const size_t positionSlash=chemin.rfind("/")+1;

    const int tailleNom=chemin.size()-positionSlash;

    chemin.copy(chemin2,tailleNom,positionSlash);
    chemin2[tailleNom]='\0';

    std::string chemin3=std::string(chemin2);

    if(chemin3.rfind(".")!=std::string::npos)
    {
        const size_t positionPoint=chemin3.rfind(".");

        chemin3.erase(positionPoint);
    }

    glGetShaderiv(shader,GL_COMPILE_STATUS,&statut);
    if(statut!=GL_TRUE)     //Erreur compilation
    {
        glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&tailleLog);        //Récup taille log de compilation

        tailleLog++;
        log=(char*)malloc(tailleLog*sizeof(char));

        glGetShaderInfoLog(shader,tailleLog,&tailleLog,log);

        std::cout << "Erreur compilation de '" << chemin3 << "' :" << std::endl << log << std::endl << "Presser une touche pour quitter" << std::endl;
        free(log);
        getc(stdin);
        glDeleteShader(shader);
        return false;
    }

    return true;
}

bool Sky::checkLink(GLuint programID)
{
    GLint statut;
    char *log;
    int tailleLog;

    glGetProgramiv(programID,GL_LINK_STATUS,&statut);
    if(statut!=GL_TRUE)
    {
        glGetProgramiv(programID,GL_INFO_LOG_LENGTH,&tailleLog);

        tailleLog++;
        log=(char*)malloc(tailleLog*sizeof(char));

        glGetProgramInfoLog(programID,tailleLog,&tailleLog,log);

        printf("Erreur liage programe : '%s', presser une touche pour quitter",log);
        free(log);
        getc(stdin);
        glDeleteProgram(programID);
        return false;
    }

    return true;
}
