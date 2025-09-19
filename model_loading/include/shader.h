#include <iostream>
#include <glad/glad.h>
#include <fstream>
#include <string>
#include <sstream>

class Shader
{
public:
    unsigned int ID;

    Shader(const char *vertexFilePath, const char *fragmentFilePath)
    {
        std::ifstream vertexFile;
        std::ifstream fragmentFile;
        std::stringstream vertexStream;
        std::stringstream fragmentStream;
        const char *vertexCode;
        const char *fragmentCode;

        // activating exceptions to look out for
        vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            vertexFile.open(vertexFilePath);
            fragmentFile.open(fragmentFilePath);

            std::string vertexString;
            std::string fragmentString;

            vertexStream << vertexFile.rdbuf();
            fragmentStream << fragmentFile.rdbuf();

            vertexString = vertexStream.str();
            fragmentString = fragmentStream.str();

            vertexCode = vertexString.c_str();
            fragmentCode = fragmentString.c_str();
        }
        catch (std::fstream::failure e)
        {
            std::cout << "Failed to read shader files" << std::endl;
        }

        // creating program
        this->ID = glCreateProgram();

        // creating shaders
        unsigned int vertexShader, fragmentShader;

        vertexShader = createShader(vertexCode, GL_VERTEX_SHADER);
        vertexShader = createShader(vertexCode, GL_FRAGMENT_SHADER);

        // linking shaders to program
    }

private:
    unsigned int createShader(const char *source, GLenum type)
    {
        unsigned int shader = glCreateShader(type);

        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        int success;

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);

            std::cout << "Failed to compile shader: " << infoLog << std::endl;
        }

        return shader;
    }
};