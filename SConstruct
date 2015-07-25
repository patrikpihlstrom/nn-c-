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
src/actor/ActorManager.cpp
src/actor/NPCActor.cpp
src/actor/PlayerActor.cpp
src/actor/NPCSpawner.cpp
src/application/main.cpp
src/application/Application.cpp
src/application/Camera.cpp
src/application/Math.cpp
src/application/TextureHolder.cpp
src/application/state/State.cpp
src/application/state/MenuState.cpp
src/application/state/GameState.cpp
src/navigation/NavGraph.cpp
src/object/Object.cpp
src/object/ObjectIdTracker.cpp
src/world/Chunk.cpp
src/world/Path.cpp
src/world/Quadtree.cpp
src/world/World.cpp
src/world/procedural/Noise.cpp
src/world/procedural/RockGenerator.cpp
src/world/procedural/PathGenerator.cpp
""")

environment = Environment(CC='gcc')
environment.Append(CPPPATH='src')
environment.Append(LIBS=libs)
environment.Append(CXXFLAGS='-std=c++11')
environment.Program(target='No-Motivation', source=sources)

