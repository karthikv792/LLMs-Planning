all: random

random:
	$(MAKE) -C random

clean:
	cd random && make clean

test: random
	./random/sokoban-generator-typed -n 5 -b 1 -w 0
	./random/sokoban-generator-untyped -n 5 -b 1 -w 0
	./choose-suite.py

.PHONY: random
