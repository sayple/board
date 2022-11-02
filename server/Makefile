INC = ../include
CFLAGS = -fPIC -c  -g
LDFLAGS = -L../libsrc -lmenuS  -lhash -linput -larray -lpthread -ldl
#LDFLAGS = -L../../lib -lmenu -lprofile -lhash -lio  -lpthread
OBJS = join.o login.o 
TARGET = libjoin.so liblogin.so

all: userServer $(TARGET) test

test : test.o load.o join.o login.o 
	$(CC) -o $@ $@.o load.o join.o login.o  $(LDFLAGS)
	#cp $@ ../../bin/
test.o : test.c
	$(CC) -c -I$(INC) -g $<	
userServer: userServer.o load.o
	$(CC) -o $@ $@.o load.o $(LDFLAGS)
	#cp $@ ../../bin/

userServer.o: userServer.c
	$(CC) -c -I$(INC) -g $<

libjoin.so: join.o 
	$(LD) -shared -o $@ $<
#	cp lib$@.so ../../bin/

liblogin.so: login.o 
	$(LD) -shared -o $@ $<
#	cp lib$@.so ../../bin/

.c.o:
	$(CC) $(CFLAGS) -I$(INC) $< 

clean:
	rm *.o lib*.so loadList test userServer
