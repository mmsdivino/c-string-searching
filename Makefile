# Regra default para criar o programa
all: wordIndex-spellingSuggestion Makefile.uptodate

# Compilando o programa
wordIndex-spellingSuggestion.o: wordIndex-spellingSuggestion.c
	gcc -g -Wall -c wordIndex-spellingSuggestion.c

# Ligando o programa
wordIndex-spellingSuggestion: wordIndex-spellingSuggestion.o
	gcc -g wordIndex-spellingSuggestion.o -o a.out -ggdb

# Limpando tudo o que foi gerado automaticamente pelo comando clean
Makefile.uptodate: Makefile
	make clean
clean:
	rm -rf *.o