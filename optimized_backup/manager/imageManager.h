#pragma once
#include<qimage.h>
#include<qvector.h>
#include<qdebug.h>
#include<qmutex.h>
#include<qthread.h>
class imageManager{

public:
	//保存参数，方便撤回
	struct imgParams {
		bool colInverted=false;//颜色反转
		bool grayScale = false;//灰度
		bool mirrored = false;//镜像
		int rotation = 0;//旋转
		int contrast = 0;//对比度
		int lightness = 0;//对比度
	};
	struct ImageItem {
		QString filePath;
		QImage img;
		imgParams params;
		int id = -1;
	};
	imageManager()=default;
	~imageManager() = default;
	static imageManager* instance();
	int indexOf(int id);

	int add_Image(const QString&filePath);
	//img 已经解码好时直接入队，避免同一张图解两次
	int add_Image(const QImage& img,const QString& filePath);
	int remove_imageById(int id);
	QImage get_ImageById(int id);
	QImage render(int id);
	void set_ColInverted(int id, bool colInverted);
	void set_GrayScaled(int id, bool grayScaled);
	void set_Mirrored(int id, bool mirrored);
	void set_Contrast(int id, int val);
	void set_rotation(int id, int val);
	void set_lightness(int id, int val);
	imgParams get_params(int id);
	int get_nextId();
	int get_lastId();
	int get_CurId();
	void set_CurId(int id);
	void clear();
	void test();
private:
	QMutex mutex;
	int nextId = 0;
	int curIndex = 0;
	int curId = 0;
	QVector<ImageItem>image;

};
