DIR_INC = ./include
DIR_SRC = ./src
DIR_OBJ = ./obj
DIR_BIN = ./bin

SRC = $(wildcard ${DIR_SRC}/*.cpp)  
OBJ = $(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${SRC})) 

TARGET = server
BIN_TARGET = ${DIR_BIN}/${TARGET}
LIB = -lpthread

CC = g++
CFLAGS = -g -Wall -I${DIR_INC} -std=c++11

${BIN_TARGET}:${OBJ}
	$(CC) $(OBJ) -o $@ ${LIB} 
    
${DIR_OBJ}/%.o:${DIR_SRC}/%.cpp
	$(CC) $(CFLAGS) -c  $< -o $@

.PHONY:clean
clean:
	rm  $(BIN) $(OBJ)

