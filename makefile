CXX = g++
CXXFLAGS = -Wall -Werror -std=c++20

TARGET = connect_four
SOURCES = main.cpp jogabilidade.cpp tabela.cpp menu.cpp
OBJECT_FILES = $(SOURCES:.cpp=.o)


all: $(TARGET)

$(TARGET): $(OBJECT_FILES)
	$(CXX) $(CXXFLAGS) $(OBJECT_FILES) -o $(TARGET)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

run:
	./$(TARGET)

clean:
	rm $(TARGET) $(OBJECT_FILES)
