#include "PNG_Reader.h"

#include <imgui.h>
#include <sstream>

#include <GLFW/glfw3.h>
#include "stb_image.h"


ImTextureID PNG_Reader::PNG_Read_ImGuiTex(std::string filename)
{
    
    GLuint texture;
    int w;
    int h;
    int comp;
    unsigned char* image = stbi_load(filename.c_str(), &w, &h, &comp, 3);


    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    //if (comp == 3)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    //else if (comp == 4)
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(image);

    ImTextureID texID = (ImTextureID)(intptr_t)texture;
    return (texID);
}