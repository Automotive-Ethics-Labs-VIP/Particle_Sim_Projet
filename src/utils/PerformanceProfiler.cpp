#include "PerformanceProfiler.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <numeric>

PerformanceProfiler::PerformanceProfiler()
    : m_lastFrameTime(0.0)
    , m_currentFPS(0.0f)
    , m_currentParticleCount(0)
    , m_currentPhysicsSteps(0)
    , m_targetFPS(60.0f)
    , m_targetPhysicsSteps(100) {
}

PerformanceProfiler::~PerformanceProfiler() {
    // Optional: auto-export on destruction
}

void PerformanceProfiler::startTimer(const std::string& name) {
    m_startTimes[name] = std::chrono::high_resolution_clock::now();
}

void PerformanceProfiler::endTimer(const std::string& name) {
    auto endTime = std::chrono::high_resolution_clock::now();
    
    auto it = m_startTimes.find(name);
    if (it != m_startTimes.end()) {
        auto duration = std::chrono::duration<double, std::milli>(endTime - it->second);
        m_timingHistory[name].push_back(duration.count());
        
        // Limit history size
        if (m_timingHistory[name].size() > MAX_HISTORY_SIZE) {
            m_timingHistory[name].erase(m_timingHistory[name].begin());
        }
        
        m_startTimes.erase(it);
    }
}

void PerformanceProfiler::beginFrame() {
    m_frameStartTime = std::chrono::high_resolution_clock::now();
}

void PerformanceProfiler::endFrame() {
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::milli>(endTime - m_frameStartTime);
    m_lastFrameTime = duration.count();
    
    m_frameTimeHistory.push_back(m_lastFrameTime);
    if (m_frameTimeHistory.size() > MAX_HISTORY_SIZE) {
        m_frameTimeHistory.erase(m_frameTimeHistory.begin());
    }
}

void PerformanceProfiler::updateFPS(float fps) {
    m_currentFPS = fps;
    m_fpsHistory.push_back(fps);
    if (m_fpsHistory.size() > MAX_HISTORY_SIZE) {
        m_fpsHistory.erase(m_fpsHistory.begin());
    }
}

void PerformanceProfiler::updateParticleCount(int count) {
    m_currentParticleCount = count;
    m_particleCountHistory.push_back(count);
    if (m_particleCountHistory.size() > MAX_HISTORY_SIZE) {
        m_particleCountHistory.erase(m_particleCountHistory.begin());
    }
}

void PerformanceProfiler::updatePhysicsSteps(int steps) {
    m_currentPhysicsSteps = steps;
}

PerformanceProfiler::ProfileData PerformanceProfiler::getProfileData(const std::string& name) const {
    ProfileData data;
    data.name = name;
    
    auto it = m_timingHistory.find(name);
    if (it != m_timingHistory.end() && !it->second.empty()) {
        const auto& times = it->second;
        data.totalTime = std::accumulate(times.begin(), times.end(), 0.0);
        data.minTime = calculateMin(times);
        data.maxTime = calculateMax(times);
        data.avgTime = calculateAverage(times);
        data.callCount = times.size();
    } else {
        data.totalTime = 0.0;
        data.minTime = 0.0;
        data.maxTime = 0.0;
        data.avgTime = 0.0;
        data.callCount = 0;
    }
    
    return data;
}

float PerformanceProfiler::getAverageFPS() const {
    return calculateAverageFloat(m_fpsHistory);
}

bool PerformanceProfiler::isTargetPerformanceMet() const {
    bool fpsTarget = getAverageFPS() >= (m_targetFPS * 0.9f); // 90% of target
    // Add more performance checks as needed
    return fpsTarget;
}

