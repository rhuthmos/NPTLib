default: app

context.o: context.s
	gcc -m32 -g -c context.s

thread.o: thread.c
	gcc -Werror -m32 -O3 -g -c thread.c

app: app.c thread.o context.o
	gcc -O3 -m32 thread.o context.o -g -o app app.c

run: app
	./app

test: app
	./app 1024
	./app 1024 1

clean:
	rm -f thread.o app context.o
