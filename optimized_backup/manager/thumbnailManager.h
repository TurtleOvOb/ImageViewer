#pragma once
#include"MyLabel.h"
#include<qvector.h>
#include<qimage.h>
class thumbnailManager {

public:
	//缩略图目标边长（像素）：解码线程按它缩放，界面线程按它设标签最小尺寸
	static constexpr int ThumbnailSize = 100;

	thumbnailManager() = default;
	~thumbnailManager() = default;
	//只把已经解码好的缩略图贴到标签上，不做解码；必须在 GUI 线程调用
	MyLabel* create_Thumbnail(QWidget*parent,const QImage& thumbnail);

private:

};
