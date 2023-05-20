#include "fury/texture2d.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb/stb_image.h"

Texture2d::Texture2d(const std::string &name) {
    int width, height, nrChannels;
    unsigned char *data = stbi_load(name.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glGenTextures(1, &m_Texture);
        glBindTexture(GL_TEXTURE_2D, m_Texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    } else {
        log_error("Failed loading texture: %s", name.c_str());
    }
    stbi_image_free(data);
}

Texture2d::~Texture2d() {
    glDeleteTextures(1, &m_Texture);
}

void Texture2d::Begin() {
    glBindTexture(GL_TEXTURE_2D, m_Texture);
}

void Texture2d::End() {
    glBindTexture(GL_TEXTURE_2D, 0);
}
