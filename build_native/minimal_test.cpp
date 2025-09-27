#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    std::cout << "Testing basic OpenGL..." << std::endl;
    
    if (!glfwInit()) return -1;
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Test", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    
    glfwMakeContextCurrent(window);
    
    std::cout << "Window created, drawing..." << std::endl;
    
    int frameCount = 0;
    while (!glfwWindowShouldClose(window) && frameCount < 300) {
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Blue background
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw big red square
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(-0.7f, -0.7f);
        glVertex2f(0.7f, -0.7f);
        glVertex2f(0.7f, 0.7f);
        glVertex2f(-0.7f, 0.7f);
        glEnd();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        frameCount++;
    }
    
    std::cout << "Test complete" << std::endl;
    glfwTerminate();
    return 0;
}
