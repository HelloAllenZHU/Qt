#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMediaRecorder>

class QAudioRecorder;
class QAudioProbe;
class QAudioBuffer;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void clickStart();

    void clickStop();

    void clickSave();

    void processBuffer( const QAudioBuffer& buffer );

private:
    Ui::Widget *ui;

    QAudioRecorder  *m_audioRecorder = nullptr;
    QAudioProbe     *m_probe = nullptr;
};
#endif // WIDGET_H
