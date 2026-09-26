#pragma once
#include<qpixmap.h>
#include<qvector.h>
#include<qdebug.h>
#include<qobject.h>
class imageManager :public QObject{
	Q_OBJECT
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
		QPixmap pix;
		imgParams params;
		int id = -1;
	};
	imageManager()=default;
	~imageManager() = default;
	static imageManager* instance();
	int indexOf(int id);

	int add_Image(const QString&filePath);
	int remove_imageById(int id);
	QPixmap get_ImageById(int id);
	QPixmap render(int id);
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
private:
	int nextId = 0;
	int curIndex = 0;
	int curId = 0;
	QVector<ImageItem>image;
private slots:
	//void initTestCase();
	//void firstTest();
	//void secondTest();
	//void cleanupTestCase();
	
};