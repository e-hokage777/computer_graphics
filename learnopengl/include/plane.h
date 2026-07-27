#pragma once
#include "primitve.h"

class Plane : public Primitive
{
public:
    Plane(glm::vec3 scale = glm::vec3(1.0f), glm::vec3 color = glm::vec3(0.776f, 0.765f, 0.710f), glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), vector<string> textures = vector<string>()) : Primitive(scale, color, position, textures)
    {
        setup();
    };

private:
    void setup()
    {
        // top left
        vertices.push_back(Vertex{
            glm::vec3(-0.5f, 0.5f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec2(0.0f, 1.0f)});

        // bottom left
        vertices.push_back(Vertex{
            glm::vec3(-0.5f, -0.5f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec2(0.0f, 0.0f)});

        // bottom right
        vertices.push_back(Vertex{
            glm::vec3(0.5f, -0.5f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec2(1.0f, 0.0f)});

        // top right
        vertices.push_back(Vertex{
            glm::vec3(0.5f, 0.5f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec2(1.0f, 1.0f)});

        // preparing indices
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(0);
        indices.push_back(2);
        indices.push_back(3);

        // preparing mesh
        mesh = Mesh(vertices, indices, textures);
    }
};
