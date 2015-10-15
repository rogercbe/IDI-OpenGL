TEMPLATE    = app
QT         += opengl

LIBS += -lGLEW
INCLUDEPATH +=  /usr/include/glm

HEADERS += MyGLWidget.h

INCLUDEPATH += ./Model
SOURCES += ./Model/model.cpp

SOURCES += main.cpp \
        MyGLWidget.cpp
