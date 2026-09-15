#pragma once
#include<qpixmap.h>
#include<qvector.h>
#include<qdebug.h>
class imageManager {
public:
	struct ImageItem {
		QString filePath;
		QPixmap pix;
		int id = -1;
	};
	imageManager();
	~imageManager();
	static imageManager* instance();
	int indexOf(int id);
	int add_Image(const QString&filePath);
	void remove_imageById(int id);
	QPixmap get_ImageById(int id) ;
	int get_nextId();
	int get_lastId();
	int get_CurId();
	int get_Count();
	void set_CurId(int id);
	void clear();
private:
	int nextId = 0;
	int curIndex = 0;
	int curId = 0;
	QVector<ImageItem>image;
};