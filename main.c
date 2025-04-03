///Obayda Sarraj
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 31 // Maximum length of the word

// Define a node structure for a doubly linked list
typedef struct node* ptr ;
struct node {
    char data[MAX_LENGTH];
    ptr next ;
    ptr prev ;
};
typedef struct node* LinkedList ;
typedef struct node* pos ;

// Function prototypes
LinkedList creation();
void insertLast(char Line[] , LinkedList l);
int load(LinkedList list);
void printList(LinkedList list);
void addSpaces(int maxSize , LinkedList list);
void removeSpaces(int maxSize , LinkedList list);
LinkedList radixSort(int maxSize , LinkedList listBeforeSorting);
void clearList(LinkedList list);
LinkedList addWord(int maxSize , LinkedList list);
void deleteWord(int maxSize , LinkedList list);
void deleteNode(char word[] , LinkedList  list);
void printInFile(LinkedList list);
LinkedList copy(LinkedList list);

int main() {
    LinkedList myList = creation();
    LinkedList sorted = creation();

    int maxSize ;
    int numOfOperation = 0 ;

    // Menu driven interface for user operations
    do
    {
        printf("\nPlease select one of operations:\n");
        printf("1. Load the strings\n");
        printf("2. Print the strings before sorting\n");
        printf("3. Sort the strings\n");
        printf("4. Print the sorted strings\n");
        printf("5. Add a new word to the list\n");
        printf("6. Delete a word from the list\n");
        printf("7. Save to output file\n");
        printf("8. Exit\n\n");
        scanf("%d", &numOfOperation);

        switch (numOfOperation)
        {
            case 1:
                maxSize = load(myList);
                printf("The File has been loaded\n");
                break;

            case 2:
                printList(myList);
                break;

            case 3:
                sorted = radixSort(maxSize , myList);
                printf("The Data has been sorted\n");
                break;

            case 4:
                printList(sorted);
                break;

            case 5:
                sorted = addWord(maxSize , sorted);
                break;

            case 6:
                deleteWord(maxSize , sorted);
                break;

            case 7:
                printInFile(sorted);
                printf("The Data has been printed to output file\n");
                break;

            case 8:
                printf("Exiting program. Goodbye!\n");
                break;

            default:
                printf("No such operation!  please try again.\n");
                break;
        }
    }while(numOfOperation != 8);
    return 0;
}

// Function to create list an empty list
LinkedList creation()
{
    // Reserve a place in memory
    LinkedList list = (LinkedList)malloc(sizeof(struct node));
    if (list == NULL)
    {
        printf("Out of memory!");
        return NULL;
    }
    // Make the pointers to point on NULL
    list->next = NULL;
    list->prev = NULL;
    return list;
}

// Function to create a copy of list
LinkedList copy(LinkedList list)
{
    LinkedList copy = creation();
    pos p = list->next ;
    while(p != NULL)
    {
        insertLast(p->data,copy);
        p = p->next ;
    }
    return copy ;
}

// Function to insert nodes at the end of the list
void insertLast(char Line[], LinkedList l)
{
    // Reserve a place in memory
    LinkedList newNode = (LinkedList)malloc(sizeof(struct node));
    if (newNode == NULL) {
        printf("Out of memory!");
        return;
    }

    strcpy(newNode->data, Line);


    pos p = l;
    while (p->next != NULL) {
        p = p->next;
    }

    newNode->next = NULL;
    newNode->prev = p;
    p->next = newNode;
}

// Function to load all data in file "input"
int load(LinkedList list)
{
    FILE *read = fopen("input.txt","r");
    if(read == NULL) {
        printf("Error: input.txt file not found!\n");
        return 0;
    }

    char Line[MAX_LENGTH];
    int maxSize = 0 ;
    int isEmpty = 1;

    while (fgets(Line, sizeof(Line), read) != NULL)
    {
        isEmpty = 0;
        // Remove trailing newline if present
        if (Line[strlen(Line) - 1] == '\n') {
            Line[strlen(Line) - 1] = '\0';
        }
        if((Line[0] >= 'A' && Line[0] <= 'Z') || (Line[0] >= 'a' && Line[0] <= 'z'))
        {
            insertLast(Line, list);
        }
        if(strlen(Line) > maxSize)
            maxSize = strlen(Line);
    }

    fclose(read);

    if (isEmpty) {
        printf("Warning: input.txt is empty!\n");
    }

    return maxSize ;
}

// Function to print the list
void printList(LinkedList list)
{
    pos p = list->next ;
    if(p == NULL) {
        printf("The File is Empty\n");
        return;
    }
    printf("\n");
    while(p != NULL)
    {
        printf("%s\n",p->data);
        p = p->next ;
    }
}

