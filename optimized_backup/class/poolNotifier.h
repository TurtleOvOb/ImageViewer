#pragma once
#include<qobject.h>
#include<qimage.h>
class poolNotifier :public QObject {
	Q_OBJECT
public:
	poolNotifier(QObject*parent);
	~poolNotifier() = default;
signals:
	//id<0 表示解码失败；thumbnail 是解码线程缩好的缩略图
	void done(int id,const QImage& thumbnail);


};
