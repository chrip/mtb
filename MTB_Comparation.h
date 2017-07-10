#ifndef MTB_COMPARATION_H
#define MTB_COMPARATION_H

#include <QtGui>
class ImageContainer;

class MTB_Comparation : public QObject, public QRunnable {
    Q_OBJECT

public:
    MTB_Comparation(ImageContainer *img1, ImageContainer *img2);
    void run();
private:
    int computeDifference(int level, int positionOfFirstPixel);
    ImageContainer *img1;
    ImageContainer *img2;
    int offset;
    const quint64 oneShift0;
    const quint64 oneShift8;
    const quint64 oneShift16;
    const quint64 oneShift24;
    const quint64 oneShift32;
    const quint64 oneShift40;
    const quint64 oneShift48;
    const quint64 oneShift56;
signals:
    void updateProgressBarValue(double val);
};

#endif // MTB_COMPARATION_H
