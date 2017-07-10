#ifndef TABMENU_H
#define TABMENU_H

#include <QtGui>
#include <QTabWidget>
#include <QToolBar>

class MainWindow;


class TabMenu : public QTabWidget {

    Q_OBJECT

public:
    TabMenu(MainWindow *mainWindow);
    void addNewImages(const QStringList &fileNames);

public slots:
    void open();

signals:
    void previousImage();
    void nextImage();
    void zoomIn();
    void zoomOut();
    void zoomNormal();
    void zoomFit();

private:

    QToolBar *createToolBar();
    QWidget *createFileTab();
    QWidget *createComputeTab();
    QWidget *createViewTab();
    MainWindow *mainWindow;

    QAction *openAct;
    QAction *saveAct;

    QAction *previousAct;
    QAction *nextAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *zoomNormalAct;
    QAction *zoomFitAct;

    QAction *computeAct;


};


#endif // TABMENU_H
