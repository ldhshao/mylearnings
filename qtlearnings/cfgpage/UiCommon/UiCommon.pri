INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

UICOMMON = UiCommon
HEADERS +=\
          $$UICOMMON/cwidgetbutton.h \
          $$UICOMMON/pagecontainer.h \
          $$UICOMMON/uipage.h        \
          $$UICOMMON/qkeytools_global.h \
          $$UICOMMON/qkeytools.h        \
          $$UICOMMON/uistatectl.h

SOURCES +=\
          $$UICOMMON/cwidgetbutton.cpp \
          $$UICOMMON/pagecontainer.cpp \
          $$UICOMMON/uipage.cpp        \
          $$UICOMMON/qkeytools.cpp     \
          $$UICOMMON/uistatectl.cpp

FORMS += \
          $$UICOMMON/pagecontainer.ui \
          $$UICOMMON/qkeytools.ui
