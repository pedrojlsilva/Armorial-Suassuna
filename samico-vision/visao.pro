QT -= gui
QT += network
QT += widgets

CONFIG += c++14 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
LIBS *= -lprotobuf -lGLU -pthread -lsfml-graphics -lsfml-window -lsfml-system

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH *= include/3rd_party
SOURCES += \
    Ball/ball.cpp \
    KalmanFilter/kalman.cpp \
    KalmanFilter/kalmanstate.cpp \
    KalmanFilter/matrix.cpp \
    LossFilter/loss.cpp \
    NoiseFilter/noise.cpp \
    PathPlanning/path.cpp \
    grsSimulator/grsSimulator.cpp \
    Robot/robot.cpp \
    types/object.cpp \
    vision.cpp \
    include/3rd_party/messages_robocup_ssl_detection.pb.cc \
    include/3rd_party/netraw.cpp \
    include/3rd_party/robocup_ssl_client.cpp \
    include/3rd_party/messages_robocup_ssl_geometry.pb.cc \
    include/3rd_party/messages_robocup_ssl_refbox_log.pb.cc \
    include/3rd_party/messages_robocup_ssl_wrapper.pb.cc \
    include/3rd_party/grSim_Commands.pb.cc \
    include/3rd_party/grSim_Packet.pb.cc \
    include/3rd_party/grSim_Replacement.pb.cc \
    types/frame.cpp \
    samico.cpp \
    mrctimer.cpp \
    samico.cpp \
    entity/entity.cpp \
    entity/armorialvisionupdater/armorialvisionupdater.cpp \
    utils/fieldareas.cpp \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SUBDIRS += \
    visao.pro

DISTFILES += \
    src/License \
    include/3rd_party/protobuf/messages_robocup_ssl_detection.proto \
    include/3rd_party/protobuf/messages_robocup_ssl_refbox_log.proto \
    include/3rd_party/protobuf/messages_robocup_ssl_wrapper.proto \
    include/3rd_party/protobuf/messages_robocup_ssl_geometry.proto \
    include/3rd_party/protobuf/grSim_Commands.proto \
    include/3rd_party/protobuf/grSim_Packet.proto \
    include/3rd_party/protobuf/grSim_Replacement.proto \
    include/3rd_party/protobuf/protobuf.sh \
    utils/arial.ttf \
    utils/bluerobot.png \
    utils/yellowrobot.png \
    utils/arial.ttf



HEADERS += \
    Ball/ball.h \
    include/filters.h \
    include/timer.h \
    KalmanFilter/kalman.hpp \
    KalmanFilter/kalmanstate.h \
    KalmanFilter/matrix.h \
    PathPlanning/path.h \
    grsSimulator/grsSimulator.h \
    LossFilter/loss.h \
    NoiseFilter/noise.h \
    Robot/robot.h \
    include/3rd_party/netraw.h \
    include/3rd_party/robocup_ssl_client.h \
    include/3rd_party/util.h \
    include/filters.h \
    include/3rd_party/messages_robocup_ssl_detection.pb.h \
    include/3rd_party/messages_robocup_ssl_geometry.pb.h \
    include/3rd_party/messages_robocup_ssl_wrapper.pb.h \
    include/3rd_party/messages_robocup_ssl_refbox_log.pb.h \
    include/3rd_party/grSim_Commands.pb.h \
    include/3rd_party/grSim_Packet.pb.h \
    include/3rd_party/grSim_Replacement.pb.h \
    types/frame.h \
    samico.h \
    mrctimer.h \
    types/object.h \
    samico.h \
    entity/entity.h \
    entity/armorialvisionclient/armorialvisionclient.h \
    utils/fieldareas.h \
    utils/color.h

