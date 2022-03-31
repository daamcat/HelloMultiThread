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

  m_dummy = new DummyWhile(dynamic_cast<QWidget*>(this),ui->groupBox, &m_stopped,10,m_numberOfIterations);

  connect(ui->pushButtonStartCancel , &QPushButton::released , this , &MainWindow::slotPushButtonStartCancelReleased);
  connect(m_dummy , &DummyWhile::signalUpdateUi , this , &MainWindow::updateUi);
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
  m_dummy->setTotal(m_total);
  m_dummy->startUsingQtConcurrent();
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
