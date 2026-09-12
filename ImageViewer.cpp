#include "ImageViewer.h"

ImageViewer::ImageViewer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ImageViewerClass())
{
    ui->setupUi(this);
    connect(ui->selecFolder, &QPushButton::clicked, this, &ImageViewer::selectFolder);
    manager = new thumbnailManager();

  
}

ImageViewer::~ImageViewer()
{
    delete ui;
}
//添加缩略图
void ImageViewer::addPics(QFileInfo fileInfo)
{
      
    MyLabel* label=manager->create_Thumbnail(this,fileInfo.absoluteFilePath());

    if (label!=nullptr) {
        connect(label, &MyLabel::clicked, this, &ImageViewer::switchImage);
        if (col >=2) {
            col = 0;
            row++;
        }
        ui->gridLayout->addWidget(label,row,col,Qt::AlignCenter);
        col++;
   }
    else {
        qDebug() << "pixmap为空";
    }
}
//槽函数：切换workSpace显示的图片
void ImageViewer::switchImage(int id)
{
    if (view) {
        //qDebug() << "移除view";
        ui->verticalLayout_4->removeWidget(view);
        delete view;
        view = nullptr;
     }
    /*qDebug() << "添加view";*/
    view = new MyGraphicsView(this);
    ui->verticalLayout_4->addWidget(view);
    manager->setCurid(id);
    view->setPixmap(manager->get_thumbnail(id)->pixmap());
}
void ImageViewer::on_btnLast_clicked()
{
    int lastId = manager->get_curId() - 1;
    if (lastId <0) {
        qDebug() << "已经是第一张";
        return;
    }
    else {
        switchImage(lastId);
    }

}
void ImageViewer::on_btnNext_clicked()
{
    int nextId = manager->get_curId()+1;
    if (nextId >= manager->get_count()) {
        qDebug() << "已经是最后一张";
        return;
    }
    else {
        switchImage(nextId);
    }

}
//从文件夹中添加图片
void ImageViewer::selectFolder() {
    QString folderPath = QFileDialog::getExistingDirectory(this,"选择文件夹","D:/");
    QDir dir(folderPath);
    if (!dir.exists()) {
        qDebug() << "文件夹不存在!";
        return;
    }
    QFileInfoList fileInfoList=dir.entryInfoList(QDir::Files|QDir::NoDotAndDotDot);
    for (QFileInfo info : fileInfoList) {
        QString suffix = info.suffix();
        if (suffix == "jpg" || suffix == "png" || suffix == "svg") {
            addPics(info);
       }
    }

}
