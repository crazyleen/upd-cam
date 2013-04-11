#ifndef RECORDER_H
#define RECORDER_H

#include <QObject>
#include <QThread>
#include <QFileInfo>
#include <QProcess>
class Recorder : public QThread
{
    Q_OBJECT
public:
    explicit Recorder(QObject *parent = 0);
    void run();

signals:
    void convertFinish(void);
public slots:
    void mainwindowClose(void);
    void record(QString picFmt);
    void avconvFinish(int num,QProcess::ExitStatus);
private:
    QProcess avconv;
    QString picDir;
    QString mkdir(QString folder);
    void deleteDirectory(QFileInfo fileList);
};

#endif // RECORDER_H
