#pragma once
#include<qpixmap.h>
#include<qvector.h>
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
	int indexOf(int id)const;
	int add_Image(const QString&filePath);
	QPixmap get_ImageById(int id) const;
	int get_CurId();
	int get_Count();
	void set_CurId(int id);
	void clear();
private:

	int nextId = 0;
	int curId = 0;
	QVector<ImageItem>image;
};