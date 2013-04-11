#include "recorder.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QProcess>

Recorder::Recorder(QObject *parent) :
    avconv(),
    QThread(parent)
{
    qDebug() << "recorder constructor";
    mkdir(QString("video"));
}

QString Recorder::mkdir(QString folder) {
    QDir *dir = new QDir;
    bool exist = dir->exists(folder);
    if(!exist)
    {
        exist = dir->mkpath(folder);
        if(!exist)
            folder.clear();
    }

    return folder;
}

void Recorder::run()
{
    exec();
}

void Recorder::deleteDirectory(QFileInfo fileList)
{

    if(fileList.isDir()){
        int childCount =0;
        QString dir = fileList.filePath();
        QDir thisDir(dir);
        childCount = thisDir.entryInfoList().count();
        QFileInfoList newFileList = thisDir.entryInfoList();
        if(childCount>2){
            for(int i=0;i<childCount;i++){
                if(newFileList.at(i).fileName().operator ==(".")|newFileList.at(i).fileName().operator ==("..")){
                    continue;
                }
                deleteDirectory(newFileList.at(i));
            }
        }
        fileList.absoluteDir().rmpath(fileList.fileName());
    }else if(fileList.isFile()){
        fileList.absoluteDir().remove(fileList.fileName());
    }

}

void Recorder::mainwindowClose(void) {
    qDebug() << "recorder recv mainwindowClose";
    avconv.waitForFinished();
    exit();
}

void Recorder::avconvFinish(int num,QProcess::ExitStatus status) {
    qDebug() << "avconv exit with " << num << status;
    qDebug() << "record delete folder " << picDir;
    deleteDirectory(QFileInfo(picDir));
    emit convertFinish();
}

void Recorder::record(QString picFmt)
{
    //TODO: convert jpg to video
    QStringList strList = picFmt.split("/", QString::SkipEmptyParts);
    if(strList.size() < 2)
    {
        qDebug() << "#record empty picFmt";
        return;
    }
    qDebug() << strList << "size" << strList.size();
    QString fileFmt = strList.at(strList.size() - 1);
    picDir = picFmt.mid(0, picFmt.size() - fileFmt.size() - 1);
    QString videoFile = "video/" + strList.at(strList.size() - 2) + ".flv";

    QString program = "avconv";
    QStringList arguments;
    arguments << "-f" << "image2" << "-i" << picFmt << "-r" << "5" << videoFile;
    avconv.start(program, arguments);
    connect(&avconv, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(avconvFinish(int,QProcess::ExitStatus)));
}
