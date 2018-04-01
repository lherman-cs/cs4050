## Compiler
CC=g++
## Linker
LD=$(CC)
## Flags
CPPFLAGS = -Wall -g -DLINUX -std=c++11
#CPPFLAGS= 
LFLAGS = -lglut -L/usr/lib -L/usr/X11R6/lib -lXi -lXmu -lGL -lGLU -lm

TARGETS= $(shell ls *.cpp)
OBJECTS= $(shell ls *.cpp | cut -d'.' -f1 | xargs printf "%s.o\n" )
PROG= assn2

$(PROG): $(OBJECTS)
	$(CC) -o $@ $? $(LFLAGS)

$(OBJECTS): $(TARGETS)
	$(CC) $(CPPFLAGS) -c $?

test: clean $(PROG)
	$(shell read -p "Input model_path: " model_path; ./$(PROG) $$model_path)

clean:
	rm -f $(PROG) $(OBJECTS)

# TARGETS = $(PROGFILES:.cpp=)

# PROGFILES = \
#         assn2.cpp \
# 				utils.cpp \
#         $(NULL)

# targets default: $(TARGETS)

# $(PROGFILES:.cpp=): assn2.o utils.o 
# 	$(CC) -o assn2 $? ${LFLAGS}

# depend :
# 	makedepend ${PROGFILES}
# # DO NOT DELETE

# test:
# 	make
# 	./assn2

# clean:
# 	rm -f assn2 assn2.o utils.o
