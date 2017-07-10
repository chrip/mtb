#include <QtGui>
#include "MainWindow.h"
#include "MTB_Main.h"
#include "TabMenu.h"
#include "ImageViewer.h"
#include "ImageContainer.h"
#include "SideBar.h"
#include <QStatusBar>
#include <QProgressBar>

/**
 * Build the main window with tabnav on top, sidebar on the left and image viewer
 * in the center.
 *
 */
MainWindow::MainWindow()
    : applicationName("Median Treshold Bitmap"),
    maxImages(10),
    currentProgress(0){

    // create tab menu
    TabMenu *tabMenu = new TabMenu(this);

    // create main image view
    SideBar* sideBar = new SideBar(this);

    // create main image view
    ImageViewer* imageViewer = new ImageViewer(this);

    // create vertical layout
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setAlignment(Qt::AlignTop);
    // add tabMenu and imageViewer
    vLayout->addWidget(tabMenu);

    // create horizontal layout
    QHBoxLayout *hLayout = new QHBoxLayout();

    hLayout->addWidget(sideBar);
    hLayout->addWidget(imageViewer, 1);
    vLayout->addLayout(hLayout, 1);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(vLayout);

    setCentralWidget(centralWidget);
    setWindowTitle(tr(applicationName));
    setWindowIcon(QIcon(":/icons/CS-Icon.svg"));
    statusbar = new QStatusBar(this);
    progressBar = new QProgressBar(statusbar);
    progressBar->setFixedSize(200, 18);
    progressBar->hide();
    statusbar->addPermanentWidget(progressBar);
    setStatusBar(statusbar);

    connect(this, SIGNAL(showImage(int)),
            imageViewer, SLOT(updatePixmap(int)));

    connect(this, SIGNAL(addPreviewImage(int)),
            sideBar, SLOT(addPreviewImage(int)));

    connect(tabMenu, SIGNAL(previousImage()),
            imageViewer, SLOT(previousImage()));

    connect(tabMenu, SIGNAL(nextImage()),
            imageViewer, SLOT(nextImage()));

    connect(tabMenu, SIGNAL(zoomIn()),
            imageViewer, SLOT(zoomIn()));

    connect(tabMenu, SIGNAL(zoomOut()),
            imageViewer, SLOT(zoomOut()));

    connect(tabMenu, SIGNAL(zoomNormal()),
            imageViewer, SLOT(zoomNormal()));

    connect(tabMenu, SIGNAL(zoomFit()),
            imageViewer, SLOT(zoomFit()));

    resize(800, 600);

    // begin tests please delete this lines
    QStringList testData;
    //testData.append("/home/schaef/RGB.png");
    testData.append("/home/schaef/DSC_2132.JPG");
    testData.append("/home/schaef/DSC_2133.JPG");
    testData.append("/home/schaef/DSC_2134.JPG");
    tabMenu->addNewImages(testData);    
    // end test
}

/**
 * Enlist given image in MainWindow::imageContainerList
 * The list is sorted from darkest to brightest image
 *
 * @param image to enlist
 */
void MainWindow::enlistNewImage(ImageContainer *img){

    if (imageContainerList.length() < maxImages){
        for(int pos = 0; pos <= imageContainerList.length(); pos++){
            if(imageContainerList.isEmpty() || imageContainerList.length() == pos || img->getMean() <= imageContainerList[pos]->getMean()){
                imageContainerList.insert(pos, img);
                // signal is connected with slot SideBar::addPreviewImage(int)
                emit addPreviewImage(pos);
                return;
            }
        }        
    }
    else{
        delete img;
    }
}

/**
 * Search the image with the given image path an emit a show event with position of the image
 *
 * @param full file path of image
 */
void MainWindow::showImage(QString imagePath){
    for(int pos = 0; pos <= imageContainerList.length(); pos++){
        if(imageContainerList[pos]->getImagePath() == imagePath){
            // signal is connected with slot ImageViewer::updatePixmap(int)
            emit showImage(pos);
            return;
        }
    }

}

/**
 * Shows a progressbar at the left bottom of main window.
 * Hides the progressbar if progress is greater than 99
 *
 * @param Value to be add to the current value of progress
 */
void MainWindow::updateProgressBarValue(double val){

    if(currentProgress + val > 99.0){
        progressBar->hide();
        currentProgress = 0;
        return;
    }
    if(!progressBar->isVisible()){
        progressBar->show();
    }

    currentProgress += val;
    this->progressBar->setValue(currentProgress);
}

