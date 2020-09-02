QT       += core gui axcontainer gui-private network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adddata.cpp \
    changepassword.cpp \
    displaydata.cpp \
    exportdata.cpp \
    ipmanagement.cpp \
    main.cpp \
    mainwindowclient.cpp \
    querydata.cpp \
    queryresult.cpp \
    resident.cpp \
    saveexcel.cpp \
    ./QXlsx/xlsxabstractooxmlfile.cpp \
    ./QXlsx/xlsxabstractsheet.cpp \
    ./QXlsx/xlsxcell.cpp \
    ./QXlsx/xlsxcellformula.cpp \
    ./QXlsx/xlsxcellrange.cpp \
    ./QXlsx/xlsxcellreference.cpp \
    ./QXlsx/xlsxchart.cpp \
    ./QXlsx/xlsxchartsheet.cpp \
    ./QXlsx/xlsxcolor.cpp \
    ./QXlsx/xlsxconditionalformatting.cpp \
    ./QXlsx/xlsxcontenttypes.cpp \
    ./QXlsx/xlsxdatavalidation.cpp \
    ./QXlsx/xlsxdocpropsapp.cpp \
    ./QXlsx/xlsxdocpropscore.cpp \
    ./QXlsx/xlsxdocument.cpp \
    ./QXlsx/xlsxdrawing.cpp \
    ./QXlsx/xlsxdrawinganchor.cpp \
    ./QXlsx/xlsxformat.cpp \
    ./QXlsx/xlsxmediafile.cpp \
    ./QXlsx/xlsxnumformatparser.cpp \
    ./QXlsx/xlsxrelationships.cpp \
    ./QXlsx/xlsxrichstring.cpp \
    ./QXlsx/xlsxsharedstrings.cpp \
    ./QXlsx/xlsxsimpleooxmlfile.cpp \
    ./QXlsx/xlsxstyles.cpp \
    ./QXlsx/xlsxtheme.cpp \
    ./QXlsx/xlsxutility.cpp \
    ./QXlsx/xlsxworkbook.cpp \
    ./QXlsx/xlsxworksheet.cpp \
    ./QXlsx/xlsxzipreader.cpp \
    ./QXlsx/xlsxzipwriter.cpp \
    ./QXlsx/xlsxcelllocation.cpp \
    ./QXlsx/xlsxdatetype.cpp

HEADERS += \
    adddata.h \
    changepassword.h \
    displaydata.h \
    exportdata.h \
    ipmanagement.h \
    mainwindowclient.h \
    querydata.h \
    queryresult.h \
    resident.h \
    saveexcel.h \
    ./QXlsx/xlsxabstractooxmlfile.h \
    ./QXlsx/xlsxabstractooxmlfile_p.h \
    ./QXlsx/xlsxabstractsheet.h \
    ./QXlsx/xlsxabstractsheet_p.h \
    ./QXlsx/xlsxcell.h \
    ./QXlsx/xlsxcellformula.h \
    ./QXlsx/xlsxcellformula_p.h \
    ./QXlsx/xlsxcellrange.h \
    ./QXlsx/xlsxcellreference.h \
    ./QXlsx/xlsxcell_p.h \
    ./QXlsx/xlsxchart.h \
    ./QXlsx/xlsxchartsheet.h \
    ./QXlsx/xlsxchartsheet_p.h \
    ./QXlsx/xlsxchart_p.h \
    ./QXlsx/xlsxcolor_p.h \
    ./QXlsx/xlsxconditionalformatting.h \
    ./QXlsx/xlsxconditionalformatting_p.h \
    ./QXlsx/xlsxcontenttypes_p.h \
    ./QXlsx/xlsxdatavalidation.h \
    ./QXlsx/xlsxdatavalidation_p.h \
    ./QXlsx/xlsxdocpropsapp_p.h \
    ./QXlsx/xlsxdocpropscore_p.h \
    ./QXlsx/xlsxdocument.h \
    ./QXlsx/xlsxdocument_p.h \
    ./QXlsx/xlsxdrawinganchor_p.h \
    ./QXlsx/xlsxdrawing_p.h \
    ./QXlsx/xlsxformat.h \
    ./QXlsx/xlsxformat_p.h \
    ./QXlsx/xlsxglobal.h \
    ./QXlsx/xlsxmediafile_p.h \
    ./QXlsx/xlsxnumformatparser_p.h \
    ./QXlsx/xlsxrelationships_p.h \
    ./QXlsx/xlsxrichstring.h \
    ./QXlsx/xlsxrichstring_p.h \
    ./QXlsx/xlsxsharedstrings_p.h \
    ./QXlsx/xlsxsimpleooxmlfile_p.h \
    ./QXlsx/xlsxstyles_p.h \
    ./QXlsx/xlsxtheme_p.h \
    ./QXlsx/xlsxutility_p.h \
    ./QXlsx/xlsxworkbook.h \
    ./QXlsx/xlsxworkbook_p.h \
    ./QXlsx/xlsxworksheet.h \
    ./QXlsx/xlsxworksheet_p.h \
    ./QXlsx/xlsxzipreader_p.h \
    ./QXlsx/xlsxzipwriter_p.h \
    ./QXlsx/xlsxcelllocation.h \
    ./QXlsx/xlsxdatetype.h

FORMS += \
    adddata.ui \
    changepassword.ui \
    displaydata.ui \
    exportdata.ui \
    ipmanagement.ui \
    mainwindowclient.ui \
    querydata.ui \
    queryresult.ui \
    saveexcel.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
