#include "myglwidget.h"

#include <iostream>

/*!
 * \brief Конструктор MyGLWidget::MyGLWidget
 * \param parent Родительский виджет
 *
 */
MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
  sizeLines = 0;
  sizeVertices = 0;
  protectType = 0;

  rotation[0] = 0.0;
  rotation[1] = 0.0;
  rotation[2] = 0.0;

  translation[0] = 0.0;
  translation[1] = 0.0;
  translation[2] = 0.0;

  modelScale = 100;

  model = NULL;
}

/*!
 * \brief Инициализация контекста OpenGL для виджета.
 *
 * Эта функция вызывается один раз перед началом отрисовки сцены в OpenGL.
 * Она настраивает начальные параметры для контекста OpenGL, такие как тест
 * глубины и цвет очистки фона. Это обеспечивает правильное отображение
 * 3D-моделей в виджете.
 */
void MyGLWidget::initializeGL() {
  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
  f->glEnable(GL_DEPTH_TEST);
  f->glClearColor(colorBackground.redF(), colorBackground.greenF(),
                  colorBackground.blueF(), colorBackground.alphaF());
}

/*!
 * \brief Отрисовывает текущую модель в виджете OpenGL.
 *
 * Эта функция вызывается каждый раз, когда виджет OpenGL нуждается в
 * перерисовке. Она настраивает параметры отображения, устанавливает цвет фона,
 * масштаб и центрирование модели. Затем, если путь к файлу модели был
 * установлен, функция отрисовывает модель с учетом выбранного стиля и цвета
 * линий.
 */
void MyGLWidget::paintGL() {
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
  f->glClearColor(colorBackground.redF(), colorBackground.greenF(),
                  colorBackground.blueF(), colorBackground.alphaF());
  f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (!filePath.isEmpty()) {
    glTranslatef(0.0f, 0.0f, -3.0f);
    setProjection();
    float x_center, y_center, z_center, scale;
    glEnable(GL_DEPTH_TEST);
    computeCenterAndScale(model, x_center, y_center, z_center, scale);
    glScalef(scale, scale, scale);
    glTranslatef(-x_center, -y_center, -z_center);
    prepareRenderingVertices();
    if (currentVertexType != NONE) {
      verticesType();
      drawModelVertices(GL_POINTS);
    }
    lineType();
    drawModelVertices(GL_LINES);
  }
}

/**
 * \brief Отрисовывает вершины модели с заданным режимом отображения.
 *
 * Эта функция применяет трансформации к вершинам модели и отображает их
 * в соответствии с переданным режимом отрисовки (точки или линии).
 * Трансформации включают вращение и масштабирование.
 *
 * \param mode Режим отрисовки для вершин (например, GL_POINTS или
 * GL_LINE_LOOP).
 */
void MyGLWidget::drawModelVertices(GLenum mode) {
  glBegin(mode);
  for (const auto &vertex : rendering_vertexes) {
    glVertex3f(vertex.x, vertex.y, vertex.z);
  }
  glEnd();
}

/**
 * @brief Подготавливает вершины для отрисовки, организуя их в пары, которые
 * представляют отрезки.
 *
 * Функция проходит по всем граням модели и для каждой грани формирует пары
 * вершин, которые представляют собой отрезки для отрисовки. Вершины берутся из
 * вектора трансформированных вершин, который был получен после применения
 * различных трансформаций (вращение, перемещение, масштабирование) к
 * оригинальным вершинам модели.
 *
 * Результат сохраняется в векторе rendering_vertexes, который будет использован
 * при последующей отрисовке модели.
 */
void MyGLWidget::prepareRenderingVertices() {
  rendering_vertexes.clear();
  for (int i = 0; i < model->num_of_faces; i++) {
    Face face = model->faces[i];
    for (int j = 0; j < face.num_of_vertexes; j++) {
      int vertexIndex1 = face.vertexes[j] - 1;
      int vertexIndex2 = face.vertexes[(j + 1) % face.num_of_vertexes] - 1;
      rendering_vertexes.push_back(transformed_vertexes[vertexIndex1]);
      rendering_vertexes.push_back(transformed_vertexes[vertexIndex2]);
    }
  }
}

