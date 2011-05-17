/******************************************************************************
 *  Program:    book_store.c                                                  *
 *  Programmer: Thomas Shaffer                                                *
 *  Course:     CSC 2300-001                                                  *
 *  Assignment: 2                                                             *
 *  Date:       23 October 2008                                               *
 *                                                                            *
 *  Description:                                                              *
 *      Plus or Minus Perfect Books financial simulator.  It takes a finalized*
 *      list of categories and two books for each category.  The information  *
 *      is used to decide the best publishers/distributors by calculating the *
 *      the best profit given the cost and quantity of the given publishers.  *
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER      256
#define NUM_CAT     10
#define NUM_BOOKS   2
#define MAX_ARRAY   20

/* Prototypes */
int run_store();
int obtainCatInfo( char *category_array[] );
int obtainBooks( char *category_array[], int num_cat, char *author[], 
                 char *title[], float price[], int quantity[] );
int obtainPublishers( int num_publishers, char *publishers[] );
int obtainPublishersInfo( char *books[], char *publishers[], 
                          float wholesale[][MAX_ARRAY],
                          int copies[][MAX_ARRAY], int num_publishers, 
                          int array_size );
int calculatePublishers( float retail[], float wholesale[][MAX_ARRAY], 
                         int quantity[], int copies[][MAX_ARRAY],
                         float wholesale_order[], float book_av[],
                         float grossprofit[], float percprofit[],
                         int num_category, int num_publishers, int array_size );
void displayCatInfo( char *category_array[], int num_cat, char *author[],
                     char *title[], float retail[], int quantity[] );
void displayResults( char *publishers[], float wholesale_order[], 
                     float book_av[], float percprofit[], 
                     int num_publishers );

int main( void )
{
	printf( "Welcome to Plus or Minus Books Financial Simulation!\n" );

    if ( run_store() == -1 )
    {
        printf( "Aborting!\n" );
        return -1;
    }

    printf( "\nThank you for using Plus or Minus Books Financial " );
	printf( "Simulation!\n" );

    return 0;
}

/******************************************************************************
 *  run_store                                                                 *
 *                                                                            *
 *  Takes care of the main aspects of the store applications.                 *
 *  Preconditions:                                                            *
 *      None.                                                                 *
 *  Postconditions:                                                           *
 *      Returns -1 on failure, 0 on sucess.                                   *
 *****************************************************************************/
