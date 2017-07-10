#include "MTB_ImagePyramide.h"
#include "ImageContainer.h"
//#include <iostream>

MTB_ImagePyramide::MTB_ImagePyramide(ImageContainer* img, int totalNumberOfImages):
        imgContainer(img),
        totalNumberOfImages(totalNumberOfImages),
        currentLevel(0){   
}

void MTB_ImagePyramide::run(){
    // too few images
    if(totalNumberOfImages < 1){ // TODO change to 3 after testing
        emit updateProgressBarValue(100.0 / totalNumberOfImages);
        return;
    } else if(currentLevel < 2){
        emit updateProgressBarValue(5.0 / totalNumberOfImages); // add 5% to the progress
    }

    int width = imgContainer->getImageWidth() / pow(2, currentLevel);
    int height = imgContainer->getImageHeight() / pow(2, currentLevel);

    // iterator for even row
    QVector<uchar>::const_iterator grayImgEven = imgContainer->getGrayImage(currentLevel)->begin();
    // iterator for odd row
    QVector<uchar>::const_iterator grayImgOdd = imgContainer->getGrayImage(currentLevel)->begin() + width;

    int borderSize = imgContainer->getBorderSize(currentLevel);

    // binary bitmap with 1 pixel black border at scaling level 6 (smallest image):
    //
    //    0 00000000000000 0
    //    0 10101001101100 0
    //    0 01010100110110 0
    //    0 10101010011011 0
    //    0 00000000000000 0
    //
    // serialized in the vector we get this:
    //
    //    0 00000000000000 00 10101001101100 00 01010100110110 00 10101010011011 00 00000000000000 0
    //
    int imageSizeInclusiveBorder = (width + (2 * borderSize)) * (height + (2 * borderSize));
    int firstPixelOffset = ((((2 * borderSize) + width) * borderSize) + borderSize);
    QVector<uchar> *monoImg = new QVector<uchar>(imageSizeInclusiveBorder, 0);
    QVector<uchar>::iterator monoImgEven = monoImg->begin() + firstPixelOffset;
    QVector<uchar>::iterator monoImgOdd =  monoImg->begin() + firstPixelOffset + width + (2 * borderSize);

    // exclusion bitmap contains false for all pixels with a value +/-4 from median
    QVector<uchar> *exclusionBmp = new QVector<uchar>(imageSizeInclusiveBorder, 0);
    QVector<uchar>::iterator exclusionEven = exclusionBmp->begin() + firstPixelOffset;
    QVector<uchar>::iterator exclusionOdd = exclusionBmp->begin() + firstPixelOffset + width + (2 * borderSize);

    // boundaries for exclusion bitmap
    uchar lowerBound = (imgContainer->getMedian() <= 4)? 0 : (imgContainer->getMedian() - 4);
    uchar upperBound = (imgContainer->getMedian() >= 251)? 255 : (imgContainer->getMedian() + 4);

    // next smaller image 1/4 of the size of the actual image
    QVector<uchar> *smallImage = new QVector<uchar>((width / 2) * (height / 2));
    QVector<uchar>::iterator smallImageIter = smallImage->begin();

    int wPlusBorders = width + (4 * borderSize); // help variable to jump two times over the two pixels of the black border
    int wMinus1 = width - 1;
    int hMinus1 = height - 1;
    // iter 2 rows and 2 cols in one cycle
    for(int row = 0; row < hMinus1;
        row+=2,
        grayImgEven+=width,
        grayImgOdd+=width,
        monoImgEven+=wPlusBorders,
        monoImgOdd+=wPlusBorders,
        exclusionEven+=wPlusBorders,
        exclusionOdd+=wPlusBorders) {

        for(int col = 0; col < wMinus1;
            col+=2, grayImgEven++,
            grayImgOdd++,
            monoImgEven++,
            monoImgOdd++,
            exclusionEven++,
            exclusionOdd++) {

            // Compute allways a block of 4 pixel in one loop cycle
            //
            // Computation sequence of pixels in binary and exclusion bitmap
            //
            // 1.) pixel(x, y)
            // 2.) pixel(x+1, y)
            // 3.) pixel(x, y+1)
            // 4.) pixel(x+1, y+1)
            //
            // In the same cycle we compute a pixel of the next smaller image (the mean of the 4 pixel above).
            //

            // compute pixel in even row and even collumn
            if(*grayImgEven > upperBound){
                *monoImgEven = 1;
                *exclusionEven = 1;
            }
            else if(*grayImgEven < lowerBound){
                *exclusionEven = 1;
            }

            int smallImgGrayVal = *grayImgEven;

            // go in the same row one pixel to the right
            grayImgEven++;
            monoImgEven++;
            exclusionEven++;

            // compute pixel in even row and odd collumn
            if(*grayImgEven > upperBound){
                *monoImgEven = 1;
                *exclusionEven = 1;
            }
            else if(*grayImgEven < lowerBound){
                *exclusionEven = 1;
            }

            smallImgGrayVal += *grayImgEven;

            // compute pixel in odd row and even collumn
            if(*(grayImgOdd) > upperBound){
                *monoImgOdd = 1;
                *exclusionOdd = 1;
            }
            else if(*grayImgOdd < lowerBound){
                *exclusionOdd = 1;
            }

            smallImgGrayVal += *grayImgOdd;

            // go in the same row one pixel to the right
            grayImgOdd++;
            monoImgOdd++;
            exclusionOdd++;

            // compute pixel in odd row and odd collumn
            if(*grayImgOdd > upperBound){
                *monoImgOdd = 1;
                *exclusionOdd = 1;
            }
            else if(*grayImgOdd < lowerBound){
                *exclusionOdd = 1;
            }

            smallImgGrayVal += *grayImgOdd;

            // "smallImgGrayVal" is the sum of all 4 processed pixels. Divide it by 4 to get the mean.
            *smallImageIter = smallImgGrayVal >> 2; // bit shift: same as "smallImgGrayVal / 4
            smallImageIter++;
        }
    }

    imgContainer->setMonoBitmap(currentLevel, monoImg);
    imgContainer->setExclusionBitmap(currentLevel, exclusionBmp);

//    int hPlus2 = height + (2 * borderSize); // plus 2 times pixel for the border
//    int wPlus2 = width + (2 * borderSize); // plus 2 times pixel for the border
//    QImage mono = QImage(wPlus2, hPlus2, QImage::Format_ARGB32);
//    QVector<uchar>::const_iterator monoIter = monoImg->begin();
//    for(int row = 0; row < hPlus2; row++){
//        for(int col = 0; col < wPlus2; col++){
//            if(*monoIter){
//                mono.setPixel(col, row, qRgb(255,255,255));
//            }else{
//                mono.setPixel(col, row, qRgb(0,0,0));
//            }
//            monoIter++;
//        }
//    }
//
//    QTime t = QTime::currentTime();
//    mono.save("/home/schaef/median/median" + QString::number(currentLevel) + t.toString("_mm_ss_zzz") + ".jpg");

    // stop if max level reached or if next level image size is to small
    if(currentLevel < imgContainer->getMaxScalingLevel()){
        imgContainer->setGrayImage(currentLevel + 1, smallImage);        
        currentLevel++;
        run();
    }
    else{
        emit updateProgressBarValue(5.0 / totalNumberOfImages); // add 5% to the progress
        delete smallImage;
        imgContainer->deleteGrayImagePyramide();
        imgContainer->setReadyForComparation(true);

        // signal is connected with slot MTB_Main::startComparation(ImageContainer*)
        emit startComparation(imgContainer);
    }
    //    QImage gray = QImage(width, height, QImage::Format_ARGB32);
    //    QVector<uchar>::const_iterator grayImg = imgContainer->getGrayImage()->begin();
    //    for(int row = 0; row < height; row++){
    //        for(int col = 0; col < width; col++){
    //            gray.setPixel(col, row, qRgb(*grayImg,*grayImg,*grayImg));
    //            grayImg++;
    //        }
    //    }
    //    gray.save("/home/schaef/gray.jpg");


    //    QImage small = QImage(width/2, height/2, QImage::Format_ARGB32);
    //    QVector<uchar>::const_iterator smallIter = smallImage->begin();
    //    for(int row = 0; row < height/2; row++){
    //        for(int col = 0; col < width/2; col++){
    //
    //            small.setPixel(col, row, qRgb(*smallIter,*smallIter,*smallIter));
    //            smallIter++;
    //        }
    //    }
    //    small.save("/home/schaef/small.jpg");
}
