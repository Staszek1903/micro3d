cmake_minimum_required(VERSION 3.7.2)

find_package(SFML 2 REQUIRED COMPONENTS graphics audio)

# all source files
set(SRC 
IOStuff/program.cpp 
IOStuff/console.cpp  
IOStuff/touchbuffer.cpp   
IOStuff/renderer.cpp 
IOStuff/touchable.cpp  
IOStuff/ui.cpp  
IOStuff/vect.cpp 
IOStuff/programstage.cpp 
IOStuff/line.cpp 
micro3d/color.cpp 
micro3d/shaders.cpp 
micro3d/rasterization.cpp 
micro3d/transformation.cpp 
micro3d/point3f.cpp 
micro3d/micro3d.cpp 
micro3d/mesh.cpp 
micro3d/model.cpp 
micro3d/rendercontext.cpp 
micro3d/renderstate.cpp 
benchmark.cpp 
demo.cpp 
lighttest.cpp 
menu.cpp 
terrain.cpp 
main.cpp 
 )
 
# define the pong target
add_executable(exec ${SRC})

target_link_libraries(exec sfml-audio sfml-graphics)