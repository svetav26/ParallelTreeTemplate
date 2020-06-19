CXX      = g++
CXXFLAGS = -O2
LIBS     = 
INCLUDE  = 
OUT_NAME = example
FILES    = main.cpp

OBJECTS  = $(FILES:.cpp=.o)

all: $(OUT_NAME)

%.o: %.cpp
	$(CXX) $(INCLUDE) -c $(CXXFLAGS) -o $@ $<

$(OUT_NAME): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(OUT_NAME) $(OBJECTS) $(LIBS)

clean:
	rm -f $(OBJECTS) $(OUT_NAME)