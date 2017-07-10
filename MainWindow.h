#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>

class TabMenu;
class QProgressBar;
class ImageContainer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:    
    MainWindow();
    const QList<ImageContainer*> &getImageContainerList(){ return imageContainerList; }

signals:
    void showImage(int index);
    void addPreviewImage(int imageIndex);
public slots:
    void enlistNewImage(ImageContainer *i);
    void showInfoDialog(QString msg){ QMessageBox::information(this, tr(applicationName), msg); }
    void updateProgressBarValue(double val);
    void showImage(QString imagePath);

private:
    const char *applicationName;
    const int maxImages;
    QStatusBar *statusbar;
    QProgressBar *progressBar;
    double currentProgress;
    QList<ImageContainer*> imageContainerList;
};

#endif // MAINWINDOW_H
