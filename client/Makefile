OBJS = client.o
SRCS = $(OBJS:.o=.c)
INC = ../include
client: $(OBJS)
	$(CC) -o $@ $(OBJS) -lpthread

.c.o:	$(SRCS)
	$(CC)	-c -I$(INC) $< -I ../

clean:
	rm *.o client
