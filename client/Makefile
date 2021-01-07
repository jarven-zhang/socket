DIR_SRC = ./
DIR_INC = ./

SRC = ./*.cpp

TARGET = client
LIB = -lpthread -L/usr/lib64/ -lglog

CC = g++
CFLAGS = -g -Wall -I${DIR_INC} -std=c++11

${TARGET}:${DIR_SRC}/${SRC}
	$(CC) ${CFLAGS} $< -o $@ ${LIB} 
    

.PHONY:clean
clean:
	rm  $(TARGET)
