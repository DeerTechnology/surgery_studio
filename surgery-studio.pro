QT       += core gui network serialport serialbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
RC_ICONS = logo3.ico
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QTPLUGIN+=qlinuxfb

SOURCES += \
    appinit.cpp \
    canbus/electronic.cpp \
    canbus/handler_analysis.cpp \
    canbus/lightlz_analysis.cpp \
    canbus/syringepump_analysis.cpp \
    main.cpp \
    homepage.cpp \
    modules/needleproperty.cpp \
    modules/serialhandler.cpp \
    modules/taskhandler.cpp \
    pages/addtaskpage.cpp \
    pages/needleselection.cpp \
    pages/select_worker.cpp \
    quiwidget.cpp \
    canbus/canhandler.cpp \
    canbus/heating_analysis.cpp \
    ui_modules/custom_progressbar.cpp \
    ui_modules/cirlceprogressbar.cpp \
    ui_modules/custom_lineedit.cpp \
    ui_modules/custom_scrollarea.cpp \
    ui_modules/custom_switchbutton.cpp \
    ui_modules/customlabel.cpp \
    ui_modules/numKeyboard/numkeyboard.cpp \
    ui_modules/setting_version.cpp \
    ui_modules/surgery.cpp \
    ui_modules/syringe_pump.cpp \
    ui_modules/tablewidget_radius.cpp \
    ui_modules/warning.cpp

HEADERS += \
    appinit.h \
    canbus/device_analysis.h \
    canbus/electronic.h \
    canbus/handler_analysis.h \
    canbus/lightlz_analysis.h \
    canbus/syringepump_analysis.h \
    device_analysis1.h \
    head.h \
    homepage.h \
    main.h \
    modules/needleproperty.h \
    modules/serialhandler.h \
    modules/taskhandler.h \
    canbus/canhandler.h \
    canbus/heating_analysis.h \
    pages/addtaskpage.h \
    pages/needleselection.h \
    pages/select_worker.h \
    quiwidget.h \
    ui_modules/custom_progressbar.h \
    ui_modules/cirlceprogressbar.h \
    ui_modules/custom_lineedit.h \
    ui_modules/custom_scrollarea.h \
    ui_modules/custom_switchbutton.h \
    ui_modules/customlabel.h \
    ui_modules/numKeyboard/numkeyboard.h \
    ui_modules/setting_version.h \
    ui_modules/surgery.h \
    ui_modules/syringe_pump.h \
    ui_modules/tablewidget_radius.h \
    ui_modules/warning.h

FORMS += \
    homepage.ui \
    pages/addtaskpage.ui \
    pages/needleselection.ui \
    pages/select_worker.ui \
    ui_modules/cirlceprogressbar.ui \
    ui_modules/numKeyboard/numkeyboard.ui \
    ui_modules/setting_version.ui \
    ui_modules/surgery.ui \
    ui_modules/syringe_pump.ui \
    ui_modules/tablewidget_radius.ui \
    ui_modules/warning.ui

RESOURCES += \
    PadUI.qrc \
    qrcs/main.qrc \
    qrcs/qss.qrc \


#TRANSLATIONS += \
#    syringe-pump_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations
INCLUDEPATH += ui_modules \
               modules \
               ui_modules/numKeyboard \
               pages
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
