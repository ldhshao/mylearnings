INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

UICOMMON = UiCommon
HEADERS +=\
          $$UICOMMON/cwidgetbutton.h \
          $$UICOMMON/pagecontainer.h \
          $$UICOMMON/uipage.h        \
          $$UICOMMON/qkeytools_global.h \
          $$UICOMMON/qkeytools.h        \
          $$UICOMMON/uistatectl.h       \
          $$UICOMMON/ibindobj.h         \
          $$UICOMMON/bindobj.h          \
          $$UICOMMON/cbinder.h          \
          $$UICOMMON/ckeydnedit.h       \
          $$UICOMMON/ckeydncombobox.h   \
          $$UICOMMON/cenablemngr.h

SOURCES +=\
          $$UICOMMON/cwidgetbutton.cpp \
          $$UICOMMON/pagecontainer.cpp \
          $$UICOMMON/uipage.cpp        \
          $$UICOMMON/qkeytools.cpp     \
          $$UICOMMON/uistatectl.cpp    \
          $$UICOMMON/bindobj.cpp       \
          $$UICOMMON/cbinder.cpp       \
          $$UICOMMON/ckeydnedit.cpp    \
          $$UICOMMON/ckeydncombobox.cpp\
          $$UICOMMON/cenablemngr.cpp

FORMS += \
          $$UICOMMON/pagecontainer.ui \
          $$UICOMMON/qkeytools.ui
