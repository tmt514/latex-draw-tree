FILE?=input.in

all: a.out

a.out:
	g++ --std=c++11 main.cpp

run: a.out
	./a.out < inputs/$(FILE) > output.tex
	pdflatex output.tex
	open output.pdf

clean:
	rm output.*
	rm a.out
