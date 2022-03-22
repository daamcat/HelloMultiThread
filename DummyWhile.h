#ifndef DUMMYWHILE_H
#define DUMMYWHILE_H


#include <QWidget>
#include <QEvent>
struct ProgressEvent : public QEvent
{
  enum {EventId = QEvent::User};
  explicit ProgressEvent(bool saved_ , const QString& message_) : QEvent(static_cast<QEvent::Type>(EventId)) ,
    m_saved(saved_),
    m_message(message_)
  {};

  const bool m_saved;
  const QString m_message;

};

class DummyWhile
{
public:
  DummyWhile(QWidget* mainWindow , volatile bool* stopped, int seconds, int iterations);

  void start();


private:
  void doWhileInThread();

  QWidget* m_receiver;
  volatile bool* m_stopped;
  int m_seconds;
  int m_iterations;

};

#endif // DUMMYWHILE_H