int run_store( )
{
    int i = 0;                      // Counter variable.
    int num_category = -1;          // Number of categories.
    int num_publishers = 0;         // Number of publishers
    char *input = ( (char*) malloc( BUFFER ) );   // Temp array for input. 
    char *category_array[NUM_CAT];  // Category array.
 
    if ( input == NULL )
    {
        printf( "Unable to allocate memory!\n" );
        return -1;
    }
   
    // Obtain the categories and place the number of categories
    // into variable, if -1 then failure occured.
    num_category = obtainCatInfo( category_array );
    if ( num_category == -1 )
    {
        printf( "Unable to obtain category information.\n" );
        free( (void*) input );
        return -1;
    }

    // Now have the number of categories so calculate size of arrays other 
    // arrays.
    int array_size = NUM_BOOKS*num_category;    //Calculate array size.
    char *author[array_size];       // Book array
    char *title[array_size];        // Book array
    float retail[array_size];       // Book store array
    int quantity[array_size];       // Book store array

    if ( obtainBooks( category_array, num_category, author, title, retail, 
                      quantity ) == -1 )
    {
        printf( "Unable to obtain book information!.\n" );
        free( (void*) input );
        return -1;
    }

    displayCatInfo( category_array, num_category, author, title, retail, 
                    quantity );

    // Obtain publisher/distributer number to run calculations.
    while ( num_publishers <= 0 || num_publishers > 10 )
    {
        printf( "\nHow many publishers/distributors to compare (max 10): " );
        if ( fgets( input, BUFFER, stdin ) != NULL )
        {
            num_publishers = atoi( input );
        }
        else
        {
            printf( "Unable to obtain input!\n" );
            free( (void*) input );
            return -1;
        }
    }
    
    // Set up publisher arrays.
    float wholesale[num_publishers][MAX_ARRAY];
    float book_av[num_publishers];
    float grossprofit[MAX_ARRAY];
    float percprofit[num_publishers];
    float wholesale_order[num_publishers];
    int copies[num_publishers][MAX_ARRAY];
    char *publishers[num_publishers];

    // Obtain publishers
    if ( obtainPublishers( num_publishers, publishers ) == -1 )
    {
        printf( "Unable to obtain publisher information!\n" );
        free( (void*) input );
        return -1;
    }

    // Obtain information about publishers
    if ( obtainPublishersInfo( title, publishers, wholesale, copies, 
                               num_publishers, array_size ) == -1 )
    {
        printf( "Unable to obtain publisher information!\n" );
        free( (void*) input );
        return -1;
    }

    // Calculate information.
    if ( calculatePublishers( retail, wholesale, quantity, copies,
                              wholesale_order, book_av, grossprofit,
                              percprofit, num_category,  num_publishers,
                              array_size ) == -1 )
    {
        printf( "Unable to perform calculations!\n" );
        free( (void*) input );
        return -1;
    }

    // Display results.
    displayResults( publishers, wholesale_order, book_av, percprofit, 
                    num_publishers );

    // Free allocated memory.
    free( (void*) input );

    for ( i = 0; i <= num_category-1; i++ )
    {
        free( (void*) category_array[i] );
    }

    for ( i = 0; i <= array_size-1; i++ )
    {
        free( ( void* ) author[i] );
        free( ( void* ) title[i] );
    }

    return 0;
}

/******************************************************************************
 *  obtainCatInfo                                                             *
 *                                                                            *
 *  Obtains the list of categories and their information from the user.       *
 *  Preconditions:                                                            *
 *      A char* array used to hold the category names.                        *
 *  Postconditions:                                                           *
 *      If the program is unable to allocate memory for the strings then the  *
 *      method returns a -1; otherwise, the method fills the array from the   *
 *      parameter with the category names and returns a the number of         *
 *      categories.                                                           *
 *****************************************************************************/
int obtainCatInfo( char* category_array[] )
{
    int num_cat = 0;    // Number of categories.
    // Temporary char array.
    char* category = ( ( char* ) malloc( BUFFER) );

    // Check that malloc worked.
    if ( category == NULL )
    {
        printf( "Unable to allocate memory!\n" );
        free( (void*) category );
        return -1;
    }   

    printf( "Please input at least three categories and no more than 10.\n" );
    printf( "Input \"finish\" when done.\n" );

    // Obtain categories until reach the max number of categories
    // or the user decides to finish entering input.
    while ( num_cat <= NUM_CAT-1 )
    {
        printf( "Input category #%d: ", num_cat+1 );

        // Check to see that fgets() worked.
        if ( fgets( category, BUFFER, stdin ) != NULL )
        {
            // Remove new line character.
            category[strlen(category)-1] = '\0';

            // Check to see if user has inputed the exiting
            // input.  If not then add the category into the
            // list.
            if ( strcmp( category, "finish" ) == 0 )
            {
                // Quit the loop only if the user has inputed
                // at least 3 categories.
                if ( num_cat >= 3 )
                {
                    free( ( void* ) category );
                    return num_cat;
                }
            }
            else
            {
                // Allocate memory in the array for the category name.
                category_array[num_cat] = ( ( char* ) malloc( 
                            sizeof( strlen( category )  + 1) ) );

                // Check that malloc worked.
                if ( category_array[num_cat] == NULL )
                {
                    free( ( void* ) category );
                    return -1;
                }

                // Copy the category name into the array.
                strcpy( category_array[num_cat], category );
                num_cat++;
            }
        }
        else
        {
            return -1;
        }
    }

    free( ( void* ) category );
    return num_cat;
}

