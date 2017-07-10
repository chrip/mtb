#include <QtGui>

#include "MainWindow.h"
#include "ImageViewer.h"
#include "ImageContainer.h"
#include <QtWidgets/QStatusBar>

/**
 * ImageViewer is a QGraphicsView in the center of the main window.
 * It shows the preview image. User can scroll in/out and drag and drop to see details of the preview image.
 *
 * @param parent widget
 */
ImageViewer::ImageViewer(MainWindow *parent)
    : mainWindow(parent),    
    graphicsScene(new QGraphicsScene()),
    myPixmap(0),
    currentImageIndex(-1) {

    
    graphicsScene->setBackgroundBrush(Qt::lightGray);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setScene(graphicsScene);


}
ImageViewer::~ImageViewer(){
    delete myPixmap;
    delete graphicsScene;
}


/**
 * Shows the image at given index position in the center QGraphicsView
 *
 * @param index of image in MainWindow::imageContainerList which will be shown.
 */
void ImageViewer::updatePixmap(int index){

    if(mainWindow->getImageContainerList().isEmpty()){
        return;
    }

    delete myPixmap;
    myPixmap = graphicsScene->addPixmap(QPixmap::fromImage(mainWindow->getImageContainerList()[index]->getOriginalImage()));
    if(currentImageIndex == -1){
        fitInView(myPixmap, Qt::KeepAspectRatio);
    }
    currentImageIndex = index;
    QString fac = QString::number(matrix().m11());
    mainWindow->statusBar()->showMessage(fac + "-times magnified");
}

/**
 * Show previous image in the center QGraphicsView
 *
 */
void ImageViewer::previousImage(){
    updatePixmap((mainWindow->getImageContainerList().length() + currentImageIndex - 1) % mainWindow->getImageContainerList().length());
}

/**
 * Shows next image in the center QGraphicsView
 *
 */
void ImageViewer::nextImage(){
    updatePixmap((currentImageIndex + 1) % mainWindow->getImageContainerList().length());
}

/**
 * Handel mouse wheel events to zoom in/out
 *
 */
void ImageViewer::wheelEvent(QWheelEvent *event){

    if(myPixmap){        

        if(event->delta() < 0) {
            zoomOut();
        }
        else if(event->delta() > 0){
            zoomIn();
        }
    }

}

/**
 * Magnify the preview image by manipulating the QGraphicsView matrix
 *
 */
void ImageViewer::zoomIn()
{
    double factor = matrix().m11();
    if((factor * 1.25) < 10){
        scale(1.25, 1.25);
    }
    QString fac = QString::number(matrix().m11());
    mainWindow->statusBar()->showMessage(fac + tr("-times magnified"));
}

/**
 * Downsize the preview image by manipulating the QGraphicsView matrix
 *
 */
void ImageViewer::zoomOut()
{
    double factor = matrix().m11();
    if((factor * 0.75) > 0.01){
        scale(0.8, 0.8);
    }
    QString fac = QString::number(matrix().m11());
    mainWindow->statusBar()->showMessage(fac + tr("-times magnified"));
}

/**
 * Show image 1:1 enlargement scale by reset the QGraphicsView matrix to the identity matrix
 *
 */
void ImageViewer::zoomNormal()
{
    resetMatrix();
    QString fac = QString::number(matrix().m11());
    mainWindow->statusBar()->showMessage(fac + tr("-times magnified"));
}

/**
 * Fit the preview image size to main window size
 *
 */
void ImageViewer::zoomFit()
{
    if(myPixmap && ((myPixmap->boundingRect().size().height() > size().height()) ||
                    (myPixmap->boundingRect().size().width() > size().width()))){
        fitInView(myPixmap, Qt::KeepAspectRatio);
    }
    QString fac = QString::number(matrix().m11());
    mainWindow->statusBar()->showMessage(fac + tr("-times magnified"));
}

