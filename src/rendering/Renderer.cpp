#include "Renderer.h"
#include <iostream>
#include <cmath>

Renderer::Renderer() 
    : m_window(nullptr)
    , m_windowWidth(1280)
    , m_windowHeight(720)
    , m_viewMin(-50.0f, -50.0f)
    , m_viewMax(50.0f, 50.0f)
    , m_fps(0.0f)
    , m_lastFrameTime(0.0)
    , m_frameCount(0)
    , m_fpsUpdateTime(0.0) {
}

Renderer::~Renderer() {
    cleanup();
}

bool Renderer::initialize(int width, int height, const std::string& title) {
    // Set error callback
    glfwSetErrorCallback(errorCallback);
    
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }
    
    // Configure GLFW for absolute minimum OpenGL (macOS compatible)
    // Don't set any version hints - use whatever macOS provides
    
    // Create window
    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    
    m_windowWidth = width;
    m_windowHeight = height;
    
    // Make context current
    glfwMakeContextCurrent(m_window);
    
    // Set callbacks
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
    
    // Enable vsync
    glfwSwapInterval(1);
    
    // Setup OpenGL
    if (!setupOpenGL()) {
        cleanup();
        return false;
    }
    
    m_lastFrameTime = glfwGetTime();
    m_fpsUpdateTime = m_lastFrameTime;
    
    return true;
}

void Renderer::cleanup() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    glfwTerminate();
}

bool Renderer::setupOpenGL() {
    // Print OpenGL info for debugging
    std::cout << "[RENDER] OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "[RENDER] OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
    
    // Set viewport
    glViewport(0, 0, m_windowWidth, m_windowHeight);
    
    // Use the simplest possible OpenGL setup
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Set up orthographic projection
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Set clear color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    
    std::cout << "[RENDER] OpenGL setup complete" << std::endl;
    return true;
}

void Renderer::clear(const glm::vec3& clearColor) {
    glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::renderParticleSystem(const ParticleSystem& system) {
    const auto& particles = system.getParticles();
    
    // Debug output every 60 frames (1 second at 60 FPS)
    static int frameCount = 0;
    if (frameCount % 60 == 0) {
        std::cout << "[RENDER] Rendering " << particles.size() << " particles" << std::endl;
        if (!particles.empty()) {
            std::cout << "[RENDER] First particle at (" << particles[0].position.x 
                     << ", " << particles[0].position.y << ")" << std::endl;
        }
    }
    frameCount++;
    
    for (const auto& particle : particles) {
        renderParticle(particle);
    }
}

void Renderer::renderParticle(const Particle& particle) {
    // Color based on velocity for visual interest
    float speed = glm::length(particle.velocity);
    float normalizedSpeed = std::min(speed / 20.0f, 1.0f); // Normalize to 0-1
    
    glm::vec3 color;
    if (normalizedSpeed < 0.5f) {
        // Blue to green
        color = glm::vec3(0.0f, normalizedSpeed * 2.0f, 1.0f - normalizedSpeed * 2.0f);
    } else {
        // Green to red  
        color = glm::vec3((normalizedSpeed - 0.5f) * 2.0f, 1.0f - (normalizedSpeed - 0.5f) * 2.0f, 0.0f);
    }
    
    // Make sure particles are always visible with minimum brightness
    color.r = std::max(color.r, 0.3f);
    color.g = std::max(color.g, 0.3f);
    color.b = std::max(color.b, 0.3f);
    
    drawCircle(particle.position, particle.radius, color);
}

void Renderer::drawCircle(const glm::vec2& center, float radius, const glm::vec3& color, int segments) {
    glColor3f(color.r, color.g, color.b);
    
    // Debug: print first circle draw
    static bool firstDraw = true;
    if (firstDraw) {
        std::cout << "[RENDER] Drawing first circle at (" << center.x << ", " << center.y 
                 << ") with radius " << radius << " and color (" << color.r << ", " << color.g << ", " << color.b << ")" << std::endl;
        glm::vec2 screenPos = worldToScreen(center);
        std::cout << "[RENDER] Screen coordinates: (" << screenPos.x << ", " << screenPos.y << ")" << std::endl;
        firstDraw = false;
    }
    
    // Make particles much smaller
    float screenRadius = 0.02f; // Much smaller - only 2% of screen
    
    glBegin(GL_TRIANGLE_FAN);
    
    // Center vertex
    glm::vec2 screenCenter = worldToScreen(center);
    glVertex2f(screenCenter.x, screenCenter.y);
    
    // Circle vertices
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        float x = screenCenter.x + screenRadius * cos(angle);
        float y = screenCenter.y + screenRadius * sin(angle);
        glVertex2f(x, y);
    }
    
    glEnd();
}

glm::vec2 Renderer::worldToScreen(const glm::vec2& worldPos) const {
    // Simple mapping: world coordinates -100 to +100 → screen coordinates -1 to +1
    float x = worldPos.x / 100.0f; // Map -100,100 to -1,1
    float y = worldPos.y / 100.0f;
    
    // Clamp to screen bounds
    x = std::max(-1.0f, std::min(1.0f, x));
    y = std::max(-1.0f, std::min(1.0f, y));
    
    return glm::vec2(x, y);
}

void Renderer::present() {
    glfwSwapBuffers(m_window);
    updateFPS();
}

bool Renderer::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void Renderer::pollEvents() {
    glfwPollEvents();
}

void Renderer::setViewport(const glm::vec2& min, const glm::vec2& max) {
    m_viewMin = min;
    m_viewMax = max;
}

void Renderer::setWindowSize(int width, int height) {
    m_windowWidth = width;
    m_windowHeight = height;
    glViewport(0, 0, width, height);
}

void Renderer::updateFPS() {
    double currentTime = glfwGetTime();
    m_frameCount++;
    
    // Update FPS every second
    if (currentTime - m_fpsUpdateTime >= 1.0) {
        m_fps = m_frameCount / (currentTime - m_fpsUpdateTime);
        m_frameCount = 0;
        m_fpsUpdateTime = currentTime;
        
        // Update window title with FPS
        std::string title = "Particle Simulation - FPS: " + std::to_string(static_cast<int>(m_fps));
        glfwSetWindowTitle(m_window, title.c_str());
    }
    
    m_lastFrameTime = currentTime;
}

// Static callbacks
void Renderer::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    if (renderer) {
        renderer->setWindowSize(width, height);
    }
}

void Renderer::errorCallback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}
