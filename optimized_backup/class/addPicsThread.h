#pragma once
#include<qrunnable.h>
#include<qobject.h>
#include<imageManager.h>
#include<qthread.h>
#include"poolNotifier.h"
class addPicsThread :public QRunnable {
public:
	addPicsThread(poolNotifier* parent,QString filePath);
	~addPicsThread() = default;
private:
	poolNotifier* notifier;
	QString filePath;
	void run()override;
};
