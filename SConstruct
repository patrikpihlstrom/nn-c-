print '\x1b[31m' + "\n--------------------No-Motivation--------------------" + '\033[0m'

libs = Split("""
sfml-graphics
sfml-window
sfml-system
""")

sources = Split("""
src/application/main.cpp
src/application/Application.cpp
src/application/Camera.cpp
src/application/Math.cpp
src/application/TextureHolder.cpp
src/map/Map.cpp
src/map/Quadtree.cpp
src/object/Entity.cpp
src/object/GameObject.cpp
src/object/Light.cpp
src/object/Object.cpp
src/object/ObjectIdTracker.cpp
src/object/PlayerEntity.cpp
src/object/ShadowUpdater.cpp
""")

Program(target = 'No-Motivation', source = sources, LIBS = libs, CXXFLAGS = "-std=c++11")

