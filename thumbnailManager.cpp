#include"thumbnailManager.h"

thumbnailManager::thumbnailManager()
{
	
}

thumbnailManager::~thumbnailManager()
{
}

MyLabel* thumbnailManager::create_Thumbnail(QWidget* parent,QString filePath)
{
	QPixmap pixmap(filePath);
	MyLabel* label = new MyLabel(parent);
	label->setMinimumSize(100, 100);
	label->setScaledContents(true);
	label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	
	if (!pixmap.isNull()) {
		thumbnails.append(label);
		label->setId(count++);
		label->setPixmap(pixmap.scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
		label->setPix(pixmap);
		return label;
	}
	else{
		qDebug() << "pixmap为空";
		return nullptr;
	}

}

MyLabel* thumbnailManager::get_thumbnail(const int&id) const
{
	if (thumbnails.at(id)) {
		return thumbnails.at(id);
	}
	else {
		return nullptr;
	}
	
}

int thumbnailManager::get_curId()
{
	return this->curId;
}

int thumbnailManager::get_count()
{
	return this->count;
}

void thumbnailManager::setCurid( int id)
{
	this->curId = id;
	qDebug() << "当前图片id:" << curId;
}
