SHELL=/bin/sh

subdirs = zsm tkb lbr basic11

all:
	@for i in ${subdirs}; do \
		echo Making all in $$i ; \
		(cd $$i; ${MAKE} all) ; \
	done

clean:
	@for i in ${subdirs}; do \
		echo Cleaning in $$i ; \
		(cd $$i; ${MAKE} clean) ; \
	done
