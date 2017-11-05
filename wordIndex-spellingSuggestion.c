#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sys/stat.h>
#define ALPHABET_SIZE	26
#define MAX_WORD	80
#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

int distancia;
char aux[50];
const char delim[]= ".,:;`/\"\'§ªº°'+-_(){}[]<>*&^%$#@!?~/|\\=1234567890 \t\n";

void strlwr(char* str) {
	size_t i;
	size_t len = strlen(str);

	for (i = 0; i < len; i++)
		str[i] = tolower((unsigned char) str[i]);
}

typedef struct ListLine{
	int value;
	struct ListLine *next;
} ll;

/*typedef struct ListWords{
	char *data;
	struct ListWords *nextWord;
} lw;

lw* listaOrdem = NULL;

void insertList(char *palavra){
	strlwr(palavra);
	lw* q;
	for (q=listaOrdem;q!=NULL;q=q->nextWord){
		if (strcmp(palavra,q->data)==0) return;
	}
	
	lw* novo=(lw*)malloc(sizeof(lw*));
	novo->data=(char*)calloc(MAX_WORD,sizeof(char));
	if (novo==NULL || novo->data == NULL){
		puts("Erro de memoria."); exit(1);
	}
	strcpy(novo->data,palavra);
	
	if (listaOrdem==NULL) listaOrdem=novo;
	else {
		lw* a;
		a=listaOrdem;
		while (a->nextWord!=NULL) a=a->nextWord;
		a->nextWord=novo;
		listaOrdem=a;
	}
}*/

typedef struct TrieTree {
	char valueChar;
	int flag;
	char *word;
	struct ListLine *lines;
	struct TrieTree *v[ALPHABET_SIZE];
} tt;

tt* root = NULL;

tt* createRoot(){
	int i;
	tt* q=(tt*)malloc(sizeof(tt));
	for (i=0; i<ALPHABET_SIZE; i++) q->v[i]=NULL;
	q->flag=0;
	q->lines=NULL;
	return q;
}

tt* createNode(char carac){
	int i;
	tt* q=(tt*)malloc(sizeof(tt));
	for (i=0; i<ALPHABET_SIZE; i++) q->v[i]=NULL;
	q->valueChar=carac;
	q->flag=0;
	q->lines=NULL;
	return q;
}

ll* createNodeList(){
	ll* q=(ll*)malloc(sizeof(ll));
	q->value=0;
	q->next=NULL;
	return q;
}

/*void insertWord(char *palavra, char *palavraOrig) {
	int i, index;
	if (root == NULL) root = createNode();
	tt*q=root;
	for (i=0;i<strlen(palavra)-1; ++i){
		index = palavra[i]-'a';
		if (q->v[index]==NULL) q->v[index]=createNode();
		q = q->v[index];
	}
	if (i==strlen(palavra)-1){
		q->flag=1;
		q->word=(char*)malloc(MAX_WORD);
		strcpy(q->word,palavraOrig);
	}
}*/

void insertWord(char *palavra, char *palavraOrig) {
	int i;
	if (strcmp(palavra,"")==0) return;
	if (root == NULL) root = createRoot();
	tt*current=root;
	for (i=0;i<strlen(palavra); ++i){
		char currentChar = tolower(palavra[i]);
		int index = currentChar-'a';
		assert(index>=0);
		if (current->v[index]!=NULL)
			current = current ->v[index];
		else{
			tt *new = createNode(currentChar);
			current->v[index]=new;
			current=new;
		}
		if (i==strlen(palavra)-1){
			current->flag=1;
			current->word=(char*)calloc(MAX_WORD,sizeof(char));
			strcpy(current->word,palavraOrig);
		}
	}
}


int search(char* palavra){
	tt* current = root;
	int i;
	//if (current==NULL) return 0;
	for (i=0; i < strlen(palavra); ++i){
		char currentChar = tolower(palavra[i]);
		int index = currentChar - 'a';
		assert(index>=0);
		if (current->v[index]!=NULL)
			current=current->v[index];
		else
			return 0;
		if (i==strlen(palavra)-1 && current->flag==0)
			return 0;
	}
	return 1;	
}

tt* searchNode(char* word){
	tt* current = root;
	int i;
	//if (current==NULL) return current;
	for (i=0; i < strlen(word); ++i){
		char currentChar = tolower(word[i]);
		int index = currentChar - 'a';
		assert(index>=0);
		if (current->v[index]!=NULL)
			current=current->v[index];
		else
			return current;
		if (i==strlen(word)-1 && current->flag==0)
			return current;
	}
	return current;	
}

void insertLine(char *palavra, int where){
	tt* q;
	ll* a, *new;
	q=searchNode(palavra);
	if (q->lines==NULL){
		q->lines=createNodeList();
		q->lines->value=where;
	}
	else {
		ll* b;
		for (b=q->lines;b!=NULL;b=b->next) if (b->value==where) return;
		new=createNodeList();
		new->value=where;
		a=q->lines;
		while(a->next!=NULL) a=a->next;
		a->next=new;
	}
}

/*void printIndice(char* palavra){
	int i,index;
	tt *q=root;
	ll *p;
	for (i=0;i<strlen(palavra)-1;i++){
		index = palavra[i] - 'a';
		if (q->v[index]!=NULL) q=q->v[index];
		else break;
	}
	printf("%s ",q->word);
	for (p=q->lines;p!=NULL;p=p->next){
		printf("%d",p->value);
		printf(", ");
	}
	puts("");
}*/

