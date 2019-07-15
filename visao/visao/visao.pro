QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Ball/Ball.cpp \
    include/3rd_party/robocup_ssl_client.cpp \
    include/3rd_party/robocup_ssl_server.cpp \
    KalmanFilter/kalman.cpp \
    LossFilter/Loss.cpp \
    NoiseFilter/Noise.cpp \
    Robot/Robot.cpp \
    main.cpp \
    kalmanstate.cpp \
    matrix.cpp \
    position.cpp \
    velocity.cpp \
    object.cpp \
    types/object.cpp \
    types/position.cpp \
    types/velocity.cpp \
    main.cpp \
    types/angle.cpp


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SUBDIRS += \
    visao.pro \
    visao.pro

DISTFILES += \
    src/License \


HEADERS += \
    Ball/Ball.hpp \
    include/3rd_party/robocup_ssl_client.h \
    include/3rd_party/robocup_ssl_server.h \
    include/timer.h \
    KalmanFilter/kalman.hpp \
    LossFilter/Loss.hpp \
    NoiseFilter/Noise.hpp \
    Robot/Robot.hpp \
    include/filters.h \
    kalmanstate.h \
    matrix.h \
    position.h \
    velocity.h \
    object.h \
    types/object.h \
    types/position.h \
    types/velocity.h \
    types/angle.h \
    types/types.h

