#ifndef SIDEBARBUTTON_H
#define SIDEBARBUTTON_H

#include <QtGui>
#include <QToolButton>
class MainWindow;

class SideBarButton : public QToolButton {
    Q_OBJECT

public:
    SideBarButton(MainWindow *mainWindow, int imageIndex);

signals:
    void showImage(QString imagePath);
public slots:
    void imageClicked();

private:
    MainWindow *mainWindow;
    QString imagePath;
};

#endif // SIDEBARBUTTON_H
