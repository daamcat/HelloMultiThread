#include "MainWindow.h"
#include "ui_MainWindow.h"


#include <QPushButton>
#include <QThreadPool>
#include <QtCore>
#include <QFuture>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  m_dummy = new DummyWhile(dynamic_cast<QWidget*>(this), &m_stopped,10,100);

  connect(ui->pushButtonStartCancel , &QPushButton::released , this , &MainWindow::slotPushButtonStartCancelReleased);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::slotPushButtonStartCancelReleased()
{
  m_stopped = true;
  if (QThreadPool::globalInstance()->activeThreadCount())
    // If there are some threads running,...
  {
    QThreadPool::globalInstance()->waitForDone();
  }
  if (ui->pushButtonStartCancel->text() == tr("Cancel"))
  {
    ui->pushButtonStartCancel->setText(tr("Start"));
    return;
  }

  ui->pushButtonStartCancel->setText(tr("Cancel"));

  m_stopped = false;
  m_dummy->start();

}

bool MainWindow::event(QEvent* event)
{
  if (event->type() == static_cast<QEvent::Type>(ProgressEvent::EventId))
  {
    ProgressEvent* progressEvent = static_cast<ProgressEvent*>(event);
    Q_ASSERT(progressEvent);
    ui->lineEdit->setText(progressEvent->m_message);
    return true;
  }
  return QMainWindow::event(event);
}
