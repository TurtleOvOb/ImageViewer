#include "imageManager.h"
namespace {
	//灰度化
	QPixmap toGray(const QImage&src) {
		QImage img = src.convertToFormat(QImage::Format_ARGB32);
		for (int i = 0; i < img.height(); i++) {
		   QRgb*rgb=reinterpret_cast<QRgb*>(img.scanLine(i));
		   for (int j = 0; j < img.width(); j++) {
			  const QRgb pixel=rgb[j];//获取rgb数据（只读）
			  const int g = (qRed(pixel) * 11 + qGreen(pixel) * 16 + qBlue(pixel) * 5) >> 5;//计算灰度公式
			  rgb[j] = qRgba(g, g, g, qAlpha(pixel));//填充回像素
		   }
		}
		return QPixmap::fromImage(img);
	}
	//对比度+亮度计算
	QPixmap contrastAndLightness(const QImage& src,int contrast,int lightness) {
		//先计算每个值改变对比度后的值，获取一个表
		uchar contrastMap[256];
		double factor = 1.0 + contrast / 100.0;
		for (int i = 0; i < 256; i++) {
			double contrastVal=(i - 128)* factor + 128+lightness;
			contrastMap[i] = uchar(qBound(0, int(contrastVal + 0.5), 255));
		}
		QImage img = src.convertToFormat(QImage::Format_ARGB32);
		for (int i = 0; i < img.height(); i++) {
			QRgb* rgb = reinterpret_cast<QRgb*>(img.scanLine(i));
			for (int j = 0; j < img.width(); j++) {
				const QRgb pixel = rgb[j];//获取rgb数据（只读）
				rgb[j] = qRgba(contrastMap[qRed(pixel)], contrastMap[qGreen(pixel)], contrastMap[qBlue(pixel)], qAlpha(pixel));//填充回像素
			}
		} 
		return QPixmap::fromImage(img);
	}
	//旋转
	QPixmap rotate(const QImage& src, int rotation) {
		qDebug() << "2";
		QImage img = src.convertToFormat(QImage::Format_ARGB32);
		QTransform t;
		int width = img.width();
		int height = img.height();
		t.translate(width / 2, height / 2);
		t.rotate(rotation);
		t.translate(-width / 2, -height / 2);
	    img=img.transformed(t, Qt::SmoothTransformation);
		return QPixmap::fromImage(img);
	}
}

//获取静态实例
imageManager* imageManager::instance() {
	static imageManager images;
	return &images;

}
//根据id查找返回下标
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
//id+ndexOf移除图片数据
int imageManager::remove_imageById(int id)
{
	
	int index = indexOf(id);
	if (index >= 0) {
		image.removeAt(index);
		qDebug() << "数据移除成功，索引"<<index;
		qDebug() << "当前image大小" << image.size();
		//removeAt后数据自动前移，不用加
		if (index < image.size()) {
			return image[index].id;
		}
		else {
			return -1;
		}
	
	}
	else {
		qDebug() << "找不到数据，索引：" << index;
	}
}
//根据id+indexOf函数返回图片数据
QPixmap imageManager::get_ImageById(int id) 
{

	int index = indexOf(id);
	if (index < 0) {
		return QPixmap();
	}
	else {
		return image.at(index).pix;
	}

	

}
//不要返回QPixmap的引用或指针，这里返回的是一个临时变量，直接值传递，否则就是悬空引用/指针
//根据图像参数对图像进行处理
QPixmap imageManager::render(int id)
{
	int index = indexOf(id);
	if (index == -1) {
		qDebug() << "未找到图片";
		return QPixmap();
	}
	imgParams params = image.at(index).params;
	QPixmap pix = image[index].pix;
	QImage img = pix.toImage();
	//颜色反转处理
	if (params.colInverted) {
	
		if (img.format() != QImage::Format_ARGB32) {
            img = img.convertToFormat(QImage::Format_ARGB32);
        }
            img.invertPixels();
			qDebug() << "颜色反转，正在回填";
		    pix = QPixmap::fromImage(img);
	}
	//灰度化处理
	if (params.grayScale) {
		if (img.format() != QImage::Format_ARGB32) {
			img = img.convertToFormat(QImage::Format_ARGB32);
		}
		 img = pix.toImage();
		 pix=toGray(img);
	}
	//镜像处理
	if (params.mirrored) {
		if (img.format() != QImage::Format_ARGB32) {
			img = img.convertToFormat(QImage::Format_ARGB32);
		}
		img = pix.toImage();
		img = img.mirrored(true, false);
		pix = QPixmap::fromImage(img);
	}
	//对比度+亮度处理
	if (params.contrast != 0||params.lightness!=0) {
		if (img.format() != QImage::Format_ARGB32) {
			img = img.convertToFormat(QImage::Format_ARGB32);
		}
		img = pix.toImage();
		pix = contrastAndLightness(img,params.contrast,params.lightness);
	}
	//旋转处理
	if (params.rotation != 0) {
		if (img.format() != QImage::Format_ARGB32) {
			img = img.convertToFormat(QImage::Format_ARGB32);
		}
		img = pix.toImage();
		pix = rotate(img, params.rotation);
	}
	return pix;
}
//设置图片颜色反转参数
void imageManager::set_ColInverted(int id, bool colInverted)
{
	int index = indexOf(id);
	imgParams&params = image[index].params;
	params.colInverted = colInverted;
}
//设置灰度化参数
void imageManager::set_GrayScaled(int id, bool grayScaled)
{
	int index = indexOf(id);
	imgParams& params = image[index].params;
	params.grayScale = grayScaled;
}
//设置镜像参数
void imageManager::set_Mirrored(int id, bool mirrored) {
	int index = indexOf(id);
	imgParams& params = image[index].params;
	params.mirrored = mirrored;
}
//设置对比度参数
void imageManager::set_Contrast(int id, int val)
{
	int index = indexOf(id);
	imgParams& params = image[index].params;
	params.contrast = val;
}
//设置旋转参数
void imageManager::set_rotation(int id, int val)
{
	int index = indexOf(id);
	imgParams& params = image[index].params;
	params.rotation = val;
}
//设置亮度参数
void imageManager::set_lightness(int id, int val)
{
	int index = indexOf(id);
	imgParams& params = image[index].params;
	params.lightness = val;
}
//获取图像参数
imageManager::imgParams imageManager::get_params(int id)
{
	int index = indexOf(id);
	imgParams params = image[index].params;
	return params;
}
//获取当前id的下一个id（在数组中的位置）
int imageManager::get_nextId() {
	int nextIndex = curIndex + 1;
	if (nextIndex >= 0 && nextIndex < image.size()) {
		return image.at(nextIndex).id;
	}
	else {
		return -1;
	}
}
//获取当前id的上一个id（在数组中的位置）
int imageManager::get_lastId() {
	int  lastIndex = curIndex - 1;

	if (lastIndex >= 0 && lastIndex < image.size()) {
		return image.at(lastIndex).id;
	}
	else {
		return -1;
	}
}
//获取当前id
int imageManager::get_CurId()
{
	return curId;
}
//设置当前Id
void imageManager::set_CurId(int id)
{
	this->curId = id;
}
//清空图像数据
void imageManager::clear() {
	//手动删空后不执行if内部，但任何情况下curId和nextId都应该重置
	curId = 0;
	nextId = 0;
	if (!image.isEmpty()) {
		image.clear();
	}
}

