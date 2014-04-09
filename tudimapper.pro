#-------------------------------------------------
#
# Project created by QtCreator 2014-04-08T10:42:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tudimapper
TEMPLATE = app

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
    compound_widgets/template_selection_widget/thumbnailgridview.cpp \
    compound_widgets/template_selection_widget/templateviewwidget.cpp \
    compound_widgets/template_selection_widget/templatefolderviewmodel.cpp \
    compound_widgets/template_selection_widget/folderitem.cpp

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
    compound_widgets/template_selection_widget/thumbnailgridview.h \
    compound_widgets/template_selection_widget/templateviewwidget.h \
    compound_widgets/template_selection_widget/templatefolderviewmodel.h \
    compound_widgets/template_selection_widget/folderitem.h

FORMS    += mainwindow.ui \
    compound_widgets/import_widget/importdialog.ui \
    compound_widgets/collider_widget/colliderwidget.ui \
    compound_widgets/sprite_widget/spritewidget.ui \
    compound_widgets/template_selection_widget/templateviewwidget.ui

unix {
INCLUDEPATH += /usr/local/include

LIBS += -L/usr/local/lib \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_highgui \
    -lopencv_photo \
}

win32 {
INCLUDEPATH += C:\opencv248\build\install\include
LIBS += C:\\opencv248\\build\\lib\\libopencv_core248.dll.a \
    C:\\opencv248\\build\\lib\\libopencv_highgui248.dll.a \
    C:\\opencv248\\build\\lib\\libopencv_imgproc248.dll.a \
    C:\\opencv248\\build\\lib\\libopencv_photo248.dll.a
}

RESOURCES += \
    images.qrc \
