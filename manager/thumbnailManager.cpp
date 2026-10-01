#include"thumbnailManager.h"

MyLabel* thumbnailManager::create_Thumbnail(QWidget* parent)
{
	MyLabel* label = new MyLabel(parent);
	label->setMinimumSize(100, 100);
	label->setScaledContents(true);
	label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	return label;

}

MyLabel* thumbnailManager::create_Thumbnail(QWidget* parent, const QImage& img)
{

	QPixmap pix = QPixmap::fromImage(img);
	MyLabel* label = new MyLabel(parent);
	label->setMinimumSize(100, 100);
	label->setScaledContents(true);
	label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	
	if (!pix.isNull()) {
		label->setPixmap(pix);
		return label;
	}
	else{
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
