cat:
	gcc -o cat cat.c
	./cat cat.c

copy2:
	gcc -o file1 copy2.c
	./file1 copy2.c file1

backward:
	gcc -o file1 backward.c
	./file1 backward.c

pread:
	gcc -o file2 pread.c
	./file2 pread.c

copytest1:
	gcc -c  copytest1.c t.h t.c
	gcc -o copytest1 copytest1.o  t.o
	./copytest1 file1 file2


copytest2:
	gcc -c  copytest2.c t.h t.c
	gcc -o copytest2 copytest2.o  t.o
	./copytest2 file1 file2

clean:
	rm -rf *.o
	rm -rf *.gch
.PHONY: clean


