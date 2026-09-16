#include "imageManager.h"

imageManager::imageManager()
{
}

imageManager::~imageManager()
{
}
//
imageManager* imageManager::instance() {
	static imageManager images;
	return &images;

}
//根据id查找返回位置
int imageManager::indexOf(int id)
{
	for (int i = 0; i < image.size(); i++) {
		if (image.at(i).id == id) {
			curIndex = i;
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
		return image.at(index).pix;
	}

	

}
//不要返回QPixmap的引用或指针，这里返回的是一个临时变量，直接值传递，否则就是悬空引用/指针
QPixmap imageManager::render(int id)
{
	qDebug() << id;
	int index = indexOf(id);
	imgParams params = image.at(index).params;
	QPixmap pix = image.at(index).pix;
	
	QImage img = pix.toImage();
	if (params.colInverted) {
		if (img.format() != QImage::Format_ARGB32) {
            img = img.convertToFormat(QImage::Format_ARGB32);
        }
            img.invertPixels();
		    pix = QPixmap::fromImage(img);
	
	}

	return pix;
}
//
void imageManager::set_ColInverted(int id, bool colInverted)
{
	int index = indexOf(id);
	imgParams&params = image[index].params;
	params.colInverted = colInverted;

}
//
int imageManager::get_nextId() {
	int nextIndex = curIndex + 1;
	if (nextIndex >= 0 && nextIndex < image.size()) {
		return image.at(nextIndex).id;
	}
}
//
int imageManager::get_lastId() {
	int  lastIndex = curIndex - 1;
	if (lastIndex >= 0 && lastIndex < image.size()) {
		return image.at(lastIndex).id;
	}
}
//
int imageManager::get_CurId()
{
	return curId;
}


//
void imageManager::set_CurId(int id)
{
	this->curId = id;
}
//
void imageManager::clear() {
	if (!image.isEmpty()) {
		image.clear();
		curId = 0;
	}

}

