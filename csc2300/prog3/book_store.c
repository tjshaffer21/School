/******************************************************************************
 *  book_store                                                                *
 *  Thomas Shaffer                                                            *
 *  CSC 2300-001                                                              *
 *  Assignment: 3                                                             *
 *  11 November 2008                                                          *
 *                                                                            *
 *  Plus or Minus Books inventory database.  The application takes in a file  *
 *  called "bookinput.txt" and uses the information inside to create a data-  *
 *  base to be used in ordering the books for the store.  After the info-     *
 *  mation is read in the user will be given options on how to display the    *
 *  information.                                                              *
 *                                                                            *
 *  Algorithms:                                                               *
 *      Quicksort, Bubblesort                                                 *
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define BUFFER 256

// Creates a structure that holds information about a book within the bookstore.
typedef struct
{
    char *title;                    // Title of the book.
    char *first_name;               // First name of the author.
    char *last_name;                // Last name of the author.
    char initial;                   // Author's middle initial.
    float wholesale;                // Wholesale price of book.
    float retail;                   // Retail price of book.
    int quantity;                   // Quantity of the book.
    long int isbn;                  // ISBN number of the book.
    int published;                  // Year published.
    int published_orig;             // Original published year

    // The type of formation available for the book.
    // P - Paperback, S - Softcover, H - Hard cover, C - Cassette, D - CD
    enum { P, S, H, C, D } format;

    // The length of the book depending of the media that it is on.
    union {
        int pages;        // Paperback, soft cover, hard cover, num tapes.
        float time;       // Length of CD (HH.MM)
    } length;
} BOOK;

/* Prototypes */
int run_store( );
int obtainInput( FILE *file, BOOK *books, int numbooks );
void printInfo( BOOK *books, int numbooks );
void userChoices( BOOK *books, int numbooks );
void quickSort( BOOK *books, int begin, int end );
int bubbleSort( BOOK *books, int numbooks, int last_swap );
void swap( BOOK *book1, BOOK *book2 );
void printSort( int choice, BOOK *books, int numbooks );

int main( void )
{
    printf ( "\nWelcome to the Plus or Minus Book Store Inventory List!\n" );
    
    if ( run_store( ) == -1 )
    {
        printf( "Aborting!\n\n" );
        return -1;
    }

    printf ( "Thank you for using Plus or Minus Book Store!\n\n" );
    return 0;
}

/******************************************************************************
 *  run_store                                                                 *
 *  Abstracts the details of the store's operations from the main method.     *
 *  Preconditions:                                                            *
 *      None.                                                                 *
 *  Postconditions:                                                           *
 *      If the program fails then a -1 one is returned, otherwise 0.          *
 *****************************************************************************/
int run_store( )
{
    FILE *file;         // Pointer to the input file.
    char input[BUFFER]; // Input buffer.
    int numbooks = 0;   // Number of books.
    int i = 0;          // Counter.
    BOOK *books;        // Array of book stuctures.

    // Point the file pointer to the input file.
    // Return a NULL if file cannot be opened.
    if ( (file = fopen( "bookfile.txt", "r" ) ) == NULL )
    {
        printf( "Input file could not be open!\n" );
        return -1;
    }
    
    // Obtain the number of books from the first
    // line of the input file.
    if ( fgets( input, BUFFER, file ) != NULL )
    {
        // Convert into an integer.
        numbooks = atoi( input );

        // If atoi() fails or returns a value that is less <= 0 then return
        // an error.
        if ( numbooks > 0 )
        {
            // Allocate memory for the array of structs.
            books = ( BOOK* ) malloc( numbooks * sizeof( BOOK ) );

            // Check that allocation did not fail.
            if ( books == NULL )
            {
                printf( "Unable to allocate memory!\n" );
                return -1;
            }
        }
        else
        {
            printf( "Number of books must be greater than 0.\n" );
            return -1;
        }
    }
    else
    {
        printf( "Unable to get input!\n" );
        return -1;
    }

    // Obtain information from the file, print the information, and then
    // give the user a set of sorting options.
    if ( obtainInput( file, books, numbooks ) == -1 )
    {
        return -1;
    }

    printInfo( books, numbooks );
    userChoices( books, numbooks );

    // Free used memory.
    for ( i = 0; i <= numbooks-1; i++ )
    {
        free( ( void* ) ( *( books+i ) ).title );
        free( ( void* ) ( *( books+i ) ).last_name );
        free( ( void* ) ( *( books+i ) ).first_name );
    }

    free( (void*) books );

    return 0;
}

