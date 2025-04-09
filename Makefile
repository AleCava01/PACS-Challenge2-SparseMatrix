# look for the first C++ compilator available
ifneq ($(shell which clang++),)
    CXX = clang++
else ifneq ($(shell which g++),)
    CXX = g++
else ifneq ($(shell which cl),)
    CXX = cl
else
    $(error C++ compiler not found!)
endif

CXXFLAGS ?= -std=c++20 -Wall -Wextra


SRC_DIR  = src
OBJ_DIR  = obj
SOURCES  = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS  = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

EXEC     = main
LDFLAGS ?= 
LDLIBS  ?= 

all: $(EXEC)

$(EXEC): $(OBJECTS) | $(OBJ_DIR)
	$(CXX) $(OBJECTS) -o $(EXEC)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# create obj dir if it doesn't exists
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# cleaning rules
clean:
	$(RM) *.o $(EXEC) *.dat *.exe
	$(RM) $(wildcard $(SRC_DIR)/*.exe)
	
distclean:
	$(RM) -r $(OBJ_DIR) $(EXEC) *.o
	$(RM) $(EXEC) $(EXEC_MUPARSER)
	$(RM) *~
