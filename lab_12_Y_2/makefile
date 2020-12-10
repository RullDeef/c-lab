PYTHON := $(shell command -v py)

.PHONY : run

run : awork/awork.pyd
	cp awork/awork.pyd src/awork.pyd ;\
	$(PYTHON) ./src/app.py
