#include "addPicsThread.h"
#include"thumbnailManager.h"





addPicsThread::addPicsThread(poolNotifier*notifier, QString filePath)
{
	this->notifier=notifier;
	this->filePath = filePath;
}

//在线程池里执行：整张图只解码一次，原图入队，缩略图顺着信号带回 GUI 线程
void addPicsThread::run()
{
	QImage img(filePath);
	if (img.isNull()) {
		emit notifier->done(-1, QImage());
		return;
	}
	int id = imageManager::instance()->add_Image(img, filePath);
	QImage thumbnail = img.scaled(thumbnailManager::ThumbnailSize, thumbnailManager::ThumbnailSize,
		Qt::KeepAspectRatio, Qt::SmoothTransformation);
	emit notifier->done(id, thumbnail);
}