// Function to make all strings have the same length for being able to sorted them
void addSpaces(int maxSize , LinkedList list)
{
    pos p = list->next ;
    while(p != NULL)
    {
        int currentSize = strlen(p->data);
        if(currentSize < maxSize)
        {
            for(int i=currentSize ; i<maxSize ; i++)
            {
                p->data[i] = ' ' ;
            }
            p->data[maxSize] = '\0' ;
        }
        p = p->next ;
    }
}

// Function to remove spaces for being able to search on the word and delete it
void removeSpaces(int maxSize , LinkedList list)
{
    pos p = list->next ;
    while (p != NULL)
    {
        for(int i=0 ; i<maxSize  ; i++)
        {
            if(p->data[i] == ' ')
                p->data[i] = '\0' ;
        }
        p = p->next ;
    }
}

// Function to remove all nodes form the list
void clearList(LinkedList list)
{
    pos p = list->next ;
    list->next = NULL;
    pos temp;
    while (p != NULL)
    {
        temp = p;
        p = p->next;
        free(temp);
    }
}

// Function to sort the words according to the alphabet
LinkedList radixSort(int maxSize , LinkedList listBeforeSorting)
{
    LinkedList list = copy(listBeforeSorting) ;
    // Same length
    addSpaces(maxSize,list);
    // Array of linked lists
    LinkedList Arr[63];
    // Initialize the indices list
    for(int i=0 ; i<63 ; i++)
    {
        Arr[i] = creation();
    }
    // Loop through each character from right to left
    for (int i=maxSize-1 ; i >= 0 ; --i)
    {
        // Move nodes to indices list based on the current character
        pos p = list->next ;
        while (p != NULL)
        {
            // Check if the character is within a valid range
            if(p->data[i] == ' ')
            {
                insertLast(p->data , Arr[0]);
            }
            else if(p->data[i] >= '0' && p->data[i] <= '9')
            {
                insertLast(p->data , Arr[p->data[i]-47]) ;
            }
            else if(p->data[i] >= 'A' && p->data[i] <= 'Z')
            {
                insertLast(p->data , Arr[p->data[i]-54]) ;
            }
            else if(p->data[i] >= 'a' && p->data[i] <= 'z')
            {
                insertLast(p->data , Arr[p->data[i]-60]);
            }
            p = p->next ;
        }
        // clear list
        clearList(list);

        // collecting the words from Arr and insert them in the list
        for(int j=0 ; j < 63 ; ++j)
        {
            pos q = Arr[j]->next ;
            while(q != NULL)
            {
                insertLast(q->data , list);
                q = q->next ;
            }
            // clear the array
            clearList(Arr[j]);
        }
    }
    return list ;
}

// Function to add word from the user on the list and sorted it
LinkedList addWord(int maxSize , LinkedList list)
{
    removeSpaces(maxSize,list);
    char word[1000];
    printf("Please enter new word : \n");
    scanf("%s" , word);
    while(strlen(word) > 30 || !((word[0] >= 'A' && word[0] <= 'Z') || (word[0] >= 'a' && word[0] <= 'z')))
    {
        printf("Error! Please check the word begins with a character AND does not exceed 30 characters : \n");
        scanf("%s" , word);
    }
        insertLast(word , list);
    return radixSort(maxSize , list);
}

// Function to delete word from the user on the list and sorted it
void deleteWord(int maxSize , LinkedList list)
{
    removeSpaces(maxSize,list);
    char word[MAX_LENGTH] ;
    printf("Please enter the word you want to delete it : \n");
    scanf("%s" , word);
    deleteNode(word , list);
}

// Function to delete a determent node from the list
void deleteNode(char word[] , LinkedList list) {
    if (list->next != NULL)
    {
        pos p = list->next;
        while(p != NULL && strcmp(p->data, word) != 0)
        {
            p = p->next;
        }

        if(p != NULL)
        {
            if (p->prev != NULL) {
                    p->prev->next = p->next;
            }
            if (p->next != NULL) {
                    p->next->prev = p->prev;
            }
            free(p);
            printf("The word has been deleted\n");
        }
        else
        {
            printf("Data does not exist! Please try again\n");
        }

    }
    else
    {
        printf("List is empty!\n");
    }
}

// Function to print list in file "output"
void printInFile(LinkedList list)
{
    FILE * write = fopen("output.txt" , "w");
    if (write == NULL) {
        printf("Error: Could not create output.txt!\n");
        return;
    }

    pos p = list->next ;

    if (p == NULL) {
        printf("Warning: List is empty, nothing to write to output.txt!\n");
    }

    while (p != NULL)
    {
        fprintf(write , "%s\n" , p->data);
        p = p->next ;
    }
    fclose(write);
}


////                           ------> تم بحمد الله  <------
