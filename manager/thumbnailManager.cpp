#include"thumbnailManager.h"

thumbnailManager::thumbnailManager()
{
	
}

thumbnailManager::~thumbnailManager()
{
}

MyLabel* thumbnailManager::create_Thumbnail(QWidget* parent,const QString& filePath)
{
	QPixmap pixmap(filePath);
	MyLabel* label = new MyLabel(parent);
	label->setMinimumSize(100, 100);
	label->setScaledContents(true);
	label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	
	if (!pixmap.isNull()) {
		label->setPixmap(pixmap.scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
		return label;
	}
	else{
		qDebug() << "pixmap为空";
		delete label;
		return nullptr;
	}

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
