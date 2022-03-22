#include "DummyWhile.h"

#include <QThread>
#include <QApplication>
#include <QtConcurrent>

DummyWhile::DummyWhile(QWidget* mainWindow, volatile bool* stopped, int seconds, int iterations) :
  m_receiver(mainWindow),
  m_stopped(stopped),
  m_seconds(seconds) ,
  m_iterations(iterations)
{

}

void DummyWhile::start()
{
  // Ref: https://doc.qt.io/qt-5/qtconcurrentrun.html#using-member-functions
  QtConcurrent::run(this, &DummyWhile::doWhileInThread);
}

void DummyWhile::doWhileInThread()
{
  int msecsPerIteration = std::round(1000*m_seconds/m_iterations);
  for (int i=0; i<m_iterations; i++)
  {
    if (*m_stopped)
    {
      break;
    }
    QThread::msleep(msecsPerIteration);

    QApplication::postEvent(m_receiver,new ProgressEvent(true,QString::number(i)));
  }
}
