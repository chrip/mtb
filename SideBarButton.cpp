#include "SideBarButton.h"
#include "MainWindow.h"
#include "ImageContainer.h"

/**
 * A SideBarButton is a QButton which shows a icon of the associated image and the image path.
 * By clicking on this button the associated image will be shown in the center ImageViewer
 *
 * @param main window
 * @param index to find the right image in the MainWindow::imageContainerList
 */
SideBarButton::SideBarButton(MainWindow *mainWindow, int imageIndex)
    : mainWindow(mainWindow) {

    this->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    this->imagePath = mainWindow->getImageContainerList()[imageIndex]->getImagePath();

    int pathLength = imagePath.length();
    int slashPos = imagePath.lastIndexOf(QDir::separator()); // TODO test this under Windows and Mac
    QString shortImagePath(this->imagePath);

    // if file name is to long then crop it
    if((pathLength - slashPos) > 15){
        shortImagePath.remove(0, pathLength - 10);
        shortImagePath.push_front("...");
    }
    else{
        shortImagePath.remove(0, slashPos + 1);
    }

    this->setText(shortImagePath);
    QIcon icon(this->imagePath);
    this->setIcon(icon);
    QSize iconSize(icon.actualSize(QSize(150,150)));
    this->setIconSize(QSize(150, iconSize.height()));

    connect(this, SIGNAL(clicked()), SLOT(imageClicked()));

}

void SideBarButton::imageClicked(){
    // signal is connected with slot MainWindow::showImage(QString)
    emit showImage(imagePath);
}
