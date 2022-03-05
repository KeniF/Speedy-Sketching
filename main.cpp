#include "mainwindow.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow *w = new MainWindow(NULL);
  w->showNormal();
  return a.exec();
}
