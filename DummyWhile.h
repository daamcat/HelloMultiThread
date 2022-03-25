#ifndef DUMMYWHILE_H
#define DUMMYWHILE_H

#include "ThreadWidget.h"

#include <QWidget>
#include <QGroupBox>


class DummyWhile : public QObject
{
  // We needed to derive this class from QObject in order to call its function member using QTimer::SingleShot.
  Q_OBJECT
public:
  DummyWhile(QWidget* mainWindow , QGroupBox* groupBox, volatile bool* stopped, int seconds, int iterations);
  ~DummyWhile();
  void startUsingQtConcurrent();

  void setTotal(int32_t total);

signals:
  void signalUpdateUi();


private:
  void checkIfDone();

  QWidget* m_receiver;
  volatile bool* m_stopped;
  int m_seconds;
  int m_iterations;
  const int m_pollTimeOut = 100;

  int32_t m_total; // Total number of iterations.
  int32_t m_done; // Number of processed iterations.

  QGroupBox* m_groupBox;

  QVector<ThreadWidget*> m_threadWidgets;


};

#endif // DUMMYWHILE_H
