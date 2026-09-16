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
#include"imageManager.h"
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
    thumbnailManager* thumbnails=nullptr;
    int row=0;
    int col=0;
    Ui::ImageViewerClass *ui;
private slots:
    void selectFolder();
    void switchImage(int id);
    void switchImage(int id,QPixmap pix);
    void on_btnLast_clicked();
    void on_btnNext_clicked();
    void on_btnRemove_clicked();
    void on_btnInvert_clicked();
    void on_btnGrayScale_clicked();
    void on_btnMirror_clicked();
    void on_btnWithdraw_clicked();

};

