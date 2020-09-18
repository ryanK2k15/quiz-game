/**
* @Author Ryan Kelleher
* @StudentID R00069805
* @Date 03/05/2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

char* readFileContents(char *filename);
int numOfQuestions(char* str);
char* lowercase(char* str);
void writeToFile(char *filename, int score, int numQuestions, int level);
void displayClue(int level, char* ans);
void swap (int *a, int *b);
void randomize (int arr[], int n);
typedef struct {char* question; char* answer;}Quiz;

int main(int argc, char *argv[])  {

    /*declare and initialise variables*/

    //read questions and answers from the file
	char* fileContents = readFileContents(argv[1]);
	//get the number of questions in the file
	int numQuestions = numOfQuestions(fileContents);

	//get difficulty level
	int level = atoi(argv[2]);

	if(level == 6){
        //clue is randomly generated from the 5 difficulty levels
        srand(time(NULL));
        level = rand()%((5+1)-1) + 1;
	}

	//dynamically allocated array of structs to store qusetions and answers
	Quiz* quiz = malloc(numQuestions * sizeof *quiz);

	char *userAnswer = (char*)malloc(26 * sizeof(char));

    int score = 0;

    char* token = NULL;
    //split the first question and answer in the file
    token = strtok(fileContents, "?");
    //store the first question
    quiz[0].question = token;

    int i = 0;
    int j = 1;
    //split the rest of the questions and answers in the file
    while (token != NULL)
    {
        token = strtok(NULL, "\n ");
        quiz[i].answer = token;
        quiz[j].question = strtok(NULL, "?");
        i++;
        j++;
    }
    quiz[i].question = '\0';//add null terminator
    quiz[i].answer = '\0';//add null terminator

    int index = 0;//keeps track of the current question and answer

    int MAXWORDSIZE = 20;

    //keep track of the correct answers
    char **correctAnswers = (char**)malloc(26 * sizeof(char*));
    for(i = 0; i < MAXWORDSIZE; i++)
        correctAnswers[i] =  (char*)malloc(26 * sizeof(char));
    int numCorrect = 0;

    //keep track of the incorrect answers
    char **incorrectAnswers = (char**)malloc(26 * sizeof(char*));
    for(i = 0; i < MAXWORDSIZE + 1; i++)
        incorrectAnswers[i] =  (char*)malloc(26 * sizeof(char));
    int numIncorrect = 0;


    //questions are asked in random order
    srand(time(0));
    int random[numQuestions];
    for (i = 0; i < numQuestions; i++){
        random[i] = i;
    }
    int n = sizeof(random)/ sizeof(random[0]);
    //randomize function generates random permutation of the array
    randomize(random, n);

    printf("Difficulty Level: %d\n", level);
    printf("Total Questions: %d\n", numQuestions);

    do{
        //get length of the answer
        int length = strlen(quiz[random[index]].answer);

        printf("\n%s\n", quiz[random[index]].question);

        //display a clue depending on the difficulty level
        displayClue(level, quiz[random[index]].answer);

        //get user's answer
        printf("%s", "Your Answer: ");
        scanf("%s", userAnswer);

        //case insensitive
        userAnswer = lowercase(userAnswer);
        quiz[random[index]].answer = lowercase(quiz[random[index]].answer);

        //check if the user's answer is correct
        if(strcmp(quiz[random[index]].answer, userAnswer) == 0) {
                printf("Correct!\n");
                //keep track of correctly answered questions
                score++;
                //store correct answers
                strncpy(correctAnswers[numCorrect], quiz[random[index]].answer, length);
                numCorrect++;
        }
        else{
            printf("Incorrect!\n");
            //store incorrect answers
            strncpy(incorrectAnswers[numIncorrect], quiz[random[index]].answer, length);
            numIncorrect++;
        }

        index++;

    }while(index != numQuestions);

    printf("\nScore: %d/%d\n", score, numQuestions);

    char choice = 0;

    //ask user if they want to view the correct answers to the questions they got wrong
    printf("\nDo you want to view correct answers? y/n: ");
    scanf(" %c", &choice);
    if(choice == 'y'){
        printf("\nCorrect Answers to questions that were wrong: ");
        for(i = 0; i < numIncorrect; i++){
            printf("\n%s ", incorrectAnswers[i]);
        }
        printf("\n");
    }

    //write the results to file
    writeToFile("C:\\Users\\ryank\\Programming\\C Programming\\quiz_history.txt", score, numQuestions, level);

	return 0;
}

