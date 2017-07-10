#ifndef ADDFILETREAD_H
#define ADDFILETREAD_H

#include <QObject>
#include <QRunnable>
class QImage;
class ImageContainer;

class AddFile : public QObject, public QRunnable {
    Q_OBJECT

public:
    AddFile(QString imagePath, int totalNumberOfImages);
    ~AddFile();
    void run();
    void displayError(QImage *image);

signals:
    void newInputImageLoaded(ImageContainer *img);
    void errorByLoadingImage(QString imagePath);
    void updateImageViewer();
    void updateProgressBarValue(double val);

private:
    QString imagePath;
    int totalNumberOfImages;
    int cumputeMaxScaleLevel(int width, int height);
};

#endif // ADDFILETREAD_H
