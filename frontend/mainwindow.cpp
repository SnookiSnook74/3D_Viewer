#include "mainwindow.h"

#include "gif.h"
#include "myglwidget.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  QSurfaceFormat format;
  // Устанавливаем количество образцов для многократного сглаживания
  format.setSamples(4);
  ui->openGLWidget->setFormat(format);  // Применяем формат к виджету
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_open_file_clicked() {
  QString filePath =
      QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("(*.obj)"));
  QFileInfo fileInfo(filePath);
  QString fileName = fileInfo.fileName();
  if (fileName.size() > 0) {
    ui->openGLWidget->filePath = filePath;
    ui->openGLWidget->fileOpen();
    ui->file_name->setText("File Name: " + fileName);
    ui->vertexes_count->setText(
        "Vertices Count: " + QString::number(ui->openGLWidget->countVertexes));
    ui->ribs_count->setText("Ribs Count: " +
                            QString::number(ui->openGLWidget->countRibs));
  }
}

void MainWindow::on_background_color_clicked() {
  QColor colour = QColorDialog::getColor(Qt::white, this, "Select color:");
  ui->openGLWidget->colorBackground = colour;
  ui->openGLWidget->update();
}

void MainWindow::on_vertices_color_clicked() {
  QColor colour = QColorDialog::getColor(Qt::white, this, "Select color:");
  ui->openGLWidget->colorVertices = colour;
  ui->openGLWidget->update();
}

void MainWindow::on_line_color_clicked() {
  QColor colour = QColorDialog::getColor(Qt::white, this, "Select color:");
  ui->openGLWidget->colorLines = colour;
  ui->openGLWidget->update();
}

void MainWindow::on_line_solid_activated(int index) {
  ui->openGLWidget->currentLineType = static_cast<MyGLWidget::LineType>(index);
  ui->openGLWidget->update();
}

void MainWindow::on_scale_line_valueChanged(int value) {
  ui->openGLWidget->sizeLines = (double)value;
  ui->openGLWidget->update();
}

void MainWindow::on_scale_vertices_valueChanged(int value) {
  ui->openGLWidget->sizeVertices = (double)value;
  ui->openGLWidget->update();
}

void MainWindow::on_vertices_slider_activated(int index) {
  ui->openGLWidget->currentVertexType =
      static_cast<MyGLWidget::VertexType>(index);
  ui->openGLWidget->update();
}

void MainWindow::on_scale_value_valueChanged(int value) {
  ui->openGLWidget->modelScale = value;
  ui->openGLWidget->updateTransformedVertices();
  ui->openGLWidget->update();
}

void MainWindow::on_rotation_x_valueChanged(double arg1) {
  ui->openGLWidget->rotation[0] = arg1;
  ui->openGLWidget->updateTransformedVertices();
  ui->openGLWidget->update();
}

void MainWindow::on_rotation_y_valueChanged(double arg1) {
  ui->openGLWidget->rotation[1] = arg1;
  ui->openGLWidget->updateTransformedVertices();
  ui->openGLWidget->update();
}

void MainWindow::on_rotation_z_valueChanged(double arg1) {
  ui->openGLWidget->rotation[2] = arg1;
  ui->openGLWidget->updateTransformedVertices();
  ui->openGLWidget->update();
}

void MainWindow::on_protection_activated(int index) {
  ui->openGLWidget->protectType = index;
  ui->openGLWidget->update();
}

void MainWindow::on_translate_x_valueChanged(double arg1) {
  ui->openGLWidget->translation[0] = arg1;
  ui->openGLWidget->updateTransformedVertices();
  ui->openGLWidget->update();
}

void MainWindow::on_translate_y_valueChanged(double arg1) {
  ui->openGLWidget->translation[1] = arg1;
  ui->openGLWidget->updateTransformedVertices();
  ui->openGLWidget->update();
}

void MainWindow::on_translaye_z_valueChanged(double arg1) {
  ui->openGLWidget->translation[2] = arg1;
  ui->openGLWidget->updateTransformedVertices();
  ui->openGLWidget->update();
}

