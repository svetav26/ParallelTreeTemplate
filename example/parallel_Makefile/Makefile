# Путь можно заменить на ../../lib/Linux_x86 для 32-битного linux.
PT_PATH  = ../../lib/Linux_x86_64

CXX      = g++
CXXFLAGS = -O2
LIBS     = -L$(PT_PATH) -lParallelTree -Wl,-rpath=$(PT_PATH)
INCLUDE  = -I ../../include
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