/******************************************************************************
 *  obtainInput                                                               *
 *  Takes the input from the text file and enters it into the array of        *
 *  structs.                                                                  *
 *  Preconditions:                                                            *
 *      A pointer to the open file.                                           *
 *      The array of book structures.                                         *
 *      An integer for the number of arrays.                                  *
 *  Postconditions:                                                           *
 *      If input fails or memory allocation fails the -1 is returned.         *
 *      Success returns the 0.                                                *
 *      The structures in the array are filled with the information from the  *
 *      input file.                                                           *
 ******************************************************************************/
int obtainInput( FILE *file, BOOK *books, int numbooks )
{

    char input[BUFFER];     // Temporary array for input.
    char f_name[BUFFER];    // Temporary array for first name.
    char l_name[BUFFER];    // Temporary array for last name.
    char character;         // Temporary character value.
    int temp1;              // Temporary int value.
    int temp2;              // Temporary int value.
    float temp3;            // Temporary float value.
    float temp4;            // Temporary float value.
    int i = 0;              // Counter

    printf( "Reading input...\n" );
    
    // Process the rest of the file until EOF.
    while ( i < numbooks )
    {
        // Obtain the title.
        if ( fgets( input, BUFFER, file ) != NULL )
        {
            input[strlen( input )-1] = '\0';

            ( books+i )->title = ( char* ) malloc( 
                                        (strlen( input )+1) * sizeof( char ) );
            strcpy( ( books+i )->title, input );
            strcat( ( books+i )->title, "\0" );
        }
        else
        {
            printf( "Unable to obtain input!\n" );
            return -1;
        }

        // Obtain the author's name.
        if ( fgets( input, BUFFER, file ) != NULL )
        {
            input[strlen( input )-1] = '\0';
                
            // Scan the input buffer into temporary strings.
            if ( sscanf( input, "%[^', '], %[^', '], %c", l_name, 
                f_name, &character ) < 3 )
            {
                character = '\0';
            }

            // Allocate memory.
            ( books+i )->last_name = 
                    ( char * ) malloc( (strlen( l_name )+1) * sizeof( char ) );
            ( books+i )->first_name = 
                    ( char * ) malloc( strlen( (f_name )+1) * sizeof( char ) );

            // Copies strings into the structure.
            strcpy( ( books+i )->last_name, l_name );
            strcpy( ( books+i )->first_name, f_name );

            strcat( ( books+i )->last_name, "\0" );
            strcat( ( books+i )->first_name, "\0" );

            // Check that the character is an alphabetic character.
            if ( isalpha( character ) )
            {
                ( books+i )->initial = character;
            }
        }
        else
        {
            printf( "Unable to obtain input!\n" );
            return -1;
        }

        // Obtain the wholesale, retail, quantity, and ISBN numbers.    
        if ( fgets( input, BUFFER, file ) != NULL )
        {
            // Check that all values are inputed.
            if ( sscanf( input, "%f %f %d %d", &temp3, &temp4, &temp1,
                    &temp2 ) < 4 )
            {
                printf( "Unable to obtain input!\n" );
                return -1;
            }
            else
            {
                // Check that w_value is not negative.
                if ( temp3 < 0.00 )
                {
                    printf( "Invalid wholesale value!\n" );
                    return -1;
                }
                else
                {
                    ( books+i )->wholesale = temp3;
                }

                // Check that r_value is not negative.
                if ( temp4 < 0.00 )
                {
                    printf( "Invalid retail price!\n" );
                    return -1;
                }
                else
                {
                    ( books+i )->retail = temp4;
                }
 
                // Check that temp1 is not negative.
                if ( temp1 < 0 )
                {
                    printf( "Invalid quantity!\n" );
                    return -1;
                }
                else
                {
                    ( books+i )->quantity = temp1;
                }

                // Check that temp2 is not negative.
                if ( temp2 < 0000000000 )
                {
                    printf( "Invalid ISBN number!\n" );
                    return -1;
                }
                else
                {
                    ( books+i )->isbn = temp2;
                }
            }
        }
        else
        {
            printf( "Unable to obtain input!\n" );
            return -1;
        }

        // Obtain the publication year, first publication year, the enum
        // type, and the union value.
        if ( fgets( input, BUFFER, file ) != NULL ) 
        {
            if ( sscanf( input, "%d %d %c %f", &temp1, &temp2, &character,
                    &temp3 ) < 4 )
            {
                printf( "Unable to obtain input.\n" );
                return -1;
            }

            // Check for negative values.
            if ( temp1 < 0000 )
            {
                printf( "Invalid publication year!\n" );
                return -1;
            }
            else
            {
                ( books+i )->published = temp1;
            }

            // Check for negative values.
            if ( temp2 < 0000 )
            {
                printf( "Invalid publication year!\n" );
                return -1;
            }
            else
            {
                ( books+i )->published_orig = temp2;
            }

            // Check that the character is an actual character and is
            // between the values of the enumerated type.
            if ( !isalpha( character ) &&
                 !(character > P || character < D) )
            {
                printf( "Invalid media type!\n" );
                return -1;
            }
            else
            {
                ( books+i )->format = character;
            }

            // Check for negative values.
            if ( temp3 < 0.00 )
            {
                printf( "Invalid length!\n" );
                return -1;
            }
            else
            {
                // Place value into the union based on the enumerated
                // type.
                if ( character != 'D' )
                {
                    ( ( books+i )->length ).pages = ( ( int ) temp3 );
                }
                else if ( character == 'D' )
                {
                    ( ( books+i )->length ).time = temp3;
                }
				else
				{
					printf( "Invalid media type!\n" );
					return -1;
			    }
            }
        }
        else
        {
            printf( "Unable to obtain input!\n" );
            return -1;
        }
  
        i++;
    }
    
    // Close the file stream.
    if ( fclose( file ) != 0 )
    {
        printf( "Failed to close file stream!\n" );
    }

    return 0;
}