/******************************************************************************
 *  obtainBooks                                                               *
 *                                                                            *
 *  Obtain two books for each category.                                       *
 *  Preconditions:                                                            *
 *      The char* array that holds the categories.                            *
 *      The number of categories.                                             *
 *      The char* array that holds the author name.                           *
 *      The char* array that holds the title.                                 *
 *      A float array to hold the retail price.                               *
 *      A int array to hold the quantity.                                     *
 *  Postconditions:                                                           *
 *      Fills the arrays with the information obtained from the user.         *
 *      Returns a -1 if an error is encountered; else, returns a 0.           *
 *****************************************************************************/
int obtainBooks( char* cat[], int num_cat, char* author[], char* title[], 
                 float price[], int quantity[] )
{
    // Counter variables.
    int counter = 0;        // Counter for categories.
    int counter_two = 0;    // Counter for other arrays.
    int i = 0;              // Counter for book input.

    // Temporary variables to hold information.
    char *temp = ( (char*) malloc( BUFFER ) );
    float r_price = 0.0;        // Holds retail value to do input checking.
    int q_quantity = 0;         // Holds quantity value to do input checking.

    if ( temp == NULL )
    {
        printf( "Unable to allocate memory!\n" );
        return -1;
    }

    // Iterate through all of the categories.
    for ( counter = 0; counter <= num_cat-1; counter++ )
    {
        printf( "\n%s's books.\n\n", cat[counter] );

        // Add as many books that are defined by NUM_BOOKS
        i = 0;
        while ( i < NUM_BOOKS )
        {
            // For each part of the book, check to make sure input is 
            // obtains.
            // Also remove the \n character given by fgets().
            // And before copying the string, allocate room in the array.
            // Must use, strcpy() because we cannot use temp's memory address.
            printf( "Author: " );
            if ( fgets( temp, BUFFER, stdin ) != NULL )
            {
                temp[strlen(temp)-1] = '\0';
                author[counter_two] = ( ( char* ) malloc( 
                                                    strlen( temp ) + 1 ) );

                if ( author[counter_two] == NULL )
                {
                    free( (void*) temp );
                    return -1;
                }

                strcpy( author[counter_two], temp );
            }
            else
            {
                free ( (void*) temp );
                return -1;
            }

            printf( "Title: " );
            if ( fgets( temp, BUFFER, stdin ) != NULL )
            {
                temp[strlen(temp)-1] = '\0';
                title[counter_two] = ( ( char* ) malloc( strlen( temp ) + 1 ) );

                if ( title[counter_two] == NULL )
                {
                    free( (void*) temp );
                    return -1;
                }

                strcpy( title[counter_two], temp );
            }
            else
            {
                free( (void*) temp );
                return -1;
            }
        
            // Only except value if it is greater than 0.0
            // Invalid input from atof returns a 0.0.
            r_price = 0.0;
            do
            {
                printf( "Retail price: " );
                if ( fgets( temp, BUFFER, stdin ) != NULL )
                {
                    r_price = atof( temp );
                }
                else
                {
                    free( (void*) temp );
                    return -1;
                }
            }while( r_price < 0.0 );
            price[counter_two] = r_price;
            
            // Only except the value if it is greater than 0.
            // Invalid input from atoi() returns a 0.
            q_quantity = 0;
            do
            {
                printf( "Quantity: " );
                if ( fgets( temp, BUFFER, stdin ) != NULL )
                {
                    q_quantity = atoi( temp );
                }
                else
                {
                    free( (void*) temp );
                    return -1;
                }
            }while( q_quantity < 0 );
            quantity[counter_two] = q_quantity;
            
            // Give a space between books.           
            printf( "\n" );
            counter_two++;
            i++;
        }
    }

    free( (void*) temp );
    return 0;
}

/******************************************************************************
 *  obtainPublishers                                                          *
 *                                                                            *
 *  Obtains the name of the publishers.                                       *
 *  Preconditions:                                                            *
 *      The number of publishers the user wants.                              *
 *      A char* array to hold the publishers                                  *
 *  Postconditions:                                                           *
 *      Returns a 0 on success and a -1 on failure.                           *
 *      Fills the char array with strings.                                    *
 *****************************************************************************/
