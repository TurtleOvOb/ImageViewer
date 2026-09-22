#pragma once
#include"MyLabel.h"
#include<qvector.h>
#include<imageManager.h>
class thumbnailManager {

public:
	thumbnailManager();
	~thumbnailManager();
	MyLabel* create_Thumbnail(QWidget*parent,const QString& filePath);
	//void clear();
	//int get_curId();
	//int get_count();
	//void setCurid(int id);
	//bool delete_Thumbnail();
private:

};