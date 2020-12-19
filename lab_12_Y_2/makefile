PYTHON := $(shell command -v py)

.PHONY : run

run : src/awork.pyd
	$(PYTHON) ./src/app.py

src/awork.pyd :
	cd awork; $(MAKE) build; cp awork.pyd ../src/awork.pyd
