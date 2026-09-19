#include "ImageViewer.h"

ImageViewer::ImageViewer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ImageViewerClass())
{

    ui->setupUi(this);
    menu = new QMenu(this);
    menu->addAction("复制",this,&ImageViewer::clipToborad);
    borad = QApplication::clipboard();
    ui->contrastSlider->setRange(0, 100);
    ui->spinBox_rotation->setRange(-360, 360);
    ui->btnInvert->setDisabled(true);
    ui->btnGrayScale->setDisabled(true);
    ui->btnMirror->setDisabled(true);

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
//
void ImageViewer::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton) {
        menu->exec(event->globalPos());
    }
    QWidget::mousePressEvent(event);
}
//槽函数：切换workSpace显示的图片
void ImageViewer::switchImage(int id)
{
    //若果有任何一个参数变化，就不用在这里刷新一次了，直接用switchImage(int id,QPixmap pix)
    if (view) {
        ui->verticalLayout_4->removeWidget(view);
        delete view;
        view = nullptr;
     }
    imageManager::imgParams params = imageManager::instance()->get_params(id);
    //神秘bug，如果在这个地方判断参数，在给图像添加任意以下效果时会导致图像删除失败且不断叠加view
    view = new MyGraphicsView(this);
    ui->verticalLayout_4->addWidget(view);
    imageManager::instance()->set_CurId(id);
    view->setPixmap(imageManager::instance()->get_ImageById(id));
    ui->btnInvert->setDisabled(false);
    ui->btnGrayScale->setDisabled(false);
    ui->btnMirror->setDisabled(false);
    if (params.colInverted) {
        ui->btnInvert->setChecked(true);
    }
    else {
        ui->btnInvert->setChecked(false);
    }
    if (params.grayScale) {
        ui->btnGrayScale->setChecked(true);
    }
    else {
        ui->btnGrayScale->setChecked(false);
    }
    if (params.mirrored) {
        ui->btnMirror->setChecked(true);
    }
    else {
        ui->btnMirror->setChecked(false);
    }
   

}
void ImageViewer::switchImage(int id,QPixmap pix)
{
    if (pix.isNull()) {
        qDebug() << "ImageViewer::switchImage pix Null";
        return;
    }
    if (view) {
        ui->verticalLayout_4->removeWidget(view);
        delete view;
        view = nullptr;
        qDebug() << "delete view 2";
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
    ui->btnInvert->setDisabled(true);
    ui->btnGrayScale->setDisabled(true);
    ui->btnMirror->setDisabled(true);
   int curId = imageManager::instance()->get_CurId();
   int index= imageManager::instance()->indexOf(curId);
   QLayoutItem*item=ui->gridLayout->takeAt(index);
   //移除控件，对应的数据以及清空view
   if (item != nullptr) {
       if (QWidget* widget = item->widget()) {
           widget->deleteLater();
           int nextId=imageManager::instance()->remove_imageById(curId);
           if (nextId != -1) {
               switchImage(nextId);
           }
           else {
               qDebug() << "已经是最后一张";
               if (view) {
                   ui->verticalLayout_4->removeWidget(view);
                   delete view;
                   view = nullptr;
               }
           }
 /*          delete view;
           view = new MyGraphicsView(this);
           ui->verticalLayout_4->addWidget(view);*/
       }
   }

}
//可能的优化：选中图片前禁止点击这几个按钮
//信号槽：图片颜色反转
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
        return;
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
//信号槽：图片灰度化
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
        return;
    }
}
//信号槽：图片镜像反转
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
        return;
    }
}
//信号槽：操作撤回
void ImageViewer::on_btnWithdraw_clicked()
{
}
//信号槽：还原
void ImageViewer::on_btnRestore_clicked()
{
    ui->btnInvert->setChecked(false);
    ui->btnGrayScale->setChecked(false);
    ui->btnMirror->setChecked(false);
    ui->contrastSlider->setValue(0);
    ui->spinBox_rotation->setValue(0);
}
//信号槽：另存为
void ImageViewer::on_btnSaveAs_clicked()
{
  QString savePath=  QFileDialog::getSaveFileName(this, "保存文件", "D:/", "图片文件 (*.png *.jpg);;所有文件 (*)");
  int curId = imageManager::instance()->get_CurId();
  QPixmap pix=imageManager::instance()->render(curId);
  if (!savePath.isEmpty()) {
      if (!pix.save(savePath)) {
          qDebug() << "异常问题：图片无法保存";
          return;
      }
  }
  else {
      qDebug() << "取消保存";
      return;
  }
        
    
}

void ImageViewer::clipToborad()
{
    int curId = imageManager::instance()->get_CurId();
        QPixmap pix = imageManager::instance()->render(curId);
        if (!pix.isNull()) {
            borad->setPixmap(pix);
        }
        else {
            qDebug() << "图片不存在huoweixuanzhongrnhetupian,无法复制";
        }
    

}
