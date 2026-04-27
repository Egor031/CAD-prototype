#pragma once

#include <imgui.h>
#include <sstream>

#include <GLFW/glfw3.h>
#include "stb_image.h"
struct GLFWwindow;

class PNG_Reader
{
public:
    PNG_Reader() = default;
    ~PNG_Reader() = default;

    ImTextureID PNG_Read_ImGuiTex(std::string);

};