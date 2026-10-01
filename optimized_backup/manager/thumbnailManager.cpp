#include"thumbnailManager.h"
#include<qpixmap.h>

MyLabel* thumbnailManager::create_Thumbnail(QWidget* parent,const QImage& thumbnail)
{
	if (thumbnail.isNull()) {
		return nullptr;
	}
	MyLabel* label = new MyLabel(parent);
	label->setMinimumSize(ThumbnailSize, ThumbnailSize);
	label->setScaledContents(true);
	label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	
	//QPixmap 只能在 GUI 线程构造，所以转换放在这里而不是解码线程
	label->setPixmap(QPixmap::fromImage(thumbnail));
	return label;
}


//void thumbnailManager::clear() {
//	if (!thumbnails.isEmpty()) {
//		for (QLabel* label : thumbnails) {
//			delete label;
//			label = nullptr;
//		}
//	
//	}
//}
//int thumbnailManager::get_curId()
//{
//	return this->curId;
//}
//
//int thumbnailManager::get_count()
//{
//	return this->count;
//}
//
//void thumbnailManager::setCurid( int id)
//{
//	this->curId = id;
//	//qDebug() << "当前图片id:" << curId;
//}
