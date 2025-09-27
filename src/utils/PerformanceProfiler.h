#ifndef PERFORMANCE_PROFILER_H
#define PERFORMANCE_PROFILER_H

#include <chrono>
#include <string>
#include <vector>
#include <unordered_map>

class PerformanceProfiler {
public:
    struct ProfileData {
        double totalTime;
        double minTime;
        double maxTime;
        double avgTime;
        int callCount;
        std::string name;
    };
    
    PerformanceProfiler();
    ~PerformanceProfiler();
    
    // Timing functions
    void startTimer(const std::string& name);
    void endTimer(const std::string& name);
    
    // Frame timing
    void beginFrame();
    void endFrame();
    
    // System metrics
    void updateFPS(float fps);
    void updateParticleCount(int count);
    void updatePhysicsSteps(int steps);
    
    // Data access
    ProfileData getProfileData(const std::string& name) const;
    float getCurrentFPS() const { return m_currentFPS; }
    float getAverageFPS() const;
    int getCurrentParticleCount() const { return m_currentParticleCount; }
    double getFrameTime() const { return m_lastFrameTime; }
    
    // Performance analysis
    bool isTargetPerformanceMet() const;
    std::string getPerformanceReport() const;
    
    // Configuration
    void setTargetFPS(float target) { m_targetFPS = target; }
    void setTargetPhysicsSteps(int target) { m_targetPhysicsSteps = target; }
    
    // Reset and cleanup
    void reset();
    void clearHistory();
    
    // Export profiling data
    bool exportToFile(const std::string& filename) const;
    
private:
    // Timer data
    std::unordered_map<std::string, std::chrono::high_resolution_clock::time_point> m_startTimes;
    std::unordered_map<std::string, std::vector<double>> m_timingHistory;
    
    // Frame timing
    std::chrono::high_resolution_clock::time_point m_frameStartTime;
    double m_lastFrameTime;
    std::vector<double> m_frameTimeHistory;
    
    // Performance metrics
    float m_currentFPS;
    std::vector<float> m_fpsHistory;
    int m_currentParticleCount;
    std::vector<int> m_particleCountHistory;
    int m_currentPhysicsSteps;
    
    // Targets
    float m_targetFPS;
    int m_targetPhysicsSteps;
    
    // History limits
    static const size_t MAX_HISTORY_SIZE = 1000;
    
    // Helper functions
    double calculateAverage(const std::vector<double>& values) const;
    double calculateMin(const std::vector<double>& values) const;
    double calculateMax(const std::vector<double>& values) const;
    float calculateAverageFloat(const std::vector<float>& values) const;
    
    void trimHistory();
};

// RAII Timer class for automatic timing
class ScopedTimer {
public:
    ScopedTimer(PerformanceProfiler& profiler, const std::string& name)
        : m_profiler(profiler), m_name(name) {
        m_profiler.startTimer(m_name);
    }
    
    ~ScopedTimer() {
        m_profiler.endTimer(m_name);
    }
    
private:
    PerformanceProfiler& m_profiler;
    std::string m_name;
};

// Macro for easy profiling
#define PROFILE_SCOPE(profiler, name) ScopedTimer _timer(profiler, name)

#endif // PERFORMANCE_PROFILER_H
