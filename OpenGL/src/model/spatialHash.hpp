#pragma once

#include "../config.hpp"

class SpatialHashGrid {
public:

    SpatialHashGrid() = default;

    SpatialHashGrid(glm::vec3 minBound, glm::vec3 maxBound, glm::ivec3 dimensions) 
        : minBound(minBound), maxBound(maxBound), dimensions(dimensions) { }

    void newClient(const int clientID, const glm::vec3& position, float visionRadius, glm::ivec3& outIndices) {
        insert(clientID, position, visionRadius, outIndices);
    }

    std::vector<int> getNeighbors(const glm::vec3& position, float visionRadius) {
        std::vector<int> neighbors;

        // Index that sets client cells, i1 is the min cell and i2 is the max cell
        glm::ivec3 i1 = getCellIndex(position - glm::vec3(visionRadius));
        glm::ivec3 i2 = getCellIndex(position + glm::vec3(visionRadius));

        int neighbors_count = NEIGHBORS_CAP;
        for(int x=i1.x; x<=i2.x; x++)
            for(int y=i1.y; y<=i2.y; y++)
                for(int z=i1.z; z<=i2.z; z++) {
                    int k = key(glm::vec3(x, y, z));
                    
                    for(int id: cells[k]) {
                        neighbors.push_back(id);
                        if(neighbors_count-- == 0) return neighbors;
                    }
                }

        return neighbors;
    }

    void removeClient(const int id, glm::ivec3& indices) {
        int k = key(indices);
        cells[k].erase(id);
    }

    void update(int id, glm::vec3& position, float visionRadius, glm::ivec3& indices) {
        removeClient(id, indices);
        insert(id, position, visionRadius, indices);
    }

private:
    glm::vec3 minBound;
    glm::vec3 maxBound;
    glm::ivec3 dimensions;
    std::unordered_map<int, std::unordered_set<int>> cells;

    void insert(const int id, const glm::vec3& position, float visionRadius, glm::ivec3& outCellIndex) {
        glm::ivec3 cellIdx = getCellIndex(position);
        int k = key(cellIdx);
        cells[k].insert(id);

        outCellIndex = cellIdx;
    }

    int key(glm::ivec3 cell) {
        // Multiplying by the last axis guarantees uniqueness
        return cell.x + (cell.y * dimensions.x) + (cell.z * dimensions.x * dimensions.y);
    }

    glm::ivec3 getCellIndex(const glm::vec3& position) {
        glm::vec3 normalizedPos = glm::clamp((position - minBound) / (maxBound - minBound), 0.0f, 1.0f);

        int xIndex = static_cast<int>(normalizedPos.x * (dimensions.x - 1));
        int yIndex = static_cast<int>(normalizedPos.y * (dimensions.y - 1));
        int zIndex = static_cast<int>(normalizedPos.z * (dimensions.z - 1));

        return glm::ivec3(xIndex, yIndex, zIndex);
    }
};