#ifndef IMAGECONTAINER_H
#define IMAGECONTAINER_H

#include <QVector>
#include <QImage>
#include <QDebug>

class ImageContainer {

public:
    ImageContainer(QImage *image, QVector<uchar> *grayImage, unsigned char median, unsigned char mean, QString imagePath, int maxScalingLevel);
    ~ImageContainer();
    const QImage &getOriginalImage() { return *image; }    

    int getImageWidth(){ return image->width(); }
    int getImageHeight(){ return image->height(); }
    int getMaxScalingLevel() { return maxScalingLevel; }
    uchar getMean() { return mean; }
    uchar getMedian() { return median; }
    QString getImagePath() { return imagePath; }

    const QVector<uchar> *getGrayImage(int level) { return grayImagePyramide.at(level); }
    const QVector<uchar> *getMonoImage(int level) { qDebug() << "get" << level << monoBmpPyramide.size(); return monoBmpPyramide.at(level); }
    const QVector<uchar> *getExclusionBitmap(int level) { return exclusionBmpPyramide.at(level); }
    void setGrayImage(int level, QVector<uchar> *grayImage) { grayImagePyramide[level] = grayImage; }
    void setMonoBitmap(int level, QVector<uchar> *monoBmp) { qDebug() << "set" << level << monoBmpPyramide.size(); monoBmpPyramide[level] = monoBmp; }
    void setExclusionBitmap(int level, QVector<uchar> *exclusionBmp) { exclusionBmpPyramide[level] = exclusionBmp; }
    void setReadyForComparation(bool status){ readyForComparation = status; }
    bool isReadyForComparation(){ return readyForComparation; }
    void deleteGrayImagePyramide();
    int getPositionOfFirstPixel(int level, int field, int offsetX, int offsetY);
    int getNextLevelXOffset(int xOffset);
    int getNextLevelYOffset(int yOffset, int level);
    int getBorderSize(int level);

private:    
    QImage *image;
    QVector<QVector<uchar> *> grayImagePyramide;
    QVector<QVector<uchar> *> monoBmpPyramide;
    QVector<QVector<uchar> *> exclusionBmpPyramide;
    uchar median;
    uchar mean;
    QString imagePath;
    bool readyForComparation;
    int maxScalingLevel;
};

#endif // IMAGECONTAINER_H
