CC = gcc				# compiler to use

TARGET = proj2a			# name of the executable file to make
SRC = proj2a.c 			# source files
OBJ = proj2a.o			# object files
LDFLAGS = -lpthread		# libraries to link

# default target
all: $(TARGET) 			

# link object files to the executable
# $@ is the target, $^ are all the dependencies
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# compile source files (.c) to object files (.o)
%.o: %.c
	$(CC) -c $< -o $@

# clean build files (remove executable and object files)
clean:
	rm -f $(TARGET) $(OBJ)

# to run the program
run: $(TARGET)
	./$(TARGET)  

# phony targets
.PHONY: all clean run