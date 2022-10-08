SOURCES := $(wildcard */Makefile)
apps := $(foreach source,$(SOURCES),$(dir $(source)))

$(apps):
	$(MAKE) -C $@ $(TARGET)

all:
	$(foreach app, $(apps), $(MAKE) -C $(app) $@ || exit;)

clean:
	$(foreach app, $(apps), $(MAKE) -C $(app) $@ || exit;)

install:
	$(foreach app, $(apps), $(MAKE) -C $(app) $@ || exit;)

.PHONY: $(apps) all clean install