/**
 * @brief Обновляет трансформированные вершины модели, применяя вращение,
 * перемещение и масштабирование.
 *
 * Для каждой вершины оригинальной модели функция последовательно применяет:
 *   1. Вращение вокруг осей X, Y и Z;
 *   2. Перемещение вдоль осей X, Y и Z;
 *   3. Масштабирование с заданным коэффициентом.
 *
 * Результат каждой трансформации сохраняется в векторе transformed_vertexes,
 * который будет использоваться при последующей отрисовке модели.
 */
void MyGLWidget::updateTransformedVertices() {
  // Углы вращения в радианах
  float angleX = rotation[0] * (3.1415926f / 180.0f);  // Градусы в радианы
  float angleY = rotation[1] * (3.1415926f / 180.0f);  // Градусы в радианы
  float angleZ = rotation[2] * (3.1415926f / 180.0f);  // Градусы в радианы

  for (int i = 0; i < model->total_vertexes; i++) {
    Vertex original_vertex = model->vertexes[i];
    // Вращение
    Vertex rotatedX = rotateX(original_vertex, angleX);
    Vertex rotatedY = rotateY(rotatedX, angleY);
    Vertex rotatedZ = rotateZ(rotatedY, angleZ);
    // Перемещение
    Vertex translatedX = translateX(rotatedZ, translation[0]);
    Vertex translatedY = translateY(translatedX, translation[1]);
    Vertex translatedZ = translateZ(translatedY, translation[2]);
    // Масштабирование
    Vertex scaledVertex = scale_vertex(translatedZ, modelScale);
    // Сохраняем трансформированную вершину в векторе
    transformed_vertexes[i] = scaledVertex;
  }
}

/**
 * @brief Устанавливает проекцию для виджета OpenGL.
 *
 * Эта функция устанавливает либо перспективную, либо ортографическую проекцию
 * в зависимости от значения переменной protectType. Перспективная проекция
 * используется, если protectType равно 0, в противном случае используется
 * ортографическая проекция.
 */
void MyGLWidget::setProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float aspectRatio = (GLfloat)width() / (GLfloat)height();
  if (protectType == CENTRAL) {
    // Перспективная проекция
    gluPerspective(45.0, aspectRatio, 0.1, 100.0);
  } else {
    // Ортографичеcкая проекция
    float orthoSize = 1.0;
    glOrtho(-orthoSize * aspectRatio, orthoSize * aspectRatio, -orthoSize,
            orthoSize, -100, 100);
  }
  glMatrixMode(GL_MODELVIEW);
}

/*!
 * \brief Открывает и загружает модель из файла.
 *
 * Эта функция открывает модель на основе пути к файлу, указанного в переменной
 * filePath. Если модель успешно загружена, функция также вычисляет количество
 * рёбер в модели. В случае ошибки при загрузке модели функция просто возвращает
 * управление.
 */
void MyGLWidget::fileOpen() {
  const char *fileName;
  QByteArray str = filePath.toLocal8Bit();
  fileName = str.constData();
  _reset_model(&model);
  _get_model(fileName, &model);
  countVertexes = model->total_vertexes;
  countRibs = model->num_of_faces * 2;
  // Инициализация вектора трансформированных вершин
  transformed_vertexes.resize(model->total_vertexes);
  // Обновление трансформированных вершин после загрузки модели
  updateTransformedVertices();
}

/*!
 * \brief Устанавливает стиль и цвет линий для отображения модели.
 *
 * Эта функция определяет, будет ли линия отображаться как пунктирная или
 * сплошная, а также устанавливает цвет линии. Если цвет не был задан, функция
 * устанавливает цвет линии на белый по умолчанию.
 */
void MyGLWidget::lineType() {
  if (currentLineType == DOTTED) {
    glLineStipple(2, 0x00FF);  // Устанавливаем длину разрыва
    glEnable(GL_LINE_STIPPLE);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }
  // Если цвет не установлен, устанавливаем в белый цвет
  if (!colorLines.isValid()) {
    colorLines = QColor(255, 255, 255);
  }
  glLineWidth(sizeLines);

  glColor3f(colorLines.redF(), colorLines.greenF(), colorLines.blueF());
}

