#include "MTB_Comparation.h"
#include "ImageContainer.h"
#include <QtConcurrentRun>

MTB_Comparation::MTB_Comparation(ImageContainer *img1, ImageContainer *img2):
        img1(img1),
        img2(img2),
        offset(0),
        oneShift0(1),                   // 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000001
        oneShift8(256),                 // 00000000 00000000 00000000 00000000 00000000 00000000 00000001 00000000
        oneShift16(65536),              // 00000000 00000000 00000000 00000000 00000000 00000001 00000000 00000000
        oneShift24(16777216),           // 00000000 00000000 00000000 00000000 00000001 00000000 00000000 00000000
        oneShift32(4294967296),         // 00000000 00000000 00000000 00000001 00000000 00000000 00000000 00000000
        oneShift40(1099511627776),      // 00000000 00000000 00000001 00000000 00000000 00000000 00000000 00000000
        oneShift48(281474976710656),    // 00000000 00000001 00000000 00000000 00000000 00000000 00000000 00000000
        oneShift56(72057594037927936)   // 00000001 00000000 00000000 00000000 00000000 00000000 00000000 00000000
{
    //    oneShift0 = 1;
    //    oneShift8 = oneShift0 << 8;
    //    oneShift16 = oneShift0 << 16;
    //    oneShift24 = oneShift0 << 24;
    //    oneShift32 = oneShift0 << 32;
    //    oneShift40 = oneShift0 << 40;
    //    oneShift48 = oneShift0 << 48;
    //    oneShift56 = oneShift0 << 56;
}


void MTB_Comparation::run(){

    int currentXOffset = 0;
    int currentYOffset = 0;

    for(int level = img1->getMaxScalingLevel(); level >= 0; level--){

        currentXOffset = img1->getNextLevelXOffset(currentXOffset);
        currentYOffset = img1->getNextLevelYOffset(currentYOffset, level);

        int positionOfFirstPixel0 = img1->getPositionOfFirstPixel(level, 0, currentXOffset, currentYOffset);

        //QFuture<int> future0 = QtConcurrent::run(this, &MTB_Comparation::computeDifference, level, positionOfFirstPixel0);
        computeDifference(level, positionOfFirstPixel0);

    }

}

int MTB_Comparation::computeDifference(int level, int offset){


    const QVector<uchar> *mono1 = img1->getMonoImage(level);
    const QVector<uchar> *exclusion1 = img1->getExclusionBitmap(level);

    const QVector<uchar> *mono2 = img2->getMonoImage(level);
    const QVector<uchar> *exclusion2 = img2->getExclusionBitmap(level);

    QVector<uchar>::const_iterator monoBoolIter1 = mono1->begin();
    QVector<uchar>::const_iterator monoBoolIter2 = mono2->begin() + offset;

    // char is a 8 bit datatype
    // 8 * 8 bit = 64 bit
    // by reinterpret_cast the char vector to an int64 we can compute 8 chars in one step
    QVector<quint64>::const_iterator monoIntIter1 = reinterpret_cast<QVector<quint64>::const_iterator>(monoBoolIter1);
    QVector<quint64>::const_iterator monoIntIter2 = reinterpret_cast<QVector<quint64>::const_iterator>(monoBoolIter2);

    QVector<uchar>::const_iterator exclusionBoolIter1 = exclusion1->begin();
    QVector<uchar>::const_iterator exclusionBoolIter2 = exclusion2->begin() + offset;

    QVector<quint64>::const_iterator exclusionIntIter1 = reinterpret_cast<QVector<quint64>::const_iterator>(exclusionBoolIter1);
    QVector<quint64>::const_iterator exclusionIntIter2 = reinterpret_cast<QVector<quint64>::const_iterator>(exclusionBoolIter2);

    quint64 sizeBool = mono1->size();
    QVector<quint64>::const_iterator end = monoIntIter1 + ((sizeBool / 8) - 8);

    int sum = 0;

    QTime startTime = QTime::currentTime();

    for(; monoIntIter1 < end; monoIntIter1++, monoIntIter2++, *exclusionIntIter1++, *exclusionIntIter2++){

        quint64 res = (((*monoIntIter1 ^ *monoIntIter2) & *exclusionIntIter1) & *exclusionIntIter2);
        if(res){

            sum += !!(res & oneShift0);
            sum += !!(res & oneShift8);
            sum += !!(res & oneShift16);
            sum += !!(res & oneShift24);
            sum += !!(res & oneShift32);
            sum += !!(res & oneShift40);
            sum += !!(res & oneShift48);
            sum += !!(res & oneShift56);

        }

    }
    QTime endTime = QTime::currentTime();
    qDebug() << "Summe: " << sum << startTime.msecsTo(endTime);
    return sum;
}
