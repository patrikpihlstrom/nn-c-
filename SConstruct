print '\x1b[31m' + "\n--------------------No-Motivation--------------------" + '\033[0m'

libs = Split("""
sfml-graphics
sfml-window
sfml-system
lua5.2
luabind
""")

sources = Split("""
src/actor/Actor.cpp
src/actor/ActorIdTracker.cpp
src/actor/ActorHolder.cpp
src/actor/ActorManager.cpp
src/actor/NPCActor.cpp
src/actor/PlayerActor.cpp
src/application/main.cpp
src/application/Application.cpp
src/application/Camera.cpp
src/application/Math.cpp
src/application/TextureHolder.cpp
src/world/World.cpp
src/world/procedural/RockGenerator.cpp
src/world/Quadtree.cpp
src/object/Object.cpp
src/object/ObjectIdTracker.cpp
""")

environment = Environment(CC='gcc')
environment.Append(CPPPATH='src')
environment.Append(LIBS=libs)
environment.Append(CXXFLAGS='-std=c++11')
environment.Program(target='No-Motivation', source=sources)

