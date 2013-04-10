/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QPainter>
#include <QImage>
#include "imageviewer.h"

ImageViewer::ImageViewer()
{
    filename_cnt = 0;
    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    setCentralWidget(scrollArea);

    createActions();
    createMenus();

    setWindowTitle(tr("webcame"));

    resize(640, 480);

    openLogoFile(":/images/logo.jpg");
}

ImageViewer::~ImageViewer()
{
    this->deleteDirectory(QFileInfo("tmp"));
    this->close();
}

void ImageViewer::deleteDirectory(QFileInfo fileList)
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

void ImageViewer::updateImage(const QString &clientStr, const QByteArray &jpegBuffer)
{
        qDebug() << clientStr << " jpeg size = " << jpegBuffer.size()/1024 << "kb";
        QImage image;
        image.loadFromData(jpegBuffer, "jpg");

        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen(Qt::red, 20, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawText(image.width() / 20, image.height() / 10, QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"));
        imageLabel->setPixmap(QPixmap::fromImage(image));
        painter.end();

        saveImage(image);

        scaleFactor = 1.0;

        capureAct->setEnabled(true);
        fitToWindowAct->setEnabled(true);
        updateActions();

        if (!fitToWindowAct->isChecked())
            imageLabel->adjustSize();

}

void ImageViewer::saveImage(QImage &image)
{
    //write to file
    QString fmt("tmp/%1-%2.jpg");
    QString filename = fmt.arg("test2").arg(this->filename_cnt++);
    image.save(filename);
}

void ImageViewer::openLogoFile(const QString &fileName)
{
    if (!fileName.isEmpty()) {
        QImage image(fileName);
        imageLabel->setPixmap(QPixmap::fromImage(image));
        scaleFactor = 1.0;

        capureAct->setEnabled(true);
        fitToWindowAct->setEnabled(true);
        updateActions();

        if (!fitToWindowAct->isChecked())
            imageLabel->adjustSize();
    }
}

//! [1]
void ImageViewer::open()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("Open File"), QDir::currentPath());

    if(!fileName.isEmpty())
        openLogoFile(fileName);
}

QString ImageViewer::getCaptureDir() {
    QString folder("capture/");
    QDir *captureDir = new QDir;
    bool exist = captureDir->exists(folder);
    if(!exist)
    {
        exist = captureDir->mkdir(folder);
        if(!exist)
            folder.clear();
    }

    return folder;
}

void ImageViewer::capture()
{
    Q_ASSERT(imageLabel->pixmap());

    QString fileName = getCaptureDir().append(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_zzz").append(".jpg"));
    fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save File"), fileName, tr("Images (*.png *.xpm *.jpg)"));
    if(!fileName.isEmpty())
        imageLabel->pixmap()->save(fileName);
}

/**
 * capture quickly, save file to capture/dd_MM_yyyy_hh_mm_ss.jpg
 */
void ImageViewer::capureQuick()
{
    QString fileName = getCaptureDir().append(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_zzz").append(".jpg"));
    imageLabel->pixmap()->save(fileName);
}

void ImageViewer::zoomIn()
{
    scaleImage(1.25);
}

void ImageViewer::zoomOut()
{
    scaleImage(0.8);
}

void ImageViewer::normalSize()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}


void ImageViewer::fitToWindow()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        normalSize();
    }
    updateActions();
}



void ImageViewer::about()
{
    QMessageBox::about(this, tr("About webcam"),
            tr("<p> <b>webcam</b> designed by QunChao Ma</p>"));
}


void ImageViewer::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    capureAct = new QAction(tr("&capture..."), this);
    capureAct->setShortcut(tr("Ctrl+P"));
    capureAct->setEnabled(false);
    connect(capureAct, SIGNAL(triggered()), this, SLOT(capture()));

    capureActQuick = new QAction(tr("&capture quick"), this);
    capureActQuick->setShortcut(tr("Ctrl+Shift+P"));
    capureActQuick->setEnabled(true);
    connect(capureActQuick, SIGNAL(triggered()), this, SLOT(capureQuick()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    zoomInAct = new QAction(tr("Zoom &In (25%)"), this);
    zoomInAct->setShortcut(tr("Ctrl++"));
    zoomInAct->setEnabled(false);
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAct = new QAction(tr("Zoom &Out (25%)"), this);
    zoomOutAct->setShortcut(tr("Ctrl+-"));
    zoomOutAct->setEnabled(false);
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

    normalSizeAct = new QAction(tr("&Normal Size"), this);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    normalSizeAct->setEnabled(false);
    connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

    fitToWindowAct = new QAction(tr("&Fit to Window"), this);
    fitToWindowAct->setEnabled(true);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));
    connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}


void ImageViewer::createMenus()
{
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addAction(capureAct);
    fileMenu->addAction(capureActQuick);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    viewMenu = new QMenu(tr("&View"), this);
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    viewMenu->addAction(normalSizeAct);
    viewMenu->addSeparator();
    viewMenu->addAction(fitToWindowAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(helpMenu);
}


void ImageViewer::updateActions()
{
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}



void ImageViewer::scaleImage(double factor)
{
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}



void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

