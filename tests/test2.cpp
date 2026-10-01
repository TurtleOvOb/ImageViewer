#include"test2.h"

void test2::init()
{
    tmpDir = new QTemporaryDir;
    QVERIFY(tmpDir->isValid());
}

void test2::cleanup()
{
    delete tmpDir;
    tmpDir = nullptr;
}

// 生成一张 8x8 的纯色 PNG，返回路径；失败返回空字符串
QString test2::makeSolidImage(const QString& fileName, const QColor& color)
{
    QImage img(8, 8, QImage::Format_ARGB32);
    img.fill(color);
    const QString path = tmpDir->filePath(fileName);
    return img.save(path, "PNG") ? path : QString();
}
// 左半红、右半蓝，用来验证镜像
QString test2::makeSplitImage(const QString& fileName)
{
    QImage img(8, 8, QImage::Format_ARGB32);
    img.fill(QColor(255, 0, 0));
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 4; x < img.width(); ++x) {
            img.setPixelColor(x, y, QColor(0, 0, 255));
        }
    }
    const QString path = tmpDir->filePath(fileName);
    return img.save(path, "PNG") ? path : QString();
}

QColor test2::pixelAt(const QImage& img, int x, int y)
{
    return img.pixelColor(x, y);
}

void test2::test_addImage()
{
    imageManager mgr;
    int imgA= mgr.add_Image(makeSolidImage("imgA.png",QColor(255,0,0)));
    QVERIFY(imgA >= 0);
    QImage img=mgr.get_ImageById(imgA);
    QVERIFY(!img.isNull());

}

void test2::test_remove_imageById()
{
    imageManager mgr;
    int imgA= mgr.add_Image(makeSolidImage("imgA", QColor(255, 0, 0)));
    int imgB= mgr.add_Image(makeSolidImage("imgB", QColor(255, 0, 0)));
    int imgC= mgr.add_Image(makeSolidImage("imgC", QColor(255, 0, 0)));
    QVERIFY(imgA >= 0);
    QVERIFY(imgB >= 0);
    QVERIFY(imgC >= 0);
     imgA=mgr.remove_imageById(imgA);
     QVERIFY(imgA >= 0);
    QImage loadedB= mgr.get_ImageById(imgB);
    QVERIFY(!loadedB.isNull());
}

void test2::test_switchSeq()
{
    imageManager mgr;
    int imgA = mgr.add_Image(makeSolidImage("imgA", QColor(255, 0, 0)));
    QVERIFY(imgA >= 0);
    int lastId = mgr.get_lastId();
    QCOMPARE(lastId, -1);
    int nextId = mgr.get_nextId();
    QCOMPARE(nextId, -1);
}

void test2::test_paramsInput()
{
    imageManager mgr;
    int imgA = mgr.add_Image(makeSolidImage("imgA", QColor(255, 0, 0)));
    int imgB = mgr.add_Image(makeSolidImage("imgB", QColor(255, 0, 0)));
    QVERIFY(imgA >= 0);
    QVERIFY(imgB >= 0);
    mgr.set_ColInverted(imgA, true);
    mgr.set_rotation(imgA, 78);
    mgr.set_lightness(imgA, 91);
  imageManager::imgParams paramsA=mgr.get_params(imgA);
  imageManager::imgParams paramsB = mgr.get_params(imgB);
  QVERIFY(paramsA.colInverted);
  QCOMPARE(paramsA.rotation, 78);
  QCOMPARE(paramsA.lightness, 91);

  QVERIFY(!paramsB.colInverted);
  QVERIFY(!paramsB.grayScale);
  QVERIFY(!paramsB.mirrored);
  QCOMPARE(paramsB.rotation, 0);
  QCOMPARE(paramsB.lightness, 0);
  QCOMPARE(paramsB.contrast, 0);

    
}

void test2::test_rotation()
{
    imageManager mgr;
    int imgA=mgr.add_Image(makeSplitImage("imgA"));
    QVERIFY(imgA >= 0);
    mgr.set_rotation(imgA,90);
    QImage rendered=mgr.render(imgA);
    QColor colA=pixelAt(rendered,7 , 0);
    QCOMPARE(colA.red(), 255);
}

void test2::test_invert()
{
    imageManager mgr;
    int imgA = mgr.add_Image(makeSolidImage("imgA", QColor(255, 0, 0)));
    mgr.set_ColInverted(imgA,true);
   QImage rendered=mgr.render(imgA);
   QVERIFY(!rendered.isNull());
 QColor colA=pixelAt(rendered, 0, 0);
 QCOMPARE(colA.red(), 255 - 255);
 QCOMPARE(colA.green(), 255 - 0);
 QCOMPARE(colA.blue(), 255 - 0);
 QCOMPARE(colA.alpha(), 255);

}

void test2::test_grayScale()
{
    imageManager mgr;
    int imgA = mgr.add_Image(makeSolidImage("imgA", QColor(255, 0, 0)));
    mgr.set_GrayScaled(imgA,true);
   QImage rendered=mgr.render(imgA);
   QColor colA = pixelAt(rendered, 0, 0);
   QCOMPARE(colA.red(), colA.green());
   QCOMPARE(colA.green(), colA.blue());
   QVERIFY(colA.red() >= 0);
   QVERIFY(colA.red() <= 255 );
}

void test2::test_mirror()
{
    imageManager mgr;
    int imgA = mgr.add_Image(makeSplitImage("imgA"));
    mgr.set_Mirrored(imgA, true);
    QImage rendered=mgr.render(imgA);
   QColor colA=pixelAt(rendered,rendered.width()-1,0);
   QCOMPARE(colA.red(),255);
}

void test2::test_wrongId()
{
    imageManager mgr;
    int imgA = mgr.add_Image(makeSolidImage("imgA", QColor(255, 0, 0)));
   QImage wrongImg= mgr.get_ImageById(9999);
   QVERIFY(wrongImg.isNull());
   QImage wrongImg2 = mgr.get_ImageById(-22);
   QVERIFY(wrongImg2.isNull());
}

void test2::test_wrongParam()
{
    imageManager mgr;
    int imgA = mgr.add_Image(makeSolidImage("imgA", QColor(255, 0, 0)));
    mgr.set_Contrast(imgA,1145);
    mgr.set_lightness(imgA, -1145);
    mgr.set_rotation(imgA, 1145);
    imageManager::imgParams params = mgr.get_params(imgA);
    QVERIFY(params.contrast >= -100);
    QVERIFY(params.contrast <=100);
    QVERIFY(params.lightness >= -100);
    QVERIFY(params.lightness <= 100);
    QVERIFY(params.rotation >= -360);
    QVERIFY(params.rotation <= 360);
}


QTEST_MAIN(test2);
