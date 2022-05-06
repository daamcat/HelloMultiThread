#include "MyRunnable.h"

#include <QThreadPool>

MyRunnable::MyRunnable(ThreadWidget* threadWidget) :
  m_threadWidget(threadWidget)
{

}


MyRunnable::~MyRunnable()
{

}

void MyRunnable::run()
{
  m_threadWidget->loopInThreadInvokeMethod();
}
