#include <QCoreApplication>
#include <QStringList>
#include <QProcess>
#include <QObject>
#include <QDebug>

class XXX : public QObject
{
    Q_OBJECT
public:
    explicit XXX(QObject *parent = nullptr ) : QObject(parent) {

    }

signals:
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QStringList param;
    param << "-token" << "RsTatqvrf9MebuC3I6eZ3jgsHm3mJBDIMXjDCMN6:pKszt8giAT0rqblsLm8Rb-dgn30=:eyJzY29wZSI6InBlaXlpbiIsImRlYWRsaW5lIjoxNjU4MjE4ODIxfQ=="
          << "-local" << "c:/x.mp4"
          << "-key" << "x.mp4";


    auto p = new QProcess;
    p->start( "./upload.exe", param );

    /*XXX x;
    x.connect( p, &QProcess::readyReadStandardOutput, [=](){
        qDebug() << p->readLine();
    });*/
    p->waitForFinished();

    qDebug() << "result: "<< p->readAll();
    return a.exec();
}
