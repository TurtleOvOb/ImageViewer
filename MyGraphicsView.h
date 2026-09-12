#include<qgraphicsview.h>
#include<qgraphicsscene.h>
#include<qpixmap.h>
#include<qgraphicsitem.h>
#include<qevent.h>
class MyGraphicsView :public QGraphicsView {
public:

	MyGraphicsView(QWidget*parent);

	~MyGraphicsView();
	void setPixmap(QPixmap pixmap);
private:
	bool isFirstLoad = true;
	float scaleFactor = 1.1;
	QGraphicsScene* scene=nullptr;
	QGraphicsPixmapItem* item=nullptr;
	void resizeEvent(QResizeEvent*event)override;
	void wheelEvent(QWheelEvent*event)override;
};