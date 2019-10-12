#
# Warthog Robotics
# University of Sao Paulo (USP) at Sao Carlos
# http://www.warthog.sc.usp.br/
#
# This file is part of WRBackbone project.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# Application info
TEMPLATE = app
DESTDIR  = bin
TARGET   = WRBackbone
VERSION  = 1.0.2
CONFIG += c++14

# Qt config
CONFIG += console
CONFIG -= app_bundle
QT -= gui

# Optimization flags
QMAKE_CFLAGS  -= -O
QMAKE_CFLAGS  -= -O1
QMAKE_CFLAGS  += -O2
QMAKE_CXXFLAGS  -= -O
QMAKE_CXXFLAGS  -= -O1
QMAKE_CXXFLAGS  += -O2

QMAKE_CXXFLAGS += -std=gnu++98
QMAKE_LFLAGS += -std=gnu++98

# Libraries definitions
LIBS *= -lomniORB4 -lGEARSystem -lomnithread

# Temporary dirs
OBJECTS_DIR = tmp/obj
MOC_DIR = tmp/moc
UI_DIR = tmp/moc
RCC_DIR = tmp/rc

# Source files
INCLUDEPATH *= . src
SOURCES += \
    src/main.cc

# Other files
DISTFILES += \
    pre-build.sh \
    LICENSE
