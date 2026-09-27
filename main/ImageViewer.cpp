#include "ImageViewer.h"

ImageViewer::ImageViewer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ImageViewerClass())
{

    ui->setupUi(this);
    menu = new QMenu(this);
    menu->addAction("复制",this,&ImageViewer::clipToborad);
    borad = QApplication::clipboard();
    ui->gridLayout->setAlignment(Qt::AlignTop);
    ui->contrastSlider->setTracking(true);
    ui->contrastSlider->setRange(-100, 100);
    ui->lightnessSlider->setTracking(true);
    ui->lightnessSlider->setRange(-100, 100);
    ui->spinBox_rotation->setRange(-360, 360);
    wkWidgetDisabled(true);
    fileWidgetDisabled(true);
    view = new MyGraphicsView(this);
    ui->verticalLayout_4->addWidget(view);
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
        QMessageBox::warning(this, "错误", "图片不存在!", QMessageBox::Ok);
        return;
    }
}
//控件启用开关
void ImageViewer::wkWidgetDisabled(bool con)
{
    ui->btnInvert->setDisabled(con);
    ui->btnGrayScale->setDisabled(con);
    ui->btnMirror->setDisabled(con);
    ui->contrastSlider->setDisabled(con);
    ui->lightnessSlider->setDisabled(con);
    ui->spinBox_rotation->setDisabled(con);
}
void ImageViewer::fileWidgetDisabled(bool con)
{
    ui->btnNext->setDisabled(con);
    ui->btnLast->setDisabled(con);
    ui->btnRemove->setDisabled(con);
    ui->btnRestore->setDisabled(con);
    ui->btnSaveAs->setDisabled(con);
}
//鼠标点击事件
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
    qDebug() << "准备切换图片，图片id:" << id;
    //若果有任何一个参数变化，就不用在这里刷新一次了，直接用switchImage(int id,QPixmap pix)
    imageManager::imgParams params = imageManager::instance()->get_params(id);
    //神秘bug，如果在这个地方判断参数，在给图像添加任意以下效果时会导致图像删除失败且不断叠加view
    imageManager::instance()->set_CurId(id);
    view->setPixmap(imageManager::instance()->get_ImageById(id),true);
    wkWidgetDisabled(false);
    //切换图片时同步参数
    if (params.colInverted) {
        qDebug() << "colInverted同步";
        if (ui->btnInvert->isChecked()) {
            emit ui->btnInvert->toggled(true);
        }
        else {
            ui->btnInvert->setChecked(true);
        }
   
        
       
    }
    else {
        //emit ui->btnInvert->toggled(false);
        ui->btnInvert->setChecked(false);
    }
    if (params.grayScale) {
        if (ui->btnGrayScale->isChecked()) {
            emit ui->btnGrayScale->toggled(true);
        }
        else {
            ui->btnGrayScale->setChecked(true);
        }
    }
    else {
        //emit ui->btnGrayScale->toggled(false);
        ui->btnGrayScale->setChecked(false);
    }
    if (params.mirrored) {
        if (ui->btnMirror->isChecked()) {
            emit ui->btnMirror->toggled(true);
        }
        else {
            ui->btnMirror->setChecked(true);
        }
    }
    else {
        //emit ui->btnMirror->toggled(false);
        ui->btnMirror->setChecked(false);
    }
    //不用判0，valueChanged信号只有在值变化的时候才会发出，判0会导致不触发setValue，不触发setValue会导致值不变，信号就发不出
    if (params.contrast == ui->contrastSlider->value()) {
         emit  ui->contrastSlider->valueChanged(params.contrast);
     }
    else{
        ui->contrastSlider->setValue(params.contrast);
    }
    if (params.lightness == ui->lightnessSlider->value()) {

        emit  ui->lightnessSlider->valueChanged(params.lightness);
    }
    else {
        ui->lightnessSlider->setValue(params.lightness);
    }  if (params.rotation == ui->spinBox_rotation->value()) {
        emit  ui->spinBox_rotation->valueChanged(params.rotation);
    }
    else {
        ui->spinBox_rotation->setValue(params.rotation);
    }
     
    

}
void ImageViewer::switchImage(int id,QPixmap pix)
{
    if (pix.isNull()) {
        qDebug() << "ImageViewer::switchImage pix Null";
        return;
    }
    imageManager::instance()->set_CurId(id);
    view->setPixmap(pix,false);
    
}
//从文件夹中添加图片
void ImageViewer::selectFolder() {
    QString folderPath = QFileDialog::getExistingDirectory(this,"选择文件夹","D:/");
    if (folderPath.isEmpty())return;
    QDir dir(folderPath);
    if (!dir.exists()) {
        QMessageBox::warning(this, "错误", "文件夹不存在!", QMessageBox::Ok);
        return;
    }
    QFileInfoList fileInfoList=dir.entryInfoList(QDir::Files|QDir::NoDotAndDotDot);
    if (fileInfoList.isEmpty()) {
        QMessageBox::warning(this, "警告", "文件夹内不存在图片!", QMessageBox::Ok);
        return;
    }
    while (QLayoutItem* item = ui->gridLayout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
         }
  
    }
    imageManager::instance()->clear();
    row = 0;
    col = 0;
   
    for (QFileInfo info : fileInfoList) {
        QString suffix = info.suffix();
        if (suffix == "jpg" || suffix == "png" || suffix == "svg" || suffix == "JPG"
            || suffix == "jpeg" || suffix == "bmp" || suffix == "webp" || suffix == "PNG") {
            addPics(info);
       }
        else {
            continue;
        }
    }
    fileWidgetDisabled(false);
    int index = imageManager::instance()->indexOf(0);
    if (index > 0) {
        switchImage(index);
    }
    else {
        return;
    }

}
//槽函数：切换上一张
void ImageViewer::on_btnLast_clicked()
{
    int lastId = imageManager::instance()->get_lastId();
    if (lastId != -1) {
     switchImage(lastId);
    }
    else {
        qDebug() << "已经是第一张";
        return;
    }

}
//槽函数：切换下一张
void ImageViewer::on_btnNext_clicked()
{//当前计算方式，若遇到已被删除的id，函数不会执行查找逻辑，但实际上并不希望这样，效果应该是直接跳过
    //，所以应该改为在imageManager直接使用index判定，imageManager内部直接返回
    //计算公式，index=indexOf(curId); index++/index--;if()....;return image.at(index).id;
    //更改后
    int nextId = imageManager::instance()->get_nextId();
    if (nextId != -1) {
        switchImage(nextId);
    }
    else {
        qDebug() << "已经是最后一张";
        return;
    }


}
//从布局中移除控件
void ImageViewer::on_btnRemove_clicked()
{   //takeAt和removeAt在移除数据后都会使后面的数据往前排，因此都要计算位置
    wkWidgetDisabled(true);
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
                   view->setPixmap(QPixmap());
               }
           }
       }
   }

}
//信号槽：图片颜色反转
void ImageViewer::on_btnInvert_toggled(bool checked)
{
    int curId = imageManager::instance()->get_CurId();
    if (checked) {
        qDebug() << "colInverted同步触发";
        imageManager::instance()->set_ColInverted(curId, true);
    }
    else {
        imageManager::instance()->set_ColInverted(curId, false);
    }
    QPixmap pix = imageManager::instance()->render(curId);
    if (!pix.isNull()) {
        switchImage(curId, pix);
    }
    else {
        return;
    }
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
        switchImage(curId, pix);
    }
    else {
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
        switchImage(curId, pix);
    }
    else {
        return;
    }
}
//信号槽：还原
void ImageViewer::on_btnRestore_clicked()
{
    ui->btnInvert->setChecked(false);
    ui->btnGrayScale->setChecked(false);
    ui->btnMirror->setChecked(false);
    ui->contrastSlider->setValue(0);
    ui->lightnessSlider->setValue(0);
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
          qDebug() << "图片无法保存";
          return;
      }
  }
  else {
      return;
  }
        
    
}
//信号槽：对比度变化
void ImageViewer::on_contrastSlider_valueChanged(int val)
{
  int curId=imageManager::instance()->get_CurId();
  imageManager::instance()->set_Contrast(curId, val);
  QPixmap pix = imageManager::instance()->render(curId);
  if (!pix.isNull()) {
      switchImage(curId, pix);
  }
  else {
      return;
  }
}
//信号槽：亮度变化
void ImageViewer::on_lightnessSlider_valueChanged(int val)
{
    int curId = imageManager::instance()->get_CurId();
    imageManager::instance()->set_lightness(curId, val);
    QPixmap pix = imageManager::instance()->render(curId);
    if (!pix.isNull()) {
        switchImage(curId, pix);
    }
    else {
        return;
    }
}
//信号槽：旋转角度变化
void ImageViewer::on_spinBox_rotation_valueChanged(int val)
{
  
    int curId = imageManager::instance()->get_CurId();
    imageManager::instance()->set_rotation(curId, val);
    QPixmap pix = imageManager::instance()->render(curId);
    if (!pix.isNull()) {
        switchImage(curId, pix);
    }
    else {
        return;
    }
}
//剪切到粘贴板
void ImageViewer::clipToborad()
{
    int curId = imageManager::instance()->get_CurId();
        QPixmap pix = imageManager::instance()->render(curId);
        if (!pix.isNull()) {
            borad->setPixmap(pix);
        }
        else {
            QMessageBox::warning(this, "错误", "未选中图片或图片不存在!", QMessageBox::Ok);
        }
    

}
