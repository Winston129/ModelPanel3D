#include "MainPage.h"

int main(){
    MainPage main_window;

    return 0;
}





// Глобальные переменные для поворота
// float yaw = 0.0f;
// float pitch = 0.0f;
// bool mousePressed = false;
// double lastX, lastY;

// Функция обработки событий мыши
// void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
//     if (button == GLFW_MOUSE_BUTTON_LEFT) {
//         if (action == GLFW_PRESS) {
//             mousePressed = true;
//             glfwGetCursorPos(window, &lastX, &lastY);
//         } else if (action == GLFW_RELEASE) {
//             mousePressed = false;
//         }
//     }
// }

// Функция обработки движения мыши
// void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
//     if (mousePressed) {
//         double dx = xpos - lastX;
//         double dy = ypos - lastY;
//         yaw += (float)dx * 0.5f;
//         pitch += (float)dy * 0.5f;
//         lastX = xpos;
//         lastY = ypos;
//     }
// }

// Функция отрисовки куба
// void drawCube() {
//     glBegin(GL_QUADS);

//     // Передняя грань (z = 1.0f)
//     glColor3f(1.0f, 0.0f, 0.0f); // Красная
//     glVertex3f(-0.5f, -0.5f, 0.5f);
//     glVertex3f( 0.5f, -0.5f, 0.5f);
//     glVertex3f( 0.5f,  0.5f, 0.5f);
//     glVertex3f(-0.5f,  0.5f, 0.5f);

//     // Задняя грань (z = -1.0f)
//     glColor3f(0.0f, 1.0f, 0.0f); // Зелёная
//     glVertex3f(-0.5f, -0.5f, -0.5f);
//     glVertex3f(-0.5f,  0.5f, -0.5f);
//     glVertex3f( 0.5f,  0.5f, -0.5f);
//     glVertex3f( 0.5f, -0.5f, -0.5f);

//     // Верхняя грань
//     glColor3f(0.0f, 0.0f, 1.0f); // Синяя
//     glVertex3f(-0.5f,  0.5f, -0.5f);
//     glVertex3f(-0.5f,  0.5f,  0.5f);
//     glVertex3f( 0.5f,  0.5f,  0.5f);
//     glVertex3f( 0.5f,  0.5f, -0.5f);

//     // Нижняя грань
//     glColor3f(1.0f, 1.0f, 0.0f); // Жёлтая
//     glVertex3f(-0.5f, -0.5f, -0.5f);
//     glVertex3f( 0.5f, -0.5f, -0.5f);
//     glVertex3f( 0.5f, -0.5f,  0.5f);
//     glVertex3f(-0.5f, -0.5f,  0.5f);

//     // Правая грань
//     glColor3f(0.0f, 1.0f, 1.0f); // Голубая
//     glVertex3f( 0.5f, -0.5f, -0.5f);
//     glVertex3f( 0.5f,  0.5f, -0.5f);
//     glVertex3f( 0.5f,  0.5f,  0.5f);
//     glVertex3f( 0.5f, -0.5f,  0.5f);

//     // Левая грань
//     glColor3f(1.0f, 0.0f, 1.0f); // Розовая
//     glVertex3f(-0.5f, -0.5f, -0.5f);
//     glVertex3f(-0.5f, -0.5f,  0.5f);
//     glVertex3f(-0.5f,  0.5f,  0.5f);
//     glVertex3f(-0.5f,  0.5f, -0.5f);

//     glEnd();
// }

// int main() {
//     // === GLFW ===
//     if (!glfwInit()) {
//         std::cerr << "Failed to initialize GLFW" << std::endl;
//         return -1;
//     }

//     GLFWwindow* window = glfwCreateWindow(800, 600, "Rotating Cube", nullptr, nullptr);
//     if (!window) {
//         std::cerr << "Failed to create GLFW window" << std::endl;
//         glfwTerminate();
//         return -1;
//     }
//     glfwMakeContextCurrent(window);

//     // === GLEW ===
//     if (glewInit() != GLEW_OK) {
//         std::cerr << "Failed to initialize GLEW" << std::endl;
//         return -1;
//     }

//     // Включаем Z-буфер
//     glEnable(GL_DEPTH_TEST);

//     // === Eigen пример ===
//     Eigen::Matrix3f m;
//     m << 1, 2, 3,
//          4, 5, 6,
//          7, 8, 9;
//     std::cout << "Eigen test matrix:\n" << m << std::endl;

//     // === ImGui (пока только версия) ===
//     IMGUI_CHECKVERSION();
//     ImGui::CreateContext();
//     std::cout << "Dear ImGui version: " << ImGui::GetVersion() << std::endl;
//     ImGui::DestroyContext();

//     // === Настройка перспективы ===
//     int width, height;
//     glfwGetFramebufferSize(window, &width, &height);

//     glMatrixMode(GL_PROJECTION);
//     glLoadIdentity();
//     gluPerspective(45.0, (double)width / (double)height, 0.1, 100.0);
//     glMatrixMode(GL_MODELVIEW);

//     // === Подключаем обработчики мыши ===
//     glfwSetMouseButtonCallback(window, mouseButtonCallback);
//     glfwSetCursorPosCallback(window, cursorPositionCallback);

//     // === Главный цикл ===
//     while (!glfwWindowShouldClose(window)) {
//         glfwPollEvents();

//         // Очистка экрана
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//         // Работа с матрицей модели/вида
//         glLoadIdentity();
//         glTranslatef(0.0f, 0.0f, -3.0f); // Отодвигаем назад
//         glRotatef(pitch, 1.0f, 0.0f, 0.0f);
//         glRotatef(yaw, 0.0f, 1.0f, 0.0f);

//         // Рисуем куб
//         drawCube();

//         // Показываем
//         glfwSwapBuffers(window);
//     }

//     glfwDestroyWindow(window);
//     glfwTerminate();
//     return 0;
// }
