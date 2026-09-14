#include "imageManager.h"

imageManager::imageManager()
{
}

imageManager::~imageManager()
{
}
imageManager* imageManager::instance() {
	static imageManager images;
	return &images;

}
//根据id查找返回位置
int imageManager::indexOf(int id)const
{
	for (int i = 0; i < image.size(); i++) {
		if (image.at(i).id == id) {
			return i;
		}
	}
	return -1;
}
//添加图片
int imageManager::add_Image(const QString& filePath)
{
	if (!filePath.isEmpty()) {
		ImageItem item;
		item.filePath = filePath;
		QPixmap pixmap(filePath);
		if (!pixmap.isNull()) {
			item.pix = pixmap;
			item.id = nextId++;
			image.append(item);
			return item.id;
		}
		else {
			return -1;
		}
	}
	else {
		return false;
	}

}
//根据id+ndexOf移除图片数据
void imageManager::remove_imageById(int id)
{
	int index = indexOf(id);
	if (index >= 0) {
		image.removeAt(index);
		qDebug() << "数据移除成功，索引"<<index;
		return ;
	}
	else {
		qDebug() << "找不到数据，索引：" << index;
	}
}
//根据id+indexOf函数返回图片数据
QPixmap imageManager::get_ImageById(int id) 
{
	const int index=indexOf(id);
	if (index < 0) {
		return QPixmap();
	}
	else {
		curIndex = index;
		return image.at(index).pix;
	}

	

}



int imageManager::get_CurId()
{
	return curId;
}

int imageManager::get_Count()
{
	return image.size();
}

void imageManager::set_CurId(int id)
{
	this->curId = id;
}
void imageManager::clear() {
	if (!image.isEmpty()) {
		image.clear();
		curId = 0;
	}

}

