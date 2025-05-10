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
    // path STL
    const std::string& m_path_model_stl;
    // Window size
    const float WIDTH_WINDOW = 1920.0f;
    const float HEIGHT_WINDOW = 1080.0f;
    // SHADERS
    GLuint shaderProgram;
    GLuint VAO, VBO;
    // helper var Panel
    int m_vertex_count = 0;

    /*=== VIEW ===*/
    glm::mat4 m_camera_position = glm::mat4(1.0f);
    glm::vec3 m_vec_camera_position = glm::vec3(1.0f);
    float m_radius_to_centre = 3.0f;    //radius from camera to centre  
    float m_velocity_to_centre = 1.0f;  //camera approach speed to the centre
    // Angles
    float m_pitch_camera=0.0f, m_yaw_camera=90.0f;
    float m_last_x_mouse=0.0f, m_last_y_mouse=0.0f;

    // file STL parse
    struct Vertex {
        float x, y, z;
    };
    struct Face {
        Vertex normal;
        Vertex v1, v2, v3;
        uint16_t reserved;
    };


public:
    MainPage(const std::string& path);
    ~MainPage();

    // Main functions
    GLFWwindow* InitialisationGL();
    int WindowModelPanel();
    
    // Drow model
    void DrowPanel(GLFWwindow* main_window);
    void UpdateUniform(GLFWwindow* main_window);
    void CreateShaders();

    // Mouse Wheel Scroll
    void MouseScrollCallback(double xoffset, double yoffset);

    // Mouse Cursor Position
    void MousePosCallback(GLFWwindow* main_window, double position_x, double position_y);
    void CalculateRotationAngles(GLFWwindow* main_window, double position_x, double position_y);


    void SetUpPanel();
    // Parsing 
    bool parseSTL(std::vector<Face>& faces);
};

#endif

/*┌─────────────────────────────────────────────────┐*/
/*│         VBO - Loads data into GPU memory        │*/
/*├─────────────────────────────────────────────────┤*/
/*│  VAO - Specifies which part of the VBO to load  │*/
/*├─────────────────────────────────────────────────┤*/
/*│              "m_", this "member"                │*/
/*└─────────────────────────────────────────────────┘*/