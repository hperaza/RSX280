SHELL=/bin/sh

subdirs = cksboot mktask sym2inc vol180 vtcp

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
