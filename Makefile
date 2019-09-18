CC= gcc
CXX= g++
DEBUG=-O0 -g

all: clean asm.exe union-sample.exe

.c.o:
	$(CC) $(DEBUG) -c -o $@ $<
.cpp.o:
	$(CXX) $(DEBUG) -c -o $@ $<  -std=c++11

asm.exe: assem-template.o
	$(CXX) -o $@ $< -std=c++11

union-sample.exe: union-sample.o
	$(CXX) -o $@ $< -std=c++11

test: asm.exe
	asm.exe < sum.asm > my.obj
	# ../asm.exe < sum.asm > sum.obj
	diff my.obj ../sum.obj 

clean:
	rm -f *.o *~ \#*\#
