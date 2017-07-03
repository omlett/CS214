/*
//	Pranav Katkamwar
//	Assign0 - Fall 2016 - CS:214
//	readme.pdf, testcases.txt, pointersorter.c
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* define struct //
/*/

typedef struct Node {
	char *word;
	struct Node *next;
} Node;
	
Node *newNode(char *word) {
	Node *node = malloc(sizeof(Node));
	node->word = word;
	node->next = NULL;
	return node;
}


/* convert linked list of words to array of words //
/*/

char **newArray(Node *head, int length) {
	Node *ptr = head;
	Node *prev = NULL;
	int count = length;
	char **list = malloc(sizeof(char *)*length);
	while (count > 0) {
		prev = ptr;
		list[length-count] = ptr->word;
		ptr = ptr->next;
		
		count--;

		free (prev);
	}

	return list;
}

/* sort via selection sort //
/*/

char **magicHappens(char **list, int total_words){
	int x = 0;
	int y = 0;

	for (x = 0; x <= total_words; x++){
		int begin = x;
		
		for (y = x + 1; y <= total_words; y++){
			int lexcheck = strcmp(list[begin], list[y]);
			
			if (lexcheck > 0){
				begin = y;
			}
		}
		
		if (begin != x) {
			char *temp = list[x];
			list[x] = list[begin];
			list[begin] = temp;
		}
	}
	
	return list;
}

/* print a list of words //
/*/

void printList(char **list, int list_length) {
	int x = 0;

	for (x = 0; x <= (list_length - 1); x++) {
		printf("%s\n", list[x]);
	}
}


/* get words from the input string, send them to be sorted, print them //
/*/

void weedout(char *iString) {
	char *tracker = iString;
	char *wordID = iString;

	Node *head = NULL;
	int totalWords = 0;
	
	while (*wordID!= '\0') {
		while (isalpha(*tracker)) {
			tracker++;
		}

	int wordLength = tracker - wordID;
	
	if (wordLength != 0) {
		char *word = (char *) malloc(wordLength + 1);
		memcpy(word, wordID, wordLength);
		word[wordLength] = '\0';
		Node *hold = newNode(word);
		hold->next = head;
		head = hold;
		
		totalWords++;
	}

	if (*tracker == '\0') {
		// printf("\n tracker finished.\n");
		break;
	}

	wordID = tracker;
	tracker++;
	wordID++;
	}

	char **list = magicHappens(newArray(head, totalWords), totalWords-1);
	printList(list, totalWords);
	
}

/* Main - handles basic incorrect entries
/*/
	
int main(int argc, char ** argv) {

	if (argc != 2){
		printf("\nBad Input\n");
		return 1;
	}
	else{
		weedout(argv[1]);
	}

	return 0;
}
