# Host test suite for lib/scale.c and lib/moon.c - no Pebble SDK needed.
# See docs/design/02-moon-phase.md and docs/design/03-barograph.md.
#
# Usage: make -C tests && ./tests/run

CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -g
LDLIBS = -lm

SRC = ../src/c/lib/scale.c ../src/c/lib/moon.c ../src/c/lib/units.c ../src/c/lib/wmo.c \
      ../src/c/sample_schedule.c \
      test_scale.c test_moon.c test_units.c test_wmo.c test_place_change.c \
      test_sample_schedule.c test_main.c

run: $(SRC) shim.h ../src/c/lib/scale.h ../src/c/lib/moon.h ../src/c/lib/units.h ../src/c/lib/wmo.h \
    ../src/c/sample_schedule.h
	$(CC) $(CFLAGS) -o run $(SRC) $(LDLIBS)

# JS-side tests for src/pkjs/pack.js and geocode.js - no Pebble SDK or
# `pebble build` needed, unlike tools/fake_payload.js's standalone mode
# (js/message_keys.js is a hand-written stub, see that file's header comment).
.PHONY: js
js:
	NODE_PATH="$(CURDIR)/js" node pack.test.js
	NODE_PATH="$(CURDIR)/js" node geocode.test.js
	NODE_PATH="$(CURDIR)/js" node trail.test.js
	node sample_hours.test.js

.PHONY: check
check: run js
	./run

.PHONY: clean
clean:
	rm -f run
