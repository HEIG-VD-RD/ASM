CFLAGS += -g -m32 -no-pie -O0

TARGETS = hello hello_protecc attacc

all : $(TARGETS)

hello : main.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -fno-stack-protector -o $@ $^

hello_protecc : main.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^

attacc : main_attacc.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGETS)
