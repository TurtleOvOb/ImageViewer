#include "addPicsThread.h"





addPicsThread::addPicsThread(poolNotifier*notifier, QString filePath)
{
	this->notifier=notifier;
	this->filePath = filePath;
}

void addPicsThread::run()
{
	//qDebug() << QThread::currentThreadId()<<"开始";
	//imageManager::instance()->test();
	//qDebug() << QThread::currentThreadId() << "结束";
	int id = imageManager::instance()->add_Image(filePath);
	    QImage img=imageManager::instance()->get_ImageById(id);
		if (img.isNull()) {
			emit notifier->done(-1, QImage());
			return;
		}
		QImage thumbnail = img.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);

	 emit notifier->done(id,thumbnail);
}
