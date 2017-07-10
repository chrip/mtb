# -------------------------------------------------
# Project created by QtCreator 2010-02-01T21:06:37
# -------------------------------------------------
QT += widgets concurrent
TARGET = MTB
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    ImageViewer.cpp \
    MainWindow.cpp \
    TabMenu.cpp \
    MTB_Main.cpp \
    AddFile.cpp \
    ImageContainer.cpp \
    SideBar.cpp \
    SideBarButton.cpp \
    MTB_ImagePyramide.cpp \
    MTB_Comparation.cpp
HEADERS += ImageViewer.h \
    MainWindow.h \
    TabMenu.h \
    MTB_Main.h \
    #AddFileTread.h \
    AddFile.h \
    ImageContainer.h \
    SideBar.h \
    SideBarButton.h \
    MTB_ImagePyramide.h \
    MTB_Comparation.h
RESOURCES += resources.qrc
