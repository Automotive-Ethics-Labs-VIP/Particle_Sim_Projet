#include "JSONExporter.h"
#include <iostream>
#include <iomanip>
#include <sstream>

JSONExporter::JSONExporter()
    : m_maxFrames(1000)
    , m_exportOnDestroy(false)
    , m_autoExportFilename("simulation_data.json")
    , m_firstFrameTime(0.0)
    , m_lastFrameTime(0.0) {
}

JSONExporter::~JSONExporter() {
    if (m_exportOnDestroy && !m_frames.empty()) {
        exportToFile(m_autoExportFilename);
    }
}

void JSONExporter::captureFrame(const ParticleSystem& system, double timestamp, int frameNumber, float fps) {
    SimulationFrame frame;
    frame.timestamp = timestamp;
    frame.frameNumber = frameNumber;
    frame.fps = fps;
    
    const auto& particles = system.getParticles();
    frame.particleCount = particles.size();
    
    // Convert particles to data structure
    for (const auto& particle : particles) {
        ParticleData data;
        data.position_x = particle.position.x;
        data.position_y = particle.position.y;
        data.velocity_x = particle.velocity.x;
        data.velocity_y = particle.velocity.y;
        data.acceleration_x = particle.acceleration.x;
        data.acceleration_y = particle.acceleration.y;
        data.mass = particle.mass;
        data.radius = particle.radius;
        data.timestamp = timestamp;
        
        frame.particles.push_back(data);
    }
    
    // Track timing for data rate calculation
    if (m_frames.empty()) {
        m_firstFrameTime = timestamp;
    }
    m_lastFrameTime = timestamp;
    
    // Add frame to collection
    m_frames.push_back(frame);
    
    // Limit memory usage
    if (m_frames.size() > m_maxFrames) {
        m_frames.erase(m_frames.begin());
    }
}

void JSONExporter::addCustomData(const std::string& key, const std::string& value) {
    m_customData.push_back({key, value});
}

bool JSONExporter::exportToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for export: " << filename << std::endl;
        return false;
    }
    
    file << "{\n";
    file << "  \"simulation_data\": {\n";
    file << "    \"metadata\": {\n";
    file << "      \"total_frames\": " << m_frames.size() << ",\n";
    file << "      \"start_time\": " << std::fixed << std::setprecision(6) << m_firstFrameTime << ",\n";
    file << "      \"end_time\": " << std::fixed << std::setprecision(6) << m_lastFrameTime << ",\n";
    file << "      \"duration\": " << (m_lastFrameTime - m_firstFrameTime) << ",\n";
    file << "      \"data_rate_mb_per_hour\": " << getDataRate() << "\n";
    file << "    },\n";
    
    // Custom data
    if (!m_customData.empty()) {
        file << "    \"custom_data\": {\n";
        for (size_t i = 0; i < m_customData.size(); ++i) {
            file << "      \"" << m_customData[i].first << "\": \"" << m_customData[i].second << "\"";
            if (i < m_customData.size() - 1) file << ",";
            file << "\n";
        }
        file << "    },\n";
    }
    
    // Frames data
    file << "    \"frames\": [\n";
    for (size_t i = 0; i < m_frames.size(); ++i) {
        file << frameToJSON(m_frames[i]);
        if (i < m_frames.size() - 1) file << ",";
        file << "\n";
    }
    file << "    ]\n";
    file << "  }\n";
    file << "}\n";
    
    file.close();
    
    std::cout << "Exported " << m_frames.size() << " frames to " << filename << std::endl;
    std::cout << "Data rate: " << getDataRate() << " MB/hour" << std::endl;
    
    return true;
}

bool JSONExporter::exportCurrentFrame(const std::string& filename) const {
    if (m_frames.empty()) {
        std::cerr << "No frames to export" << std::endl;
        return false;
    }
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for export: " << filename << std::endl;
        return false;
    }
    
    file << "{\n";
    file << "  \"current_frame\": " << frameToJSON(m_frames.back()) << "\n";
    file << "}\n";
    
    file.close();
    return true;
}

void JSONExporter::clearData() {
    m_frames.clear();
    m_customData.clear();
    m_firstFrameTime = 0.0;
    m_lastFrameTime = 0.0;
}

size_t JSONExporter::getTotalDataSize() const {
    size_t totalSize = 0;
    for (const auto& frame : m_frames) {
        totalSize += calculateFrameSize(frame);
    }
    return totalSize;
}

double JSONExporter::getDataRate() const {
    if (m_frames.size() < 2) return 0.0;
    
    double durationHours = (m_lastFrameTime - m_firstFrameTime) / 3600.0;
    if (durationHours <= 0.0) return 0.0;
    
    double totalSizeMB = getTotalDataSize() / (1024.0 * 1024.0);
    return totalSizeMB / durationHours;
}

std::string JSONExporter::frameToJSON(const SimulationFrame& frame) const {
    std::stringstream ss;
    ss << "      {\n";
    ss << "        \"timestamp\": " << std::fixed << std::setprecision(6) << frame.timestamp << ",\n";
    ss << "        \"frame_number\": " << frame.frameNumber << ",\n";
    ss << "        \"fps\": " << std::fixed << std::setprecision(2) << frame.fps << ",\n";
    ss << "        \"particle_count\": " << frame.particleCount << ",\n";
    ss << "        \"particles\": [\n";
    
    for (size_t i = 0; i < frame.particles.size(); ++i) {
        ss << particleToJSON(frame.particles[i]);
        if (i < frame.particles.size() - 1) ss << ",";
        ss << "\n";
    }
    
    ss << "        ]\n";
    ss << "      }";
    
    return ss.str();
}

std::string JSONExporter::particleToJSON(const ParticleData& particle) const {
    std::stringstream ss;
    ss << "          {\n";
    ss << "            \"position\": [" << particle.position_x << ", " << particle.position_y << "],\n";
    ss << "            \"velocity\": [" << particle.velocity_x << ", " << particle.velocity_y << "],\n";
    ss << "            \"acceleration\": [" << particle.acceleration_x << ", " << particle.acceleration_y << "],\n";
    ss << "            \"mass\": " << particle.mass << ",\n";
    ss << "            \"radius\": " << particle.radius << "\n";
    ss << "          }";
    
    return ss.str();
}

size_t JSONExporter::calculateFrameSize(const SimulationFrame& frame) const {
    // Rough estimate of JSON size in bytes
    size_t frameOverhead = 200; // metadata, brackets, etc.
    size_t particleSize = 150; // estimated bytes per particle in JSON
    return frameOverhead + (frame.particles.size() * particleSize);
}
