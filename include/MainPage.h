#ifndef MAIN_H
#define MAIN_H

#include <GL/glew.h>    // Подключаем GLEW и OpenGL
#include <GLFW/glfw3.h>
#include <GL/glu.h> // Для gluPerspective

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Eigen/Dense> 
#include "imgui.h"
#include <iostream>

class MainPage
{
private:
    GLuint shaderProgram;
    GLuint VAO, VBO;
    float position_x_mouse_pix=0.0f, position_y_mouse_pix=0.0f;
    float position_x_mouse_norm=0.0f, position_y_mouse_norm=0.0f;
    // Rotate Panel
    glm::mat4 matrix_panel_rotate = glm::mat4(1.0f);
    float panel_rotation_velocity = 1.5f;
    float last_position_x_mouse=0.0f, last_position_y_mouse=0.0f;
    float pitch=0.0f, yaw=0.0f;

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
};

#endif // MAIN_H

/*
! VBO - Loads data into GPU memory
! VAO - Specifies which part of the VBO to load
*/