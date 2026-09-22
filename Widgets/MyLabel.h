#pragma once
#include<qlabel.h>
class MyLabel :public QLabel {
	Q_OBJECT
public:
	explicit MyLabel(QWidget*parent);
	~MyLabel();

signals:
	void clicked();
	
private:

	void mouseReleaseEvent(QMouseEvent*event)override;
	
};