int obtainPublishers( int num_publishers, char *publishers[] )
{
    int i = 0;                                  // Loop variable.
    char *temp = ( (char*) malloc( BUFFER ) );  // Temporary array for input.

    if ( temp == NULL )
    {
        printf( "Unable to allocate memory!\n" );
        return -1;
    }

    // Iterate through until the end of the publisher array and add
    // publishers to that array.
    for ( i = 0; i <= num_publishers-1; i++ )
    {
        printf( "Name of #%d publisher: ", i+1 );
        if ( fgets( temp, BUFFER, stdin ) != NULL )
        {
            temp[strlen(temp)-1] = '\0';
            publishers[i] = ( ( char* ) malloc( strlen( temp ) + 1 ) );
            strcpy( publishers[i], temp );
        }
        else
        {
            printf( "Unable to obtain input.\n" );
            free( (void*) temp );
            return -1;
        }
    }

    free( (void*) temp );
    return 0;
}

/******************************************************************************
 *  obtainPublisherInfo                                                       *
 *                                                                            *
 *  Obtain data about the publishers.                                         *
 *  Preconditions:                                                            *
 *      char* array that holds all the books.                                 *
 *      char* array that holds all the publishers/distributors.               *
 *      2D float array to hold wholesale values of each book for each         * 
 *      publisher.                                                            *
 *      int array to hold number of copies of each book for each publisher.   *
 *      An int that holds the number of publishers                            *
 *      An int that holds the array size for other arrays.                    *
 *  Postconditions:                                                           *
 *      Returns a 0 on sucess and -1 on failure.                              *
 *      Fills the wholesale array and copies array.                           *
 *****************************************************************************/
int obtainPublishersInfo( char* books[], char* publishers[], 
                          float wholesale[][MAX_ARRAY], 
                          int copies[][MAX_ARRAY],
                          int num_publishers, int array_size )
{
    // Counter variables
    int i = 0;
    int j = 0;
    char *input = ( (char*) malloc( BUFFER ) );     // Holds input.
    float w_price = 0.0;          // wholesale price value checking
    int n_copies = 0;             // number of copies value checking
   
    // Check that malloc succeded.
    if ( input == NULL )
    {
        return -1;
        printf( "Memory allocation failed!" );
    }

    // Iterate through the publishers.
    for ( i = 0; i <= num_publishers-1; i++ )
    {
        printf( "\n%s:\n", publishers[i]);
        // Iterate until reaching the end of array_size
        // so to add information into other arrays.
        for ( j = 0; j <= array_size-1; j++ )
        {
            printf( "\t%s:\n", books[j] );

            // Loop until a non-negative value is entered.
            w_price = 0.0;
            do
            {
                printf( "\t\tWholesale price: " );
                if ( fgets( input, BUFFER, stdin ) != NULL )
                {
                    w_price = atof( input );
                }
                else
                {
                    printf( "Unable to obtain input.\n" );
     
                    free( (void*) input );
                    return -1;
                }
            }
            while( w_price < 0.0 );
            wholesale[i][j] = w_price;

            // Loop until if a non-negative value is entered
            n_copies = 0;
            do
            {
                printf( "\t\tQuantity: " );
                if ( fgets( input, BUFFER, stdin ) != NULL )
                {
                    n_copies = atoi( input );
                }
                else
                {
                    printf( "Unable to obtain input.\n" );
    
                    free( (void*) input );
                    return -1;
                }
            }while( n_copies < 0 );
            copies[i][j] = n_copies;
        }
    }

    free( (void*) input );
    return 0;
}

