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
    // Rote Camera
    glm::mat4 m_camera_scale = glm::mat4(1.0f);
    float m_default_view_z = -3.0f;
    float m_velocity_view_z = 1.0f;

    //! я хуй знает что это
    // Position Camera
    float position_x_mouse_pix=0.0f, position_y_mouse_pix=0.0f;
    float position_x_mouse_norm=0.0f, position_y_mouse_norm=0.0f;

    /*=== MODEL ===*/
    // Panel matrix parameters
    glm::mat4 matrix_panel_model = glm::mat4(1.0f);
    glm::mat4 matrix_panel_view = glm::mat4(1.0f);
    glm::mat4 matrix_panel_projection = glm::mat4(1.0f);

    // Rotate Panel
    float panel_rotation_velocity = 1.5f; //!del
    float last_position_x_mouse=0.0f, last_position_y_mouse=0.0f;
    float pitch=0.0f, yaw=-90.0f;

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

    // Mouse Scroll
    void MouseScrollCallback(GLFWwindow* main_window, double xoffset, double yoffset);

    // Mouse Position
    void MousePosCallback(GLFWwindow* main_window, double position_x, double position_y);

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