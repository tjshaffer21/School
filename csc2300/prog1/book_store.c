/******************************************************************************
 *  Program:      book_store.c                                                *
 *  Programmer:   Thomas Shaffer                                              *
 *  Course:       CSC 2300-001                                                *
 *  Assignment:   1                                                           *
 *  Date:         25 September 2008                                           *
 *                                                                            *
 *  Description:                                                              * 
 *  A simulation of a book store to check the financial parameters.  The      *
 *  simulation takes the number of books per given category, the average      * 
 *  price per book, and the percentage at which the book will be sold.        *
 *  With the information from the user, the simulation will calculate the     *
 *  selling price and gross receipts of each category.  With that information *
 *  the store will also calculate the average selling price for each book in  *
 *  the book store.                                                           *
 *****************************************************************************/
#include <stdio.h>

#define MAX_ARRAY_SIZE 10
#define MAX_CATEGORY_NAME 15

void run_store();
void num_of_books( int books[] );
void avg_price( float cat_price[] );
void selling_percent( float selling_per[] );
void calculate_selling_price( float avg_price[], float percent[],
                              float s_price[] );
void calculate_gross_receipts( int book[], float s_price[], 
                               float g_receipts[] );
void calculate_store_avg( int book[], float g_receipts[] );
void display_cat_data( int books[], float avg_price[], float selling_per[] );
void display_selling_price( int books[], float s_price[] );
void display_gross_receipts( int books[], float g_receipts[] );
void display_store_avg( int books, float gross, float avg );
void list_cat( int index, char list[] );

int main( void )
{
    char cont = 'y';
    int check = 1;

    // Run the simulation at least once.
    while ( cont != 'n' )
    {
        check = 1;
        run_store();

        // If cont != y || cont != n then continue asking to continue.
        while ( check != 0 )
        {
            // Empties stdin buffer.
            while( getchar() != '\n' );

            printf( "Do you wish to run the simulation again? (y/n) " );
            scanf( "%c", &cont );

            if ( cont == 'y' || cont == 'n' )
            {
                check = 0;
            }
        }
    }  

    printf( "\nThank you for using Plus or Minus Books simulation.\n\n" );

    return 0;
}

/******************************************************************************
 *  run_store                                                                 *
 *                                                                            *
 *  The main user interface for the application.                              *
 *  Preconditions:                                                            *
 *      None.                                                                 *
 *  Postconditions:                                                           *
 *      None.                                                                 *
 *****************************************************************************/
void run_store()
{
    // Arrays to hold user input.
    int book_num[MAX_ARRAY_SIZE] = { 0 };
    float avg_book_price[MAX_ARRAY_SIZE] = { 0.0 };
    float percent[MAX_ARRAY_SIZE] = { 0.0 };
    float selling_price[MAX_ARRAY_SIZE] = { 0.0 };
    float gross_receipts[MAX_ARRAY_SIZE]  = { 0.0 };

    printf( "\nWelcome to Plus or Minus Books!\n\n" );

    // Obtain user input.
    num_of_books( book_num );
    printf( "\n" );
    avg_price( avg_book_price );
    printf( "\n" );
    selling_percent( percent );
    printf( "\n" );

    // Display information.
    display_cat_data( book_num, avg_book_price, percent );
    printf( "\n" );

    // Perform calculations and display information.
    printf( "Performing financial calculations...\n\n" );

    calculate_selling_price( avg_book_price, percent, selling_price );
    display_selling_price( book_num, selling_price );
    printf( "\n" );

    calculate_gross_receipts( book_num, selling_price, gross_receipts );
    display_gross_receipts( book_num, gross_receipts );
    printf( "\n" );

    calculate_store_avg( book_num, gross_receipts );
    printf( "\n" );
}

/******************************************************************************
 *  num_of_books                                                              *
 *                                                                            *
 *  Obtains the number of books for each category.                            *
 *  Preconditions:                                                            *
 *      An integer array that holds the number of books for each category.    *
 *      The user should input a value higher than 0; however, if an incorrect *
 *      value is used then it is defaulted to 0.                              *
 *  Postconditions:                                                           *
 *      The array is filled with the user's input.                            *
 *****************************************************************************/
void num_of_books( int books[] )
{
    // Array used to obtain category names.
    char category[MAX_CATEGORY_NAME] = { '~' };

    // Counter variables.
    int i = 0;
    int j = 0;

    printf( "How many books will Plus or Minus carry for each category?\n" );
    printf( "Enter an integer (0).\n\n" );

    // Loop through all of the categories.
    for ( i = 0; i <= MAX_ARRAY_SIZE-1; i++ )
    {
        list_cat( i, category );

        // Obtain the entire name of the category.
        j = 0;
        while ( category[j] != '~' )
        {
            printf( "%c", category[j] );
            j++;
        }

        printf( ": " );
        scanf( "%d", &books[i] );

        if ( books[i] < 0 )
        {
            books[i] = 0;
        }
    }
}

