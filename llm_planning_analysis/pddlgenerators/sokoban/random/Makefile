TYPED=sokoban-generator-typed
UNTYPED=sokoban-generator-untyped

all: $(TYPED) $(UNTYPED)

$(TYPED): $(TYPED).cpp
	$(CXX) $(TYPED).cpp -o $(TYPED)

$(UNTYPED): $(UNTYPED).cpp
	$(CXX) $(UNTYPED).cpp -o $(UNTYPED)

clean:
	rm -f $(TYPED) $(UNTYPED)
