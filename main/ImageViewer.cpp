#include "ImageViewer.h"

ImageViewer::ImageViewer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ImageViewerClass())
{
    ui->setupUi(this);
    connect(ui->selecFolder, &QPushButton::clicked, this, &ImageViewer::selectFolder);
    thumbnails = new thumbnailManager();

  
}

ImageViewer::~ImageViewer()
{
    delete ui;
}
//添加缩略图
void ImageViewer::addPics(QFileInfo fileInfo)
{
  MyLabel* label= thumbnails->create_Thumbnail(this,fileInfo.absoluteFilePath());
  int id = imageManager::instance()->add_Image(fileInfo.absoluteFilePath());

    if (label!=nullptr) {
        connect(label, &MyLabel::clicked, this, [this, id] {switchImage(id); });
        if (col >=2) {
            col = 0;
            row++;
        }
        ui->gridLayout->addWidget(label,row,col,Qt::AlignCenter);
        col++;
   }
    else {
        qDebug() << "label为空";
    }
}
//槽函数：切换workSpace显示的图片
void ImageViewer::switchImage(int id)
{
    qDebug() << id;
    if (view) {
        //qDebug() << "移除view";
        ui->verticalLayout_4->removeWidget(view);
        delete view;
        view = nullptr;
     }
    /*qDebug() << "添加view";*/
    view = new MyGraphicsView(this);
    ui->verticalLayout_4->addWidget(view);
    imageManager::instance()->set_CurId(id);
    view->setPixmap(imageManager::instance()->get_ImageById(id));
}
void ImageViewer::on_btnLast_clicked()
{
    int lastId = imageManager::instance()->get_CurId() - 1;
    if (lastId <0) {
        //qDebug() << "已经是第一张";
        return;
    }
    else {
        switchImage(lastId);
    }

}
void ImageViewer::on_btnNext_clicked()
{
    int nextId = imageManager::instance()->get_CurId() +1;
    if (nextId >= imageManager::instance()->get_Count()) {
        //qDebug() << "已经是最后一张";
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
    while (QLayoutItem* item = ui->gridLayout->takeAt(0)) {
        delete item;
        imageManager::instance()->clear();
        //thumbnails->clear();
    }
    row = 0;
    col = 0;
   
    for (QFileInfo info : fileInfoList) {
        QString suffix = info.suffix();
        if (suffix == "jpg" || suffix == "png" || suffix == "svg") {
            addPics(info);
        
       }
    }

}
