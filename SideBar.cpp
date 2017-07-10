#include "SideBar.h"
#include "SideBarButton.h"
#include "MainWindow.h"
#include "ImageContainer.h"

/**
 * The sidebar at the left of the main window. Contains the preview image icons
 *
 * @param parent window
 */
SideBar::SideBar(MainWindow *parent)
    : mainWindow(parent),
    contentWidget(new QWidget()),
    vLayout(new QVBoxLayout())
{
    setFixedWidth(200);
    vLayout->setAlignment(Qt::AlignTop);
    contentWidget->setLayout(vLayout);

    setWidget(contentWidget);
    setWidgetResizable(true);

}

/**
 * Add a image preview to the sidebar
 *
 * @param position of image. The images are sorted from brigthest to darkest.
 * An image with imageIndex 0 will be inserted at the top,
 * an image with index 1 will be appended under it and so on.
 */
void SideBar::addPreviewImage(int imageIndex){

    SideBarButton *button = new SideBarButton(mainWindow, imageIndex);
    connect(button, SIGNAL(showImage(QString)), mainWindow, SLOT(showImage(QString)));

    vLayout->insertWidget(imageIndex, button);

    // show at beginning the first image in ImageViewer
    if(mainWindow->getImageContainerList().size() == 1){
        button->imageClicked();
    }
}
