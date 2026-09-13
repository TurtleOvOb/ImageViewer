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

void MyGraphicsView::setPixmap(const QPixmap &pixmap)
{
	item = scene->addPixmap(pixmap);
	//QGraphicsView::fitInView(item, Qt::KeepAspectRatio);
}

void MyGraphicsView::resizeEvent(QResizeEvent* event)
{
QGraphicsView::resizeEvent(event);
if (item!=NULL&&isFirstLoad) {
	fitInView(item,Qt::KeepAspectRatio);
	isFirstLoad = false;
}
}

void MyGraphicsView::wheelEvent(QWheelEvent* event)
{
	//qDebug() << "view滚轮";
	if (event->modifiers() & Qt::CTRL) {
		if (event->angleDelta().y() > 0) {
			//qDebug() << "放大";
			this->scale(scaleFactor, scaleFactor);
		}
		if (event->angleDelta().y() < 0) {
			//qDebug() << "缩小";
			this->scale(1.0/scaleFactor, 1.0 / scaleFactor);
		}
		event->accept();
	}
	
	QGraphicsView::event(event);
}