/******************************************************************************
 *  avg_price                                                                 *
 *                                                                            *
 *  Obtains the average price for books in each category.                     *
 *  Preconditions:                                                            *
 *      A floating point array that holds the average price per category.     *
 *      The user should input a value above 0.00; however, if not then the    *
 *      default is 5.00.                                                      *
 *  Postconditions:                                                           *
 *      The array is filled with the user's input.                            *
 *****************************************************************************/
void avg_price( float avg[] )
{
    char category[MAX_CATEGORY_NAME] = { '~' };

    // Counter variables
    int i = 0;
    int j = 0;

    printf( "What is the average price for the books in each category?\n" );
    printf( "Enter a non-negative float (5.00).\n\n" );

    // Iterate through all of the categories.
    for ( i = 0; i <= MAX_ARRAY_SIZE-1; i++ )
    {
        list_cat( i, category );

        // Print the entire category name.
        j = 0;
        while ( category[j] != '~' )
        {
            printf( "%c", category[j] );
            j++;
        }

        printf( ": " );
        scanf( "%f", &avg[i] );

        // Set to default if price is <= 0.
        if ( avg[i] <= 0.00 )
        {
            avg[i] = 5.00;
        }
    }
}

/******************************************************************************
 *  selling_percent                                                           *
 *                                                                            *
 *  Obtains the selling percent from the user, for each category.             *
 *  Preconditions:                                                            *
 *      A floating array that holds the selling percent for each category.    *
 *      Input from user cannot be less than zero or greater than 100.         *
 *      If the user inputs an incorrect value then the default value is used. *
 *      The default is 50%.                                                   *
 *  Postconditions:                                                           *
 *      The input from the user is calculated to decimal form and inserted    *
 *      into the floating point array from parameter.                         *
 *****************************************************************************/
void selling_percent( float selling_per[] )
{
    char category[MAX_CATEGORY_NAME] = { '~' };
    int price[MAX_ARRAY_SIZE] = { 0 };

    // Counter variables
    int i = 0;
    int j = 0;
 
    printf( "What is the sell percentage for each category?\n" );
    printf( "Enter an integer value (50).\n\n" );

    // Loop through each book category.
    for ( i = 0; i <= MAX_ARRAY_SIZE-1; i++ )
    {
        j = 0;

        list_cat( i, category );

        // Print the category's entire name.
        while ( category[j] != '~' )
        {
            printf( "%c", category[j] );
            j++;
        }

        printf( ": " );
        scanf( "%d", &price[i] );

        // If price[i] is not within the bounds of [0, 100] then set
        // to the default value of 50% (0.5).
        if ( price[i] < 0 || price[i] > 100 )
        {
            selling_per[i] = 0.5;
        }
        else
        {
            selling_per[i] = ( ( ( float ) price[i] ) / 100.0 );
        }
    }
}

/******************************************************************************
 *  calculate_selling_price                                                   *
 *                                                                            *
 *  Calculates the selling price by multiplying the average price and the     *
 *  selling percentage.                                                       *
 *  Preconditions:                                                            *
 *      A float array holding the average price.                              *
 *      A float array holding the selling percentage.                         *
 *      A float array to hold the selling price.                              *
 *  Postconditions:                                                           *
 *      Displays the selling price for each category.                         *
 *****************************************************************************/
void calculate_selling_price( float avg_price[], float percent[],
                              float s_price[] )
{
    // Counter variable.
    int i = 0;

    // Loop through arrays and calculate the selling price.
    for( i = 0; i <= MAX_ARRAY_SIZE-1; i++ )
    {
        s_price[i] = avg_price[i] * percent[i];
    }
}

/******************************************************************************
 *  calculate_gross_receipts                                                  *
 *                                                                            *
 *  Calculates the gross receipts by multiplying the amount of books in a     *
 *  category with the selling price.                                          *
 *  Preconditions:                                                            *
 *      An integer array that holds how many books are in each category.      *
 *      A float array that holds the selling price for each category.         *
 *      A float array that holds the gross receipts for each category.        *
 *  Postconditions:                                                           *
 *      The gross receipts for each category is calculated and stored within  *
 *      the g_receipts array.                                                 *
 *****************************************************************************/
void calculate_gross_receipts( int books[], float s_price[], 
                               float gross_receipts[] )
{
    // Counter variable.
    int i = 0;

    // loop through arrays and calculate gross receipts for each category.
    for ( i = 0; i <= MAX_ARRAY_SIZE-1; i++ )
    {
        gross_receipts[i] = s_price[i] * ((float) books[i]);
    }
}

