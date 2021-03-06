#ifndef MYRUNNABLE_H
#define MYRUNNABLE_H

#include "ThreadWidget.h"

#include <QRunnable>
#include <QWidget>
#include <QGroupBox>
#include <QMap>


class MyRunnable : public QRunnable
/*
Ref: https://stackoverflow.com/questions/5447498/what-are-access-specifiers-should-i-inherit-with-private-protected-or-public
in inheritence syntax, here "public" means:
  * All public members of base class become public members of derived class.
  * All protected members of base class become protected members of derived class.
In other words, no change in access level of members in derived class.

"private" would mean all members become private!
"protected" would mean all members become protected!
*/

{
public:
  explicit MyRunnable(ThreadWidget* threadWidget);
  ~MyRunnable();


private:
  void run() override;

  ThreadWidget* m_threadWidget = nullptr;


};

#endif // MYRUNNABLE_H
