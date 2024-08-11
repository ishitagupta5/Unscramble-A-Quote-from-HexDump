#include "word.h"

// ishitagupta :)

uint32_t strlength(const char *const word)
{
    uint32_t len = 0;

    while (word[len] != '\0')
    {
        len++;
    }

    return len;
}

void strcopy(char *const destination, const char *const source)
{
    uint32_t placeholder = 0;

    while (source[placeholder] != '\0')
    {
        destination[placeholder] = source[placeholder];
        placeholder++;
    }

    destination[placeholder] = '\0';
}

// pointer to a struct Word named word, intializes the char* word to NULL and uint16_t offset = 0
void init_word(struct Word *word)
{
    word->word = NULL;
    word->offset = 0;
}

// checks if char* is NULL, if not frees it.
// allocates memory for pointer with space for null terminator
// use strlength & strcopy to get length of wrd, then copy into array
void update_word(struct Word *word, const char *const wrd, uint16_t offset)
{
    if (word->word != NULL)
    {
        free(word->word);
    }

    word->word = malloc((strlength(wrd) + 1) * sizeof(char));
    strcopy(word->word, wrd);
    word->offset = offset;
}

// output the word
void display_word(FILE *out, struct Word word)
{
    fprintf(out, "%-20s", word.word);
    fprintf(out, " @ Offset: %02x\n", word.offset);
}

// frees the memory
void free_word(struct Word word)
{
    free(word.word);
}

// store size, initalize the count, size field represents how large it currently is
// count is how many Word objects in array
// words pointer is the array for the Word Struct objects
void init_list(struct Word_List *word_list, uint16_t size)
{
    word_list->words = malloc(size * sizeof(struct Word));
    word_list->size = size;
    word_list->count = 0;
}

// add the array after the last element
// when size == count, relloc to double the size of an array
void add_word(struct Word_List *word_list, struct Word word)
{

    if (word_list->count == word_list->size)
    {
        word_list->size *= 2;
        word_list->words = realloc(word_list->words, word_list->size * sizeof(struct Word));
    }

    word_list->words[word_list->count] = word;
    word_list->count++;
}

// supplied index which is atleast 0 and less than the count of the words
// copy of the word at the index is returned
// if index is invalid, then free the memory
struct Word get_word_at_index(struct Word_List word_list, uint16_t index)
{
    struct Word result;

    if (index < word_list.count)
    {
        init_word(&result);
        update_word(&result, word_list.words[index].word, word_list.words[index].offset);
    }

    else
    {
        init_word(&result);
        update_word(&result, "Word Not Found", 0);
    }

    return result;
}

// free the memory
void free_list(struct Word_List word_list)
{
    for (uint16_t i = 0; i < word_list.count; i++)
    {
        free_word(word_list.words[i]);
    }
    free(word_list.words);
}

// loop through list and display
void display_words(FILE *out, struct Word_List word_list)
{
    for (uint16_t i = 0; i < word_list.count; i++)
    {
        display_word(out, word_list.words[i]);
    }
}