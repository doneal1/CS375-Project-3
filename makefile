#
# Specifiy the target
all:	submission

# Specify the object files that the target depends on
# Also specify the object files needed to create the executable

submission:	backtrack.o
	g++	backtrack.o	-o submission

# Specify how the object files should be created from source files

backtrack.o:	backtrack.cpp
	g++ -g -Wall -std=c++11 -c backtrack.cpp


# Specify the object files and executables that are generated
# and need to be removed to re-compile the whole thing
clean:
	rm -f *.o submission
