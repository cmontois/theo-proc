all: hoc.tab.c hoc.tab.h lex.yy.c hoc


hoc.tab.c: hoc.y
	bison hoc.y

hoc.tab.h: hoc.y
	bison -d hoc.y

lex.yy.c: hoc.l
	lex hoc.l

hoc : hoc.tab.c lex.yy.c
	gcc hoc.tab.c lex.yy.c -o testExp3 symbol.c codeHoc.c io.c code.c -ll -ly -lm -D DEBUG_C -D DEBUG_E -D DEBUG_L

clean: 
	echo "suppression des programmes"
	rm -rf hoc.tab.c hoc.tab.h lex.yy.c  testExp3 


