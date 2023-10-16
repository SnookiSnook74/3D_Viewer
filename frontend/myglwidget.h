#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H
#define GL_SILENCE_DEPRECATION

#include <QMessageBox>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QWidget>

#include "../backend/3d_parser.h"
#include "glu.h"

#define CENTRAL 0

class MyGLWidget : public QOpenGLWidget {
  Q_OBJECT

 public:
  explicit MyGLWidget(QWidget *parent = nullptr);
  enum LineType { SOLID, DOTTED };
  enum VertexType { NONE, CIRCULE, SQUARE };
  std::vector<Vertex> transformed_vertexes;
  std::vector<Vertex> rendering_vertexes;
  void updateTransformedVertices();
  void prepareRenderingVertices();

  LineType currentLineType;
  VertexType currentVertexType;

  Model *model;
  QColor colorBackground;
  QColor colorVertices;
  QColor colorLines;

  QString filePath;

  double modelScale;
  double rotation[3];
  double translation[3];

  int countVertexes;
  int countRibs;

  int sizeLines;
  int sizeVertices;

  int protectType;
  int scaleX;
  int scaleY;
  int scaleZ;

  void fileOpen();

 protected:
  void setProjection();
  void drawModelVertices(GLenum mode);
  void computeCenterAndScale(Model *model, float &x_center, float &y_center,
                             float &z_center, float &scale);
  void initializeGL() override;
  void paintGL() override;
  void lineType();
  void verticesType();

  QPoint mousePosition;
  void mouseMoveEvent(QMouseEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
};

#endif  // MYGLWIDGET_H
