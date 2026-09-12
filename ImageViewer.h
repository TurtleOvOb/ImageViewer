#pragma once

#include <QtWidgets/QWidget>
#include "ui_ImageViewer.h"
#include<qfiledialog.h>
#include<qdebug.h>
#include<qdir.h>
#include<qlabel.h>
#include<qimage.h>
#include"MyGraphicsView.h"
#include"thumbnailManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ImageViewerClass; };
QT_END_NAMESPACE

class ImageViewer : public QWidget
{
    Q_OBJECT

public:
    ImageViewer(QWidget *parent = nullptr);
    ~ImageViewer();
    void addPics(QFileInfo fileInfo);

private:
    MyGraphicsView* view = nullptr;
    thumbnailManager* manager=nullptr;
    int row=0;
    int col=0;
    Ui::ImageViewerClass *ui;
private slots:
    void selectFolder();
    void switchImage(int &id);
};

