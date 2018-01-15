# Shahar Palmor & Elad Aharon
# 307929927 311200786

CC = g++
VERSION = -std=c++98
CLIENT_CPP_FILES = ./src/client/*.cpp ./src/client/reversi/*.cpp ./src/server/StringHandler.cpp
CLIENT_H_FILES = ./src/client/*.h ./src/client/reversi/*.h ./src/server/StringHandler.h

SERVER_H_FILES = ./src/server/*.h ./src/server/Commands/*.h
SERVER_CPP_FILES = ./src/server/*.cpp ./src/server/Commands/*.cpp

CLIENT_O_FILES = ./obj/client_obj/*
SERVER_O_FILES = ./obj/server_obj/*

create_all: obj server_exe client_exe clean

client_exe: $(CLIENT_CPP_FILES) $(CLIENT_H_FILES)
	$(CC) $(VERSION) -c $(CLIENT_CPP_FILES)
	mv *.o obj/client_obj
	$(CC) $(VERSION) $(CLIENT_O_FILES) -o ./exe/client_exe

server_exe: $(SERVER_CPP_FILES) $(SERVER_H_FILES)
	$(CC) $(VERSION) -c $(SERVER_CPP_FILES) -pthread
	mv *.o obj/server_obj
	$(CC) $(VERSION) $(SERVER_O_FILES) -o ./exe/server_exe -pthread

obj:
	mkdir obj
	mkdir obj/client_obj
	mkdir obj/server_obj
	mkdir exe

clean:	
	rm -rf obj/

cleanALL:	
	rm -rf obj/
	rm -rf exe/
