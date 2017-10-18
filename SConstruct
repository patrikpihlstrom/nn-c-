print '\x1b[31m' + "\n--------------------No-Motivation--------------------" + '\033[0m'

libs = Split("""
sfml-graphics
sfml-window
sfml-system
""")

sources = Split("""
src/actor/Actor.cpp
src/actor/ActorIdTracker.cpp
src/actor/ActorManager.cpp
src/actor/NNActor.cpp
src/actor/nn/NeuralNet.cpp
src/application/main.cpp
src/application/Application.cpp
src/application/Camera.cpp
src/application/Math.cpp
src/application/TextureHolder.cpp
src/application/state/State.cpp
src/application/state/MenuState.cpp
src/application/state/GameState.cpp
src/application/ui/Button.cpp
src/object/Object.cpp
src/object/ObjectIdTracker.cpp
src/world/Chunk.cpp
src/world/Quadtree.cpp
src/world/Region.cpp
src/world/World.cpp
""")

environment = Environment(CC='gcc')
#environment.Append(CPPPATH='.')
environment.Append(LIBS=libs)
environment.Append(CXXFLAGS='-std=c++11')
environment.Program(target='No-Motivation', source=sources)

