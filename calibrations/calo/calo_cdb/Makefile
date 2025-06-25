##
# CaloCDB
#
# @file
# @version 0.1

# Define your compiler and flags (adjust as needed)
CXX = g++
CXXFLAGS = -g -std=c++20 -Wall -Wextra -pedantic \
-Wshadow -Wconversion -Wsign-conversion -Wnull-dereference \
-Wformat=2 -Wduplicated-cond -Wuseless-cast -Wold-style-cast \
-Wnoexcept -Woverloaded-virtual

# ROOT and sPHENIX flags
ROOT_CPPFLAGS = $(shell root-config --cflags | sed 's/-I/-isystem /g')
ROOT_LIBFLAGS = `root-config --libs`
sPHENIX_CPPFLAGS = -isystem $(OFFLINE_MAIN)/include -isystem $(OFFLINE_MAIN)/rootmacros
sPHENIX_LIBFLAGS = -L$(OFFLINE_MAIN)/lib -lcalo_io -lcdbobjects -lsphenixnpc -lemcNoisyTowerFinder

# Combine all C++ flags for compilation
ALL_CPPFLAGS = $(ROOT_CPPFLAGS) $(sPHENIX_CPPFLAGS)

# Define your source files and output executables
# Use a wildcard to automatically pick up all .C files in 'macros'
SRCS = $(wildcard macros/*.C)

# List all your executable targets explicitly
ALL_TARGETS = bin/genStatus \
              bin/filter-datasets

# Define a stamp file for Cppcheck
CPPCHECK_STAMP = .cppcheck_ran_stamp

#--- Default target: builds all programs and runs Cppcheck ---
# all: $(ALL_TARGETS) cppcheck
all: $(ALL_TARGETS)

#--- Rule for building individual executables (adjust as needed if they have unique dependencies) ---
# This is a pattern rule that says: to build 'bin/X' from 'macros/X.C'
bin/%: macros/%.C
	@echo "Building $@ from $<"
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $(ALL_CPPFLAGS) -o $@ $< $(ROOT_LIBFLAGS) $(sPHENIX_LIBFLAGS)

#--- Cppcheck target ---
# This target now creates/updates the stamp file if any SRCS are newer
$(CPPCHECK_STAMP): $(SRCS)
	@echo "Running Cppcheck static analysis..."
    # Cppcheck command with all necessary include paths and suppressions
	cppcheck --std=c++20 --enable=all "-D R__LOAD_LIBRARY(X)=" \
	--suppress=missingIncludeSystem \
	--suppress=missingInclude \
	--force \
	--checkers-report=cppcheck_checkers.txt \
	--check-level=exhaustive \
	$(SRCS) # Check all source files explicitly listed in SRCS or a specific directory
	@touch $@ # Update the timestamp of the stamp file after successful run

# Make the 'cppcheck' phony target depend on the actual stamp file target
# This ensures 'make cppcheck' runs the stamp file rule
cppcheck: $(CPPCHECK_STAMP)

#--- Clean target ---
clean:
	@echo "Cleaning up..."
	rm -rf bin/ # Remove the entire bin directory
	rm -f cppcheck_checkers.txt $(CPPCHECK_STAMP) # Remove the stamp file too
    # Add any other generated files to clean up here

.PHONY: all cppcheck clean
# end