/******************************************************************************
 *  printInfo                                                                 *
 *  Prints out all of the book information given by the user.                 *
 *  Preconditions:                                                            *
 *      A pointer to the array of book structures.                            *
 *      The number of books in the array.                                     *
 *  Preconditions:                                                            *
 *      Prings out all the book information onto the terminal.                *
 *****************************************************************************/
void printInfo( BOOK *books, int numbooks )
{
    int i = 0;
	
    for ( i = 0; i <= numbooks-1; i++ )
    {
        printf( "Title:\t\t\t%s\n", ( books+i )->title );
        printf( "Author:\t\t\t%s, %s", ( books+i )->last_name,
                 ( books+i )->first_name );

        // If initial == \0 then print a newline character, else
        // print the value of initial.
        if ( ( books+i )->initial != '\0' )
        {
            printf( " %c.\n", ( books+i )->initial );
        }
        else
        {
            printf( "\n" );
        }
       
        printf( "ISBN:\t\t\t%010ld\n", ( books+i )->isbn );
        printf( "Wholesale Cost:\t\t$%.2f\n", ( books+i )->wholesale );
        printf( "Retail Cost:\t\t$%.2f\n", ( books+i )->retail );
        printf( "Publication:\t\t%d\n", ( books+i )->published );
        printf( "Orig. Publication:\t%d\n", ( books+i )->published_orig );
        printf( "Media:\t\t\t%c\n", ( books+i )->format );

        if ( ( *( books+i ) ).format == D )
        {
            printf( "Length:\t\t\t%.2f\n", ( ( books+i )->length ).time );
        }
        else if( (*(books+i)).format == C )
        {
            printf( "Num. Tapes:\t\t%d\n", ( ( books+i )->length ).pages );
        }
        else
        {
            printf( "Num. Pages:\t\t%d\n", ( ( books+i )->length ).pages );
        }

        printf( "\n" );
    }

}

