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
int imageManager::indexOf(int id)const
{
	for (int i = 0; i < image.size(); i++) {
		if (image.at(i).id == id) {
			return i;
		}
	}
	return -1;
}
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

QPixmap imageManager::get_ImageById(int id) const
{
	const int index=indexOf(id);
	return index < 0?QPixmap():image.at(index).pix;

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

