#
ESPHOME_NAME	= esp-omnik
DEVICE		= $(ESPHOME_NAME)

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

# Compile
all:: compile
clean::
	$(RM) --recursive .esphome
compile: .esphome/build/$(ESPHOME_NAME)/.pioenvs/$(ESPHOME_NAME)/firmware.bin
.esphome/build/$(ESPHOME_NAME)/.pioenvs/$(ESPHOME_NAME)/firmware.bin: bin/esphome $(ESPHOME_NAME).yaml
	. bin/activate; \
	esphome compile $(ESPHOME_NAME).yaml
$(ESPHOME_NAME).yaml: secrets.yaml
	touch "$@"

# Upload
upload: compile
	. bin/activate; \
	esphome upload --device $(DEVICE) $(ESPHOME_NAME).yaml

# Logs
logs: compile
	. bin/activate; \
	esphome logs --device $(DEVICE) $(ESPHOME_NAME).yaml