void MainWindow::on_clean_all_clicked() {
  ui->openGLWidget->colorLines = QColor(255, 255, 255);
  ui->openGLWidget->colorVertices = QColor(255, 255, 255);
  ui->openGLWidget->colorBackground = QColor(0, 0, 0);
  ui->translate_x->setValue(0.0);
  ui->translate_y->setValue(0.0);
  ui->translaye_z->setValue(0.0);
  ui->rotation_x->setValue(0.0);
  ui->rotation_y->setValue(0.0);
  ui->rotation_z->setValue(0.0);
  ui->vertices_slider->setCurrentIndex(0);
  ui->protection->setCurrentIndex(0);
  ui->line_solid->setCurrentIndex(0);
  ui->openGLWidget->currentLineType = static_cast<MyGLWidget::LineType>(0);
  ui->openGLWidget->sizeLines = 0;
  ui->openGLWidget->currentVertexType = static_cast<MyGLWidget::VertexType>(0);
  ui->openGLWidget->sizeVertices = 0;
  ui->scale_value->setValue(0);
  ui->scale_vertices->setValue(0);
  ui->scale_line->setValue(1);
  ui->openGLWidget->filePath = "";
  ui->file_name->setText("File Name: ");
  ui->vertexes_count->setText("Vertices Count: ");
  ui->ribs_count->setText("Ribs Count: ");
  _reset_model(&ui->openGLWidget->model);
  ui->openGLWidget->update();
}

void MainWindow::saveSettings() {
  // Получаем путь к директории исполняемого файла
  QString settingsPath =
      QCoreApplication::applicationDirPath() + "/settings.ini";
  QSettings settings(settingsPath, QSettings::IniFormat);

  settings.beginGroup("Settings");

  // Сохраняем путь к модели
  settings.setValue("modelPath", ui->openGLWidget->filePath);

  // Сохраняем цвета
  settings.setValue("backgroundColor", ui->openGLWidget->colorBackground);
  settings.setValue("lineColor", ui->openGLWidget->colorLines);
  settings.setValue("vertexColor", ui->openGLWidget->colorVertices);

  // Сохраняем положение

  settings.setValue("rotation_x", ui->rotation_x->value());
  settings.setValue("rotation_y", ui->rotation_y->value());
  settings.setValue("rotation_z", ui->rotation_z->value());

  // Cохраняем позицию

  settings.setValue("translate_x", ui->translate_x->value());
  settings.setValue("translate_y", ui->translate_y->value());
  settings.setValue("translate_z", ui->translaye_z->value());

  // Позиции слайдеров

  settings.setValue("scale_model", ui->scale_value->value());
  settings.setValue("scale_vertices", ui->scale_vertices->value());
  settings.setValue("scale_line", ui->scale_line->value());

  // Позиции окон выбора
  settings.setValue("protection", ui->protection->currentIndex());
  settings.setValue("box_line", ui->line_solid->currentIndex());
  settings.setValue("box_vertices", ui->vertices_slider->currentIndex());

  settings.endGroup();  // Закончить группировку
}

void MainWindow::loadSettings() {
  QString fileName;
  QString settingsPath =
      QCoreApplication::applicationDirPath() + "/settings.ini";
  QSettings settings(settingsPath, QSettings::IniFormat);

  settings.beginGroup("Settings");

  fileName = settings.value("modelPath").toString();

  if (!fileName.isEmpty()) {
    ui->openGLWidget->filePath = fileName;
    ui->openGLWidget->fileOpen();

    QFileInfo fileInfo(fileName);
    QString onlyFileName = fileInfo.fileName();

    ui->file_name->setText("File Name: " + onlyFileName);
    ui->vertexes_count->setText(
        "Vertices Count: " + QString::number(ui->openGLWidget->countVertexes));
    ui->ribs_count->setText("Ribs Count: " +
                            QString::number(ui->openGLWidget->countRibs));
  }

  ui->openGLWidget->colorBackground =
      settings.value("backgroundColor", QColor(0, 0, 0)).value<QColor>();
  ui->openGLWidget->colorLines =
      settings.value("lineColor", QColor(255, 255, 255)).value<QColor>();
  ui->openGLWidget->colorVertices =
      settings.value("vertexColor", QColor(255, 255, 255)).value<QColor>();

  ui->rotation_x->setValue(settings.value("rotation_x").toDouble());
  ui->rotation_y->setValue(settings.value("rotation_y").toDouble());
  ui->rotation_z->setValue(settings.value("rotation_z").toDouble());

  ui->translate_x->setValue(settings.value("translate_x").toDouble());
  ui->translate_y->setValue(settings.value("translate_y").toDouble());
  ui->translaye_z->setValue(settings.value("translate_z").toDouble());

  ui->scale_value->setValue(settings.value("scale_model").toInt());
  ui->scale_vertices->setValue(settings.value("scale_vertices").toInt());
  ui->scale_line->setValue(settings.value("scale_line").toInt());

  ui->openGLWidget->currentLineType = static_cast<MyGLWidget::LineType>(
      settings.value("box_line", MyGLWidget::SOLID).toInt());
  ui->line_solid->setCurrentIndex(ui->openGLWidget->currentLineType);

  ui->openGLWidget->currentVertexType = static_cast<MyGLWidget::VertexType>(
      settings.value("box_vertices", MyGLWidget::NONE).toInt());
  ui->vertices_slider->setCurrentIndex(ui->openGLWidget->currentVertexType);

  ui->openGLWidget->protectType = settings.value("protection", 0).toInt();
  ui->protection->setCurrentIndex(ui->openGLWidget->protectType);

  settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent *event) {
  saveSettings();
  _reset_model(&ui->openGLWidget->model);
  QMainWindow::closeEvent(event);
}