/******************************************************************************
 *  calculatePublishers                                                       *
 *                                                                            *
 *  Calculates the wholesale order, book availability, and profit percentage  *
 *  from the publishers.                                                      *
 *  Preconditions:                                                            *
 *      float array for the retail price.                                     *
 *      2d float array that holds the wholesale prices.                       *
 *      int array that holds the quantity desired.                            *
 *      2d int array that holds the copies available.                         *
 *      float array to hold the wholesale order.                              *
 *      float array to hold the book availability.                            *
 *      float array to hold the profit percentage.                            *
 *      int that holds the number of categories.                              *
 *      int that holds the number of publishers.                              *
 *      int that holds the array size (may be different than MAX_ARRAY)       *
 *  Postconditions:                                                           *
 *      Returns -1 on failure, else return 0.                                 *
 *      Fills the arrays with their values.                                   *
 *****************************************************************************/
int calculatePublishers( float retail[], float wholesale[][MAX_ARRAY],
                         int quantity[], int copies[][MAX_ARRAY], 
                         float wholesale_order[], float book_av[], 
                         float grossprofit[], float percprofit[],
                         int num_category, int num_publisher, 
                         int array_size )
{
    // Counter variables
    int i = 0;
    int j = 0;

    float pct[num_publisher][MAX_ARRAY];       // Percentage array.
    float profit[num_publisher][MAX_ARRAY];   // Profit per book.
    float book_pct = 0.0;                     //Holds book percentage.

    printf( "Performing calculations!\n" );

    // Iterate through all of the publishers.
    for ( i = 0; i <= num_publisher-1; i++ )
    {
        // Obtain the wholesale order by summing the wholesale price
        // of each book.  wholesale price * quantity
        j = 0;
        while ( j <= array_size-1 )
        {
            wholesale_order[i] = wholesale_order[i] + 
                                ( wholesale[i][j] * quantity[j] );
            j++;
        }

        // Calculate the percentage available from each publisher.
        for ( j = 0; j <= array_size-1; j++ )
        {
            pct[i][j] = ( (float) copies[i][j] / (float) quantity[j] );
            if ( ( pct[i][j] ) > 1.4 )
            {
                pct[i][j] = 1.4;
            }
        }

        // Calculate overall availability.
        book_pct = 0.0;
        for ( j = 0; j <= array_size-1; j++ )
        {
            book_pct = book_pct + (pct[i][j]);
        }
        book_av[i] = ((float) (book_pct/(num_category*NUM_BOOKS)));
        
        // Calculate book profit
        for ( j = 0; j <= array_size-1; j++ )
        {
            profit[i][j] = ( ( retail[j] - wholesale[i][j] ) * 
                           quantity[j] );
        }

        // Calculate gross profit.
        for ( j = 0; j <= array_size-1; j++ )
        {
            grossprofit[i] = grossprofit[i] + profit[i][j];
        }

        // Calculate percprofit.
        percprofit[i] = grossprofit[i] / wholesale_order[i];
    }

    return 0;
}
/******************************************************************************
 *  displayCatInfo                                                            *
 *                                                                            *
 *  Displays all the information of the categories on screen.                 *
 *  Preconditions:                                                            *
 *      A char* array that holds the name of categories.                      *
 *      A interger holding the number of categories.                          *
 *      A char* array of authors.                                             *
 *      A char* array of titles.                                              *
 *      A float array of prices.                                              *
 *      int for the actual size of arrays.                                    *                        
 *  Postconditions:                                                           *
 *      Prints all the categories and their books onto the screen.            *
 *****************************************************************************/
void displayCatInfo( char* category_array[], int num_cat, char* author[], 
                     char* title[], float retail[], int quantity[] )
{
    // Counter variables
    int i = 0;
    int j = 0;
	int counter = 0;
	
    // Iterate through the categories and print their information.
    for ( i = 0; i <= num_cat-1; i++ )
    {
        printf( "Category:\t%s\n", category_array[i] );

        for ( j = 0; j <= NUM_BOOKS-1; j++ )
        {
            printf( "\tAuthor:    %s\n", author[counter] );
            printf( "\tBook:      \"%s\"\n", title[counter] );
            printf( "\tRetail:    $%.2f\n", retail[counter] );
            printf( "\tQuantity:  %d\n\n", quantity[counter] );
			
			counter++;
        }
    }
}