std::string PerformanceProfiler::getPerformanceReport() const {
    std::stringstream ss;
    
    ss << "=== Performance Report ===\n";
    ss << "Current FPS: " << std::fixed << std::setprecision(1) << m_currentFPS << "\n";
    ss << "Average FPS: " << std::fixed << std::setprecision(1) << getAverageFPS() << "\n";
    ss << "Target FPS: " << m_targetFPS << "\n";
    ss << "Target Met: " << (isTargetPerformanceMet() ? "YES" : "NO") << "\n";
    ss << "Current Particles: " << m_currentParticleCount << "\n";
    ss << "Last Frame Time: " << std::fixed << std::setprecision(2) << m_lastFrameTime << " ms\n";
    
    if (!m_frameTimeHistory.empty()) {
        ss << "Average Frame Time: " << std::fixed << std::setprecision(2) 
           << calculateAverage(m_frameTimeHistory) << " ms\n";
        ss << "Min Frame Time: " << std::fixed << std::setprecision(2) 
           << calculateMin(m_frameTimeHistory) << " ms\n";
        ss << "Max Frame Time: " << std::fixed << std::setprecision(2) 
           << calculateMax(m_frameTimeHistory) << " ms\n";
    }
    
    ss << "\n=== Timing Breakdown ===\n";
    for (const auto& pair : m_timingHistory) {
        if (!pair.second.empty()) {
            ProfileData data = getProfileData(pair.first);
            ss << pair.first << ":\n";
            ss << "  Calls: " << data.callCount << "\n";
            ss << "  Avg: " << std::fixed << std::setprecision(3) << data.avgTime << " ms\n";
            ss << "  Min: " << std::fixed << std::setprecision(3) << data.minTime << " ms\n";
            ss << "  Max: " << std::fixed << std::setprecision(3) << data.maxTime << " ms\n";
            ss << "  Total: " << std::fixed << std::setprecision(2) << data.totalTime << " ms\n\n";
        }
    }
    
    return ss.str();
}

void PerformanceProfiler::reset() {
    m_currentFPS = 0.0f;
    m_currentParticleCount = 0;
    m_currentPhysicsSteps = 0;
    m_lastFrameTime = 0.0;
}

void PerformanceProfiler::clearHistory() {
    m_timingHistory.clear();
    m_frameTimeHistory.clear();
    m_fpsHistory.clear();
    m_particleCountHistory.clear();
    m_startTimes.clear();
}

bool PerformanceProfiler::exportToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open profiling file: " << filename << std::endl;
        return false;
    }
    
    file << "{\n";
    file << "  \"performance_profile\": {\n";
    file << "    \"summary\": {\n";
    file << "      \"current_fps\": " << m_currentFPS << ",\n";
    file << "      \"average_fps\": " << getAverageFPS() << ",\n";
    file << "      \"target_fps\": " << m_targetFPS << ",\n";
    file << "      \"target_met\": " << (isTargetPerformanceMet() ? "true" : "false") << ",\n";
    file << "      \"current_particles\": " << m_currentParticleCount << ",\n";
    file << "      \"last_frame_time_ms\": " << m_lastFrameTime << "\n";
    file << "    },\n";
    
    file << "    \"timing_data\": {\n";
    size_t count = 0;
    for (const auto& pair : m_timingHistory) {
        if (!pair.second.empty()) {
            ProfileData data = getProfileData(pair.first);
            file << "      \"" << pair.first << "\": {\n";
            file << "        \"call_count\": " << data.callCount << ",\n";
            file << "        \"average_ms\": " << data.avgTime << ",\n";
            file << "        \"min_ms\": " << data.minTime << ",\n";
            file << "        \"max_ms\": " << data.maxTime << ",\n";
            file << "        \"total_ms\": " << data.totalTime << "\n";
            file << "      }";
            
            if (++count < m_timingHistory.size()) file << ",";
            file << "\n";
        }
    }
    file << "    }\n";
    file << "  }\n";
    file << "}\n";
    
    file.close();
    return true;
}

double PerformanceProfiler::calculateAverage(const std::vector<double>& values) const {
    if (values.empty()) return 0.0;
    return std::accumulate(values.begin(), values.end(), 0.0) / values.size();
}

double PerformanceProfiler::calculateMin(const std::vector<double>& values) const {
    if (values.empty()) return 0.0;
    return *std::min_element(values.begin(), values.end());
}

double PerformanceProfiler::calculateMax(const std::vector<double>& values) const {
    if (values.empty()) return 0.0;
    return *std::max_element(values.begin(), values.end());
}

float PerformanceProfiler::calculateAverageFloat(const std::vector<float>& values) const {
    if (values.empty()) return 0.0f;
    return std::accumulate(values.begin(), values.end(), 0.0f) / values.size();
}
