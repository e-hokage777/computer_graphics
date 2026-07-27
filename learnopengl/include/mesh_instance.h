#pragma once

#include <iostream>
#include <vector>
#include "glm/gtc/matrix_transform.hpp"
#include "shader.h"
#include "mesh.h"

// struct Vertex
// {
//     glm::vec3 Position;
//     glm::vec3 Normal;
//     glm::vec2 TexCoords;
// };

struct InstanceTransform
{
    glm::vec3 translation;
    glm::vec3 scale;
    float rotation;
};

// struct Texture
// {
//     unsigned int id;
//     std::string type;
//     std::string path;
// };

class MeshInstance
{
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    float numInstances;
    std::vector<glm::mat4> instanceTransforms;
    MeshInstance(std::vector<Vertex> vertices,
                 std::vector<unsigned int> indices,
                 std::vector<Texture> textures,
                 float numInstances,
                 std::vector<glm::mat4> instanceTransforms = {})
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        this->numInstances = numInstances;
        this->instanceTransforms = instanceTransforms;

        this->setup();
    }

    MeshInstance() {}

    void Draw(Shader &shader)
    {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // activate texture unit first
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);
            shader.setInt(("material." + name + number).c_str(), i); // why is here set float and not set int
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, this->numInstances);
        glBindVertexArray(0);
    }

private:
    unsigned int VAO, VBO, EBO, instanceVBO;
    void setup()
    {
        // generate buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glGenBuffers(1, &instanceVBO);

        // move data to buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                     &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, instanceTransforms.size() * sizeof(glm::mat4), &instanceTransforms[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // set up vao states
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)offsetof(Vertex, TexCoords));

        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        for (unsigned int i = 0; i < 4; i++)
        {
            glEnableVertexAttribArray(3 + i);
            glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                                  (void *)(i * sizeof(glm::vec4)));
            glVertexAttribDivisor(3 + i, 1);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBindVertexArray(0);
    };
};