#ifndef MAIN_PAGE_H
#define MAIN_PAGE_H

#include <iostream>
#include <vector>
#include <string>
// #include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Eigen/Dense> 
#include "imgui.h"
#include "HelperFunc.h"


class MainPage
{
private:
    GLuint shaderProgram;
    GLuint VAO, VBO;
    // Panel
    int vertex_count = 0;
    // Mouse
    float position_x_mouse_pix=0.0f, position_y_mouse_pix=0.0f;
    float position_x_mouse_norm=0.0f, position_y_mouse_norm=0.0f;
    // Rotate Panel
    glm::mat4 matrix_panel_rotate = glm::mat4(1.0f);
    float panel_rotation_velocity = 1.5f;
    float last_position_x_mouse=0.0f, last_position_y_mouse=0.0f;
    float pitch=0.0f, yaw=0.0f;
    // file STL parse
    struct Vertex {
        float x, y, z;
    };
    struct Face {
        Vertex normal;
        Vertex v1, v2, v3;
        uint16_t reserved;
    };
    // HelperFunc helper_func;


public:
    MainPage();
    ~MainPage();

    GLFWwindow* InitialisationGL();
    int WindowModelPanel();
    void MouseCallback(GLFWwindow* main_window, double position_x, double position_y);
    int MouseRotatePanel(GLFWwindow* main_window);
    void CreateShaders();
    void DrowPanel();
    void SetUpCube();
    bool parseSTL(const std::string& filename, std::vector<Face>& faces);
};

#endif

/*
! VBO - Loads data into GPU memory
! VAO - Specifies which part of the VBO to load
*/