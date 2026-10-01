#pragma once
#include<qlabel.h>
class MyLabel :public QLabel {
	Q_OBJECT
public:
	MyLabel();
	explicit MyLabel(QWidget*parent);
	~MyLabel();
signals:
	void clicked();
private:
	void mouseReleaseEvent(QMouseEvent*event)override;
	
};