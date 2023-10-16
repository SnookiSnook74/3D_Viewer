#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define GL_SILENCE_DEPRECATION

#include <QColorDialog>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QMainWindow>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void saveSettings();
  void loadSettings();

 private slots:
  void on_open_file_clicked();
  void on_make_gif_clicked();
  void on_screenshot_start_clicked();

  void on_background_color_clicked();
  void on_vertices_color_clicked();
  void on_line_color_clicked();
  void on_line_solid_activated(int index);

  void on_scale_line_valueChanged(int value);
  void on_clean_all_clicked();
  void on_scale_value_valueChanged(int value);

  void on_scale_vertices_valueChanged(int value);
  void on_vertices_slider_activated(int index);

  void on_protection_activated(int index);

  void on_translate_x_valueChanged(double arg1);
  void on_translate_y_valueChanged(double arg1);
  void on_translaye_z_valueChanged(double arg1);

  void on_rotation_x_valueChanged(double arg1);
  void on_rotation_y_valueChanged(double arg1);
  void on_rotation_z_valueChanged(double arg1);

  void closeEvent(QCloseEvent *event) override;

 private:
  Ui::MainWindow *ui;
};
#endif  // MAINWINDOW_H
