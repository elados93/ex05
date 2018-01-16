cmake_minimum_required(VERSION 3.8)
project(ex04)

set(CMAKE_CXX_STANDARD 98)

include_directories(include src/gtest_src/googletest/include ${gtest_src}/include ${gtest_src})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98 -pthread")

set(TEST_FILES src/test src/test/StringHandlerTest.cpp src/test/roomsHandlerTest.cpp)

set(REMOTE_FILES src/client/Client.cpp src/client/Client.h
        src/server/Task.h src/server/ThreadPool.cpp src/server/ThreadPool.h
        src/server/Server.cpp src/server/Server.h src/server/Commands/CommandManager.cpp
        src/server/Commands/CommandManager.h src/server/Commands/StartCommand.cpp src/server/Commands/StartCommand.h
        src/server/Commands/PlayCommand.cpp src/server/Commands/PlayCommand.h src/server/Commands/ListCommand.cpp
        src/server/Commands/ListCommand.h src/server/Commands/JoinCommand.cpp src/server/Commands/JoinCommand.h
        src/server/Commands/CloseCommand.cpp src/server/Commands/CloseCommand.h src/server/Commands/Command.cpp
        src/server/HandleGame.cpp src/server/HandleGame.h src/server/StringHandler.cpp src/server/StringHandler.h
        src/server/Commands/Command.h src/server/RoomsHandler.cpp src/server/RoomsHandler.h)

set(REVERSI_FILES src/client/reversi/AIPlayer.cpp src/client/reversi/AIPlayer.h
        src/client/reversi/Board.cpp src/client/reversi/Board.h
        src/client/reversi/Cell.cpp src/client/reversi/Cell.h
        src/client/reversi/ConsolePrinter.cpp src/client/reversi/ConsolePrinter.h
        src/client/reversi/GameManager.cpp src/client/reversi/GameManager.h
        src/client/reversi/GameRules.h src/client/reversi/Printer.h
        src/client/reversi/GenralDef.h src/client/reversi/GameState.cpp
        src/client/reversi/GameState.h src/client/reversi/ReversiDefaultRules.cpp
        src/client/reversi/ReversiDefaultRules.h src/client/reversi/Player.h
        src/client/reversi/HumanPlayer.cpp src/client/reversi/HumanPlayer.h src/client/reversi/Point.cpp
        src/client/reversi/Point.h src/client/reversi/RemoteGameManager.cpp src/client/reversi/RemoteGameManager.h)

add_executable(Client ${REMOTE_FILES} ${REVERSI_FILES} src/client/mainClient.cpp)

add_executable(Server ${REMOTE_FILES} ${REVERSI_FILES} src/server/mainServer.cpp)

add_executable(TestReversi ${REVERSI_FILES} ${REMOTE_FILES} ${TEST_FILES} src/test/gtest_main.cpp)
