#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_TEXT 2000
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

struct WordInfo {
    char word[MAX_WORD_LEN];
    int count;
    int length;
};

struct WordInfo dictionary[MAX_WORDS];
int total_words = 0;
char user_text[MAX_TEXT] = "";

void clear_input();
void make_lowercase(char word[]);
void clean_word(char word[]);
void process_text();
void show_dictionary();
void find_exact_word();
void find_partial_word();
void sort_words();
void sort_alphabetically();
void sort_by_count();
void sort_by_length();

int main() {
    int choice;
    
    printf("=== Text Analyzer And Search Engine ===\n");
    
    do {
        printf("\nMain Menu:\n");
        printf("1. Enter and analyze text\n");
        printf("2. Show all words\n");
        printf("3. Find exact word\n");
        printf("4. Find partial word\n");
        printf("5. Sort words\n");
        printf("6. Exit\n");
        printf("Your choice: ");
        scanf("%d", &choice);
        clear_input();
        
        switch(choice) {
            case 1:
                printf("Enter your text (max %d characters):\n", MAX_TEXT);
                fgets(user_text, MAX_TEXT, stdin);
                user_text[strcspn(user_text, "\n")] = '\0';
                process_text();
                printf("Text analyzed! Found %d words.\n", total_words);
                break;
            case 2:
                show_dictionary();
                break;
            case 3:
                find_exact_word();
                break;
            case 4:
                find_partial_word();
                break;
            case 5:
                sort_words();
                break;
            case 6:
                printf("Goodbye! Hope to see you soon!\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while(choice != 6);
    
    return 0;
}

void clear_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void make_lowercase(char word[]) {
    for (int i = 0; word[i] != '\0'; i++) {
        word[i] = tolower(word[i]);
    }
}

void clean_word(char word[]) {
    char clean[MAX_WORD_LEN] = "";
    int j = 0;
    
    for (int i = 0; word[i] != '\0'; i++) {
        if (isalpha(word[i])) {
            clean[j] = word[i];
            j++;
        }
    }
    clean[j] = '\0';
    strcpy(word, clean);
}

void process_text() {
    
    char temp_text[MAX_TEXT];
    strcpy(temp_text, user_text);
    
    char current_word[MAX_WORD_LEN];
    int word_start = 0;
    int text_length = strlen(temp_text);
    
    for (int i = 0; i <= text_length; i++) {
        if (temp_text[i] == ' ' || temp_text[i] == '\0' || 
            temp_text[i] == '.' || temp_text[i] == ',' || 
            temp_text[i] == '!' || temp_text[i] == '?' || 
            temp_text[i] == ';' || temp_text[i] == ':') {
            
            if (i > word_start) {
                int word_len = i - word_start;
                if (word_len < MAX_WORD_LEN) {
                    strncpy(current_word, &temp_text[word_start], word_len);
                    current_word[word_len] = '\0';
                    
                    make_lowercase(current_word);
                    clean_word(current_word);
                    
                    if (strlen(current_word) > 0) {
                        int found = 0;
                        for (int j = 0; j < total_words; j++) {
                            if (strcmp(dictionary[j].word, current_word) == 0) {
                                dictionary[j].count++;
                                found = 1;
                                break;
                            }
                        }
                        
                        if (!found && total_words < MAX_WORDS) {
                            strcpy(dictionary[total_words].word, current_word);
                            dictionary[total_words].count = 1;
                            dictionary[total_words].length = strlen(current_word);
                            total_words++;
                        }
                    }
                }
            }
            word_start = i + 1;
        }
    }
}

void show_dictionary() {
    if (total_words == 0) {
        printf("No text analyzed yet. Please enter text first.\n");
        return;
    }
    
    printf("\nDictionary (%d words):\n", total_words);
    printf("Word\t\tCount\tLength\n");
    printf("----------------------------\n");
    
    for (int i = 0; i < total_words; i++) {
        printf("%-15s\t%d\t%d\n", 
               dictionary[i].word, 
               dictionary[i].count, 
               dictionary[i].length);
    }
}

void find_exact_word() {
    if (total_words == 0) {
        printf("No text analyzed yet. Please enter text first.\n");
        return;
    }
    
    char search_word[MAX_WORD_LEN];
    printf("Enter word to find: ");
    scanf("%s", search_word);
    clear_input();
    
    make_lowercase(search_word);
    clean_word(search_word);
    
    for (int i = 0; i < total_words; i++) {
        if (strcmp(dictionary[i].word, search_word) == 0) {
            printf("Found: %s (Count: %d, Length: %d)\n", 
                   dictionary[i].word, dictionary[i].count, dictionary[i].length);
            return;
        }
    }
    
    printf("Word '%s' not found.\n", search_word);
}

void find_partial_word() {
    if (total_words == 0) {
        printf("No text analyzed yet. Please enter text first.\n");
        return;
    }
    
    char partial[MAX_WORD_LEN];
    printf("Enter partial word to search: ");
    scanf("%s", partial);
    clear_input();
    
    make_lowercase(partial);
    int found_any = 0;
    
    printf("Words containing '%s':\n", partial);
    for (int i = 0; i < total_words; i++) {
        if (strstr(dictionary[i].word, partial) != NULL) {
            printf("- %s (Count: %d, Length: %d)\n", 
                   dictionary[i].word, dictionary[i].count, dictionary[i].length);
            found_any = 1;
        }
    }
    
    if (!found_any) {
        printf("No words found containing '%s'\n", partial);
    }
}

void sort_words() {
    if (total_words == 0) {
        printf("No text analyzed yet. Please enter text first.\n");
        return;
    }
    
    int choice;
    printf("\nSort Options:\n");
    printf("1. Alphabetical order\n");
    printf("2. By count\n");
    printf("3. By length\n");
    printf("Your choice: ");
    scanf("%d", &choice);
    clear_input();
    
    switch(choice) {
        case 1:
            sort_alphabetically();
            printf("Sorted alphabetically.\n");
            show_dictionary();
            break;
        case 2:
            sort_by_count();
            printf("Sorted by count.\n");
            show_dictionary();
            break;
        case 3:
            sort_by_length();
            printf("Sorted by length.\n");
            show_dictionary();
            break;
        default:
            printf("Invalid choice.\n");
    }
}

void sort_alphabetically() {
    for (int i = 0; i < total_words - 1; i++) {
        for (int j = i + 1; j < total_words; j++) {
            if (strcmp(dictionary[i].word, dictionary[j].word) > 0) {
                struct WordInfo temp = dictionary[i];
                dictionary[i] = dictionary[j];
                dictionary[j] = temp;
            }
        }
    }
}

void sort_by_count() {
    for (int i = 0; i < total_words - 1; i++) {
        for (int j = i + 1; j < total_words; j++) {
            if (dictionary[i].count < dictionary[j].count) {
                struct WordInfo temp = dictionary[i];
                dictionary[i] = dictionary[j];
                dictionary[j] = temp;
            }
        }
    }
}

void sort_by_length() {
    for (int i = 0; i < total_words - 1; i++) {
        for (int j = i + 1; j < total_words; j++) {
            if (dictionary[i].length > dictionary[j].length) {
                struct WordInfo temp = dictionary[i];
                dictionary[i] = dictionary[j];
                dictionary[j] = temp;
            }
        }
    }
}