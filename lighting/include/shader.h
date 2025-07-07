#include <iostream>
#include <glad/gl.h>
#include <fstream>
#include <string>
#include <sstream>

class Shader
{
public:
    unsigned int ID;
    Shader(const char *vertexPath, const char *fragmentPath)
    {
        std::ifstream vertexFile;
        std::ifstream fragmentFile;
        const char *vertexCode;
        const char *fragmentCode;
        std::string vertexString;
        std::string fragmentString;

        // setting exceptions
        vertexFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
        fragmentFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

        try
        {
            vertexFile.open(vertexPath);
            fragmentFile.open(fragmentPath);

            std::stringstream vertexStream;
            std::stringstream fragmentStream;

            vertexStream << vertexFile.rdbuf();
            fragmentStream << fragmentFile.rdbuf();

            vertexString = vertexStream.str();
            fragmentString = fragmentStream.str();

            vertexCode = vertexString.c_str();
            fragmentCode = fragmentString.c_str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "Failed to read shader files" << std::endl;
        }

        unsigned int vertexShader;
        unsigned int fragmentShader;

        vertexShader = this->createShader(vertexCode, GL_VERTEX_SHADER);
        fragmentShader = this->createShader(fragmentCode, GL_FRAGMENT_SHADER);

        // creating program
        this->ID = glCreateProgram();

        glAttachShader(this->ID, vertexShader);
        glAttachShader(this->ID, fragmentShader);
        glLinkProgram(this->ID);

        int success;
        glGetProgramiv(this->ID, GL_LINK_STATUS, &success);

        if (!success)
        {
            char infoLog[512];

            glGetProgramInfoLog(this->ID, 512, NULL, infoLog);

            std::cout << "ERROR::SHADER_PROGRAM::Failed to link program::" << infoLog << std::endl;
        }
    }

    void use()
    {
        glUseProgram(this->ID);
    }

    void setMat4(const char *name, float *value)
    {
        glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, value);
    }

private:
    unsigned int createShader(const char *source, GLenum type)
    {
        unsigned int shader;
        shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, NULL);

        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            char infoLog[512];

            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILE::Failed to compile shader::" << infoLog << std::endl;
        }

        return shader;
    }
};