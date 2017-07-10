#include "ImageContainer.h"
#include "math.h"

/**
 * A container class to handle all necessary properties of a image
 *
 * @param original image
 * @param gray image
 * @param median
 * @param mean value
 * @param full file path of original image
 *
 */
ImageContainer::ImageContainer(QImage *image, QVector<uchar> *grayImage, uchar median, uchar mean, QString imagePath, int maxScalingLevel):
        image(image),
        grayImagePyramide(7, 0), // 7 = 1 original image + 6 smaller images
        monoBmpPyramide(7, 0),
        exclusionBmpPyramide(7, 0),
        median(median),
        mean(mean),
        imagePath(imagePath),
        readyForComparation(false),
        maxScalingLevel(maxScalingLevel){

    grayImagePyramide[0] = grayImage;

}

void ImageContainer::deleteGrayImagePyramide(){
    for(int i = 0; i < grayImagePyramide.size(); i++){
        delete grayImagePyramide[i];
    }
}

ImageContainer::~ImageContainer(){
    delete image;
    for(int i = 0; i < grayImagePyramide.size(); i++){
        delete monoBmpPyramide[i];
        delete exclusionBmpPyramide[i];
    }
}

int ImageContainer::getBorderSize(int level){
    return pow(2, this->getMaxScalingLevel() - level);
}


int ImageContainer::getNextLevelXOffset(int xOffset){
    int nextLevelXOffset = 0;
    if (xOffset){
        nextLevelXOffset = (2 * xOffset) + ((xOffset < 0) ? 1 : -1);
    }
    return nextLevelXOffset;
}

int ImageContainer::getNextLevelYOffset(int yOffset, int level){

    int borderSize = this->getBorderSize(level);
    int width = this->getImageWidth() / pow(2, level);
    int dy = (2 * borderSize) + width;

    int nextLevelYOffset = 0;
    if (yOffset){
        nextLevelYOffset = (borderSize * yOffset) + ((yOffset < 0) ? dy : -dy);
    }
    return nextLevelYOffset;
}

int ImageContainer::getPositionOfFirstPixel(int level, int field, int xOffset, int yOffset){

    //
    // field coordinates:
    //
    // (8)-(1)-(2)
    // (7)-(0)-(3)
    // (6)-(5)-(4)
    //
    int borderSize = this->getBorderSize(level);
    int width = this->getImageWidth() / pow(2, level);
    int dy = (2 * borderSize) + width;
    int positionOfFirstPixel = ((dy * borderSize) + borderSize);

    switch(field){
    case 0:
        return positionOfFirstPixel + xOffset + yOffset;
    case 1:
        return  positionOfFirstPixel + xOffset + yOffset - dy;
    case 2:
        return  positionOfFirstPixel + xOffset + yOffset + 1 - dy;
    case 3:
        return  positionOfFirstPixel + xOffset + yOffset + 1;
    case 4:
        return  positionOfFirstPixel + xOffset + yOffset + 1 + dy;
    case 5:
        return  positionOfFirstPixel + xOffset + yOffset + dy;
    case 6:
        return  positionOfFirstPixel + xOffset + yOffset -1 + dy;
    case 7:
        return  positionOfFirstPixel + xOffset + yOffset - 1;
    case 8:
        return  positionOfFirstPixel + xOffset + yOffset - 1 - dy;
    default: return 0;
    }
}