int levenshtein(char *s1, char *s2) {
    unsigned int s1len, s2len, x, y, lastdiag, olddiag;
    s1len = strlen(s1);
    s2len = strlen(s2);
    unsigned int column[s1len+1];
    for (y = 1; y <= s1len; y++)
        column[y] = y;
    for (x = 1; x <= s2len; x++) {
        column[0] = x;
        for (y = 1, lastdiag = x-1; y <= s1len; y++) {
            olddiag = column[y];
            column[y] = MIN3(column[y] + 1, column[y-1] + 1, lastdiag + (s1[y-1] == s2[x-1] ? 0 : 1));
            lastdiag = olddiag;
        }
    }
    return(column[s1len]);
}

void indice(char s1[], int where){
	char *word=strtok(s1,delim);
	while (word!=NULL){
		strlwr(word);
		if (search(word)==1) insertLine(word,where);
		word=strtok(NULL,delim);
	}
}

void printIndice(tt* node, char *prefix){
	int i;
	char currentString[MAX_WORD];
	if (node==NULL) return;
	else if (node->flag==1 && node->lines!=NULL){
		printf("%s ",node->word);
		ll*t;
		for (t=node->lines;t!=NULL;t=t->next) {
			if (t->next!=NULL) printf("%d, ",t->value);
			else printf("%d",t->value);
		}
		puts("");
	}
	for (i=0; i < ALPHABET_SIZE; ++i){
		if (node->v[i]!=NULL){
			strcpy(currentString,prefix+node->v[i]->valueChar);
			printIndice(node->v[i], currentString);
		}
	}
	/*lw* q;
	for (q=listaOrdem;q!=NULL;q=q->nextWord){
		printf("%s ",q->data);
		tt* p;
		p=searchNode(q->data);
		if (p->lines!=NULL){
			printf("%s ",p->word);
			ll*t;
			for (t=p->lines;t!=NULL;t=t->next) {
				if (t->next!=NULL) printf("%d, ",t->value);
				else printf("%d",t->value);
			}
			puts("");
		}
	}*/
}

void alphabetize(tt* node, char *prefix){
	int i;
	char currentString[MAX_WORD];
	if (node->flag==1)
		//insertList(node->word);
		printf("%s\n",node->word);
	for (i=0; i< ALPHABET_SIZE; ++i){
		if (node->v[i]!=NULL){
			strcpy(currentString,prefix+ node->v[i]->valueChar);
			alphabetize(node->v[i], currentString);
		}
	}
}

void suggestion(tt *node, char* prefix, char s1[]){
	int i;
	char comparar[MAX_WORD];
	char currentString[MAX_WORD];
	if (node->flag==1){
		strcpy(comparar,node->word);
		strlwr(comparar);
		if (levenshtein(s1,comparar)<distancia){
			distancia=levenshtein(s1,comparar);
			strcpy(aux,node->word);
		}
		/*else if (levenshtein(s1,comparar)==distancia){
			if (strcmp(aux,comparar)>0){
				strcpy(aux,comparar);
			}
		}*/
	}
	//insertList(node->word);
	//printf("%s\n",node->word);
	if (distancia==1) return;
	else {
		for (i=0; i< ALPHABET_SIZE; ++i){
			if (node->v[i]!=NULL){
				strcpy(currentString,prefix+ node->v[i]->valueChar);
				suggestion(node->v[i], currentString, s1);
			}
		}
	}
}

int main(int argc, char **argv) {
	int linha=1;
	if (argc != 3) {
		printf("Sintaxe: %s dicionario.txt texto.txt\n", argv[0]);
		return 0;
	}

	struct stat dic;
	stat(argv[1],&dic);
	char *buffer = (char*)malloc(dic.st_size);
	
	FILE* dictionary=fopen(argv[1],"r");
	if (dictionary == NULL) {
		printf("Erro, nao foi possivel abrir o dicionario!\n");
		return 0;
	}
	fread(buffer, dic.st_size, 1, dictionary);
	fclose(dictionary);
	
	if (strcmp(buffer,"")==0) return 0;
		
	char *mDic=strtok(buffer,delim);
	char *oDic= (char*)malloc(dic.st_size);
	while (mDic!=NULL){
		strcpy(oDic,mDic);
		strlwr(mDic);
		insertWord(mDic,oDic);
		mDic=strtok(NULL,delim);
	}
	
	free(buffer);
	free(mDic);

	// OPEN TEXT //
	
	struct stat tex;
	stat(argv[2],&tex);
	char *buff = (char*)malloc(tex.st_size);
	
	FILE *text = fopen(argv[2], "r");
	if (text == NULL) {
		printf("Erro, nao foi possivel abrir o texto!\n");
		return 0;
	}
	fread(buff, tex.st_size, 1, text);
	
	if (strcmp(buff,"")==0) return 0;
	
	rewind(text);
	
	char *t1=(char*)malloc(tex.st_size);
	while (fgets(t1, tex.st_size, text) != NULL) {
		strlwr(t1); //converter a string para minuscula
		indice(t1,linha);
		linha++;
	}
	free(t1);
	fclose(text);
	
	char imprimir[MAX_WORD];
	memset(&imprimir,0,MAX_WORD);
	printIndice(root,imprimir);
	
	char *mText=strtok(buff,delim);
	char *oText= (char*)malloc(tex.st_size);
	while (mText!=NULL){
		strcpy(oText,mText);
		strlwr(mText);
		if (search(mText)==0){
			distancia=1000;
			suggestion(root,imprimir, mText);
			printf("%s %s\n",oText,aux);
		}
		mText=strtok(NULL,delim);
	}
	free(mText);
	free(buff);
	return 0;
}
