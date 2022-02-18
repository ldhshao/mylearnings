INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

UTIL = Util
HEADERS +=\
          $$PWD/datacfgitem.h \
          $$PWD/jsk_json.h \
          $$PWD/json.h \
          $$UTIL/DynamicLib.h \
          $$UTIL/ItemBase.h    \
          $$UTIL/UiCfgItem.h   \
          $$UTIL/PageCfg.h     \
          $$UTIL/logger.h     \
          $$UTIL/devcfg.h

SOURCES +=\
          $$PWD/datacfgitem.cpp \
          $$PWD/jsk_json.cpp \
          $$UTIL/DynamicLib.cpp \
          $$UTIL/ItemBase.cpp \
          $$UTIL/UiCfgItem.cpp   \
          $$UTIL/PageCfg.cpp     \
          $$UTIL/logger.cpp      \
          $$UTIL/devcfg.cpp
