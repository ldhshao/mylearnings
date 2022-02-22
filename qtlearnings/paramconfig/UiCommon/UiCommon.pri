INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

UICOMMON = UiCommon
HEADERS +=\
          $$PWD/ckeybutton.h \
          $$PWD/ckeydateedit.h \
          $$PWD/ckeylabel.h \
          $$PWD/ckeytablewidget.h \
          $$PWD/cmymessagebox.h \
          $$UICOMMON/cwidgetbutton.h \
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
          $$PWD/ckeybutton.cpp \
          $$PWD/ckeydateedit.cpp \
          $$PWD/ckeylabel.cpp \
          $$PWD/ckeytablewidget.cpp \
          $$PWD/cmymessagebox.cpp \
          $$UICOMMON/cwidgetbutton.cpp \
          $$UICOMMON/uipage.cpp        \
          $$UICOMMON/qkeytools.cpp     \
          $$UICOMMON/uistatectl.cpp    \
          $$UICOMMON/bindobj.cpp       \
          $$UICOMMON/cbinder.cpp       \
          $$UICOMMON/ckeydnedit.cpp    \
          $$UICOMMON/ckeydncombobox.cpp\
          $$UICOMMON/cenablemngr.cpp

FORMS += \
          $$UICOMMON/qkeytools.ui
