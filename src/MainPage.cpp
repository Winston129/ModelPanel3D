#include "MainPage.h"

/*┌────────────────────┐*/
/*├───┤FreeFunction├───┤*/
/*└────────────────────┘*/
void Free_MousePosCallback(GLFWwindow* main_window, double position_x, double position_y);
void Free_MouseScrollCallback(GLFWwindow* main_window, double xoffset, double yoffset);


MainPage::MainPage(const std::string& path_ref) : m_path_model_stl(path_ref)
{
    WindowModelPanel();
}
MainPage::~MainPage(){}


GLFWwindow* MainPage::InitialisationGL()
{
    // Initialisation GLFW
    if(!glfwInit())
    {
        std::cerr << "Error initialisation GLFW (glfwInit)" << std::endl;
        return nullptr;
    }

    // Create window
    GLFWwindow* main_window = glfwCreateWindow(WIDTH_WINDOW, HEIGHT_WINDOW, "Model Panel", nullptr, nullptr);
    if(!main_window)
    {
        std::cerr << "Error create window (glfwCreateWindow)" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    // Current window
    glfwMakeContextCurrent(main_window);


    // Attach your object pointer (MainPage*) to the window | Attach-прикрепить
    glfwSetWindowUserPointer(main_window, this);


    // Initialisation GLEW
    GLenum err=glewInit();
    if(err!=GLEW_OK)
    {
        std::cerr << "Error initialisation GLEW (glefInit): " << glewGetErrorString(err) << std::endl;
        return nullptr;
    }


    // Scroll mouse
    glfwSetScrollCallback(main_window, Free_MouseScrollCallback);
    // Initialisation mouse
    glfwSetCursorPosCallback(main_window, Free_MousePosCallback);


    return main_window;
}


int MainPage::WindowModelPanel()
{
    // Initialisation GLFW, GLEW
    GLFWwindow* main_window = InitialisationGL();
    if(!main_window)
        return -1;


    glEnable(GL_DEPTH_TEST);
    CreateShaders(); // MainPage::CreateShaders();
    SetUpPanel();    // MainPage::SetUpPanel();
    //run
    while(!glfwWindowShouldClose(main_window))
    {
        glfwPollEvents();

        glClearColor(0.0f, 0.22666f, 0.22666, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        DrowPanel(main_window);

        glMatrixMode(GL_MODELVIEW);
        glfwSwapBuffers(main_window);
    }

    glfwDestroyWindow(main_window);
    glfwTerminate();
    return 0;
}


/*┌────────────────────┐*/
/*├───┤Mouse Scroll├───┤*/
/*└────────────────────┘*/
void Free_MouseScrollCallback(GLFWwindow* main_window, double xoffset, double yoffset)
{
    MainPage* this_main_page = static_cast<MainPage*>(glfwGetWindowUserPointer(main_window));
    if(this_main_page)
    {
        this_main_page->MouseScrollCallback(xoffset, yoffset);
    }
}


void MainPage::MouseScrollCallback(double xoffset, double yoffset)
{
    // mouse wheel scrolling
    // std::cout << "(x, y) = " << xoffset << ", " << yoffset << std::endl;

    if(yoffset == 1)
    {
        m_radius_to_centre -= m_velocity_to_centre;
    }
    else if(yoffset == -1)
    {
        m_radius_to_centre += m_velocity_to_centre;
    }
    yoffset=0;
}


/*┌──────────────────────┐*/
/*├───┤Mouse Position├───┤*/
/*└──────────────────────┘*/
void Free_MousePosCallback(GLFWwindow* main_window, double position_x, double position_y)
{
    MainPage* this_main_page = static_cast<MainPage*>(glfwGetWindowUserPointer(main_window));
    if(this_main_page)
    {
        this_main_page->MousePosCallback(main_window, position_x, position_y);
    }
}


void MainPage::MousePosCallback(GLFWwindow* main_window, double position_x, double position_y)
{
    CalculateRotationAngles(main_window, position_x, position_y);
}


void MainPage::CalculateRotationAngles(GLFWwindow* main_window, double position_x, double position_y)
{
    // Angles for camera
    float sensitivity = 0.1f;

    float deltaX = static_cast<float>(position_x - m_last_x_mouse);
    float deltaY = static_cast<float>(position_y - m_last_y_mouse);

    m_last_x_mouse = static_cast<float>(position_x);
    m_last_y_mouse = static_cast<float>(position_y);

    if(glfwGetMouseButton(main_window, GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS)
    {
        m_yaw_camera += deltaX * sensitivity;
        m_pitch_camera += deltaY * sensitivity;
    }

    // std::cout << "(m_yaw_camera, m_pitch_camera) = " << m_yaw_camera << ","<< m_pitch_camera <<std::endl;
    
    if (m_pitch_camera > 89.0f)
    {
        m_pitch_camera = 89.0f;
    }
    if (m_pitch_camera < -89.0f)
    {
        m_pitch_camera = -89.0f;
    }
}


/*┌──────────────────┐*/
/*├───┤Drow Panel├───┤*/
/*└──────────────────┘*/
void MainPage::DrowPanel(GLFWwindow* main_window)
{ 
    glUseProgram(shaderProgram);

    UpdateUniform(main_window);

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, m_vertex_count);
}


void MainPage::UpdateUniform(GLFWwindow* main_window)
{
    /*====== MATRIX VIEW ======*/

    // view position
    glm::mat4& view = m_camera_position;
    glm::vec3& vec_view = m_vec_camera_position;

    vec_view.x = m_radius_to_centre * cos(glm::radians(m_yaw_camera)) * cos(glm::radians(m_pitch_camera));
    vec_view.y = m_radius_to_centre * sin(glm::radians(m_pitch_camera));
    vec_view.z = m_radius_to_centre * sin(glm::radians(m_yaw_camera)) * cos(glm::radians(m_pitch_camera));

    view = glm::lookAt(vec_view, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    unsigned int view_loc = glGetUniformLocation(this->shaderProgram, "uView");
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));

    /*====== MATRIX MODEL ======*/
    
    // model position
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(55.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    unsigned int model_loc = glGetUniformLocation(this->shaderProgram, "uModel");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    
    // model light
    glm::vec3 light_pos = glm::vec3(0.0f, 0.0f, 10.0f);

    unsigned int light_pos_loc = glGetUniformLocation(this->shaderProgram, "lightPos");
    glUniform3fv(light_pos_loc, 1, glm::value_ptr(light_pos));

    /*====== MATRIX PROJECTION ======*/

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), WIDTH_WINDOW / HEIGHT_WINDOW, 0.1f, 100.0f);
    
    unsigned int projection_loc = glGetUniformLocation(this->shaderProgram, "uProjection");
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
}


