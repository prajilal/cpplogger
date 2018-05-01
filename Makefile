# source files.
SRC := Logger.cpp main.cpp
 
OBJ = $(SRC:.cpp=.o)
 
TARGET = cpplogger

.SUFFIXES: .cpp
 
.cpp.o:
	g++ -std=c++11 -lpthread -I/usr/local/include -c $< -o $@
 
$(TARGET): $(OBJ)
	g++ -o $(TARGET) $(OBJ) -L/usr/local/lib -lpthread 
 
clean:
	rm -f $(OBJ) $(TARGET)
