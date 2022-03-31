#ifndef THREADWIDGET_H
#define THREADWIDGET_H

#include <QWidget>
#include <QEvent>

struct ProgressEvent : public QEvent
{
  enum {EventId = QEvent::User};
  explicit ProgressEvent(int32_t threadNumber_, bool successful_ , const QString& message_) : QEvent(static_cast<QEvent::Type>(EventId)) ,
    m_threadNumber(threadNumber_),
    m_successful(successful_),
    m_message(message_)
  {};

  const int32_t m_threadNumber;
  const bool m_successful;
  const QString m_message;

};

namespace Ui {
class ThreadWidget;
}

class ThreadWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ThreadWidget(int threadNumber, volatile bool* stopped, int seconds, int iterations, QObject* dummyWhile, QWidget* parent = nullptr);
  ~ThreadWidget();

  void doWhileInThread();
  void setLineEditText(QString text);

signals:
  void signalUpdateUi();

private:
  Ui::ThreadWidget *ui;


  QWidget* m_receiver;
  volatile bool* m_stopped;
  int m_seconds;
  int m_iterations;
  const int m_pollTimeOut = 100;
  const int32_t m_threadNumber;
  QObject* m_dummyWhile;

};

#endif // THREADWIDGET_H
