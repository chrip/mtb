#ifndef MTB_MAIN_H
#define MTB_MAIN_H

#include <QObject>
class MainWindow;
class ImageContainer;

class MTB_Main : public QObject {
    Q_OBJECT
public:    
    MTB_Main(MainWindow *parent);

public slots:
    void startComputation();
    void startComparation(ImageContainer* index);

private:

MainWindow *mainWindow;


};

#endif // MTB_MAIN_H
