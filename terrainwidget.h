#ifndef TERRAINWIDGET_H
#define TERRAINWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QSlider>
#include <QVBoxLayout>
#include "terrain.h"

class TerrainWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    TerrainWidget(QWidget *parent = nullptr);
    ~TerrainWidget();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private slots:
    void updateFrequency(int value);
    void updateHeightScale(int value);

private:
    QOpenGLShaderProgram shaderProgram;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo, ebo;
    Terrain terrain;

    float frequency = 0.05f;
    float heightScale = 5.0f;
    
    QSlider *freqSlider;
    QSlider *heightSlider;
    QVBoxLayout *layout;
};

#endif // TERRAINWIDGET_H
