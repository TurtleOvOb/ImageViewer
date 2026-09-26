#pragma once
#include"MyLabel.h"
#include<qvector.h>
#include<imageManager.h>
class thumbnailManager {

public:
	thumbnailManager() = default;
	~thumbnailManager() = default;
	MyLabel* create_Thumbnail(QWidget*parent,const QString& filePath);

private:

};