#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "shader.h"
#include <vector>
#include "mesh.h"
#include "utils.h"

using namespace std;

class Primitive
{
protected:
    glm::vec3 scale;
    glm::vec3 color;
    glm::vec3 rotAxis;
    float rotAngle;
    vector<string> texturePaths;

public:
    glm::vec3 position;
    Primitive(glm::vec3 scale, glm::vec3 color = glm::vec3(0.776f, 0.765f, 0.710f), glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), vector<string> texturePaths = vector<string>(), float rotAngle = NULL, glm::vec3 rotAxis = glm::vec3(0.0f, 0.0f, 0.0f))
    {
        this->scale = scale;
        this->position = position;
        this->color = color;
        this->texturePaths = texturePaths;
        this->rotAngle = rotAngle;
        this->rotAxis = rotAxis;
        loadTextures();
    }

    void Draw(Shader shader)
    {

        shader.use();

        // setting color
        shader.uniformVec3("baseColor", color);

        // glm::mat4 transform = glm::mat4(1.0f);
        // transform = glm::translate(transform, position);
        // transform = glm::scale(transform, glm::vec3(scale, scale, scale));
        // transform = glm::rotate(transform, glm::radians(-60.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, position);
        if (this->rotAngle)
        {
            model = glm::rotate(model, glm::radians(rotAngle), glm::radians(rotAxis));
        }
        model = glm::scale(model, this->scale);

        shader.uniformMat4("model", model);
        mesh.Draw(shader);
    }

    virtual ~Primitive() = default;

protected:
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    Mesh mesh;

    virtual void setup() = 0;

private:
    void loadTextures()
    {
        for (auto path : texturePaths)
        {
            Texture texture;
            string directory = path.substr(0, path.find_last_of('/'));
            string filename = path.substr(path.find_last_of('/') + 1);
            texture.id = TextureFromFile(filename.c_str(), directory);
            texture.type = "texture_diffuse";
            texture.path = path;
            textures.push_back(texture);
        }
    }
};
