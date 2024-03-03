# Recursive makefile for the assignment 3, that call all makefiles in the subdirectories

# List of subdirectories containing code and Makefile
SUBDIRS := Section1 Section2 Section3

# Default target
all: $(SUBDIRS)

# Recursively execute make in each subdirectory
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)

# Clean up generated files
clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
