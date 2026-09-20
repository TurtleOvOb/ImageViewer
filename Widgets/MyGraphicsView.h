#pragma once
#include<qgraphicsview.h>
#include<qgraphicsscene.h>
#include<qpixmap.h>
#include<qgraphicsitem.h>
#include<qevent.h>
class MyGraphicsView :public QGraphicsView {
public:

	MyGraphicsView(QWidget*parent);

	~MyGraphicsView();
	void setPixmap(const QPixmap &pixmap);
private:
	bool isFirstLoad = true;//防止View构建后resizeEvent和滚轮缩放打架
	float scaleFactor = 1.1;
	QGraphicsScene* scene=nullptr;
	QGraphicsPixmapItem* item=nullptr;
	//保证控件完整缩放至view对的大小
	void resizeEvent(QResizeEvent*event)override;
	//
	void wheelEvent(QWheelEvent*event)override;

};