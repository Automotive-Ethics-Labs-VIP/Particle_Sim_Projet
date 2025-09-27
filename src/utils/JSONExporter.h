#ifndef JSON_EXPORTER_H
#define JSON_EXPORTER_H

#include "../particle/ParticleSystem.h"
#include <string>
#include <vector>
#include <fstream>

// Simple JSON-like data structures (avoiding external dependencies for now)
struct ParticleData {
    float position_x, position_y;
    float velocity_x, velocity_y;
    float acceleration_x, acceleration_y;
    float mass;
    float radius;
    double timestamp;
};

struct SimulationFrame {
    double timestamp;
    int frameNumber;
    std::vector<ParticleData> particles;
    float fps;
    int particleCount;
};

class JSONExporter {
public:
    JSONExporter();
    ~JSONExporter();
    
    // Data collection
    void captureFrame(const ParticleSystem& system, double timestamp, int frameNumber, float fps);
    void addCustomData(const std::string& key, const std::string& value);
    
    // Export functions
    bool exportToFile(const std::string& filename) const;
    bool exportCurrentFrame(const std::string& filename) const;
    
    // Data management
    void clearData();
    size_t getFrameCount() const { return m_frames.size(); }
    
    // Configuration
    void setMaxFrames(size_t maxFrames) { m_maxFrames = maxFrames; }
    void setExportOnDestroy(bool enabled) { m_exportOnDestroy = enabled; }
    void setAutoExportFilename(const std::string& filename) { m_autoExportFilename = filename; }
    
    // Statistics
    size_t getTotalDataSize() const;
    double getDataRate() const; // MB per hour
    
private:
    std::vector<SimulationFrame> m_frames;
    std::vector<std::pair<std::string, std::string>> m_customData;
    
    size_t m_maxFrames;
    bool m_exportOnDestroy;
    std::string m_autoExportFilename;
    
    double m_firstFrameTime;
    double m_lastFrameTime;
    
    // Helper functions
    std::string frameToJSON(const SimulationFrame& frame) const;
    std::string particleToJSON(const ParticleData& particle) const;
    std::string customDataToJSON() const;
    
    // Data size calculation
    size_t calculateFrameSize(const SimulationFrame& frame) const;
};

#endif // JSON_EXPORTER_H
