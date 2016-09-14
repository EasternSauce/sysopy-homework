IDIR=include
CC=gcc -Wall
SODIR=obj/static
DODIR=obj/dynamic
SRCDIR=src
_DEPS=list.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))
_OBJ =list.o
SOBJ=$(patsubst %,$(SODIR)/%,$(_OBJ))
DOBJ=$(patsubst %,$(DODIR)/%,$(_OBJ))
LIBS=liblist.a
LIBDIR=lib

all: liblist.a liblist.so

liblist.a: $(SOBJ)
	ar crs $(LIBDIR)/$@ $<

liblist.so: $(DOBJ)
	$(CC) -shared -fPIC -o $(LIBDIR)/$@ $< -I$(IDIR)

$(SODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -o $@ $< -c -I$(IDIR)

$(DODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -fPIC -o $@ $< -I$(IDIR) -c


.PHONY: clean

clean:
	rm -f $(LIBDIR)/*.so $(LIBDIR)/*.a $(SODIR)/*.o $(DODIR)/*.o
