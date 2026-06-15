# libcis — one front door for the build/test pipeline.
#
# Every target just shells out to the same tools/ scripts the README documents;
# this file removes the "which hidden command comes next?" guesswork and, unlike
# the conformance board (which counts missing results as red), it FAILS LOUDLY
# the moment a step actually breaks.
#
#   make            # this help
#   make doctor     # is my toolchain present?
#   make smoke      # does the LIBRARY itself build+run?  (no test corpus needed)
#   make support    # build the mandatory libsupport.a    (the all-red culprit)
#   make gate SUBTREE=thread   # per-file CLEAN/NOT-CLEAN check for one subtree
#   make test       # full pipeline: transfer -> build groups -> board
#
# Override the compiler the same way every tools/ script does:  make CXX=g++-13

CXX     ?= g++-10
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
.PHONY: help doctor smoke support transfer board test gate clean

help: ## Show this help
	@grep -E '^[a-zA-Z_-]+:.*?## ' $(MAKEFILE_LIST) | \
	  awk 'BEGIN{FS=":.*?## "}{printf "  \033[36m%-10s\033[0m %s\n",$$1,$$2}'

doctor: ## Probe the toolchain AND smoke-build the library (real check)
	python3 tools/doctor.py

smoke: ## Prove the library compiles+links+runs (no test corpus required)
	@printf '#include <vector>\n#include <algorithm>\nint main(){std::vector<int> v{3,1,2};std::sort(v.begin(),v.end());return v[0]!=1;}\n' > /tmp/cis_smoke.cpp
	$(CXX) $(CIS_FLAGS) /tmp/cis_smoke.cpp src/support.cpp $(CIS_LINK) -o /tmp/cis_smoke
	@/tmp/cis_smoke && echo "smoke: OK ($(CXX))"

support: $(SUPPORT_A) ## Build the mandatory libsupport.a (operator new/delete glue)
$(SUPPORT_A): $(LIB_SRCS)
	@mkdir -p $(dir $@)
	$(CXX) $(CIS_FLAGS) -c src/support.cpp -o $(SUPPORT_O)
	rm -f $@ && ar rcs $@ $(SUPPORT_O)
	@echo "support: built $@"

transfer: ## Run the transfer + build/run all group binaries (needs $$LIBCXX + libclang)
	python3 tools/gen_transfer.py $(SUBTREE)
	ninja -f build/build.ninja

board: ## Print the conformance board (meaningful only after a build)
	python3 tools/board.py $(BACKEND)

test: transfer board ## Full pipeline: transfer -> build groups -> board

gate: support ## Per-file CLEAN/NOT-CLEAN gate, e.g. make gate SUBTREE=thread
	@test -n "$(SUBTREE)" || { echo "usage: make gate SUBTREE=<name>  (e.g. thread, utilities)"; exit 2; }
	python3 tools/run_files.py $(SUBTREE)

clean: ## Remove generated build artifacts (keeps the source tree)
	rm -rf build/groups build/ninja build/recs build/*.ninja \
	       build/tripwire.ok build/transfer.pch test/std
