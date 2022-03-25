#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>

#include "DummyWhile.h"

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

protected:
  bool event(QEvent* event);

private:
  void updateUi();

  Ui::MainWindow *ui;

  volatile bool m_stopped = false;
  int32_t m_total; // Total number of iterations.

  int32_t m_numberOfIterations = 100;

  DummyWhile* m_dummy;

};

#endif // MAINWINDOW_H