/******************************************************************************
 *  calculate_store_avg                                                       *
 *                                                                            *
 *  Calculates the average price per book by dividing the total number of     *
 *  of books with the total gross receipts.                                   *
 *  Preconditions:                                                            *
 *      An integer array that holds the number of books.                      *
 *      A float array that holds the gross receipts.                          *
 *  Postconditions:                                                           *
 *      The total number of books, total gross receipts, and average sale     *
 *      price is sent to display_store_avg()                                  *
 *****************************************************************************/
void calculate_store_avg( int books[], float gross_receipts[] )
{
    // Calculation variables
    int total_books = 0;
    float total_gross = 0.0;
    float avg_price = 0.0;

    // Counter variable.
    int i = 0;

    // Loop through arrays and calculate totals.
    for( i = 0; i <= MAX_ARRAY_SIZE-1; i++ )
    {
        total_books = total_books + books[i];
        total_gross = total_gross + gross_receipts[i];
    }

    avg_price = total_gross / total_books;

    display_store_avg( total_books, total_gross, avg_price );
}

/******************************************************************************
 *  display_cat_data                                                          *
 *                                                                            *
 *  Prints out all the information for each category onto the screen.         *
 *  Preconditions:                                                            *
 *      An integer array that holds the number of book.                       *
 *      A float that holds the average price for each category.               *
 *      A float that holds the selling percent for each category.             *
 *  Postconditions:                                                           *
 *      The arrays are used to display the category information.              *
 *****************************************************************************/
void display_cat_data( int books[], float avg_price[], float selling_per[] )
{
    int i = 0;
    int j = 0;

    char list[MAX_CATEGORY_NAME] = { '~' };

    // Loop through each category available.
    for ( i = 0; i <= MAX_ARRAY_SIZE-1; i++ )
    {
        list_cat( i, list );

        // Loop to print the name out of the char array.
        j = 0;
        while( list[j] != '~' )
        {
            printf( "%c", list[j] );
            j++;
        }

        printf( ":\nAmount is %d, average price is $%.2f, ", books[i], 
                avg_price[i] );
        printf( "and selling percentage is %d%%.\n\n", 
                ( ( int )( selling_per[i]*100 ) ) );
    }
}

/******************************************************************************
 *  display_selling_price                                                     *
 *                                                                            *
 *  Prints out the selling price for each category onto the screen.           *
 *  Preconditions:                                                            *
 *      An integer array that holds the number of book.                       *
 *      A float array that holds the selling price for each book.             *
 *  Postconditions:                                                           *
 *      Information about the selling price for each category is displayed    *
 *      on screen.                                                            *
 *****************************************************************************/
void display_selling_price( int books[], float s_price[] )
{
    char category[MAX_CATEGORY_NAME] = { '~' };

    // Counter variables
    int i = 0;
    int j = 0;

    // Loop through all the categories.
    for ( i = 0; i <= MAX_ARRAY_SIZE-1; i++ )
    {
        list_cat( i, category );

        printf( "The selling price for " );

        // Print the entire category name.
        j = 0;
        while ( category[j] != '~' )
        {
            printf( "%c", category[j] );
            j++;
        }

        printf( " is $%.2f.\n", s_price[i] );
    }
}

/******************************************************************************
 *  display_gross_receipts                                                    *
 *                                                                            *
 *  Prints out the gross receipts for each category onto the screen.          *
 *  Preconditions:                                                            *
 *      An integer array that holds the number of book.                       *
 *      A float array that holds the gross receipt information.               *
 *  Postconditions:                                                           *
 *      Information about the gross receipts for each category is displayed   *
 *      on screen.                                                            *
 *****************************************************************************/
void display_gross_receipts( int books[], float g_receipts[] )
{
    char category[MAX_CATEGORY_NAME] = { '~' };

    // Counter variables
    int i = 0;
    int j = 0;

    // Loop through all categories.
    for ( i = 0; i <= MAX_ARRAY_SIZE-1; i++ )
    {
        list_cat( i, category );

        printf( "The gross receipts for " );

        // Print the entire category name.
        j = 0;
        while ( category[j] != '~' )
        {
            printf( "%c", category[j] );
            j++;
        }

        printf( " totals to be $%.2f.\n", g_receipts[i] );
    }
}

/******************************************************************************
 *  display_store_avg                                                         *
 *                                                                            *
 *  Prints out the total number of books, total number of gross receipts, and *
 *  the average price per book.                                               *
 *  Preconditions:                                                            *
 *      An integer that holds the total number of books.                      *
 *      A float that holds the total of gross receipts.                       *
 *      A float that holds the average price per book.                        *
 *  Postconditions:                                                           *
 *      Information is printed out on screen.                                 *
 *****************************************************************************/
