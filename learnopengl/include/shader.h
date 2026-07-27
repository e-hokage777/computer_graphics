
#pragma once

#include <iostream>
#include <fstream>
#include "glad/gl.h"
#include <sstream>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shader
{
public:
    unsigned int ID;

    Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath = NULL)
    {

        std::ifstream vertexFile;
        std::ifstream fragmentFile;
        std::stringstream vertexStream;
        std::stringstream fragmentStream;
        std::string vertexCode;
        std::string fragmentCode;

        vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            vertexFile.open(vertexPath);
            fragmentFile.open(fragmentPath);

            vertexStream << vertexFile.rdbuf();
            fragmentStream << fragmentFile.rdbuf();

            // closing file streams
            vertexFile.close();
            fragmentFile.close();

            vertexCode = vertexStream.str();
            fragmentCode = fragmentStream.str();
        }

        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
        }

        const char *vertexSource = vertexCode.c_str();
        const char *fragmentSource = fragmentCode.c_str();

        unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexSource);
        unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
        unsigned int geometry;

        this->ID = glCreateProgram();
        glAttachShader(this->ID, vertex);
        glAttachShader(this->ID, fragment);

        // geometry shader
        if (geometryPath != NULL)
        {
            geometry = createShader(GL_GEOMETRY_SHADER, geometryPath);
            glAttachShader(this->ID, geometry);
        }

        glLinkProgram(this->ID);

        int success;

        glGetProgramiv(this->ID, GL_LINK_STATUS, &success);

        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                      << infoLog << std::endl;
        }
    }

    void use()
    {
        glUseProgram(this->ID);
    }

    void uniformMat4(const char *name, glm::mat4 matrix)
    {
        glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void uniformVec3(const char *name, glm::vec3 vector)
    {
        glUniform3f(glGetUniformLocation(this->ID, name), vector.x, vector.y, vector.z);
    }

    void uniformVec2(const char *name, glm::vec2 vector){
        glUniform2f(glGetUniformLocation(this->ID, name), vector.x, vector.y);
    }

    void setFloat(const char *name, float value)
    {
        glUniform1f(glGetUniformLocation(this->ID, name), value);
    }

    void setInt(const char *name, int value)
    {
        glUniform1i(glGetUniformLocation(this->ID, name), value);
    }

    void setBool(const char *name, bool value){
        glUniform1i(glGetUniformLocation(this->ID, name), value);
    }

    

    void setTexUnit(unsigned int index, unsigned int texture, const char *name, unsigned int type)
    {
        glActiveTexture(GL_TEXTURE0 + index); // activate proper texture
        glBindTexture(type, texture);
        this->setInt(name, index);
    }

private:
    unsigned int compileShader(unsigned int type, const char *source)
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
            std::cout << "ERROR::SHADER::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }

        return shader;
    }

    unsigned int createShader(unsigned int type, const char *path)
    {
        std::ifstream file;
        const char *code;

        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            file.open(path);
            std::stringstream stream;
            std::string codeString;
            stream << file.rdbuf();
            file.close();
            codeString = stream.str();
            code = codeString.c_str();
            return compileShader(type, code);
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "Failed to load file at path: " << path << " with error " << e.what() << std::endl;
        }
    }
};
