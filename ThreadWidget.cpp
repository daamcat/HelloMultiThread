#include "ThreadWidget.h"
#include "ui_ThreadWidget.h"

#include <QThread>
#include <QApplication>
#include <QtConcurrent>
#include <QTimer>
#include <QRandomGenerator>


ThreadWidget::ThreadWidget(int threadNumber, volatile bool* stopped, int seconds, int iterations, QObject* dummyWhile, QWidget *mainWindow) :
  QWidget(mainWindow),
  ui(new Ui::ThreadWidget),
  m_receiver(mainWindow),
  m_stopped(stopped),
  m_seconds(seconds) ,
  m_iterations(iterations),
  m_threadNumber(threadNumber),
  m_dummyWhile(dummyWhile)
{
  ui->setupUi(this);
  ui->label->setText("Thread " + QString::number(m_threadNumber));
}

ThreadWidget::~ThreadWidget()
{
  delete ui;
}
void ThreadWidget::setLineEditText(QString text)
{
  ui->lineEdit->setText(text);
}


void ThreadWidget::doWhileInThread()
{
  int msecsPerIteration = std::round(1000*m_seconds/m_iterations);
  for (int i=0; i<m_iterations; i++)
  {
    if (*m_stopped)
    {
      break;
    }
    qint32 low = msecsPerIteration - 0.2*msecsPerIteration;
    qint32 high = msecsPerIteration + 0.2*msecsPerIteration;
    msecsPerIteration = QRandomGenerator::global()->bounded(low,high);
    QThread::msleep(msecsPerIteration);

    // You are inside another thread. From here you shouldn't try to access GUI:
    //ui->lineEdit->setText(QString::number(m_done) + " out of " + QString::number(m_iterations) + ".");
    QApplication::postEvent(m_dummyWhile, new ProgressEvent(m_threadNumber, true, QString::number(i) + "."));
  }
}

