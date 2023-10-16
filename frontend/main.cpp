#include <QApplication>
#include <QFont>
#include <QFontDatabase>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QFontDatabase::addApplicationFont(":/resurses/fronts/Quicksand-Bold.ttf");
  QFontDatabase::addApplicationFont(":/resurses/fronts/Quicksand-Regular.ttf");
  QFontDatabase::addApplicationFont(":/resurses/fronts/Quicksand-Medium.ttf");
  MainWindow w;
  w.show();
  w.loadSettings();
  return a.exec();
}
