
CFLAGS=-I.

testlog: dailylog.so main.c
	gcc -o testlog main.c dailylog.so $(CFLAGS)

dailylog.so: dailylog.o
	gcc -fPIC -shared -o dailylog.so dailylog.o $(CFLAGS)

dailylog.o: dailylog.c
	gcc -fPIC -c dailylog.c $(CFLAGS)
.PHONY: clean
clean:
	-rm *.o *.so testlog