/**
 * \brief Устанавливает стиль и цвет вершин для отображения модели.
 *
 * Эта функция определяет стиль отображения вершин (точек) на основе значения
 * переменной typeVerices. В зависимости от этого значения вершины могут
 * отображаться как гладкие точки или как спрайты. Если цвет вершин не
 * установлен, функция устанавливает цвет вершин на белый по умолчанию.
 */
void MyGLWidget::verticesType() {
  if (currentVertexType == CIRCULE) {
    glDisable(GL_POINT_SPRITE);
    glEnable(GL_POINT_SMOOTH);
  } else if (currentVertexType == SQUARE) {
    glDisable(GL_POINT_SMOOTH);
    glEnable(GL_POINT_SPRITE);
  }
  if (currentVertexType != NONE) {
    if (!colorVertices.isValid()) {
      colorVertices = QColor(255, 255, 255);
    }
    glPointSize(sizeVertices);
    glColor3f(colorVertices.redF(), colorVertices.greenF(),
              colorVertices.blueF());
  }
}

/*!
 * \brief Вычисляет центральную точку и масштаб для заданной модели.
 *
 * Эта функция вычисляет центральную точку и масштаб для модели,
 * чтобы ее можно было отцентровать и отмасштабировать при отображении.
 *
 * \param model Указатель на модель, для которой необходимо вычислить центр и
 * масштаб.
 * \param x_center Ссылка на переменную для сохранения координаты X
 * центральной точки.
 * \param y_center Ссылка на переменную для сохранения
 * координаты Y центральной точки.
 * \param z_center Ссылка на переменную для
 * сохранения координаты Z центральной точки.
 * \param scale Ссылка на переменную
 * для сохранения вычисленного масштаба.
 */
void MyGLWidget::computeCenterAndScale(Model *model, float &x_center,
                                       float &y_center, float &z_center,
                                       float &scale) {
  float x_min = model->vertexes[0].x, x_max = x_min;
  float y_min = model->vertexes[0].y, y_max = y_min;
  float z_min = model->vertexes[0].z, z_max = z_min;
  for (int j = 0; j < model->total_vertexes; j++) {
    Vertex vertex = model->vertexes[j];
    x_min = std::min(x_min, vertex.x);
    x_max = std::max(x_max, vertex.x);
    y_min = std::min(y_min, vertex.y);
    y_max = std::max(y_max, vertex.y);
    z_min = std::min(z_min, vertex.z);
    z_max = std::max(z_max, vertex.z);
  }
  scaleX = fabs(x_max) > fabs(x_min) ? fabs(x_max) : fabs(x_min);
  scaleY = fabs(y_max) > fabs(y_min) ? fabs(y_max) : fabs(y_min);
  scaleZ = fabs(z_max) > fabs(z_min) ? fabs(z_max) : fabs(z_min);
  if (scaleX < 1) scaleX = 1;
  if (scaleY < 1) scaleY = 1;
  if (scaleZ < 1) scaleZ = 1;

  x_center = (x_min + x_max) / 2;
  y_center = (y_min + y_max) / 2;
  z_center = (z_min + z_max) / 2;

  // Вычисляем максимальный размер модели по любой из осей
  float max_size = std::max({x_max - x_min, y_max - y_min, z_max - z_min});

  // Вычисляем масштабный коэффициент
  scale = 0.01 / max_size;
}

void MyGLWidget::mousePressEvent(QMouseEvent *mousePressed) {
  mousePosition = mousePressed->pos();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event) {
  // Чувствительность вращения
  const float sensitivity = 0.09f;
  float dx = sensitivity * (event->position().x() - mousePosition.x());
  float dy = sensitivity * (event->position().y() - mousePosition.y());

  rotation[1] += dx;
  rotation[0] += dy;

  // Обновляем трансформированные вершины после вращения
  updateTransformedVertices();

  mousePosition = event->pos();
  update();
}
