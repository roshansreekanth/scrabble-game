#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void generate_letters(int number, char *destination){
char vowels[] = {'a', 'e', 'i', 'o', 'u'};
int i;
unsigned int rand_seed;
printf("Number < 1000:\n");
scanf("%d", &rand_seed); // initialise the PRNG
getchar();
srand(rand_seed);
// Half of the letters should be vowels
for (i=0;i<number/2;i++, destination++) *destination = vowels[rand()%5];
for (;i<number;i++, destination++) *destination = rand()%26+'a';
}

void get_letter_scores(int *scores) 
{
    FILE *file = fopen("letter_values", "r");

    if((file = fopen("letter_values", "r")) == NULL)  return;
    for(int i = 0; i < 26; i++)
    {
        if(fscanf(file, "%d", &scores[i]) == EOF) break;
    }    
}

void print_letter_count(char *letters, int *letter_count_array)
{
    char used_letters[7];
    int used_letter_counter = 0;
    for(int i = 0; i < 7; i++)
    {
        char letter = letters[i];
        int letter_already_checked = 0;

        for(int i = 0; i < 7; i++)
        {
            if(letter == used_letters[i])
            {
                letter_already_checked = 1;
            }
        }
        if(letter_already_checked == 0)
        {
            printf("%c --> %d\n", letter, letter_count_array[letter - 'a']);
            used_letters[used_letter_counter] = letter;
            used_letter_counter++;
        }
    }
}

int get_word_length(char *word)
{
    int length = 0;
    int counter = 0;

    while(word[counter] != '\0')
    {
        length++;
        counter++;
    }
    return length;
}

int check_letter_valid(char c, char *letters)
{
    for(int i = 0; i < 7; i++)
    {   
        if(c == letters[i])
        {
            return 1;
        }
    }
    return 0;
}

int get_word_score(char *word, int *score_list)
{
    int i = 0;
    int score = 0;

    for(int i = 0; i < get_word_length(word); i++)
    {
        score += score_list[word[i] - 'a'];
    }

    return score;
}

void get_letter_count(char *letters, int *letter_count_array)
{
    for(int i = 0; i < 26; i++)
    {
        letter_count_array[i] = 0;
    }

    for(int i = 0; i < 7; i++)
    {
        char letter = letters[i];
        letter_count_array[letter - 'a'] += 1; 
    }
}

int check_word_valid(char *word, int *letter_occurence_array)
{
    int word_occurence_count[26];
    for(int i = 0; i < 26; i++)
    {
        word_occurence_count[i] = 0;
    }

    for(int i = 0; i < get_word_length(word); i++)
    {
        word_occurence_count[word[i] - 'a'] += 1;
    }

    for(int i = 0; i < get_word_length(word); i++)
    {
        int index = word[i] - 'a';
        if(word_occurence_count[index] > letter_occurence_array[index])
        {
            return 0;
        }
    }

    return 1;
}

void main(void)
{
    int no_of_letters = 7;
    int indexed_scores[26];
    int letter_occurence_array[26];
    int total_letter_count = 0;

    char letters[7];    
    char words[5040][8]; // 7! = 5040, which is the maximum combination of words possible with 7 letters
    //10 words is enough

    int word_scores[5040];

    generate_letters(no_of_letters, letters);
    get_letter_scores(indexed_scores);
    get_letter_count(letters, letter_occurence_array);

    char continue_choice = 'y';
    int word_counter = 0;// Counter tracking the length of the list
    int total_score = 0;

    while(continue_choice != 'n')
    {
        print_letter_count(letters, letter_occurence_array);
        char input = 0;
        int word_length = 0; // Counter tracking the length of the word
        int valid_flag = 1;
        int word_exists = 0;

        printf("Input word:\n");
        while(input != '\n')
        {
            input = getchar();
            if(input >= 'A' && input <= 'Z')
            {
                input += 32;
            }
            
            if(input != '\n')
            {

                if(check_letter_valid(input, letters) == 0)
                {
                    printf("Letter %c is not available\n", input);
                    valid_flag = 0;
                    fflush(stdin);
                    break;
                }
                
                words[word_counter][word_length] = input;
                word_length++;
            }
        }

        if(valid_flag)
        {
            words[word_counter][word_length] = '\0';
            if(check_word_valid(words[word_counter], letter_occurence_array))
            {
                for(int i = 0; i < word_counter; i++)
                {
                    if(strcmp(words[word_counter], words[i]) == 0)
                    {
                        word_exists = 1;
                    }
                }

                if(word_exists)
                {
                    printf("%s has already been entered\n", words[word_counter]);
                }
                else
                {
                    int score = get_word_score(words[word_counter], indexed_scores);
                    total_score += score;
                    printf("You used %s. Worth %d. Current total %d\n", words[word_counter], score, total_score);
                    word_scores[word_counter] = score;
                    total_letter_count += get_word_length(words[word_counter]);
                    word_counter++;
                }
                
            }
            else
            {
                printf("Not enough letters available to make: %s\n", words[word_counter]);
            }
            
        }
        printf("Would you like to continue? (y/n)\n");
        scanf("%c", &continue_choice);
        getchar(); // Clear input buffer \n
    }

    printf("You provided the following words:\n");

    for(int i = 0; i < word_counter; i++)
    {
        printf("%s ---> %d\n", words[i], word_scores[i]);
    }
    
    printf("Total score: %d\n", total_score);
    printf("Total number of letters entered: %d\n", total_letter_count);
}
