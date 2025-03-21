#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "FastNoiseLite.h"

struct Vertex {
    float x, y, z;
    float nx, ny, nz;  
    float u, v;
};


class Terrain {
public:
    Terrain();
    void generateTerrain(int width, int depth, float scale, float heightScale);
    void setupBuffers(QOpenGLBuffer &vbo, QOpenGLBuffer &ebo, QOpenGLVertexArrayObject &vao);
    int indexCount() const;

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    FastNoiseLite noise;  // Perlin noise generator
};

#endif // TERRAIN_H
