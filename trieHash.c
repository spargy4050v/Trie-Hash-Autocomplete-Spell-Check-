#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define ALPHABET_SIZE 26
#define TABLE_SIZE 1000
int i;
typedef struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    int isEndOfWord;
} TrieNode;
TrieNode* createNode() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->isEndOfWord = 0;
    for (i = 0; i < ALPHABET_SIZE; i++)
        node->children[i] = NULL;
    return node;
}
void insertWord(TrieNode* root, const char* word) {
    TrieNode* curr = root;
    for (i = 0; word[i] != '\0'; i++) {
        char c = tolower(word[i]);
        if (!isalpha(c)) continue;
        int index = c - 'a';
        if (!curr->children[index])
            curr->children[index] = createNode();
        curr = curr->children[index];
    }
    curr->isEndOfWord = 1;
}
int searchWord(TrieNode* root, const char* word) {
    TrieNode* curr = root;
    for (i = 0; word[i] != '\0'; i++) {
        char c = tolower(word[i]);
        if (!isalpha(c)) continue;
        int index = c - 'a';
        if (!curr->children[index])
            return 0;
        curr = curr->children[index];
    }
    return curr->isEndOfWord;
}
typedef struct Suggestion {
    char word[100];
    struct Suggestion* next;
} Suggestion;
Suggestion* addSuggestion(Suggestion* head, const char* word) {
    Suggestion* newNode = (Suggestion*)malloc(sizeof(Suggestion));
    strcpy(newNode->word, word);
    newNode->next = head;
    return newNode;
}
void getSuggestionsHelper(TrieNode* root, char* prefix, Suggestion** list) {
    if (root->isEndOfWord)
        *list = addSuggestion(*list, prefix);
    for (i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i]) {
            char newPrefix[100];
            sprintf(newPrefix, "%s%c", prefix, i + 'a');
            getSuggestionsHelper(root->children[i], newPrefix, list);
        }
    }
}
void getSuggestions(TrieNode* root, const char* prefix) {
    TrieNode* curr = root;
    for (i = 0; prefix[i] != '\0'; i++) {
        char c = tolower(prefix[i]);
        if (!isalpha(c)) continue;
        int index = c - 'a';
        if (!curr->children[index]) {
            printf("No suggestions found for \"%s\".\n", prefix);
            return;
        }
        curr = curr->children[index];
    }
    Suggestion* list = NULL;
    getSuggestionsHelper(curr, (char*)prefix, &list);
    if (!list) {
        printf("No suggestions found for \"%s\".\n", prefix);
        return;
    }
    printf("Suggestions for \"%s\":\n", prefix);
    Suggestion* s;
    for (s = list; s; s = s->next)
        printf("  %s\n", s->word);
}
typedef struct HashNode {
    char word[100];
    struct HashNode* next;
} HashNode;
HashNode* hashTable[TABLE_SIZE] = { NULL };
unsigned int hashFunction(const char* word) {
    unsigned int hash = 0;
    for (i = 0; word[i]; i++)
        hash = hash * 31 + word[i];
    return hash % TABLE_SIZE;
}
void insertHash(const char* word) {
    unsigned int idx = hashFunction(word);
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    strcpy(newNode->word, word);
    newNode->next = hashTable[idx];
    hashTable[idx] = newNode;
}
int checkHash(const char* word) {
    unsigned int idx = hashFunction(word);
    HashNode* n;
    for (n = hashTable[idx]; n; n = n->next)
        if (strcmp(n->word, word) == 0)
            return 1;
    return 0;
}
int main() {
    TrieNode* root = createNode();
	char* words[] = {
        "apple", "application", "apply",
        "banana", "bat", "ball",
        "cat", "cap", "can"
    };
    int n = sizeof(words) / sizeof(words[0]);
	for (i = 0; i < n; i++) {
        insertWord(root, words[i]);
        insertHash(words[i]);
    }
    char input[100];
    printf("Enter a prefix to get suggestions: ");
    scanf("%s", input);
    getSuggestions(root, input);
    printf("\nEnter a word to spell check: ");
    scanf("%s", input);
    if (checkHash(input))
        printf("? '%s' is spelled correctly.\n", input);
    else
        printf("? '%s' is not found in dictionary.\n", input);
    return 0;
}