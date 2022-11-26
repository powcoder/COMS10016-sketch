default: test

test: sketch.c test.c
	clang -DTESTING -std=c11 -Wall -pedantic -g sketch.c test.c -I/usr/include/SDL2 -o $@ \
	    -fsanitize=undefined -fsanitize=address

sketch: sketch.c
	clang -std=c11 -Wall -pedantic -g sketch.c displayfull.c -I/usr/include/SDL2 -lSDL2 -o $@ \
	    -fsanitize=undefined -fsanitize=address

%: %.c
	clang -Dtest_$@ -std=c11 -Wall -pedantic -g $@.c -o $@ \
	    -fsanitize=undefined -fsanitize=address
