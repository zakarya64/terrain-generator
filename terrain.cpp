#include "terrain.h"
#include <cmath>

Terrain::Terrain() {
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);  
    noise.SetFrequency(0.05f);
}

void Terrain::generateTerrain(int width, int depth, float scale, float heightScale) {
    vertices.clear();
    indices.clear();

    for (int z = 0; z < depth; ++z) {
        for (int x = 0; x < width; ++x) {
            float y = noise.GetNoise(float(x) * scale, float(z) * scale) * heightScale;

            vertices.push_back({
                float(x), y, float(z),  
                0, 1, 0, 
                float(x) / width, float(z) / depth
            });
        }
    }

    for (int z = 0; z < depth - 1; ++z) {
        for (int x = 0; x < width - 1; ++x) {
            int topLeft = z * width + x;
            int topRight = topLeft + 1;
            int bottomLeft = topLeft + width;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    for (size_t i = 0; i < indices.size(); i += 3) {
        Vertex &v0 = vertices[indices[i]];
        Vertex &v1 = vertices[indices[i + 1]];
        Vertex &v2 = vertices[indices[i + 2]];

        QVector3D edge1(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
        QVector3D edge2(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
        QVector3D normal = QVector3D::crossProduct(edge1, edge2).normalized();

        v0.nx += normal.x(); v0.ny += normal.y(); v0.nz += normal.z();
        v1.nx += normal.x(); v1.ny += normal.y(); v1.nz += normal.z();
        v2.nx += normal.x(); v2.ny += normal.y(); v2.nz += normal.z();
    }

    for (auto &v : vertices) {
        QVector3D n(v.nx, v.ny, v.nz);
        n.normalize();
        v.nx = n.x();
        v.ny = n.y();
        v.nz = n.z();
    }
}



void Terrain::setupBuffers(QOpenGLBuffer &vbo, QOpenGLBuffer &ebo, QOpenGLVertexArrayObject &vao) {
    vao.create();
    vao.bind();

    vbo.create();
    vbo.bind();
    vbo.allocate(vertices.data(), vertices.size() * sizeof(Vertex));

    ebo.create();
    ebo.bind();
    ebo.allocate(indices.data(), indices.size() * sizeof(unsigned int));

    vao.release();
}

int Terrain::indexCount() const {
    return indices.size();
}
