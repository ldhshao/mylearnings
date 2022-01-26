INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

UTIL = Util
HEADERS +=\
          $$UTIL/DynamicLib.h \
          $$UTIL/ItemBase.h \
          $$UTIL/UiItem.h   \
          $$UTIL/Page.h     \
          $$UTIL/devcfg.h

SOURCES +=\
          $$UTIL/DynamicLib.cpp \
          $$UTIL/ItemBase.cpp \
          $$UTIL/UiItem.cpp   \
          $$UTIL/Page.cpp     \
          $$UTIL/devcfg.cpp
