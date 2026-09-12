#include "MyLabel.h"

MyLabel::MyLabel(QWidget* parent):QLabel(parent)
{
}

MyLabel::~MyLabel()
{
}

void MyLabel::setPix(const QPixmap& pixmap)
{
	this->pix = pixmap;
}

void MyLabel::setId(int id)
{
	this->id = id;
}

QPixmap& MyLabel::pixmap()
{
	return pix;
}

void MyLabel::mouseReleaseEvent(QMouseEvent* event)
{
	QWidget::mouseReleaseEvent(event);
	emit clicked(id);
}
