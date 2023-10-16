QT += core gui opengl openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

QMAKE_CXXFLAGS += -Wall -Werror -Wextra

INCLUDEPATH += /usr/include/GL/

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

macx: {
    ICON = images/icon.icns
    LIBS += -framework OpenGL
}

unix:!macx:!android: {
    LIBS += -lGL -lGLU
}

SOURCES += \
    ../backend/3d_parser.c \
    main.cpp \
    mainwindow.cpp \
    myglwidget.cpp

HEADERS += \
    ../backend/3d_parser.h \
    gif.h \
    mainwindow.h \
    myglwidget.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resourses.qrc

DISTFILES +=
