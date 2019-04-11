#-------------------------------------------------
#
# Project created by QtCreator 2019-02-14T10:16:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    inspector.cpp \
    myopenglwidget.cpp \
    mysuperwidget.cpp \
    entity.cpp \
    hierarchy.cpp \
    componenttransform.cpp \
    component.cpp \
    shaperenderer.cpp \
    DarkStyle.cpp \
    mesh.cpp \
    resource.cpp \
    submesh.cpp

HEADERS += \
        mainwindow.h \
    inspector.h \
    myopenglwidget.h \
    mysuperwidget.h \
    entity.h \
    hierarchy.h \
    componenttransform.h \
    component.h \
    shaperenderer.h \
    DarkStyle.h \
    mesh.h \
    resource.h \
    vertexformat.h \
    submesh.h

FORMS += \
        mainwindow.ui \
    rendering.ui \
    inspector.ui \
    hierarchy.ui \
    componenttransform.ui \
    component.ui \
    shaperenderer.ui
LIBS += -lopengl32

RESOURCES += \
    resources.qrc
