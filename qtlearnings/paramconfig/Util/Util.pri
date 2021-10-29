INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

UTIL = Util
HEADERS +=\
          $$UTIL/DynamicLib.h \
          $$UTIL/ItemBase.h    \
          $$UTIL/UiCfgItem.h   \
          $$UTIL/PageCfg.h     \
          $$UTIL/devcfg.h

SOURCES +=\
          $$UTIL/DynamicLib.cpp \
          $$UTIL/ItemBase.cpp \
          $$UTIL/UiCfgItem.cpp   \
          $$UTIL/PageCfg.cpp     \
          $$UTIL/devcfg.cpp
