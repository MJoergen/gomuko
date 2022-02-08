sources  = main.cc
sources += CBoard.cc
sources += statistics.cc
sources += CRand.cc
sources += CPiece.cc
sources += CMove.cc

DEFINES  = -Wall -O3
#DEFINES += -g -DENABLE_TRACE
CC = g++

objects = $(sources:.cc=.o)
depends = $(sources:.cc=.d)

gomuko: $(objects) Makefile
		$(CC) $(DEFINES) -o $@ $(objects) -lstdc++
		mv $@ $(HOME)/bin

%.d: %.cc
		set -e; $(CC) -M $(CPPFLAGS) $(DEFINES) $(INCLUDE_DIRS) $< \
				| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
				[ -s $@ ] || rm -f $@

include $(depends)

%.o :
		$(CC) $(DEFINES) $(INCLUDE_DIRS) -c $< -o $@

clean:
		-rm $(objects)
		-rm $(depends)

