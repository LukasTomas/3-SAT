# Define variables
SRCDIR := src
SOURCES := probsat.cpp Experiment.cpp Cnf.cpp
OBJDIR := .obj
OBJECTS := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SOURCES))  # Create a list of object files in obj/

# Compiler settings
CXX := g++
CXXFLAGS := -O3  # Optimization flags

# Define the target executable
TARGET := probsat

# Rule for building the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Rule for compiling .cpp files to .o files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@mkdir -p $(dir $@)    # Create the directory if it doesn't exist
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create the object directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Clean up
clean:
	rm -rf $(OBJDIR) $(TARGET)