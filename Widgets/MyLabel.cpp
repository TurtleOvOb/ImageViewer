#include "MyLabel.h"

MyLabel::MyLabel(QWidget* parent):QLabel(parent)
{
	
	
}

MyLabel::~MyLabel()
{


}



void MyLabel::mouseReleaseEvent(QMouseEvent* event)
{
	QWidget::mouseReleaseEvent(event);
	emit clicked();
}
