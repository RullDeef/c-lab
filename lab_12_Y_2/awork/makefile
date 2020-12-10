PYTHON := $(shell command -v py)

.PHONY : build clean

build : awork.pyd

awork.pyd : setup.py src/awork.c
	$(PYTHON) setup.py build ;\
	cp build/*/*.pyd awork.pyd

clean :
	rm -rf build awork.pyd
