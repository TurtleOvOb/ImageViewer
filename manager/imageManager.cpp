#include "imageManager.h"
namespace {
	//灰度化
	QImage toGray(const QImage&src) {
		QImage img = src.convertToFormat(QImage::Format_ARGB32);
		for (int i = 0; i < img.height(); i++) {
		   QRgb*rgb=reinterpret_cast<QRgb*>(img.scanLine(i));
		   for (int j = 0; j < img.width(); j++) {
			  const QRgb pixel=rgb[j];//获取rgb数据（只读）
			  const int g = (qRed(pixel) * 11 + qGreen(pixel) * 16 + qBlue(pixel) * 5) >> 5;//计算灰度公式
			  rgb[j] = qRgba(g, g, g, qAlpha(pixel));//填充回像素
		   }
		}
		return img;
	}
	//对比度+亮度计算
	QImage contrastAndLightness(const QImage& src,int contrast,int lightness) {
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
		return img;
	}
	//旋转
	QImage rotate(const QImage& src, int rotation) {
		QImage img = src.convertToFormat(QImage::Format_ARGB32);
		QTransform t;
		int width = img.width();
		int height = img.height();
		t.translate(width / 2, height / 2);
		t.rotate(rotation);
		t.translate(-width / 2, -height / 2);
	    img=img.transformed(t, Qt::SmoothTransformation);
		return img;
	}
}

