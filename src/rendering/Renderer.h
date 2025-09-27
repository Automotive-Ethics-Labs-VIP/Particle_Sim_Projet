#ifndef RENDERER_H
#define RENDERER_H

#include "../particle/ParticleSystem.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

class Renderer {
public:
    Renderer();
    ~Renderer();
    
    // Initialization and cleanup
    bool initialize(int width, int height, const std::string& title);
    void cleanup();
    
    // Rendering
    void clear(const glm::vec3& clearColor = glm::vec3(0.2f, 0.3f, 0.3f));
    void renderParticleSystem(const ParticleSystem& system);
    void present();
    
    // Window management
    bool shouldClose() const;
    void pollEvents();
    GLFWwindow* getWindow() const { return m_window; }
    
    // Viewport and camera
    void setViewport(const glm::vec2& min, const glm::vec2& max);
    void setWindowSize(int width, int height);
    
    // Performance
    float getFPS() const { return m_fps; }
    
private:
    GLFWwindow* m_window;
    int m_windowWidth;
    int m_windowHeight;
    
    // Camera/viewport
    glm::vec2 m_viewMin;
    glm::vec2 m_viewMax;
    
    // Performance tracking
    float m_fps;
    double m_lastFrameTime;
    int m_frameCount;
    double m_fpsUpdateTime;
    
    // OpenGL setup
    bool setupOpenGL();
    
    // Rendering helpers
    void renderParticle(const Particle& particle);
    void drawCircle(const glm::vec2& center, float radius, const glm::vec3& color, int segments = 16);
    
    // Coordinate transformation
    glm::vec2 worldToScreen(const glm::vec2& worldPos) const;
    
    // Update performance metrics
    void updateFPS();
    
    // Static callbacks
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void errorCallback(int error, const char* description);
};

#endif // RENDERER_H