/*!
 * \brief Сохраняет снимок экрана виджета OpenGL.
 *
 * При вызове этой функции создается снимок экрана текущего содержимого
 * виджета OpenGL. Затем открывается диалоговое окно, позволяя пользователю
 * выбрать место для сохранения снимка и формат файла (JPEG или BMP).
 * После выбора места и формата сохранения, снимок сохраняется на диск.
 * Пользователю показывается сообщение об успешном сохранении.
 */
void MainWindow::on_screenshot_start_clicked() {
  QPixmap screenshot = ui->openGLWidget->grab();
  QString selectedFilter;
  QString screenshotPath = QFileDialog::getSaveFileName(
      this, tr("Save Screenshot"), "",
      tr("JPEG Image (*.jpg);;BMP Image (*.bmp)"), &selectedFilter);
  if (!screenshotPath.isEmpty()) {
    QString format;
    if (selectedFilter.contains("JPEG")) {
      format = "JPG";
    } else if (selectedFilter.contains("BMP")) {
      format = "BMP";
    }
    screenshot.save(screenshotPath, format.toLocal8Bit().constData());
    QMessageBox msgBox;
    msgBox.setText("Saved successfully.");
    msgBox.exec();
  }
}

/*!
 * \brief Создает GIF-анимацию из рендеринга виджета OpenGL.
 *
 * При вызове этой функции открывается диалоговое окно для сохранения файла,
 * позволяя пользователю выбрать местоположение и имя для сохраняемого GIF.
 * Если пользователь выберет файл, функция будет рендерить виджет OpenGL
 * в изображение 50 раз, масштабируя каждый кадр до размеров 640x480 и
 * добавляя его в GIF-файл.
 *
 * \note Длительность каждого кадра в GIF составляет 100 миллисекунд.
 */
void MainWindow::on_make_gif_clicked() {
  QString gifFileName = QFileDialog::getSaveFileName(
      this, tr("Save gif"), QDir::currentPath(), tr("GIF (*.gif)"));
  if (!gifFileName.isEmpty()) {
    QImage originalImage(ui->openGLWidget->size(), QImage::Format_RGB32);
    QPainter imagePainter(&originalImage);
    QImage resizedImage;
    QElapsedTimer frameTimer;
    int gifWidth = 640;
    int gifHeight = 480;
    GifWriter gifWriter;
    QByteArray fileNameByteArray = gifFileName.toLocal8Bit();
    const char *fileNameCStr = fileNameByteArray.data();
    if (GifBegin(&gifWriter, fileNameCStr, gifWidth, gifHeight, 100)) {
      frameTimer.start();
      for (int frameIndex = 0; frameIndex < 50; ++frameIndex) {
        ui->openGLWidget->render(&imagePainter);
        resizedImage = originalImage.scaled(QSize(gifWidth, gifHeight),
                                            Qt::IgnoreAspectRatio,
                                            Qt::SmoothTransformation);
        GifWriteFrame(&gifWriter, resizedImage.bits(), gifWidth, gifHeight,
                      100);
        while (frameTimer.elapsed() < 100)
          QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        frameTimer.restart();
      }
      GifEnd(&gifWriter);
      QMessageBox successMsgBox;
      successMsgBox.setText("Saved successfully.");
      successMsgBox.exec();
    } else {
      QMessageBox::warning(this, "", "Failed to start gif writer.");
    }
  } else {
    QMessageBox::warning(this, "", "Failed to save gif.");
  }
}