void display_store_avg( int books, float gross, float avg )
{
    printf( "There are a total of %d books.\n", books );
    printf( "The gross receipts total to be $%.2f.\n", gross );
    printf( "The average selling price per book is $%.2f.\n", avg );
}

/******************************************************************************
 *  list_cat                                                                  *
 *                                                                            *
 *  Holds a list of all the cagetory of books in the store.                   *
 *  Preconditions:                                                            *
 *      An integer from 0 to MAX_ARRAY_SIZE that will decide which book       * 
 *      category to retrieve.                                                 *
 *      A char array of size MAX_CATEGORY_NAME that will hold the name of     *
 *      the book category we want to use.                                     *
 *  Postconditions:                                                           *
 *      The character array is modified to hold the value of a given array    *
 *      based on the index that was given as a parameter.                     *
 *****************************************************************************/
void list_cat( int index, char list[] )
{
    // Char arrays to hold each different category.
    char business[MAX_CATEGORY_NAME] = { 'B', 'u', 's', 'i', 'n', 'e', 's',
                                         's', '~' };
    char cookbooks[MAX_CATEGORY_NAME] = { 'C', 'o', 'o', 'k', 'b', 'o', 'o', 
                                          'k', 's', '~' };
    char history[MAX_CATEGORY_NAME] = { 'H', 'i', 's', 't', 'o', 'r', 'y', 
                                        '~' };
    char law[MAX_CATEGORY_NAME] = { 'L', 'a', 'w', '~' };
    char literature[MAX_CATEGORY_NAME] = { 'L', 'i', 't', 'e', 'r', 'a', 't',
                                           'u', 'r', 'e', '~' };
    char mystery[MAX_CATEGORY_NAME] = { 'M', 'y', 's', 't', 'e', 'r', 'y', 
                                        '~' };
    char philosophy[MAX_CATEGORY_NAME] = { 'P', 'h', 'i', 'l', 'o', 's', 'o',
                                           'p', 'h', 'y', '~' };
    char scifi[MAX_CATEGORY_NAME] = { 'S', 'c', 'i', '-', 'f', 'i', '~' };
    char selfhelp[MAX_CATEGORY_NAME] = { 'S', 'e', 'l', 'f', '-', 'h', 'e', 
                                         'l', 'p', '~' };
    char sports[MAX_CATEGORY_NAME] = { 'S', 'p', 'o', 'r', 't', 's', '~' };

    // Counter variable
    int i = 0;

    // Depending on the index, decided by the order of the category list,
    // the list array is assigned the category the coincides with the index.
    if ( index == 0 )           // Business
    {
        for ( i = 0; i <= MAX_CATEGORY_NAME-1; i++ )
        {
            list[i] = business[i];
        }
    }
    else if ( index == 1 )      // Cookbooks
    {
        for ( i = 0; i <= MAX_CATEGORY_NAME-1; i++ )
        {
            list[i] = cookbooks[i];
        }
    }
    else if ( index == 2 )      // History
    {
        for ( i = 0; i <= MAX_CATEGORY_NAME-1; i++ )
        {
            list[i] = history[i];
        }
    }
    else if ( index == 3 )      // Law
    {
        for ( i = 0; i <= MAX_CATEGORY_NAME-1; i++ )
        {
            list[i] = law[i];
        }
    }
    else if ( index == 4 )      // Literature
    {
        for ( i = 0; i <= MAX_CATEGORY_NAME-1; i++ )
        {
            list[i] = literature[i];
        }
    }
    else if ( index == 5 )      // Mystery
    {
        for ( i = 0; i <= MAX_CATEGORY_NAME-1; i++ )
        {
            list[i] = mystery[i];
        }
    }
    else if ( index == 6 )      // Philosophy
    {
        for ( i = 0; i <= MAX_CATEGORY_NAME-1; i++ )
        {
            list[i] = philosophy[i];
        }
    }
    else if ( index == 7 )      // Sci-fi
    {
        for ( i = 0; i <= MAX_CATEGORY_NAME-1; i++ )
        {
            list[i] = scifi[i];
        }
    }
    else if ( index == 8 )      // Self-help
    {
        for ( i = 0; i <= MAX_CATEGORY_NAME-1; i++ )
        {
            list[i] = selfhelp[i];
        }
    }
    else                        // Sports
    {
        for ( i = 0; i <= MAX_CATEGORY_NAME-1; i++ )
        {
            list[i] = sports[i];
        }
    }
}
