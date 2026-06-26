# libcis — one front door for the build/test pipeline.
#
# Every target just shells out to the same tools/ scripts the README documents;
# this file removes the "which hidden command comes next?" guesswork and, unlike
# the conformance board (which counts missing results as red), it FAILS LOUDLY
# the moment a step actually breaks.
#
#   make            # this help
#   make bootstrap  # build the library/test toolchain (g++-10) into ./toolchain
#   make smoke      # does the LIBRARY itself build+run?  (no test corpus needed)
#   make support    # build the mandatory libsupport.a    (the all-red culprit)
#   make gate SUBTREE=thread   # per-file CLEAN/NOT-CLEAN check for one subtree
#   make test       # build the COMMITTED tests -> board   (no transfer)
#
#   make bootstrap-transfer    # add clang + the libc++ corpus (transfer toolchain)
#   make transfer              # regenerate test/std/ from the corpus, then commit
#
# The transferred tests (test/std/) are COMMITTED, so the everyday path --
# `make bootstrap` then `make test` -- needs only g++-10; it never runs the
# transfer.  Rebuild the transfer toolchain + regenerate test/std/ only when the
# corpus or the transfer tool changes.  Override the compiler the same way every
# tools/ script does:  make CXX=g++-13

# The bootstrapped toolchain (tools/bootstrap.sh); tools/config.py prefers it.
# Two scopes: GCC_OK is just g++-10 -- all the LIBRARY and the COMMITTED tests
# need; XFER_OK adds clang/libclang + the libc++ corpus, needed only to
# regenerate test/std/ (`make transfer`).
TOOLCHAIN    := $(CURDIR)/toolchain
GCC_OK       := $(TOOLCHAIN)/.gcc-ok
XFER_OK      := $(TOOLCHAIN)/.bootstrap-ok
LOCAL_GXX    := $(TOOLCHAIN)/gcc/bin/g++-10

# Default the library compiler to the local g++-10 when it has been bootstrapped.
# Guard on origin: GNU make pre-defines CXX=g++ (origin "default"), which a plain
# `?=` would NOT override -- so only set CXX when it came from neither the command
# line (`make CXX=...`) nor the environment, both of which must still win.
ifeq ($(filter command line environment,$(origin CXX)),)
CXX := $(if $(wildcard $(LOCAL_GXX)),$(LOCAL_GXX),g++-10)
endif
BACKEND ?= libcis
SUBTREE ?=

# Must mirror the libcis backend flags in tools/gen_groups.py exactly.
CIS_FLAGS := -std=gnu++20 -fcoroutines -fno-exceptions -fno-rtti -nostdinc++ \
             -Iinclude -Itest/std -Itest/std/support -O0 -w
CIS_LINK  := -nodefaultlibs -lpthread -lm -lc -lgcc_s -lgcc

SUPPORT_O := build/groups/libcis/__support.o
SUPPORT_A := build/groups/libcis/libsupport.a
LIB_SRCS  := $(shell find include src -type f 2>/dev/null)

.DEFAULT_GOAL := help
.PHONY: help bootstrap bootstrap-transfer doctor smoke support transfer groups board test gate size clean distclean

help: ## Show this help
	@grep -E '^[a-zA-Z_-]+:.*?## ' $(MAKEFILE_LIST) | \
	  awk 'BEGIN{FS=":.*?## "}{printf "  \033[36m%-10s\033[0m %s\n",$$1,$$2}'

bootstrap: $(GCC_OK) ## Build the library/test toolchain (g++-10) into ./toolchain
$(GCC_OK):
	tools/bootstrap.sh gcc

bootstrap-transfer: $(XFER_OK) ## Also build the transfer toolchain (clang + libc++ corpus)
$(XFER_OK):
	tools/bootstrap.sh transfer

doctor: $(XFER_OK) ## Probe the toolchain AND smoke-build the library (real check)
	python3 tools/doctor.py

smoke: $(GCC_OK) ## Prove the library compiles+links+runs (no test corpus required)
	@printf '#include <vector>\n#include <algorithm>\nint main(){std::vector<int> v{3,1,2};std::sort(v.begin(),v.end());return v[0]!=1;}\n' > /tmp/cis_smoke.cpp
	$(CXX) $(CIS_FLAGS) /tmp/cis_smoke.cpp src/support.cpp $(CIS_LINK) -o /tmp/cis_smoke
	@/tmp/cis_smoke && echo "smoke: OK ($(CXX))"

support: $(SUPPORT_A) ## Build the mandatory libsupport.a (operator new/delete glue)
$(SUPPORT_A): $(LIB_SRCS) | $(GCC_OK)
	@mkdir -p $(dir $@)
	$(CXX) $(CIS_FLAGS) -c src/support.cpp -o $(SUPPORT_O)
	rm -f $@ && ar rcs $@ $(SUPPORT_O)
	@echo "support: built $@"

# REGENERATION ONLY.  test/std/ is committed, so day-to-day `make test` does NOT
# run this -- it needs the heavy transfer toolchain (`make bootstrap-transfer`:
# clang/libclang + the libc++ corpus).  Run it when the corpus or the transfer
# tool (tools/transfer*.py, tools/gen_transfer.py) changes, then COMMIT test/std/.
transfer: $(XFER_OK) ## Regenerate test/std/ from the corpus (then commit it)
	python3 tools/gen_transfer.py $(SUBTREE)
	ninja -f build/build.ninja              # transfer -> manifest -> tripwire
	@echo "transfer: regenerated test/std/ -- 'git add test/std' and commit so"
	@echo "          'make test' can build the tests directly."

# Build the consolidated group binaries straight from the COMMITTED test/std/.
# No transfer, no corpus, no libclang -- only g++-10.  gen_groups.py reads the
# committed manifest + sources and emits a self-contained build/groups.ninja.
groups: $(GCC_OK) ## Build/run the group binaries from the committed test/std/
	@test -f test/std/manifest.json || { \
	  echo "test/std/manifest.json is missing.  The committed tests are absent;"; \
	  echo "regenerate them with 'make bootstrap-transfer && make transfer', then"; \
	  echo "commit test/std/.  (Day-to-day this file is checked in.)"; exit 2; }
	python3 tools/gen_groups.py --ninja    # materialize build/groups.ninja
	# -k0 builds every group it can; a red group is a missing .result that the
	# board counts, NOT a reason to abort before the board runs.  The leading `-`
	# lets `make test` proceed to `board` (whose own exit reflects conformance).
	-ninja -f build/groups.ninja $(BACKEND) -k0  # libsupport.a + all group results

board: ## Print the conformance board (meaningful only after a build)
	python3 tools/board.py $(BACKEND)

test: groups board ## Build the committed tests + print the board (no transfer)

gate: support ## Per-file CLEAN/NOT-CLEAN gate, e.g. make gate SUBTREE=thread
	@test -n "$(SUBTREE)" || { echo "usage: make gate SUBTREE=<name>  (e.g. thread, utilities)"; exit 2; }
	python3 tools/run_files.py $(SUBTREE)

size: ## Per-call/per-type code + memory overhead vs non-type-erased ref; writes .test_results/latest/
	python3 bench/size_slope.py

clean: ## Remove generated build artifacts (keeps the toolchain + source tree)
	rm -rf build/groups build/ninja build/recs build/*.ninja \
	       build/tripwire.ok build/transfer.pch test/std

distclean: clean ## Also remove the downloaded ./toolchain (forces a re-bootstrap)
	rm -rf toolchain
