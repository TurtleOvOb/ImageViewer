#pragma once
#include<qrunnable.h>
#include<qobject.h>
#include<imageManager.h>
#include<qthread.h>
#include"poolNotifier.h"
class addPicsThread :public QRunnable {
//Q_OBJECT
public:
	addPicsThread(poolNotifier* parent,QString filePath);
	~addPicsThread() = default;
//signals:
//	void done(int id);
private:
	poolNotifier* notifier;
	QString filePath;
	void run()override;
};