/******************************************************************************
 *  userChoices                                                               *
 *  Sets up a menu that gives the user a set of options on how to sort and    *
 *  display the inventory.                                                    *
 *  Preconditions:                                                            *
 *      A pointer to the array of book structures.                            *
 *      An integer of the number of books in the array.                       *
 *  Postconditions:                                                           *
 *      Depending on the choice from the user, a given display is returned.   *
 *****************************************************************************/
void userChoices( BOOK *books, int numbooks )
{
    int choice = 0;         // Users choice.
    int swaps = 0;          // Keeps track of last number of swaps.

    // Loop until the user inputs the value of 5.
    do
    {
        // Menu
        printf( "Please pick a display option.\n" );
        printf( "1) Book titles in current order.\n" );
        printf( "2) Authors in current order.\n" );
        printf( "3) Sort by ISBN number.\n" );
        printf( "4) Sort by book title.\n" );
        printf( "5) Quit\n" );
        printf( "Choice: " );
        scanf( "%d", &choice );

        // Take care of user input.
        switch ( choice )
        {
            case 1:
                printf( "\nPrinting book titles!\n" );
                printSort( choice, books, numbooks );
                break;
            case 2:
                printf( "\nPrinting authors!\n" );
                printSort( choice, books, numbooks );
                break;
            case 3:
                printf( "\nSorting by ISBN!\n" );
                quickSort( books, 0, numbooks );
                printSort( choice, books, numbooks );
                break;
            case 4:
                printf( "\nSorting by book titles!\n" );
                swaps = bubbleSort( books, numbooks, swaps );
                printSort( choice, books, numbooks );
                break;
            case 5:
                printf( "\n" );
                return;
            default:
                printf( "Invalid choice!\n\n" );
        }

        // Empty buffer.
        while( getchar() != '\n' ){}
    }while( choice != 5 );
}

/******************************************************************************
 *  quickSort                                                                 *
 *  Sorts the books using the quicksort algorithm.                            *
 *  Preconditions:                                                            *
 *      The array of books.                                                   *
 *      The beginning of the array and the end of the array.                  *
 *  Postconditions:                                                           *
 *      The array becomes sorted.                                             *
 *****************************************************************************/
void quickSort( BOOK *books, int begin, int end )
{
    // Only do work if end is greater than begin.
    if ( end > begin )
    {
        // Let the piviot be the first isbn number in the array.
        int pivot = (books+begin)->isbn;
        int l = begin + 1;
        int r = end;

        // While l < r, create the two regions of the pivot.
        while( l < r )
        {
            // If the book at offset l is greater than the
            // pivot then we swap with the book at offset r.
            if ( (books+l)->isbn <= pivot )
            {
                l++;
            }
            else
            {
                r--;
                swap( books+l, books+r );
            }
        }

        l--;

        // Swap the first book and the book at offset l.
        swap( books+begin, books+l );

        // Found the location of the pivot so use recursion to
        // find sort the other two regions.
        quickSort( books, begin, l );
        quickSort( books, r, end );
    }
}

/******************************************************************************
 *  bubbleSort                                                                *
 *  Sorts the books using the bubble sort algorithm.                          *
 *  Algorithm:                                                                *
 *      Compares two adjacent items and if they are not in order then it      *
 *      swaps them.                                                           *
 *  Preconditions:                                                            *
 *      The array of books.                                                   *
 *      The length of the array.                                              *
 *  Postconditions:                                                           *
 *      The array becomes sorted.                                             *
 *     Returns the number of swaps made.                                      *
 *****************************************************************************/