/*****************************************************************************
 *  displayResults                                                           *
 *                                                                           *
 *  Displays the results of the calculations.  Displays each publisher and   *
 *  the wholesale order cost.  Also prints out a statement of what range     *
 *  the publishers are in for book availability and profit.  Finally, prints *
 *  out a list of publishers if they have at least 80% and at least 51%      *
 *  profit.                                                                  *
 *  Preconditions:                                                           *
 *      A char* array containing the publishers                              *
 *      A float array containing the wholesale order of each publisher.      *
 *      A float array containing the book_availability for each publisher.   *
 *      A float array containing the gross profit from each publisher.       *
 *      An integer for the number of publishers.                             *
 *      An integer for the array_size of most arrays.                        *
 *  Postconditions:                                                          *
 *      The results are printed to the terminal.                             *
 ****************************************************************************/
void displayResults( char *publishers[], float wholesale_order[], 
                     float book_av[], float percprofit[], 
                     int num_publishers )
{
    int i = 0;          // Counter variable.
    int percentrange[num_publishers];
    int bookrange[num_publishers];
    int results[num_publishers];

    for ( i = 0; i <= num_publishers-1; i++ )
    {
        // Find what range the publisher belongs within books.
        if ( book_av[i]*100 < 80 )
        {
            bookrange[i] = 1;
            results[i] = 0;
        }
        else if ( book_av[i]*100 >= 80 &&
                  book_av[i]*100 <= 120 )
        {
            bookrange[i] = 2;
            results[i] = 1;
        }
        else
        {
            bookrange[i] = 3;
            results[i] = 1;
        }

        // Find what range the publisher belongs to within profit.
        if ( (percprofit[i]*100) < 25 )
        {
            percentrange[i] = 1;
        }
        else if ( (percprofit[i]*100) >= 25 &&
                  (percprofit[i]*100) < 51 )
        {
            percentrange[i] = 2;
        }
        else if ( (percprofit[i]*100) >= 51 &&
                  (percprofit[i]*100) < 75 )
        {
            percentrange[i] = 3;
            results[i] = results[i]+1;
        }
        else if ( (percprofit[i]*100) >= 76 &&
                  (percprofit[i]*100) <= 100 )
        {
            percentrange[i] = 4;
            results[i] = results[i]+1;
        }
        else
        {
            percentrange[i] = 5;
            results[i] = results[i]+1;
        }
    }

    for ( i = 0; i <= num_publishers-1; i++ )
    {
        printf( "%s\t\t$%.2f\n", publishers[i], wholesale_order[i] );

        // Display statement based on range.
        switch( bookrange[i] )
        {
            case 1:
                printf( "\tLess than 80%% of requested books!\n" );
                break;
            case 2:
                printf( "\tBetween 80%% and 120%% of requested books!\n" );
                break;
            case 3:
                printf( "\tOver 120%% of requested books!\n" );
                break;
            default:
                printf( "\tUnable to obtain requested book range!\n" );
        }

        // Display statement based on range.
        switch ( percentrange[i] )
        {
            case 1:
                printf( "\tLess than 25%% profit!\n" );
                break;
            case 2:
                printf( "\tBetween 25%% and 50%% profit!\n" );
                break;
            case 3:
                printf( "\tBetween 51%% and 75%% profit!\n" );
                break;
            case 4:
                printf( "\tBetween 76%% and 100%% profit!\n" );
                break;
            case 5:
                printf( "\tOver 100%% profit!\n" );
                break;
            default:
                printf( "\tUnable to obtain profit range!\n" );
        }
        printf( "\n" );
    }

    // Print a list of publishers if they have at least 80% of 
    // books available and at least a 51% profit.
    printf( "\nPublishers with at least 80%% of books and yield" );
    printf( " at least 51%% profit:\n" );
    for ( i = 0; i <= num_publishers-1; i++ )
    {
        if ( results[i] == 2 )
        {
            printf( "%s\n", publishers[i] );
        }
    }
}