//获取静态实例
imageManager* imageManager::instance() {
	static imageManager images;
	return &images;

}
//添加图片：解码是整条链路里最慢的一步，放在锁外做
int imageManager::add_Image(const QString& filePath)
{
	if (filePath.isEmpty()) {
		return -1;
	}
	QImage img(filePath);
	if (img.isNull()) {
		return -1;
	}

	QMutexLocker locker(&mutex);
	const int id = nextId++;
	ImageItem item;
	item.filePath = filePath;
	item.img = img;
	image.insert(id, item);
	return id;
}
//按 id 移除图片数据，删除后返回后一张图片id（id 升序的下一个，没找到或已是最后一个返回 -1）
int imageManager::remove_imageById(int id)
{
	QMutexLocker locker(&mutex);
	auto it = image.find(id);
	if (it == image.end()) {
		qDebug() << "找不到数据，id：" << id;
		return -1;
	}
	auto next = it;
	++next;
	if (next == image.end()) {
		image.erase(it);
		return -1;
	}
	const int nextId = next.key();
	image.erase(it);
	return nextId;
}
//根据id返回图片数据
QImage imageManager::get_ImageById(int id)
{
	QMutexLocker locker(&mutex);
	auto it = image.find(id);
	if (it == image.end()) {
		return QImage();
	}
	return it->img;
}
//id 是否存在，替代原来靠 indexOf 判 -1 的写法
bool imageManager::has_Image(int id)
{
	QMutexLocker locker(&mutex);
	return image.contains(id);
}
//不要返回QPixmap的引用或指针，这里返回的是一个临时变量，直接值传递，否则就是悬空引用/指针
//根据图像参数对图像进行处理
QImage imageManager::render(int id)
{
	imgParams params;
	QImage img;
	{
		//锁只用来取一份数据副本。QImage 隐式共享，这份拷贝很便宜
		QMutexLocker locker(&mutex);
		auto it = image.find(id);
		if (it == image.end()) {
			qDebug() << "未找到图片";
			return QImage();
		}
		params = it->params;
		img = it->img;
	}
	//下面整段像素处理都在锁外做，不挡着还在加图的 worker 线程
	//颜色反转处理
	if (params.colInverted) {
	
		if (img.format() != QImage::Format_ARGB32) {
            img = img.convertToFormat(QImage::Format_ARGB32);
        }
            img.invertPixels();
			qDebug() << "颜色反转，正在回填";

	}
	//灰度化处理
	if (params.grayScale) {
		if (img.format() != QImage::Format_ARGB32) {
			img = img.convertToFormat(QImage::Format_ARGB32);
		}
		 img=toGray(img);
	}
	//镜像处理
	if (params.mirrored) {
		if (img.format() != QImage::Format_ARGB32) {
			img = img.convertToFormat(QImage::Format_ARGB32);
		}
		img = img.mirrored(true, false);
	}
	//对比度+亮度处理
	if (params.contrast != 0||params.lightness!=0) {
		if (img.format() != QImage::Format_ARGB32) {
			img = img.convertToFormat(QImage::Format_ARGB32);
		}
		img = contrastAndLightness(img,params.contrast,params.lightness);
	}
	//旋转处理
	if (params.rotation != 0) {
		if (img.format() != QImage::Format_ARGB32) {
			img = img.convertToFormat(QImage::Format_ARGB32);
		}
		img = rotate(img, params.rotation);
	}
	return img;
}
//设置图片颜色反转参数
void imageManager::set_ColInverted(int id, bool colInverted)
{
	QMutexLocker locker(&mutex);
	auto it = image.find(id);
	if (it == image.end()) {
		return;
	}
	it->params.colInverted = colInverted;
}
//设置灰度化参数
void imageManager::set_GrayScaled(int id, bool grayScaled)
{
	QMutexLocker locker(&mutex);
	auto it = image.find(id);
	if (it == image.end()) {
		return;
	}
	it->params.grayScale = grayScaled;
}
//设置镜像参数
void imageManager::set_Mirrored(int id, bool mirrored) {
	QMutexLocker locker(&mutex);
	auto it = image.find(id);
	if (it == image.end()) {
		return;
	}
	it->params.mirrored = mirrored;
}
//设置对比度参数
void imageManager::set_Contrast(int id, int val)
{
	QMutexLocker locker(&mutex);
	auto it = image.find(id);
	if (it == image.end()) {
		return;
	}
	it->params.contrast = qBound(-100, val, 100);
}
//设置旋转参数
void imageManager::set_rotation(int id, int val)
{
	QMutexLocker locker(&mutex);
	auto it = image.find(id);
	if (it == image.end()) {
		return;
	}
	it->params.rotation = qBound(-360, val, 360);
}
//设置亮度参数
void imageManager::set_lightness(int id, int val)
{
	QMutexLocker locker(&mutex);
	auto it = image.find(id);
	if (it == image.end()) {
		return;
	}
	it->params.lightness = qBound(-100, val, 100);
}
//获取图像参数
imageManager::imgParams imageManager::get_params(int id)
{
	QMutexLocker locker(&mutex);
	auto it = image.find(id);
	if (it == image.end()) {
		return imgParams{};//id 不存在返回一套默认参数
	}
	return it->params;
}
//下一个 id（QMap 按 key 升序，比当前 id 大的第一个就是数组顺序上的下一张）
int imageManager::get_nextId() {
	QMutexLocker locker(&mutex);
	auto it = image.find(curId);
	if (it == image.end()) {
		return -1;
	}
	++it;
	if (it == image.end()) {
		return -1;
	}
	return it.key();
}
//上一个 id
int imageManager::get_lastId() {
	QMutexLocker locker(&mutex);
	auto it = image.find(curId);
	if (it == image.end() || it == image.begin()) {
		return -1;
	}
	--it;
	return it.key();
}
//第一张的 id，替代原来 indexOf(0) 那种"按下标找图"的写法
int imageManager::get_firstId()
{
	QMutexLocker locker(&mutex);
	if (image.isEmpty()) {
		return -1;
	}
	return image.begin().key();
}
//获取当前id
int imageManager::get_CurId()
{
	QMutexLocker locker(&mutex);
	return curId;
}
//设置当前Id
void imageManager::set_CurId(int id)
{
	QMutexLocker locker(&mutex);
	curId = id;
}
//清空图像数据
void imageManager::clear() {
	QMutexLocker locker(&mutex);
	image.clear();
	nextId = 0;
	curId = -1;
}
