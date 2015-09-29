COMPILER = g++
EXES = configtest
LIBS = ./libconfig4cpp.a 
INC_DIR = /home/pi/Downloads/config4cpp/include
all: ${EXES}

configtest:
	${COMPILER} main.cpp config.h constants.h recording.h setupFunctions.h setupFunctions.cpp ${LIBS} -I${INC_DIR} -o configtest 

clean:
	rm -f *.o ${EXES}
