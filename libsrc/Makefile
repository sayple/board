INC = ../include
CFLAGS = -fPIC -c  
OBJS1 = menu.o profile.o input.o
TARGET = libmenuS.so libhash.so libarray.so libinput.so libload.so

all: $(TARGET)

libmenuS.so: $(OBJS1) 
	$(LD) -shared -o $@ $(OBJS1)

libhash.so: hash.o 
	$(LD) -shared -o $@ $<
#	cp lib$@.so ../../bin/

libarray.so: array.o 
	$(LD) -shared -o $@ $<
#	cp lib$@.so ../../bin/

libinput.so: input.o 
	$(LD) -shared -o $@ $<
	
libload.so: load.o 
	$(LD) -shared -o $@ $<

.c.o:
	$(CC) $(CFLAGS) -I$(INC) $< 

clean:
	rm *.o lib*.so
