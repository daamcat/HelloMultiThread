#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>

#include "MyConcurrent.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();


private slots:
  void slotPushButtonStartCancelReleased();

private:
  void updateUi();

  Ui::MainWindow *ui;

  volatile bool m_stopped = false;
  int32_t m_total; // Total number of iterations.

  int32_t m_numberOfIterations = 100;

  MyConcurrent* m_concurrent;

};

#endif // MAINWINDOW_H
