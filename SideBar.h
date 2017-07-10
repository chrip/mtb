#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QtGui>
#include <QScrollArea>
#include <QVBoxLayout>

class MainWindow;

class SideBar : public QScrollArea {

    Q_OBJECT

public:

    SideBar(MainWindow *mw);

public slots:
    void addPreviewImage(int imageIndex);

private:
    MainWindow *mainWindow;
    QWidget *contentWidget;
    QVBoxLayout *vLayout;
};

#endif // SIDEBAR_H
