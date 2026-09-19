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
        //触发clicked信号时，顺便带上一个已经计算好的id，就像给MyLabel贴上了身份便利贴
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
        ui->verticalLayout_4->removeWidget(view);
        delete view;
        view = nullptr;
     }
    view = new MyGraphicsView(this);
    ui->verticalLayout_4->addWidget(view);
    imageManager::instance()->set_CurId(id);
    view->setPixmap(imageManager::instance()->get_ImageById(id));
}
void ImageViewer::switchImage(int id,QPixmap pix)
{
    if (!pix.isNull()) {
        qDebug() << "Not Null";
    }
    else {
        qDebug() << "Null";
        return;
    }
    if (view) {
        ui->verticalLayout_4->removeWidget(view);
        delete view;
        view = nullptr;
    }
    view = new MyGraphicsView(this);
    ui->verticalLayout_4->addWidget(view);
    imageManager::instance()->set_CurId(id);
    view->setPixmap(pix);

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
//槽函数：切换上一张
void ImageViewer::on_btnLast_clicked()
{
    int lastId = imageManager::instance()->get_lastId();
    switchImage(lastId);

}
//槽函数：切换下一张
void ImageViewer::on_btnNext_clicked()
{//当前计算方式，若遇到已被删除的id，函数不会执行查找逻辑，但实际上并不希望这样，效果应该是直接跳过
    //，所以应该改为在imageManager直接使用index判定，imageManager内部直接返回
    //计算公式，index=indexOf(curId); index++/index--;if()....;return image.at(index).id;
    //更改后
    int nextId = imageManager::instance()->get_nextId();
    switchImage(nextId);


}
//从布局中移除控件
void ImageViewer::on_btnRemove_clicked()
{   //takeAt和removeAt在移除数据后都会使后面的数据往前排，因此都要计算位置
   int curId = imageManager::instance()->get_CurId();
   int index= imageManager::instance()->indexOf(curId);
   QLayoutItem*item=ui->gridLayout->takeAt(index);
   //移除控件，对应的数据以及清空view
   if (item != nullptr) {
       if (QWidget* widget = item->widget()) {
           widget->deleteLater();
           imageManager::instance()->remove_imageById(curId);
           delete view;
           view = new MyGraphicsView(this);
           ui->verticalLayout_4->addWidget(view);
       }
   }

}
//可能的优化：选中图片前禁止点击这几个按钮
void ImageViewer::on_btnInvert_toggled(bool checked)
{

    int curId = imageManager::instance()->get_CurId();
    if (checked) {
        imageManager::instance()->set_ColInverted(curId, true);
    }
    else {
        imageManager::instance()->set_ColInverted(curId, false);
    }
    QPixmap pix = imageManager::instance()->render(curId);

    if (!pix.isNull()) {
        qDebug() << "invert success";
        switchImage(curId, pix);
    }
    else {
        qDebug() << "invert failed";
    }

   // int curId = imageManager::instance()->get_CurId();
   //QImage img = imageManager::instance()->get_ImageById(curId).toImage();
   //if (img.format() != QImage::Format_ARGB32) {
   //    img = img.convertToFormat(QImage::Format_ARGB32);
   //}
   //img.invertPixels();
   //QPixmap pix = QPixmap::fromImage(img);
   //if (view) {
   //    ui->verticalLayout_4->removeWidget(view);
   //    delete view;
   //    view = nullptr;
   //}
   //view = new MyGraphicsView(this);
   //ui->verticalLayout_4->addWidget(view);
   //view->setPixmap(pix);
}

void ImageViewer::on_btnGrayScale_toggled(bool checked)
{
    int curId = imageManager::instance()->get_CurId();
    if (checked) {
        imageManager::instance()->set_GrayScaled(curId, true);
    }
    else {
        imageManager::instance()->set_GrayScaled(curId, false);
    }
    QPixmap pix = imageManager::instance()->render(curId);

    if (!pix.isNull()) {
        qDebug() << "grayScaled success";
        switchImage(curId, pix);
    }
    else {
        qDebug() << "grayScaled failed";
    }
}

void ImageViewer::on_btnMirror_toggled(bool checked)
{
    int curId = imageManager::instance()->get_CurId();
    if (checked) {
        imageManager::instance()->set_Mirrored(curId, true);
    }
    else {
        imageManager::instance()->set_Mirrored(curId, false);
    }
    QPixmap pix = imageManager::instance()->render(curId);

    if (!pix.isNull()) {
        qDebug() << "mirror success";
        switchImage(curId, pix);
    }
    else {
        qDebug() << "mirror failed";
    }
}

void ImageViewer::on_btnWithdraw_clicked()
{
}
