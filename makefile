.PHONY: clean compile run
notDefault:
	@echo "No arguments runs the first recipe no matter what you call it."
compile: user.o main.o
	@gcc -w -o user user.o main.o -lm
main.o: main.c user.h
	@gcc -w -c main.c
user.o: user.c user.h
	@gcc -w -c user.c -lm
run: user
	@./user $(ARGS)
clean:
	@rm -f *.o
	@rm -f user
	@rm -f bankData.dat