int bubbleSort( BOOK *books, int numbooks, int last_swap )
{
    static int swaps = 0;       // Swap counter
    int sorted = 1;             // Used to check if array is sorted.
    int i;                      // Temporary variable
    int j;                      // Temporary variable
    int comp;                   // Comparison variable.

    // Iterate backwards through the array until the beginning is reached or
    // or no swaps are made.
    for ( i = numbooks-1; (i > 0) && sorted != 0; i-- )
    {
        // Assume that the array is sorted.
        sorted = 0;
        
        // Iterate through the array until reaching the value of i.
        for ( j = 0; j < i; j++ )
        {
            // Compare two books.
            comp = strcmp((books+j)->title, (books+(j+1))->title );

            // If book1 is greater than book2 then value will be greater than
            // zero.
            if ( comp > 0 )
            {
                swaps++;                        // Increment swwap counter.
                swap( books+j, books+(j+1) );   // Swap the books.
                sorted = 1;                     // Reset assumption to false.
            }
        }
    }

    // Print out the number of comparisons.
    if ( last_swap == 0 )
    {
        printf( "%d comparisons made!\n", swaps );
        return swaps;        
    }
    else
    {
        swaps = swaps-last_swap;
        printf( "%d comparisons made!\n", swaps );
    }

    return swaps;
}

/******************************************************************************
 *  swap                                                                      *
 *  Swaps the two items given.                                                *
 *  Preconditions:                                                            *
 *      Two points to different BOOK Structs.                                 *
 *  Postconditions:                                                           *
 *      The two pointers now point to different structs.                      *
 ******************************************************************************/
void swap( BOOK *book1, BOOK *book2 )
{
    // Allocate space for temporary structure.
    BOOK* temp = ( BOOK* ) malloc( sizeof( BOOK ) );
    
    // Let temp hold book1, then let book1 become book2 and book2
    // become temp.
    *temp = *book1;
    *book1 = *book2;
    *book2 = *temp;

    // Free temp memory.
    free( (void*) temp );
}

/******************************************************************************
 *  printSort                                                                 *
 *  Prints the results of sorting based on the decision of the user.          *
 *  Choice:                                                                   *
 *      1 - Print titles in current order                                     *
 *      2 - Print authors in current order                                    *
 *      3 - Print ISBN numbers sorted by quicksort algorithm.                 *
 *      4 - Print title  numbers sorted by bubble sort.                       *
 *  Preconditions:                                                            *
 *      The choice of the user (1-4).                                         *
 *      The array of book structs.                                            *
 *      The number of books in the array.                                     *
 *  Postconditions:                                                           *
 *      The results of the earlier sorting is outputed into terminal.         *
 *****************************************************************************/
void printSort( int choice, BOOK *books, int numbooks )
{
    int i = 0;

    printf( "\n" );

    // Prints the information based on the user choice.
    if ( choice == 1 )      // Current order, title
    {
        for ( i = 0; i <= numbooks-1; i++ )
        {
            printf( "%s\n", ( books+i )->title );
        }
    }
    else if ( choice == 2 ) // Current order, name
    {
        for ( i = 0; i <= numbooks-1; i++ )
        {
            printf( "%s, %s", ( books+i )->last_name,
                    ( books+i )->first_name );

            if ( ( books+i )->initial != '\0' )
            {
                printf( " %c.\n", ( books+i )->initial );
            }
            else
            {
                printf( "\n" );
            }
        }
    }
    else if ( choice == 3 ) // Sorted, Tite and ISBN
    {
        for ( i = 0; i <= numbooks-1; i++ )
        {
            printf( "%010ld\t\t%s\n", ( books+i )->isbn, ( books+i )->title );
        }
    }
    else                    // Sorted, title and last name.
    {
        for ( i = 0; i <= numbooks-1; i++ )
        {
            printf( "%s ", ( books+i )->title );
            printf( "by %s %s\n", ( books+i )->first_name,
                            ( books+i )->last_name );
        }
    }

    printf( "\n" );
}
