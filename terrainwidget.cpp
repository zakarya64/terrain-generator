#include "terrainwidget.h"
#include <QOpenGLShader>
#include <QMatrix4x4>
#include <QDebug>
#include <QHBoxLayout>

TerrainWidget::TerrainWidget(QWidget *parent) : QOpenGLWidget(parent) {
    // UI Controls for adjusting Perlin Noise parameters
    QWidget *controlsWidget = new QWidget(this);
    layout = new QVBoxLayout(controlsWidget);

    freqSlider = new QSlider(Qt::Horizontal);
    freqSlider->setRange(1, 100);  // Scale factor (1 = 0.01, 100 = 1.0)
    freqSlider->setValue(int(frequency * 100));

    heightSlider = new QSlider(Qt::Horizontal);
    heightSlider->setRange(1, 100);  // Height range (1 - 10)
    heightSlider->setValue(int(heightScale));

    layout->addWidget(freqSlider);
    layout->addWidget(heightSlider);
    
    connect(freqSlider, &QSlider::valueChanged, this, &TerrainWidget::updateFrequency);
    connect(heightSlider, &QSlider::valueChanged, this, &TerrainWidget::updateHeightScale);
}

TerrainWidget::~TerrainWidget() {
    makeCurrent();
    vao.destroy();
    vbo.destroy();
    ebo.destroy();
    doneCurrent();
}

void TerrainWidget::initializeGL() {
    initializeOpenGLFunctions();
    
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertexshader.glsl");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragmentshader.glsl");
    shaderProgram.link();

    terrain.generateTerrain(50, 50, frequency, heightScale);
    terrain.setupBuffers(vbo, ebo, vao);
}

void TerrainWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.bind();
    vao.bind();

    QMatrix4x4 model, view, projection;
    model.setToIdentity();
    view.lookAt(QVector3D(0, 10, 20), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    projection.perspective(45.0f, float(width()) / height(), 0.1f, 100.0f);

    shaderProgram.setUniformValue("model", model);
    shaderProgram.setUniformValue("view", view);
    shaderProgram.setUniformValue("projection", projection);

    // Set lighting uniforms
    shaderProgram.setUniformValue("lightDir", QVector3D(-0.2f, -1.0f, -0.3f));
    shaderProgram.setUniformValue("lightColor", QVector3D(1.0f, 1.0f, 1.0f));
    shaderProgram.setUniformValue("viewPos", QVector3D(0.0f, 10.0f, 20.0f));

    glDrawElements(GL_TRIANGLES, terrain.indexCount(), GL_UNSIGNED_INT, nullptr);

    vao.release();
    shaderProgram.release();
}


void TerrainWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

// Real-time update methods
void TerrainWidget::updateFrequency(int value) {
    frequency = float(value) / 100.0f;
    terrain.generateTerrain(50, 50, frequency, heightScale);
    update();
}

void TerrainWidget::updateHeightScale(int value) {
    heightScale = float(value);
    terrain.generateTerrain(50, 50, frequency, heightScale);
    update();
}
