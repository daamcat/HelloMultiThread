#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ThreadWidget.h"



#include <QPushButton>
#include <QThreadPool>
#include <QtCore>
#include <QFuture>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  m_concurrent = new MyConcurrent(dynamic_cast<QWidget*>(this),ui->groupBox, &m_stopped,10,m_numberOfIterations);

  connect(ui->pushButtonStartCancel , &QPushButton::released , this , &MainWindow::slotPushButtonStartCancelReleased);
  connect(m_concurrent , &MyConcurrent::signalUpdateUi , this , &MainWindow::updateUi);
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
    return;
  }

  updateUi();
  m_total = m_numberOfIterations;
  m_stopped = false;
  m_concurrent->setTotal(m_total);
  m_concurrent->startUsingQtConcurrent();
  //m_concurrent->startUsingQRunnable();
}

void MainWindow::updateUi()
{
  if (ui->pushButtonStartCancel->text() == tr("Cancel"))
  {
    ui->pushButtonStartCancel->setText(tr("Start"));
  }
  else
  {
    ui->pushButtonStartCancel->setText(tr("Cancel"));
  }
}
