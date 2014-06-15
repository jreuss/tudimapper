#-------------------------------------------------
#
# Project created by QtCreator 2014-04-08T10:42:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tudimapper
TEMPLATE = app

CONFIG += precompile_header

SOURCES += main.cpp\
        mainwindow.cpp \
    compound_widgets/collider_widget/colliderscene.cpp \
    compound_widgets/collider_widget/collidertreegraphicsmodel.cpp \
    compound_widgets/collider_widget/colliders/boxcollider.cpp \
    compound_widgets/collider_widget/colliders/circlecollider.cpp \
    compound_widgets/collider_widget/colliders/collider.cpp \
    compound_widgets/collider_widget/colliders/meshcollider.cpp \
    compound_widgets/collider_widget/colliders/meshnode.cpp \
    items/abstracttreegraphicsitem.cpp \
    models/abstracttreegraphicsmodel.cpp \
    models/importtreemodel.cpp \
    utility/filemanager.cpp \
    utility/imgproc.cpp \
    compound_widgets/import_widget/importdialog.cpp \
    compound_widgets/collider_widget/colliderwidget.cpp \
    items/abstracttreeitem.cpp \
    models/abstracttreemodel.cpp \
    compound_widgets/sprite_widget/animatedsprite.cpp \
    compound_widgets/sprite_widget/animation.cpp \
    compound_widgets/sprite_widget/animationssequence.cpp \
    compound_widgets/sprite_widget/animationtreeitemmodel.cpp \
    compound_widgets/sprite_widget/graphicseditview.cpp \
    compound_widgets/sprite_widget/graphicsgriditem.cpp \
    compound_widgets/ruler_widget/graphicsviewrulerwidget.cpp \
    delegates/spinboxdelegate.cpp \
    compound_widgets/sprite_widget/spritewidget.cpp \
    items/itemtemplate.cpp \
    delegates/templatedelegate.cpp \
    compound_widgets/split_widget/splitwidget.cpp \
    compound_widgets/template_selection_widget/templatefolderwidget.cpp \
    compound_widgets/template_selection_widget/folderproxy.cpp \
    compound_widgets/template_selection_widget/thumbnailgridview.cpp \
    compound_widgets/template_selection_widget/templatemodel.cpp \
    items/abstracttreepixmapitem.cpp \
    models/abstracttreepixmapmodel.cpp \
    items/itemelement.cpp \
    mainscene.cpp \
    elementtreemodel.cpp \
    compound_widgets/collider_widget/colliderdialog.cpp \
    items/abstractsceneitem.cpp \
    models/abstracttreescenemodel.cpp \
    leveltreemodel.cpp \
    compound_widgets/split_widget/splitscene.cpp \
    compound_widgets/main_toolbar/maintoolbar.cpp \
    compound_widgets/layout_widget/layoutwidget.cpp \
    utility/zoomedgraphicview.cpp \
    compound_widgets/element_list_widget/elementlistwidget.cpp

HEADERS  += mainwindow.h \
    compound_widgets/collider_widget/colliderscene.h \
    compound_widgets/collider_widget/collidertreegraphicsmodel.h \
    compound_widgets/collider_widget/colliders/boxcollider.h \
    compound_widgets/collider_widget/colliders/circlecollider.h \
    compound_widgets/collider_widget/colliders/collider.h \
    compound_widgets/collider_widget/colliders/meshcollider.h \
    compound_widgets/collider_widget/colliders/meshnode.h \
    items/abstracttreegraphicsitem.h \
    models/abstracttreegraphicsmodel.h \
    models/importtreemodel.h \
    utility/filemanager.h \
    utility/imgproc.h \
    compound_widgets/import_widget/importdialog.h \
    compound_widgets/collider_widget/colliderwidget.h \
    items/abstracttreeitem.h \
    models/abstracttreemodel.h \
    compound_widgets/sprite_widget/animatedsprite.h \
    compound_widgets/sprite_widget/animation.h \
    compound_widgets/sprite_widget/animationssequence.h \
    compound_widgets/sprite_widget/graphicseditview.h \
    compound_widgets/sprite_widget/graphicsgriditem.h \
    compound_widgets/ruler_widget/graphicsviewrulerwidget.h \
    delegates/spinboxdelegate.h \
    compound_widgets/sprite_widget/spritewidget.h \
    compound_widgets/sprite_widget/animationtreeitemmodel.h \
    items/itemtemplate.h \
    delegates/templatedelegate.h \
    compound_widgets/split_widget/splitwidget.h \
    compound_widgets/template_selection_widget/templatefolderwidget.h \
    compound_widgets/template_selection_widget/folderproxy.h \
    compound_widgets/template_selection_widget/thumbnailgridview.h \
    compound_widgets/template_selection_widget/templatemodel.h \
    stable_pch.h \
    items/abstracttreepixmapitem.h \
    models/abstracttreepixmapmodel.h \
    items/itemelement.h \
    mainscene.h \
    elementtreemodel.h \
    compound_widgets/collider_widget/colliderdialog.h \
    items/abstractsceneitem.h \
    models/abstracttreescenemodel.h \
    leveltreemodel.h \
    compound_widgets/split_widget/splitscene.h \
    compound_widgets/main_toolbar/maintoolbar.h \
    compound_widgets/layout_widget/layoutwidget.h \
    utility/zoomedgraphicview.h \
    compound_widgets/element_list_widget/elementlistwidget.h

FORMS    += mainwindow.ui \
    compound_widgets/import_widget/importdialog.ui \
    compound_widgets/collider_widget/colliderwidget.ui \
    compound_widgets/sprite_widget/spritewidget.ui \
    compound_widgets/split_widget/splitwidget.ui \
    compound_widgets/template_selection_widget/templatefolderwidget.ui \
    compound_widgets/collider_widget/colliderdialog.ui \
    compound_widgets/main_toolbar/maintoolbar.ui \
    compound_widgets/layout_widget/layoutwidget.ui \
    compound_widgets/element_list_widget/elementlistwidget.ui

unix {
INCLUDEPATH += /usr/local/include

LIBS += -L/usr/local/lib \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_highgui \
    -lopencv_photo \
}

win32:RC_ICONS += tudi_ico.ico

win32 {
INCLUDEPATH += C:\opencv248\build\install\include
LIBS += C:\\opencv248\\build\\lib\\libopencv_core248.dll.a \
    C:\\opencv248\\build\\lib\\libopencv_highgui248.dll.a \
    C:\\opencv248\\build\\lib\\libopencv_imgproc248.dll.a \
    C:\\opencv248\\build\\lib\\libopencv_photo248.dll.a
}

PRECOMPILED_HEADER += stable_pch.h

RESOURCES += \
    images.qrc \
