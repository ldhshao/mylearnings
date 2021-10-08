INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

UICOMMON = UiCommon
HEADERS +=\
          $$UICOMMON/cwidgetbutton.h \
          $$UICOMMON/pagecontainer.h \
          $$UICOMMON/uipage.h

SOURCES +=\
          $$UICOMMON/cwidgetbutton.cpp \
          $$UICOMMON/pagecontainer.cpp \
          $$UICOMMON/uipage.cpp

FORMS += \
          $$UICOMMON/pagecontainer.ui
