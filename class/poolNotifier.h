#pragma once
#include<qobject.h>
class poolNotifier :public QObject {
	Q_OBJECT
public:
	poolNotifier(QObject*parent);
	~poolNotifier() = default;
signals:
	void done(int id,const QImage&thumbnail);


};
