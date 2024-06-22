#

all::
clean::

# venv
all:: bin/activate
clean::
	$(RM) --recursive bin include lib lib64 pyvenv.cfg
bin/activate:
	python3 -m venv .

# esphome
all:: bin/esphome
bin/esphome: bin/activate
	. bin/activate; \
	pip install esphome
