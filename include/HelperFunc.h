#ifndef HELPER_FUNC_H
#define HELPER_FUNC_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class HelperFunc
{
private:
struct Vertex {
    float x, y, z;
};

struct Face {
    Vertex normal;
    Vertex v1, v2, v3;
    uint16_t reserved;
};

public:
    bool parseSTL(const std::string& path_model_stl, std::vector<Face>& faces);
};


#endif