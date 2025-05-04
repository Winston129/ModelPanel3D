#include "MainPage.h"

/*
! FreeFunction
*/
void FreeFunction_MouseCallback(GLFWwindow* main_window, double position_x, double position_y);


MainPage::MainPage()
{
    MainPage::WindowModelPanel();
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
    GLFWwindow* main_window = glfwCreateWindow(1000, 900, "Model Panel", nullptr, nullptr);
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


    // Initialisation mouse
    glfwSetCursorPosCallback(main_window, FreeFunction_MouseCallback);


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
    SetUpCube();    // MainPage::SetUpCube();
    //run
    while(!glfwWindowShouldClose(main_window))
    {
        glfwPollEvents();

        glClearColor(0.0f, 0.22666f, 0.22666, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        DrowPanel();

        glMatrixMode(GL_MODELVIEW);
        glfwSwapBuffers(main_window);
    }

    glfwDestroyWindow(main_window);
    glfwTerminate();
    return 0;
}

void FreeFunction_MouseCallback(GLFWwindow* main_window, double position_x, double position_y)
{
    MainPage* this_main_page = static_cast<MainPage*>(glfwGetWindowUserPointer(main_window));
    if(this_main_page)
    {
        this_main_page->MouseCallback(main_window, position_x, position_y);
    }
}


void MainPage::MouseCallback(GLFWwindow* main_window, double position_x, double position_y)
{
    // Load position cursor mouse (pix) | position_x_mouse, position_y_mouse
    this->position_x_mouse_pix = static_cast<float>(position_x);
    this->position_y_mouse_pix = static_cast<float>(position_y);
    std::cout << "Position (mouse) >> x:" << this->position_x_mouse_pix << ", y:" << this->position_y_mouse_pix << std::endl;
    // Get size window | width, height
    int width, height;
    glfwGetWindowSize(main_window, &width, &height);
    std::cout <<  "width: " <<  width << " height: " << height << std::endl;
    // Convert from "pix" to "norm"
    this->position_x_mouse_norm = (position_x / width) * 2.0f - 1.0f;
    this->position_y_mouse_norm = 1.0f - (position_y / height) * 2.0f;


    MouseRotatePanel(main_window);

}


int MainPage::MouseRotatePanel(GLFWwindow* main_window)
{
    glm::mat4& matrix = this->matrix_panel_rotate;
    float& last_x = this->last_position_x_mouse;
    float& last_y = this->last_position_y_mouse;
    float& pos_x = this->position_x_mouse_norm;
    float& pos_y = this->position_y_mouse_norm;

    
    float x_offset, y_offset;

    if(glfwGetMouseButton(main_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        x_offset = last_x - pos_x;
        y_offset = pos_y - last_y;

        last_x = pos_x;
        last_y = pos_y;

        this->pitch += x_offset * panel_rotation_velocity;
        this->yaw += y_offset * panel_rotation_velocity;

        matrix = glm::rotate(matrix, glm::radians(this->yaw), glm::vec3(1.0f, 0.0f, 0.0f)); //X
        matrix = glm::rotate(matrix, glm::radians(this->pitch), glm::vec3(0.0f, 1.0f, 0.0f)); //Y
        
        unsigned int model_loc = glGetUniformLocation(shaderProgram, "matrix_model");
        glUniformMatrix4fv(
            model_loc,
            1,
            GL_FALSE,
            glm::value_ptr(matrix)
        );
    }

    return 0;
}


void MainPage::DrowPanel()
{
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
                                glm::vec3(0.0f, 0.0f, 0.0f),
                                glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                            800.0f / 600.0f,
                                            0.1f, 100.0f);

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), 0.0f, 0.0f, 3.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"), 0.0f, 0.0f, 3.0f);

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, this->vertex_count);
}


