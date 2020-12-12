# the compiler to be used
CC = g++

# compiler flags
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
#  -O3    - this flag is used for production level optimization
CFLAGS = -g -Wall -O3

# The build targets
DEMO_TARGET = RBT_DEMO
UNIT_TARGET = RBT_TEST

# The object names
DEMO_OBJ = demo.o
TEST_OBJ = unittest.o

# Build target set
TARGETS = $(DEMO_TARGET) $(UNIT_TARGET)

# Object name set
OBJS = $(TEST_OBJ) $(DEMO_OBJ)

# path to any header files not in /usr/include or the current directory 
INCLUDES +=-I include/

# add the path to my library code; -L tells the linker where to find it
#LFLAGS += -L /home/newhall/mylibs

# list of libraries to link into executable; -l tells the linker which 
# library to link into the executable 
#LIBS = -lmymath -lsimple -lm -lpthread

default: $(TARGETS)

clean:
	rm *.o ${DEMO_TARGET} ${UNIT_TARGET}

cleanDemo:
	rm *.o ${DEMO_TARGET}

cleanTest:
	rm *.o ${UNIT_TARGET}

# Link RBT_DEMO binary
${DEMO_TARGET} : ${DEMO_OBJ}
	${CC} ${CFLAGS} ${LFLAGS} -o ${DEMO_TARGET} ${DEMO_OBJ} ${LIBS}

# Link RBT_TEST binary
${UNIT_TARGET} : ${TEST_OBJ}
	${CC} ${CFLAGS} ${LFLAGS} -o ${UNIT_TARGET} ${TEST_OBJ} ${LIBS}

# Generic build of objects
${OBJS}:
	${CC} -c ${CFLAGS} ${INCLUDES} ${@:.o=.cpp}