void MainPage::CreateShaders()
{
    const char* vertexShaderSource = R"glsl(
        #version 330 core
        layout (location=0) in vec3 layoutPosition;
        layout (location=1) in vec3 layoutNormal;

        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProjection;

        out vec3 outModelPos;
        out vec3 outNormal;

        void main()
        {
            outNormal = mat3(transpose(inverse(uModel))) * layoutNormal; 

            outModelPos = vec3(uModel * vec4(layoutPosition, 1.0));
            gl_Position = uProjection * uView * vec4(outModelPos, 1.0);
        }
    )glsl";


    const char* fragmentShaderSource = R"glsl(
        #version 330 core
        in vec3 outModelPos;
        in vec3 outNormal;

        uniform vec3 lightPos;
        uniform vec3 viewPos;

        out vec4 FragColor;

        void main()
        {
            vec3 norm = normalize(outNormal);
            vec3 lightDir = normalize(lightPos - outModelPos);
            float diff = max(dot(norm, lightDir), 0.0);

            vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
            FragColor = vec4(diffuse, 1.0);
        }
    )glsl";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}


void MainPage::SetUpPanel()
{
    std::vector<Face> faces;
    if (!parseSTL(faces))
    {
        std::cerr << "Model creation error" << std::endl;
    }

    std::vector<float> vertices;
    for (const auto& face : faces) {
        glm::vec3 n = {face.normal.x, face.normal.y, face.normal.z};

        glm::vec3 v1 = {face.v1.x, face.v1.y, face.v1.z};
        glm::vec3 v2 = {face.v2.x, face.v2.y, face.v2.z};
        glm::vec3 v3 = {face.v3.x, face.v3.y, face.v3.z};

        for (auto v : {v1, v2, v3}) {
            // vertex: x, y, z, nx, ny, nz
            vertices.insert(vertices.end(), {v.x, v.y, v.z, n.x, n.y, n.z});
        }
    }

    m_vertex_count = static_cast<int>(vertices.size()) / 6;

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // location=0 => "position"
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // location=1 => "normal"
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

bool MainPage::parseSTL(std::vector<Face>& faces)
{
    std::ifstream file(m_path_model_stl, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open STL file: " << m_path_model_stl << std::endl;
        return false;
    }

    char header[80];
    file.read(header, 80);

    uint32_t numTriangles = 0;
    file.read(reinterpret_cast<char*>(&numTriangles), sizeof(uint32_t));
    faces.resize(numTriangles);

    for (uint32_t i = 0; i < numTriangles; ++i) {
        Face& face = faces[i];
        file.read(reinterpret_cast<char*>(&face.normal), sizeof(Vertex));
        file.read(reinterpret_cast<char*>(&face.v1), sizeof(Vertex));
        file.read(reinterpret_cast<char*>(&face.v2), sizeof(Vertex));
        file.read(reinterpret_cast<char*>(&face.v3), sizeof(Vertex));
        file.read(reinterpret_cast<char*>(&face.reserved), sizeof(uint16_t));
    }

    return true;
}