void MainPage::CreateShaders()
{
    // const char* vertexShaderSource = R"glsl(
    //     #version 330 core
    //     layout (location = 0) in vec3 aPos;
    //     layout (location = 1) in vec3 aColor;

    //     uniform mat4 matrix_model;
    //     out vec3 ourColor;

    //     void main()
    //     {
    //         gl_Position = matrix_model * vec4(aPos, 1.0);
    //         ourColor = aColor;
    //     }
    // )glsl";

    // const char* fragmentShaderSource = R"glsl(
    //     #version 330 
    //     in vec3 ourColor;
    //     out vec4 FragColor;

    //     void main()
    //     {
    //         FragColor = vec4(ourColor, 1.0);
    //     }
    // )glsl";

    const char* vertexShaderSource = R"glsl(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;

        uniform mat4 matrix_model;
        uniform mat4 view;
        uniform mat4 projection;

        out vec3 FragPos;
        out vec3 Normal;

        void main()
        {
            FragPos = vec3(matrix_model * vec4(aPos, 1.0));
            Normal = mat3(transpose(inverse(matrix_model))) * aNormal; 
            gl_Position = projection * view * vec4(FragPos, 1.0);
        }
    )glsl";

    const char* fragmentShaderSource = R"glsl(
        #version 330 core
        in vec3 FragPos;
        in vec3 Normal;

        uniform vec3 lightPos;
        uniform vec3 viewPos;

        out vec4 FragColor;

        void main()
        {
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
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


void MainPage::SetUpCube()
{
    std::string path_model_stl = "G:/University2/3.2_course/cursach/ModelPanel_v2/model/model_panel_STL.stl";
    std::vector<Face> faces;
    if (!parseSTL(path_model_stl, faces))
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

    this->vertex_count = static_cast<int>(vertices.size()) / 6;

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // layout(location = 0): position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // layout(location = 1): normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

bool MainPage::parseSTL(const std::string& path_model_stl, std::vector<Face>& faces)
{
    std::ifstream file(path_model_stl, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open STL file: " << path_model_stl << std::endl;
        return false;
    }

    char header[80];
    file.read(header, 80); // пропускаем заголовок

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

// void MainPage::SetUpCube()
// {
//     float vertices[] = {
//         // ПЕРЕДНЯЯ грань (оранжевая)
//         -0.5f, -0.5f,  0.5f,   1.0f, 0.5f, 0.2f,
//         0.5f, -0.5f,  0.5f,   1.0f, 0.5f, 0.2f,
//         0.5f,  0.5f,  0.5f,   1.0f, 0.5f, 0.2f,
//         0.5f,  0.5f,  0.5f,   1.0f, 0.5f, 0.2f,
//         -0.5f,  0.5f,  0.5f,   1.0f, 0.5f, 0.2f,
//         -0.5f, -0.5f,  0.5f,   1.0f, 0.5f, 0.2f,

//         // ЗАДНЯЯ грань (красная)
//         -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
//         0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
//         0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
//         0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
//         -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
//         -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,

//         // ЛЕВАЯ грань (зелёная)
//         -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,
//         -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
//         -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
//         -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
//         -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,
//         -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,

//         // ПРАВАЯ грань (синяя)
//         0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
//         0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
//         0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
//         0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
//         0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
//         0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,

//         // ВЕРХНЯЯ грань (жёлтая)
//         -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
//         0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
//         0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
//         0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
//         -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
//         -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,

//         // НИЖНЯЯ грань (розовая)
//         -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 1.0f,
//         0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 1.0f,
//         0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,
//         0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,
//         -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,
//         -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 1.0f
//     };
    
//     glGenVertexArrays(1, &this->VAO);  // создаём 1 VAO (Vertex Array Object)
//     glGenBuffers(1, &this->VBO);       // создаём 1 VBO (Vertex Buffer Object)
    
//     glBindVertexArray(this->VAO);  //включаем VAO
//     glBindBuffer(GL_ARRAY_BUFFER, this->VBO);  //включаем VBO
    
//     //закидываем в VBO наши вершины
//     glBufferData(
//         GL_ARRAY_BUFFER,
//         sizeof(vertices),
//         vertices,
//         GL_STATIC_DRAW
//     );
    
//     // Описываем формат вершин
//     glVertexAttribPointer(
//         0,                  // номер атрибута в шейдере (location = 0)
//         3,                  // сколько компонентов на вершину (x, y, z)
//         GL_FLOAT,           // тип данных
//         GL_FALSE,           // не нормализовать
//         6 * sizeof(float),  // размер одного набора данных (stride)
//         (void*)0            // смещение (с первого элемента)
//     );
//     glVertexAttribPointer(
//         1,
//         3,
//         GL_FLOAT,
//         GL_FALSE,
//         6 * sizeof(float),
//         (void*)(3 * sizeof(float))  // пропускаем первые 3 эелемента (x,y,z)
//     );

//     // Enable attributes
//     glEnableVertexAttribArray(0);
//     glEnableVertexAttribArray(1);
    
    
//     //Отвязали VBO и VAO, чтобы случайно не испортить
//     glBindBuffer(GL_ARRAY_BUFFER, 0);
//     glBindVertexArray(0);
// }
