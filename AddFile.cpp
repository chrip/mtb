#include "AddFile.h"
#include <QImage>
#include <QDebug>
#include <QDateTime>
#include "math.h"
#include "ImageContainer.h"

AddFile::AddFile(QString imagePath, int totalNumberOfImages)
    :imagePath(imagePath),
    totalNumberOfImages(totalNumberOfImages){

}
AddFile::~AddFile(){

}

/**
 * AddFile runs in a own thread and computes and adds a new ImageContainer.
 * An ImageContainer contains the image, the mean, the median, the full image file path
 * and a gray image of the original image.
 *
 */
void AddFile::run(){

    QImage *image = new QImage(imagePath, "JPEG");
    if (image->isNull() && !(image->format() == QImage::Format_RGB32 || image->format() == QImage::Format_ARGB32)) {
        displayError(image);
        return;
    }
    else{
        int width = image->width();
        int height = image->height();

        int maxScaleLevel = cumputeMaxScaleLevel(width, height);
        if(maxScaleLevel < 0) {
            displayError(image);
            return;
        }

        QVector<int> histogram = QVector<int>(256, 0);
        quint64 total = 0; // this value could be very high -> int64
        emit updateProgressBarValue(20.0/totalNumberOfImages); // emit a 20% progress for this thread
        QTime startTime = QTime::currentTime();

        QVector<uchar> *grayImage = new QVector<uchar>(width * height, 0);
        QVector<uchar>::iterator grayImgIter = grayImage->begin();

        for(int row = 0; row < height; row++) {
            QRgb *pixIter = reinterpret_cast<QRgb*>(image->scanLine(row)); // cast to QRgb (0xffRRGGBB) because different plattforms have different byte orders
            for(int col = 0; col < width; col++, pixIter++, grayImgIter++) {

                int red = qRed(*pixIter);
                int green = qGreen(*pixIter);
                int blue = qBlue(*pixIter);

                uchar gray = (54*red + 183*green + 19*blue) >> 8; // >> (bit shift) 8 is equal to divide by 256

                *grayImgIter = gray;

                histogram[gray]++;
                total += gray; // intermediate step for easier computation of mean value
            }
        }

        emit updateProgressBarValue(70.0/totalNumberOfImages); // add 70% progress for this thread (20% + 70% = 90%)
        QTime endTime = QTime::currentTime();
        // compute the mean value of the image
        uint pixels = width * height;
        uchar mean = total / pixels;

        // compute the median
        uchar median = 0;
        uint halfPixels = pixels/2;
        total = 0;
        for (int i = 255; i > 0; i--){
            total += histogram.at(i);
            if(total >= halfPixels){
                median = i; // median was found
                break;
            }
        }
        emit updateProgressBarValue(10.0/totalNumberOfImages); // add 10% progress for this thread (20% + 70% + 10% = 100%)


        qDebug() << imagePath << "start: " << startTime << "end: " << endTime << "total: " << startTime.msecsTo(endTime);
        qDebug() << "  mean: " << mean;
        qDebug() << "  median: " << median;

        // signal is connected with slot MainWindow::enlistNewImage(ImageContainer*)
        emit newInputImageLoaded(new ImageContainer(image, grayImage, median, mean, imagePath, maxScaleLevel));
    }
}

void AddFile::displayError(QImage *image){
    // signal is connected with slot MainWindow::updateProgressBarValue(double)
    emit updateProgressBarValue(100.0/totalNumberOfImages); // emit a 100% progress for this thread, so the overall progress can still reach 100%

    // signal is connected with slot MainWindow::showInfoDialog(QString)
    emit errorByLoadingImage(tr("Can not open file %1").arg(imagePath));

    delete image;
}

int AddFile::cumputeMaxScaleLevel(int width, int height){

    // Compute the height of the image pyramide.
    // The higher the pyramid, the further the images can be shifted.

    // Maximum height of image pyramide is 6
    // 2⁰ = 1; // level 0: original size
    // 2¹ = 2; // level 1: half size
    // 2² = 4;
    // ...
    // 2⁶ = 64; // level 6: original size / 64
    // That means that two images can be shifted up to 64 pixels.

    // Shortest edge of smallest image (highest image in the pyramide) should be 66 pixel (64 + 1 pixel for upper and lower border)

    int maxScaleLevel = 0;
    while(true){
        if(!((maxScaleLevel < 6) && ((height / pow(2, maxScaleLevel)) > 66) && ((width / pow(2, maxScaleLevel)) > 66))){
            return maxScaleLevel--;
        }
        maxScaleLevel++;
    }

}