int numOfQuestions(char* s){
    int count = 0;
    int i;
    for(i = 0; s[i]; i++){
        if(s[i] == '?'){
            count++;
        }
    }
    return count;
}

void displayClue(int level, char* ans){
    int length = strlen(ans);
    int i = 0;

    //Only a ? is shown
    if(level == 1){
        printf("?\n");
    }

    //blank dashes seperated by spaces for every letter in the answer
    if(level == 2){
        for(i = 0; i < length; i++){
            putchar('-');
            putchar(' ');
        }
        printf("\n");
    }

    //first and last letters are shown
    if(level == 3){
        putchar(ans[0]);
        for(i = 1; i < length-1; i++){
            putchar(' ');
            putchar('-');
            putchar(' ');
        }
        putchar(ans[length-1]);
        printf("\n");
    }

    //Two random letters from the answer are shown in their correct position
    if(level == 4){
        srand(time(NULL));
        int upper = length - 1;
        int lower = 0;
        //generate 2 random numbers between 0 and length of answer - 1
        int r1 = (rand() % (upper - lower + 1)) + lower;
        int r2 = (rand() % (upper - lower + 1)) + lower;

        //if random numbers are the same generate a new number until they are not equal
        while(r1 == r2)
            r1 = (rand() % (upper - lower + 1)) + lower;

        i = 0;
        while(ans[i] != '\0') {
            //print first random letter
            if(i == r1)
                printf("%c ", toupper(ans[i]));
            //print second random letter
            if(i == r2)
                printf("%c ", toupper(ans[i]));
            //print dashes
            if(i != r1 && i != r2)
                printf("- ");
            i++;
        }
        printf("\n");
    }

    //all letters are shown in random order
    if(level == 5){
        int r[length];
        for (int i = 0; i < length; i++)
            r[i] =  i;
        randomize(r, length);
        for(i = 0; i < length; i++){
            printf("%c ", toupper(ans[r[i]]));
        }
        printf("\n");
    }

}

char* lowercase(char* str){
    for(int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
    return str;
}

char* readFileContents(char *filename){
    char *fileContents;
    long file_size;
    FILE *file = fopen(filename, "r");
    if(file == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);
    fileContents = (char*)malloc((file_size+1) * (sizeof(char)));
    fread(fileContents, file_size, 1, file);
    fileContents[file_size] = 0;
    fclose(file);

    return fileContents;

}

void writeToFile(char *filename, int score, int numQuestions, int level){
   FILE *fptr;

   fptr = fopen(filename,"a");

   if(fptr == NULL)
   {
      printf("Error Opening File!");
      exit(1);
   }

   fprintf(fptr,"quiz_history");
   fprintf(fptr," Score: %d", score);
   fprintf(fptr," Total Questions: %d", numQuestions);
   fprintf(fptr," Difficulty Level: %d\n", level);

   printf("\nResults Written to File - quiz_history.txt\n");

   fclose(fptr);
}

// A utility function to swap to integers
void swap (int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

// A function to generate a random permutation of arr[]
void randomize (int arr[], int n){
    // Use a different seed value so that we don't get same
    // result each time we run this program
    srand ( time(NULL) );

    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    for (int i = n-1; i > 0; i--)
    {
        // Pick a random index from 0 to i
        int j = rand() % (i+1);

        // Swap arr[i] with the element at random index
        swap(&arr[i], &arr[j]);
    }
}
