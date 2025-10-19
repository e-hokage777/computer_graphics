#include <iostream>
#include <glad/glad.h>
#include <stb_image.h>

class Texture
{
public:
    unsigned int ID;

    Texture(const char *path)
    {
        // generating texture and configure
        glGenTextures(1, &this->ID);
        glBindTexture(GL_TEXTURE_2D, this->ID);

        // configurating
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        unsigned char *data;
        int width, height, numChannels;

        data = stbi_load(path, &width, &height, &numChannels, 3);

        if (data == NULL)
        {
            std::cout << "Failed to load image: " << stbi_failure_reason() << std::endl;
        }
        else
        {

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
};