TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system
LIBS += -lpthread

SOURCES += \
    IOStuff/console.cpp \
    IOStuff/line.cpp \
    IOStuff/program.cpp \
    IOStuff/programstage.cpp \
    IOStuff/renderer.cpp \
    IOStuff/touchable.cpp \
    IOStuff/touchbuffer.cpp \
    IOStuff/ui.cpp \
    IOStuff/vect.cpp \
    benchmark.cpp \
    demo.cpp \
    lighttest.cpp \
    main.cpp \
    menu.cpp \
    micro3D/color.cpp \
    micro3D/mesh.cpp \
    micro3D/micro3d.cpp \
    micro3D/model.cpp \
    micro3D/multiThread/fragmentjob.cpp \
    micro3D/multiThread/fragmentworker.cpp \
    micro3D/multiThread/vertexjob.cpp \
    micro3D/multiThread/vertexworker.cpp \
    micro3D/point3f.cpp \
    micro3D/rasterization.cpp \
    micro3D/rendercontext.cpp \
    micro3D/rendercontextmt.cpp \
    micro3D/renderstate.cpp \
    micro3D/shaders.cpp \
    micro3D/transformation.cpp \
    mttest.cpp \
    terrain.cpp

HEADERS += \
    IOStuff/console.h \
    IOStuff/line.h \
    IOStuff/program.h \
    IOStuff/programstage.h \
    IOStuff/renderer.h \
    IOStuff/resource_getter.h \
    IOStuff/resource_manager.h \
    IOStuff/singleton.hpp \
    IOStuff/touchable.h \
    IOStuff/touchbuffer.h \
    IOStuff/ui.h \
    IOStuff/vect.h \
    benchmark.h \
    demo.h \
    lighttest.h \
    menu.h \
    micro3D/color.h \
    micro3D/lightpoint.h \
    micro3D/mesh.h \
    micro3D/micro3d.h \
    micro3D/model.h \
    micro3D/multiThread/fragmentjob.h \
    micro3D/multiThread/fragmentworker.h \
    micro3D/multiThread/jobqueue.h \
    micro3D/multiThread/vertexjob.h \
    micro3D/multiThread/vertexworker.h \
    micro3D/point3f.h \
    micro3D/rasterization.h \
    micro3D/rendercontext.h \
    micro3D/rendercontextmt.h \
    micro3D/renderstate.h \
    micro3D/shaders.h \
    micro3D/transformation.h \
    mttest.h \
    terrain.h
