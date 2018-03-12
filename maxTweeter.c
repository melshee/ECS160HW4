#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdarg.h>

//define the struct that represents the Tweeter and the number of tweets
typedef struct Tweet {
    char *name;
    int count;
} Tweet;

//taken from the given example code  
const char* getfield(char* line, int num){
    const char* tok = strtok(line, ",");
    int count = 1;

    while(tok != NULL) {

	if(count == num)
	    return tok;

	tok = strtok(NULL, ",\n");
        count++;
    }
    
    printf("Invalid Input Format");
    exit(0);
}

//gets the correct column of names 
int getIndex(char *line) { 
    int col = 0;
    const char *tok = strtok(line, ",");
    
    //if the line is empty invalid file
    if (line==NULL){
            printf("Invalid Input Format");
            exit(0);
    }
    
    //loop through the line
    while (tok != NULL) {
            
            //if "name" is found return that col
            if (strcmp(tok, "\"name\"") == 0) 
                    return col+1;
            
            col++;	  	
            tok = strtok(NULL, ",\n"); 	
    }
    
    //if theres no name then invalid file
    printf("Invalid Input Format");
    exit(0);
}

//finds the name and increments the count in the struct
bool getName(char *name, Tweet** myTweets, int length) {
    
    //length is not possible dont change anything
    if(length<=0){
            return false;
    }
    
    //name is not valid dont change anything
    if (name==NULL) {
            return false;
    }
    
    //mytweets is empty dont change anything
    if (myTweets==NULL) {
            return false;
    }
    
    //find the name, if its the same increment the count
    for (int i = 0; i < length; i++) {
            if (strcmp((*myTweets[i]).name, name) == 0) {
                    (*myTweets[i]).count++;
                    return true;
            }
    }
    
    //if its not in the list somehow, dont change anything
    return false;
}

//prints out the top 10 tweets
void printTweets(int length, Tweet **myTweets) {
    
    //sort the array
    int i,j;
    for (int i = 0; i < length; ++i) {                
            for (int j = i+1; j < length; ++j) {
                    
                    if ((*myTweets[j]).count > (*myTweets[i]).count) {
                            Tweet *temp = myTweets[i];       
                            myTweets[i] = myTweets[j];          
                            myTweets[j] = temp;           
                    }
            }
    }

    if(length > 10){
            length = 10;
    }

    //print it out
    for (int i = 0; i < length; i++) {
            printf("<%-20s>:", (*myTweets[i]).name);
            printf("<%d>", (*myTweets[i]).count);
            printf("\n");
    }
}


int main(int argc, char* argv[]) {

    //check to see if there are the right amount of arguments
    if (argc != 2) {
        printf("Invalid Input Format");
        exit(0);
    }
    
    //create the file based on the argument
    FILE *myFile = fopen(argv[1], "r");
    
    //check to see if the file was found
    if (myFile==NULL) {
        printf("Invalid Input Format");
        exit(0);
    }
    
    Tweet **myTweets = malloc(sizeof(Tweet*) * 20000);
    
    int length = 0;
    char tempName[50];
    Tweet *temp = NULL;
    char line[2000];
    
    if(!fgets(line, 2000, myFile)) { 
        printf("Invalid Input Format");
        exit(0);
    }
    int nameCol = getIndex(line);

    //go through the whole file
    while (fgets(line, 20000, myFile)) {
        
        //get the name from the fist name column
        strcpy(tempName, getfield(line, nameCol));
        
        //compare if the name is in the list already if not make a new entry
        if (getName(tempName, myTweets, length)) {
                //dont do anything count has been incremented for the match
        } else {
            //create the new tweet 
            Tweet *temp = malloc(sizeof(Tweet));
            (*temp).name = malloc(strlen(tempName) + 1);
            strcpy((*temp).name, tempName);
            (*temp).count = 1;
            
            //put it at the end of the tweet list
            myTweets[length] = temp;
            
            //increment length of mytweets
            length++;
        }
    }

    printTweets(length,myTweets);

    //free the memory, idk think its a c thing
    free(myTweets);
    fclose(myFile);
}
