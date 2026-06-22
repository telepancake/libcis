# libcis — one front door for the build/test pipeline.
#
# Every target just shells out to the same tools/ scripts the README documents;
# this file removes the "which hidden command comes next?" guesswork and, unlike
# the conformance board (which counts missing results as red), it FAILS LOUDLY
# the moment a step actually breaks.
#
#   make            # this help
#   make bootstrap  # download the pinned toolchain into ./toolchain
#   make doctor     # is my toolchain present?
#   make smoke      # does the LIBRARY itself build+run?  (no test corpus needed)
#   make support    # build the mandatory libsupport.a    (the all-red culprit)
#   make gate SUBTREE=thread   # per-file CLEAN/NOT-CLEAN check for one subtree
#   make test       # full pipeline: transfer -> build groups -> board
#
# `make bootstrap` fetches g++-10, libclang, and the libc++ headers + test
# corpus into ./toolchain so the build does not depend on system package
# versions/locations; every other target depends on it.  Override the compiler
# the same way every tools/ script does:  make CXX=g++-13

# The bootstrapped toolchain (tools/bootstrap.sh); tools/config.py prefers it.
TOOLCHAIN    := $(CURDIR)/toolchain
BOOTSTRAP_OK := $(TOOLCHAIN)/.bootstrap-ok
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
.PHONY: help bootstrap doctor smoke support transfer board test gate clean distclean

help: ## Show this help
	@grep -E '^[a-zA-Z_-]+:.*?## ' $(MAKEFILE_LIST) | \
	  awk 'BEGIN{FS=":.*?## "}{printf "  \033[36m%-10s\033[0m %s\n",$$1,$$2}'

bootstrap: $(BOOTSTRAP_OK) ## Download the pinned toolchain into ./toolchain
$(BOOTSTRAP_OK):
	tools/bootstrap.sh

doctor: $(BOOTSTRAP_OK) ## Probe the toolchain AND smoke-build the library (real check)
	python3 tools/doctor.py

smoke: $(BOOTSTRAP_OK) ## Prove the library compiles+links+runs (no test corpus required)
	@printf '#include <vector>\n#include <algorithm>\nint main(){std::vector<int> v{3,1,2};std::sort(v.begin(),v.end());return v[0]!=1;}\n' > /tmp/cis_smoke.cpp
	$(CXX) $(CIS_FLAGS) /tmp/cis_smoke.cpp src/support.cpp $(CIS_LINK) -o /tmp/cis_smoke
	@/tmp/cis_smoke && echo "smoke: OK ($(CXX))"

support: $(SUPPORT_A) ## Build the mandatory libsupport.a (operator new/delete glue)
$(SUPPORT_A): $(LIB_SRCS) | $(BOOTSTRAP_OK)
	@mkdir -p $(dir $@)
	$(CXX) $(CIS_FLAGS) -c src/support.cpp -o $(SUPPORT_O)
	rm -f $@ && ar rcs $@ $(SUPPORT_O)
	@echo "support: built $@"

transfer: $(BOOTSTRAP_OK) ## Run the transfer + build/run all group binaries
	python3 tools/gen_transfer.py $(SUBTREE)
	ninja -f build/build.ninja              # transfer -> manifest -> tripwire
	# build/build.ninja CANNOT build the groups stage itself: it pulls groups.ninja
	# in via `subninja`, and ninja only regenerates the top-level -f file, never a
	# subninja'd one -- so the gengroups edge never fires.  Drive it explicitly:
	python3 tools/gen_groups.py --ninja    # materialize the real groups.ninja
	# -k0 builds every group it can; a red group is a missing .result that the
	# board counts, NOT a reason to abort before the board runs.  The leading `-`
	# lets `make test` proceed to `board` (whose own exit reflects conformance).
	-ninja -f build/groups.ninja $(BACKEND) -k0  # libsupport.a + all group results

board: ## Print the conformance board (meaningful only after a build)
	python3 tools/board.py $(BACKEND)

test: transfer board ## Full pipeline: transfer -> build groups -> board

gate: support ## Per-file CLEAN/NOT-CLEAN gate, e.g. make gate SUBTREE=thread
	@test -n "$(SUBTREE)" || { echo "usage: make gate SUBTREE=<name>  (e.g. thread, utilities)"; exit 2; }
	python3 tools/run_files.py $(SUBTREE)

clean: ## Remove generated build artifacts (keeps the toolchain + source tree)
	rm -rf build/groups build/ninja build/recs build/*.ninja \
	       build/tripwire.ok build/transfer.pch test/std

distclean: clean ## Also remove the downloaded ./toolchain (forces a re-bootstrap)
	rm -rf toolchain
