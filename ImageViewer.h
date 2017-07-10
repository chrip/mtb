#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H
#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QShortcut>

class MainWindow;
class QLabel;

class ImageViewer : public QGraphicsView {
    Q_OBJECT

public:
    ImageViewer(MainWindow *parent);
    ~ImageViewer();

protected:
    void wheelEvent(QWheelEvent *event);


private slots:

    void updatePixmap(int index);

    void previousImage();
    void nextImage();
    void zoomIn();
    void zoomOut();
    void zoomNormal();
    void zoomFit();

private:
    MainWindow *mainWindow;

    QGraphicsScene *graphicsScene;
    QGraphicsPixmapItem * myPixmap;

    QShortcut *zoomInShortcut;
    QShortcut *zoomOutShortcut;

    int currentImageIndex;
};

#endif // IMAGEVIEWER_H
