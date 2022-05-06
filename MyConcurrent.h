#ifndef MYCONCURRENT_H
#define MYCONCURRENT_H

#include "ThreadWidget.h"

#include <QWidget>
#include <QGroupBox>
#include <QMap>


class MyConcurrent : public QObject
{
  // We needed to derive this class from QObject in order to call its function member using QTimer::SingleShot.
  Q_OBJECT
public:
  MyConcurrent(QWidget* mainWindow , QGroupBox* groupBox, volatile bool* stopped, int seconds, int iterations);
  ~MyConcurrent();
  void startUsingQtConcurrent();
  void startUsingQRunnable();

  void setTotal(int32_t total);

public slots:

  void announceProgress(int threadNumber_, bool successful_ , int progress_, const QString& message_);

signals:
  void signalUpdateUi();



protected:
  bool event(QEvent* event);


private:
  void checkIfDone();
  void checkIfDoneQRunnable();

  QWidget* m_receiver;
  volatile bool* m_stopped;
  int m_seconds;
  int m_iterations;
  const int m_pollTimeOut = 100;

  int32_t m_total; // Total number of iterations.
  int32_t m_done; // Number of processed iterations.

  QGroupBox* m_groupBox;

  QMap<int32_t,ThreadWidget*> m_threadWidgets;


};

#endif // MYCONCURRENT_H
