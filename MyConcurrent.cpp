#include "MyConcurrent.h"
#include "MyRunnable.h"

#include <QThread>
#include <QApplication>
#include <QtConcurrent>
#include <QTimer>
#include <QVBoxLayout>

MyConcurrent::MyConcurrent(QWidget* mainWindow, QGroupBox* groupBox, volatile bool* stopped, int seconds, int iterations) :
  m_receiver(mainWindow),
  m_stopped(stopped),
  m_seconds(seconds) ,
  m_iterations(iterations),
  m_groupBox(groupBox)
{

}

bool MyConcurrent::event(QEvent* event)
{
  if (event->type() == static_cast<QEvent::Type>(ProgressEvent::EventId))
  {
    ProgressEvent* progressEvent = static_cast<ProgressEvent*>(event);
    Q_ASSERT(progressEvent); // Just for testing. Prints warning message if the statement inside is False.
    m_threadWidgets.value(progressEvent->m_threadNumber)->setLineEditText(progressEvent->m_message);
    m_threadWidgets.value(progressEvent->m_threadNumber)->setProgress(progressEvent->m_progress);
    return true;
  }
  return QObject::event(event);
}

void MyConcurrent::announceProgress(int threadNumber_, bool successful_ , int progress_, const QString& message_)
{
  m_threadWidgets.value(threadNumber_)->setLineEditText(message_);
  m_threadWidgets.value(threadNumber_)->setProgress(progress_);
}

MyConcurrent::~MyConcurrent()
{
  *m_stopped = true;
  if (QThreadPool::globalInstance()->activeThreadCount())
  {
    QThreadPool::globalInstance()->waitForDone();
  }
}

void MyConcurrent::setTotal(int32_t total)
{
  m_total = total;
}

void MyConcurrent::startUsingQtConcurrent()
{
  qDeleteAll(m_groupBox->findChildren<ThreadWidget*>(QString(),Qt::FindDirectChildrenOnly));
  delete(m_groupBox->layout());

  m_done = 0;
  QVBoxLayout* vBoxLayout = new QVBoxLayout();
  // Ref: https://doc.qt.io/qt-5/qtconcurrentrun.html#using-member-functions
  for (int iThread = 1; iThread <= QThread::idealThreadCount(); iThread++)
  {
    ThreadWidget* widget = new ThreadWidget(iThread, m_stopped,m_seconds,m_iterations,this,m_receiver);
    vBoxLayout->addWidget(widget);
    m_threadWidgets[iThread] = widget;

    QtConcurrent::run(widget, &ThreadWidget::loopInThreadPostEvent);

  }
  m_groupBox->setLayout(vBoxLayout);
  this->checkIfDone();

}
void MyConcurrent::startUsingQRunnable()
{
  qDeleteAll(m_groupBox->findChildren<ThreadWidget*>(QString(),Qt::FindDirectChildrenOnly));
  delete(m_groupBox->layout());

  m_done = 0;
  QVBoxLayout* vBoxLayout = new QVBoxLayout();
  // Ref: https://doc.qt.io/qt-5/qtconcurrentrun.html#using-member-functions
  for (int iThread = 1; iThread <= QThread::idealThreadCount(); iThread++)
  {
    ThreadWidget* widget = new ThreadWidget(iThread, m_stopped,m_seconds,m_iterations,this,m_receiver);
    vBoxLayout->addWidget(widget);
    m_threadWidgets[iThread] = widget;

    MyRunnable* myRunnable = new MyRunnable(widget);
    QThreadPool::globalInstance()->start(myRunnable);
  }
  m_groupBox->setLayout(vBoxLayout);
  this->checkIfDoneQRunnable();
}


void MyConcurrent::checkIfDone()
{
  if (QThreadPool::globalInstance()->activeThreadCount())
  {
    QTimer::singleShot(m_pollTimeOut, this, &MyConcurrent::checkIfDone);
  }
  else
  {
    if (m_done == m_total)
    {
      QApplication::postEvent(m_receiver,new ProgressEvent(100,true,m_total,"All is done."));
    }
    else
    {
      QApplication::postEvent(m_receiver,new ProgressEvent(100,true,m_done, QString::number(m_done) + " out of " + QString::number(m_total) + " is done."));
    }
    *m_stopped = true;
    emit signalUpdateUi();
  }
}

void MyConcurrent::checkIfDoneQRunnable()
{
  if (QThreadPool::globalInstance()->activeThreadCount())
  {
    QTimer::singleShot(m_pollTimeOut, this, &MyConcurrent::checkIfDone);
  }
  else
  {
    if (m_done == m_total)
    {
      QMetaObject::invokeMethod(this,"announceProgress", Qt::QueuedConnection
                                ,Q_ARG(int,100)
                                ,Q_ARG(bool,true)
                                ,Q_ARG(int,m_total)
                                ,Q_ARG(QString,"All is done."));
    }
    else
    {
      QMetaObject::invokeMethod(this,"announceProgress", Qt::QueuedConnection
                                ,Q_ARG(int,100)
                                ,Q_ARG(bool,true)
                                ,Q_ARG(int,m_done)
                                ,Q_ARG(QString,QString::number(m_done) + " out of " + QString::number(m_total) + " is done."));
    }
    *m_stopped = true;
    emit signalUpdateUi();
  }
}
