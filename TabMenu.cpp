
#include "TabMenu.h"
#include "MainWindow.h"
#include "AddFile.h"
#include "MTB_Main.h"
#include <QHBoxLayout>
#include <QFileDialog>

class MainWindow;

/**
 * The tab menu on top
 *
 * @param parent window
 */
TabMenu::TabMenu(MainWindow *parent)
    : mainWindow(parent){

    // first time user clicked "open" open the image folder
    QDir::setCurrent(QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).last());

    QWidget *fileTab = createFileTab();
    addTab(fileTab, tr("File"));

    QWidget *viewTab = createViewTab();
    addTab(viewTab, tr("View"));

    QWidget *computeTab = createComputeTab();
    addTab(computeTab, tr("Compute"));

}

/**
 * Create the file tab and connect the actions
 */
QWidget *TabMenu::createFileTab(){


    QToolBar *fileToolBar = createToolBar();

    openAct = new QAction(QIcon(":/icons/Crystal_Clear_filesystem_folder_images.png"), tr("Open Images"), this);
    connect(openAct, SIGNAL(triggered()), SLOT(open()));
    openAct->setStatusTip(tr("Add new Images to project"));
    fileToolBar->addAction(openAct);

    saveAct = new QAction(QIcon(":/icons/Crystal_Clear_app_kfloppy.png"), tr("Save"), this);
    saveAct->setDisabled(true);
   //connect(saveAct, SIGNAL(clicked()), SLOT(open()));
    saveAct->setStatusTip(tr("Save a copy of all croped images"));
    fileToolBar->addAction(saveAct);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(fileToolBar);
    QWidget *fileTabPage = new QWidget();    
    fileTabPage->setLayout(layout);

    return fileTabPage;

}

/**
 * Create the view tab and connect the actions
 */
QWidget *TabMenu::createViewTab(){

    QToolBar *viewToolBar = createToolBar();

    previousAct = new QAction(QIcon(":/icons/Crystal_128_back.png"), tr("Previous"), this);
    connect(previousAct, SIGNAL(triggered()), SIGNAL(previousImage()));
    previousAct->setStatusTip(tr("Show previous image"));
    viewToolBar->addAction(previousAct);

    nextAct = new QAction(QIcon(":/icons/Crystal_128_forward.png"), tr("Next"), this);
    connect(nextAct, SIGNAL(triggered()), SIGNAL(nextImage()));
    nextAct->setStatusTip(tr("Show next image"));
    viewToolBar->addAction(nextAct);

    zoomInAct = new QAction(QIcon(":/icons/Crystal_Clear_action_viewmag+.png"), tr("Zoom In"), this);
    connect(zoomInAct, SIGNAL(triggered()), SIGNAL(zoomIn()));
    zoomInAct->setStatusTip(tr("Zoom preview in"));
    viewToolBar->addAction(zoomInAct);

    zoomOutAct = new QAction(QIcon(":/icons/Crystal_Clear_action_viewmag-.png"), tr("Zoom Out"), this);
    connect(zoomOutAct, SIGNAL(triggered()), SIGNAL(zoomOut()));
    zoomOutAct->setStatusTip(tr("Zoom preview out"));
    viewToolBar->addAction(zoomOutAct);

    zoomNormalAct = new QAction(QIcon(":/icons/Crystal_Clear_action_viewmag1.png"), tr("Normal"), this);
    connect(zoomNormalAct, SIGNAL(triggered()), SIGNAL(zoomNormal()));
    zoomNormalAct->setStatusTip(tr("Show preview 1:1"));
    viewToolBar->addAction(zoomNormalAct);

    zoomFitAct = new QAction(QIcon(":/icons/Crystal_Clear_action_viewmagfit.png"), tr("Fit"), this);
    connect(zoomFitAct, SIGNAL(triggered()), SIGNAL(zoomFit()));
    zoomFitAct->setStatusTip(tr("Fit preview to window"));
    viewToolBar->addAction(zoomFitAct);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(viewToolBar);
    QWidget *fileTabPage = new QWidget();
    fileTabPage->setLayout(layout);

    return fileTabPage;

}

/**
 * Create the compute tab and connect the actions
 */
QWidget *TabMenu::createComputeTab(){

    QToolBar *computeToolBar = createToolBar();

    QAction *computeAct = new QAction(QIcon(":/icons/Crystal_Clear_action_run.png"), tr("Compute"), this);

    computeAct->setStatusTip(tr("Start computation"));

    MTB_Main *mtb = new MTB_Main(mainWindow);
    connect(computeAct, SIGNAL(triggered()), mtb, SLOT(startComputation()));

    computeToolBar->addAction(computeAct);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(computeToolBar);
    QWidget *computeTabPage = new QWidget();
    computeTabPage->setLayout(layout);

    return computeTabPage;

}

/**
 * Handle a file open event by opening a QFileDialog.
 */
void TabMenu::open()
{
        QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"), QDir::currentPath(), "JPEG Format (*.JPEG *.jpeg *.JPG *.jpg)");

        if(!fileNames.isEmpty()){            
            QFileInfo info = QFileInfo(fileNames[0]);
            QDir::setCurrent(info.dir().path());
            addNewImages(fileNames);
        }    
}

/**
 * Start for each filepath in given list a own addFile thread.
 * @param StringList containing filepathes
 */
void TabMenu::addNewImages(const QStringList &fileNames){
    for (int i = 0; i < fileNames.size(); ++i) {

        AddFile *addFile = new AddFile(fileNames[i], fileNames.size());

        connect(addFile, SIGNAL(newInputImageLoaded(ImageContainer*)),
                mainWindow, SLOT(enlistNewImage(ImageContainer*)), Qt::QueuedConnection);

        connect(addFile, SIGNAL(errorByLoadingImage(QString)),
                mainWindow, SLOT(showInfoDialog(QString)), Qt::QueuedConnection);

        connect(addFile, SIGNAL(updateProgressBarValue(double)),
                mainWindow, SLOT(updateProgressBarValue(double)), Qt::QueuedConnection);

        QThreadPool::globalInstance()->start(addFile);
    }
}

QToolBar *TabMenu::createToolBar(){
    QToolBar *bar = new QToolBar(this);
    bar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    bar->setIconSize(QSize(100, 64));
    return bar;
}
