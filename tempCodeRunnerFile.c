#include<stdio.h>
#include <string.h>

void wordSentenceCounter(FILE *input, char *word, char *word2, char c, int wordcount, int sentencecount) {
    printf("Enter a word: ");
    scanf("%s", word);
    input = fopen("input.txt", "r");
    while(!feof(input)){
        fscanf(input, " %[^. ]", word2);
        if(strcasecmp(word, word2) == 0)
            wordcount++;
        c = fgetc(input);
        if(c == '.')
            sentencecount++;
    }
    fclose(input);
    printf("word appearances: %d\n", wordcount);
    printf("sentences: %d", sentencecount);
}

int main() {
    FILE *input;
    char word[50], word2[50], c;
    int wordcount=0, sentencecount=0, i=0;
    wordSentenceCounter(input, word, word2, c, wordcount, sentencecount);
    return 0;
}

