#ifndef MYRUNNABLE_H
#define MYRUNNABLE_H

#include <QRunnable>


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
  MyRunnable();
};

#endif // MYRUNNABLE_H
