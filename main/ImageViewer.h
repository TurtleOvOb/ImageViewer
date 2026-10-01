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
#include"../class/addPicsThread.h"
#include"../class/poolNotifier.h"
#include<qmenu.h>
#include<qclipboard.h>
#include<qmessagebox.h>
#include<qthreadpool.h>
#include<qhash.h>
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
    void wkWidgetDisabled(bool con);
    void fileWidgetDisabled(bool con);
    void mousePressEvent(QMouseEvent* event)override;
    MyGraphicsView* view = nullptr;
    thumbnailManager* thumbnails=nullptr;
    int row=0;
    int col=0;
    Ui::ImageViewerClass *ui;
    QMenu* menu = nullptr;
    QClipboard* borad ;
    QThreadPool pool;
    QHash<int, MyLabel*> labelById;//id -> 缩略图控件，删除和更新都按 id 查，不再依赖布局下标

private slots:
    void selectFolder();
    void switchImage(int id);
    void switchImage(int id,QPixmap pix);
    void on_btnLast_clicked();
    void on_btnNext_clicked();
    void on_btnRemove_clicked();
    void on_btnInvert_toggled(bool checked);
    void on_btnGrayScale_toggled(bool checked);
    void on_btnMirror_toggled(bool checked);
    void on_btnRestore_clicked();
    void on_btnSaveAs_clicked();
    void on_contrastSlider_valueChanged(int val);
    void on_lightnessSlider_valueChanged(int val);
    void on_spinBox_rotation_valueChanged(int val);
    void clipToborad();
};

