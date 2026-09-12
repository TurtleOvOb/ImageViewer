#pragma once
#include<qlabel.h>
class MyLabel :public QLabel {
	Q_OBJECT
public:
	explicit MyLabel(QWidget*parent);
	~MyLabel();
	void setPix(const QPixmap& pixmap);
	void setId(int id);
	QPixmap& pixmap();
signals:
	void clicked(int id);
	
private:
	int id = 0;
	QPixmap pix;
	void mouseReleaseEvent(QMouseEvent*event)override;
	
};