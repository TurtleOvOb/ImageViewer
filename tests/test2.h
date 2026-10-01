#pragma once
#include<qobject.h>
#include"imageManager.h"
#include<qimage.h>
#include<QtTest/qtest.h>
#include<qtemporarydir.h>
class test2 :public QObject {
	Q_OBJECT
private:
	QTemporaryDir *tmpDir;
	QString makeSolidImage(const QString& fileName, const QColor& color);
	QString makeSplitImage(const QString& fileName);
	QColor pixelAt(const QImage& img, int x, int y);
private slots:
	void init();
	void cleanup();


	void test_addImage();
	void test_remove_imageById();
	void test_switchSeq();
	void test_paramsInput();
	void test_rotation();
	void test_invert();
	void test_grayScale();
	void test_mirror();
	void test_wrongId();
	void test_wrongParam();

};
