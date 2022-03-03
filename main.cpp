#include "mainwindow.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[]) {
  using namespace std;
  QApplication a(argc, argv);
  MainWindow *w = new MainWindow(NULL);
  w->showMaximized();
  // w->adjustSliderH();
  return a.exec();
} // main
