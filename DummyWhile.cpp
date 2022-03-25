#include "DummyWhile.h"

#include <QThread>
#include <QApplication>
#include <QtConcurrent>
#include <QTimer>
#include <QVBoxLayout>

DummyWhile::DummyWhile(QWidget* mainWindow, QGroupBox* groupBox, volatile bool* stopped, int seconds, int iterations) :
  m_receiver(mainWindow),
  m_stopped(stopped),
  m_seconds(seconds) ,
  m_iterations(iterations),
  m_groupBox(groupBox)
{

}

DummyWhile::~DummyWhile()
{
  *m_stopped = true;
  if (QThreadPool::globalInstance()->activeThreadCount())
  {
    QThreadPool::globalInstance()->waitForDone();
  }
}

void DummyWhile::setTotal(int32_t total)
{
  m_total = total;
}

void DummyWhile::startUsingQtConcurrent()
{
  qDeleteAll(m_groupBox->findChildren<ThreadWidget*>(QString(),Qt::FindDirectChildrenOnly));
  delete(m_groupBox->layout());

  m_done = 0;
  QVBoxLayout* vBoxLayout = new QVBoxLayout();
  // Ref: https://doc.qt.io/qt-5/qtconcurrentrun.html#using-member-functions
  for (int iThread = 1; iThread < 3; iThread++)
  {
    ThreadWidget* widget = new ThreadWidget(iThread, m_stopped,m_seconds,m_iterations,m_receiver);
    vBoxLayout->addWidget(widget);
    m_threadWidgets.push_back(widget);

    QtConcurrent::run(widget, &ThreadWidget::doWhileInThread);

  }
  m_groupBox->setLayout(vBoxLayout);
  this->checkIfDone();

}


void DummyWhile::checkIfDone()
{
  if (QThreadPool::globalInstance()->activeThreadCount())
  {
    QTimer::singleShot(m_pollTimeOut, this, &DummyWhile::checkIfDone);
  }
  else
  {
    if (m_done == m_total)
    {
      QApplication::postEvent(m_receiver,new ProgressEvent(true,"All is done."));
    }
    else
    {
      QApplication::postEvent(m_receiver,new ProgressEvent(true,QString::number(m_done) + " out of " + QString::number(m_total) + " is done."));
    }
    *m_stopped = true;
    emit signalUpdateUi();
  }
}
