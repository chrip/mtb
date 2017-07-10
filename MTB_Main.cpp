#include "MTB_Main.h"
#include "MTB_ImagePyramide.h"
#include "MTB_Comparation.h"
#include "MainWindow.h"
#include "ImageContainer.h"

/**
 * The Median Treshold Bitmap Algorithmus inspired by this papers:
 *
 * "Fast, Robust Image Registration for Compositing High Dynamic Range Photographs from Handheld Exposures" by Greg Ward
 *
 * "Fast and Robust High Dynamic Range Image Generation with Camera and Object Movement" by Thorsten Grosch
 * "Robuste Generierung von High Dynamic Range Bildern" by Andreas Fode
 * "Auswahl und Implementierung eines parallelen Verfahrens zur Ausrichtung von digitalen Fotografien unter Nutzung einer heterogenen Hardwarearchitektur" by Ömer Faruk Çar
 *
 * @param main window
 */
MTB_Main::MTB_Main(MainWindow *mw):
        mainWindow(mw) {
}

void MTB_Main::startComputation(){

    for(int iter = 0; iter < mainWindow->getImageContainerList().length(); iter++){
        MTB_ImagePyramide *pyramide = new MTB_ImagePyramide(mainWindow->getImageContainerList()[iter], mainWindow->getImageContainerList().length());

        //                //qRegisterMetaType<QImage>("QImage");
        //                connect(addFile, SIGNAL(newInputImageLoaded(ImageContainer*)),
        //                        mainWindow, SLOT(enlistNewImage(ImageContainer*)), Qt::QueuedConnection);
        //
        //                connect(addFile, SIGNAL(errorByLoadingImage(QString)),
        //                        mainWindow, SLOT(showInfoDialog(QString)), Qt::QueuedConnection);

        connect(pyramide, SIGNAL(updateProgressBarValue(double)),
                mainWindow, SLOT(updateProgressBarValue(double)), Qt::QueuedConnection);
        connect(pyramide, SIGNAL(startComparation(ImageContainer*)),
                this, SLOT(startComparation(ImageContainer*)), Qt::QueuedConnection);

        QThreadPool::globalInstance()->start(pyramide);
    }
}

void MTB_Main::startComparation(ImageContainer* img){

    int index = mainWindow->getImageContainerList().indexOf(img);

    if(index < mainWindow->getImageContainerList().length()-1 && mainWindow->getImageContainerList()[index+1]->isReadyForComparation()){

        MTB_Comparation *comp = new MTB_Comparation(img, mainWindow->getImageContainerList()[index+1]);
        connect(comp, SIGNAL(updateProgressBarValue(double)),
                mainWindow, SLOT(updateProgressBarValue(double)), Qt::QueuedConnection);
        QThreadPool::globalInstance()->start(comp);
    }

    if(index > 0 && mainWindow->getImageContainerList()[index-1]->isReadyForComparation()){

        MTB_Comparation *comp = new MTB_Comparation(mainWindow->getImageContainerList()[index-1], img);
        connect(comp, SIGNAL(updateProgressBarValue(double)),
                mainWindow, SLOT(updateProgressBarValue(double)), Qt::QueuedConnection);
        QThreadPool::globalInstance()->start(comp);
    }
}
