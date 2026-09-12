#pragma once
#include"MyLabel.h"
#include<qvector.h>

class thumbnailManager {

public:
	thumbnailManager();
	~thumbnailManager();
	MyLabel* create_Thumbnail(QWidget*parent,QString filePath);
	MyLabel* get_thumbnail(const int &id)const;
	int get_curId();
	int get_count();
	void setCurid(int id);
	//bool delete_Thumbnail();
private:
	int count=0;
	int curId = 0;
	QVector<MyLabel*>thumbnails;

};