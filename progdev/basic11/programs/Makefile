DSKUTL = ../../../Tools/linux/vol180/vol180

all: update

# Update a floppy with the programs on this directory
update:
	@echo "cd basic" > copy.cmd
	@for i in *.bas *.bac; do \
		echo "delete "`basename $$i` >> copy.cmd ; \
		echo "import "$$i" "`basename $$i` >> copy.cmd ; \
	done
	@echo "dir" >> copy.cmd
	@echo "quit" >> copy.cmd
	$(DSKUTL) /dev/fd0 < copy.cmd
	@rm copy.cmd
