INC = ../include
CFLAGS = -fPIC -c 
LDFLAGS = -L../libsrc -lmenuS  -lhash -linput -lload -larray -lpthread -ldl
#LDFLAGS = -L../../lib -lmenu -lprofile -lhash -lio  -lpthread
OBJS = join.o login.o board.o #load.o 혹시안되면 여기에 넣어
TARGET = libjoin.so liblogin.so libboard.so libwriteboard.so libreadboard.so libfindboard.so libchatting.so libdataboard.so libdownload.so libupload.so libfinddata.so libdeletedata.so

all: userServer $(TARGET)

userServer: userServer.o 
	$(CC) -o $@ $@.o $(LDFLAGS)
	#cp $@ ../../bin/

userServer.o: userServer.c
	$(CC) -c -I$(INC) $<

libjoin.so: join.o 
	$(LD) -shared -o $@ $<
#	cp lib$@.so ../../bin/

liblogin.so: login.o
	$(LD) -shared -o $@  login.o
#	cp lib$@.so ../../bin/

libboard.so: board.o 
	$(LD) -shared -o $@ $<
#	cp lib$@.so ../../bin/
libwriteboard.so: writeboard.o 
	$(LD) -shared -o $@ $<
#	cp lib$@.so ../../bin/
libreadboard.so: readboard.o 
	$(LD) -shared -o $@ $<
#	cp lib$@.so ../../bin/
libfindboard.so: findboard.o 
	$(LD) -shared -o $@ $<
#	cp lib$@.so ../../bin/

libchatting.so: chatting.o 
	$(LD) -shared -o $@ $<
#	cp lib$@.so ../../bin/

libdataboard.so: databoard.o 
	$(LD) -shared -o $@ $<
#	cp lib$@.so ../../bin/

libdownload.so: download.o 
	$(LD) -shared -o $@ $<
#	cp lib$@.so ../../bin/
libupload.so: upload.o 
	$(LD) -shared -o $@ $<
#	cp lib$@.so ../../bin/
libfinddata.so: finddata.o 
	$(LD) -shared -o $@ $<
#	cp lib$@.so ../../bin/
libdeletedata.so: deletedata.o 
	$(LD) -shared -o $@ $<
#	cp lib$@.so ../../bin/
.c.o:
	$(CC) $(CFLAGS) -I$(INC) $< 

clean:
	rm *.o lib*.so userServer
