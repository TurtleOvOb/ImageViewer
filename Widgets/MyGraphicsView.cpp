#include"MyGraphicsView.h"

MyGraphicsView::MyGraphicsView(QWidget* parent):QGraphicsView(parent)
{
	scene = new QGraphicsScene(this);

	this->setScene(scene);
	this->setDragMode(QGraphicsView::ScrollHandDrag);
	this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

}

MyGraphicsView::~MyGraphicsView()
{

}

void MyGraphicsView::setPixmap(const QPixmap &pixmap, bool zoomReset)
{
	if (!item) {
		item = scene->addPixmap(pixmap);
	}
	else {
		item->setPixmap(pixmap);
	}
	//默认是 FastTransformation（最近邻采样），缩小时细纹会变成网格、细线会变成虚线，
	//跟系统看图软件一比就显得锯齿很重。这里改成平滑采样
	item->setTransformationMode(Qt::SmoothTransformation);
	//场景框必须跟着当前这张图走：不设的话它会一直记着历史上最大的那张，
	//之后切到小图时就冒出滚动条、图片被挤到左上角
	scene->setSceneRect(item->boundingRect());
	if (zoomReset) {
		resetTransform();
		isFirstLoad = true;
		fitInView(item, Qt::KeepAspectRatio);
	}
}

//resizeEvent，主要用于初始化时让图片大小适配view大小
void MyGraphicsView::resizeEvent(QResizeEvent* event)
{
QGraphicsView::resizeEvent(event);
if (item!=NULL&&isFirstLoad) {
	fitInView(item,Qt::KeepAspectRatio);
	isFirstLoad = false;
}
}
//滚轮事件，用于缩放图片
void MyGraphicsView::wheelEvent(QWheelEvent* event)
{
		if (event->angleDelta().y() > 0) {
			this->scale(scaleFactor, scaleFactor);
		}
		if (event->angleDelta().y() < 0) {
			this->scale(1.0/scaleFactor, 1.0 / scaleFactor);
		}
		event->accept();
	QGraphicsView::event(event);
}


