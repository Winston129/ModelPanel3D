#include "HelperFunc.h"


bool HelperFunc::parseSTL(const std::string& path_model_stl, std::vector<Face>& faces)
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



// bool HelperFunc::parseBinarySTL(const std::string& filename, std::vector<Face>& faces) {
//     std::ifstream file(filename, std::ios::binary);
//     if (!file.is_open()) {
//         std::cerr << "Unable to open file(path): " << filename << std::endl;
//         return false;
//     }

//     char header[80];
//     file.read(header, 80);
//     uint32_t numFaces;
//     file.read(reinterpret_cast<char*>(&numFaces), sizeof(uint32_t));

//     faces.resize(numFaces);

//     for (uint32_t i = 0; i < numFaces; ++i) {
//         Face& face = faces[i];
//         file.read(reinterpret_cast<char*>(&face.normal), sizeof(Vertex));
//         file.read(reinterpret_cast<char*>(&face.v1), sizeof(Vertex));
//         file.read(reinterpret_cast<char*>(&face.v2), sizeof(Vertex));
//         file.read(reinterpret_cast<char*>(&face.v3), sizeof(Vertex));
//         file.read(reinterpret_cast<char*>(&face.reserved), sizeof(uint16_t));
//     }

//     return true;
// }

// Глобальные переменные для вращения
// float rotationX = 0.0f;
// float rotationY = 0.0f;

// void processInput(GLFWwindow* window) {
//     const float rotSpeed = 1.0f;
//     if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//         rotationY -= rotSpeed;
//     if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//         rotationY += rotSpeed;
//     if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//         rotationX -= rotSpeed;
//     if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//         rotationX += rotSpeed;
// }

// void drawModel(const std::vector<Face>& faces) {
//     glBegin(GL_TRIANGLES);
//     for (const auto& face : faces) {
//         glNormal3f(face.normal.x, face.normal.y, face.normal.z);
//         glVertex3f(face.v1.x, face.v1.y, face.v1.z);
//         glVertex3f(face.v2.x, face.v2.y, face.v2.z);
//         glVertex3f(face.v3.x, face.v3.y, face.v3.z);
//     }
//     glEnd();
// }

// int main() {
//     std::vector<Face> faces;
//     if (!parseBinarySTL("G:/University2/3.2_course/cursach/ModelPanel_v2/model/model_panel_STL.stl", faces)) {
//         std::cerr << "Failed to parse STL file." << std::endl;
//         return -1;
//     }

//     if (!glfwInit()) {
//         std::cerr << "Failed to initialize GLFW\n";
//         return -1;
//     }

//     GLFWwindow* window = glfwCreateWindow(800, 600, "STL Viewer", nullptr, nullptr);
//     if (!window) {
//         std::cerr << "Failed to create GLFW window\n";
//         glfwTerminate();
//         return -1;
//     }
//     glfwMakeContextCurrent(window);

//     glewExperimental = GL_TRUE;
//     if (glewInit() != GLEW_OK) {
//         std::cerr << "Failed to initialize GLEW\n";
//         return -1;
//     }

//     glEnable(GL_DEPTH_TEST);
//     glEnable(GL_LIGHTING);
//     glEnable(GL_LIGHT0);
//     glEnable(GL_NORMALIZE);

//     GLfloat light_pos[] = { 0.0f, 0.0f, 1.0f, 0.0f };
//     glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

//     // Повернуть модель на 90 градусов вокруг оси Y
//     rotationY = 90.0f;

//     while (!glfwWindowShouldClose(window)) {
//         processInput(window);

//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//         glMatrixMode(GL_PROJECTION);
//         glLoadIdentity();
//         gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);

//         glMatrixMode(GL_MODELVIEW);
//         glLoadIdentity();
//         gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);

//         glRotatef(rotationX, 1, 0, 0);
//         glRotatef(rotationY, 0, 1, 0);

//         drawModel(faces);

//         glfwSwapBuffers(window);
//         glfwPollEvents();
//     }

//     glfwTerminate();
//     return 0;
// }
