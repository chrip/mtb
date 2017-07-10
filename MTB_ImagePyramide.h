#ifndef MTB_IMAGEPYRAMIDE_H
#define MTB_IMAGEPYRAMIDE_H

#include <QtGui>
class ImageContainer;

class MTB_ImagePyramide : public QObject, public QRunnable {
    Q_OBJECT
public:
    MTB_ImagePyramide(ImageContainer* img, int totalNumberOfImages);
    void run();
signals:
    void updateProgressBarValue(double val);
    void startComparation(ImageContainer* img);

private:
    ImageContainer *imgContainer;
    int totalNumberOfImages;
    int currentLevel;
};

#endif // MTB_IMAGEPYRAMIDE_H
