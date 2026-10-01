#pragma once
#include<qimage.h>
#include<qmap.h>
#include<qmutex.h>
#include<qdebug.h>

class imageManager {

public:
	//保存参数，方便撤回
	struct imgParams {
		bool colInverted = false;//颜色反转
		bool grayScale = false;//灰度
		bool mirrored = false;//镜像
		int rotation = 0;//旋转
		int contrast = 0;//对比度
		int lightness = 0;//亮度
	};
	//id 直接当 key 用，Item 里不再存一份 id
	struct ImageItem {
		QString filePath;
		QImage img;
		imgParams params;
	};

	imageManager() = default;
	~imageManager() = default;
	static imageManager* instance();

	int add_Image(const QString& filePath);
	int remove_imageById(int id);

	QImage get_ImageById(int id);
	QImage render(int id);

	bool has_Image(int id);

	void set_ColInverted(int id, bool colInverted);
	void set_GrayScaled(int id, bool grayScaled);
	void set_Mirrored(int id, bool mirrored);
	void set_Contrast(int id, int val);
	void set_rotation(int id, int val);
	void set_lightness(int id, int val);
	imgParams get_params(int id);

	int get_nextId();
	int get_lastId();
	int get_firstId();

	int get_CurId();
	void set_CurId(int id);
	void clear();

private:
	//按 id 索引，find/insert/erase 都直接按 id 定位，indexOf 那套下标换算不需要了
	QMap<int, ImageItem> image;
	//worker 线程在加图，主线程同时在读，所有碰 image 的公开方法都要加锁
	QMutex mutex;
	int nextId = 0;
	int curId = -1;//-1 表示当前没有选中任何图片
};
