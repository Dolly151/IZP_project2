#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structure for representing a bitmap
// rows - number of rows, cols - number of columns
//**data - two dimensional array, dynamically allocated in memory
typedef struct
{
    int rows;
    int cols;
    int **data;
} Image;

// strucutre to represent the coordinates of bitmap
// starting row & col index, ending row & col index
typedef struct
{
    int startRow;
    int startCol;
    int endRow;
    int endCol;
} Coordinates;

// function getColour is used to obtain a "colour" at a given coordinates
// function parameters are a pointer to the Image structure and integers representing
// the coordinates, at we check
// value at given coordinates is read into the variable colour
// if the value is 0 or 1, function returns colour, otherwise it returns -1
int getColour(Image *img, int coordRow, int coordCol)
{
    int colour = img->data[coordRow][coordCol];
    if (colour != 0 && colour != 1)
    {
        return -1;
    }
    return colour;
}
// function getImage is used to load a bitmap from a file and verify its validity
// function parameters are a pointer to a string to the name of the file we want to load
//  and a pointer to the Image structure, into which we store load values
// if there is an error while loading the file or bitmap is invalid, function return 1, otherwise 0
int getImage(const char *fileName, Image *img)
{
    //check if file name is valid
    if (fileName == NULL || strlen(fileName) == 0)
    {
        return 1;
    }
    // declare a pointer for the file input and open the file in read mode
    FILE *input;
    input = fopen(fileName, "r");

    // check if the file is opened succesfully
    if (input == NULL)
    {
        return 1;
    }

    // using the fscanf, we read and store the first two symbols as parameters of the bitmap
    // if they are not two integers,fscanf doesnt return 2 -> condition returns 1
    else if (fscanf(input, "%d %d", &img->rows, &img->cols) != 2)
    {
        fclose(input);
        return 1;
    }
    // check if parameters are positive numbers, if not, it returns 1
    else if (img->rows <= 0 || img->cols <= 0)
    {
        fclose(input);
        return 1;
    }
    // dynamically allocate memory for the 2D array (rows) and allocation success check
    img->data = malloc(img->rows * sizeof(int *));
    if (img->data == NULL)
    {
        fclose(input);
        return 1;
    }
    // allocate memory for each row of the 2D array(columns)
    for (int i = 0; i < img->rows; i++)
    {
        img->data[i] = malloc(img->cols * sizeof(int));

        // allocation success check
        if (img->data[i] == NULL)
        {
            fprintf(stderr, "Allocation failed\n");
            // if memory allocation failed, free previously allocated rows
            for (int j = i - 1; j >= 0; j--)
            {
                free(img->data[j]);
            }
            free(img->data); // free the array of row pointers
            fclose(input);
            return 1;
        }
        // read each pixel value in the data array
        for (int j = 0; j < img->cols; j++)
        {
            // if scanf fails to read an integer, it returns a value different from 1 ->condition
            // is violated and we return 1
            if (fscanf(input, "%d", &img->data[i][j]) != 1)
            {
                fclose(input);
                return 1;
            }
        }
    }
    fclose(input);
    return 0; // if everything is allright, we return 0
}
// the function argTest is used to verify the contents of the bitmap (only 0 or 1)
// and its size(should match the parameters)
// function parameters are a pointer to the Image structure
// if the bitmap meets the requirements, function returns 0, otherwise 1
int argTest(Image *img)
{
    // in the counter, we store the count of valid pixels read from bitmap
    int counter = 0;
    // iterate through the entire bitmap
    for (int i = 0; i < img->rows; i++)
    {
        for (int j = 0; j < img->cols; j++)
        {
            // if getColour returns -1(pixel at given coordinates is not 0 or 1), return 1
            if (getColour(img, i, j) == -1)
            {
                return 1;
            }
            counter++; // if the pixel is valid, increment the counter
        }
    }
    // if the counter does not equal the size of the bitmap,based on the read parameters, return 1
    if (counter != img->rows * img->cols)
    {
        return 1;
    }
    return 0;
}
// the supporting function updateCoords is used to load the coordinates, when calculating
// the coordinates of line segments or square
// function parameters are a pointer to the Coordinates structure and integers represents
// the temporary starting coordinates and the ending coordinates
void updateCoords(Coordinates *coords, int temp_startRow, int temp_startCol,
                  int endRow, int endCol)
{
    // assign the coordinates to the structure
    coords->startRow = temp_startRow;
    coords->startCol = temp_startCol;
    coords->endRow = endRow;
    coords->endCol = endCol;
}

// the function argHline is used to find the longest horizontal line in a given bitmap
// function parameter is a pointer to the Image structure
// if function finds a valid line, it prints its starting and ending coordinates
// otherwise it prints "Not Found"
void argHline(Image *img)
{
    // longestLine represents the length of the longest line found
    // tempLine stores the length of the longest line found so far
    // temp_ variables are used to store the starting coordinates of the curent line
    int longestLine = 0;
    int tempLine = 0;
    int temp_startRow = -1, temp_startCol = -1;
    Coordinates coords = {-1, -1, -1, -1}; // initialize coordinates to -1

    // iterate through each row of the bitmap
    for (int i = 0; i < img->rows; i++)
    {
        // iterate through each column of the current row
        for (int j = 0; j < img->cols; j++)
        {
            // if the pixel value on the current coordinates is 1
            if (getColour(img, i, j) == 1)
            {
                // if its start of a new line, store the starting coordinates to temp variables
                if (tempLine == 0)
                {
                    temp_startRow = i;
                    temp_startCol = j;
                }
                tempLine++; // increase the length of the current line
            }
            else
            {
                // if the line ends, check if its the longest line found so far
                if (tempLine > longestLine)
                {
                    // using updateCoords, update coordinates for the longest line
                    updateCoords(&coords, temp_startRow, temp_startCol, i, j - 1);
                    longestLine = tempLine; // update the longestLine length
                }
                tempLine = 0; // reset the temporary line length
            }
        }
        // after finishing the row, chceck if the last line is the longest
        if (tempLine > longestLine)
        {
            // using updateCoords, update coordinates for the longest line that ends at last column
            updateCoords(&coords, temp_startRow, temp_startCol, i, img->cols - 1);
            longestLine = tempLine; // update the longesLine length
        }
        tempLine = 0; // reset the temporary line length after processing the row
    }
    // if valid line was found print coordinates
    if (longestLine > 0)
    {
        printf("%d %d %d %d\n", coords.startRow, coords.startCol,
               coords.endRow, coords.endCol);
    }
    else
    {
        printf("Not found\n");
    }
}

// the function argHline is used to find the longest vertical line in a given bitmap
// function parameter is a pointer to the Image structure
// if function finds a valid line, it prints its starting and ending coordinates
// otherwise it prints "Not Found"
void argVline(Image *img)
{
    int longestLine = 0;
    int tempLine = 0;
    int temp_startRow = -1, temp_startCol = -1;
    Coordinates coords = {-1, -1, -1, -1}; // initialize coordinates to -1

    // iterate through each column of the bitmap
    for (int i = 0; i < img->cols; i++)
    {
        // iterate through each row in the current column
        for (int j = 0; j < img->rows; j++)
        {
            // if the pixel value on the current coordinates is 1
            if (getColour(img, j, i) == 1)
            {
                // if its start of a new line, store the starting coordinates to temp variables
                if (tempLine == 0)
                {
                    temp_startRow = j;
                    temp_startCol = i;
                }
                tempLine++; // increase the length of the current line
            }
            else
            {
                // if the line ends, check if its the longest line found so far
                if (tempLine > longestLine)
                {
                    // using updateCoords, update coordinates for the longest line
                    updateCoords(&coords, temp_startRow, temp_startCol, j - 1, i);
                    longestLine = tempLine; // update the longesLine length
                }
                // if the line has the same length as longestLine, but within a smaller startRow
                // index (due to the assignment)
                else if (tempLine == longestLine && temp_startRow < coords.startRow)
                {
                    updateCoords(&coords, temp_startRow, temp_startCol, j - 1, i);
                }
                tempLine = 0; // reset the temporary line length
            }
        }
        // after iterating through all columns, check if the last line is the longest
        if (tempLine > longestLine)
        {
            // using updateCoords, update coordinates for the longest line found on the current row
            updateCoords(&coords, temp_startRow, temp_startCol, img->rows - 1, i);
            longestLine = tempLine; // update the longestLine length
        }
        tempLine = 0; // reset temporary line length after each row
    }

    // if valid line was found, print coordinates
    if (longestLine > 0)
    {
        printf("%d %d %d %d\n", coords.startRow, coords.startCol,
               coords.endRow, coords.endCol);
    }
    else
    {
        printf("Not found\n");
    }
}

// the function argSquare is used to find the biggest square (corresponding to the assignment)
// function parameter is a pointer to the Image structure
// if function finds a valid square, it prints its starting and ending coordinates
// otherwise it prints "Not Found"
void argSquare(Image *img)
{
    // in max_square, we store max possible square size based on the smaller dimension of bitmap
    int max_square = (img->rows < img->cols) ? img->rows : img->cols;
    int biggestSquare = 0;                 // biggestSquare tracks size of the largest found square
    int found1x1 = 0;                      // found1x1 tracks if 1x1 square has already been found
    Coordinates coords = {-1, -1, -1, -1}; // initialize coordinates to -1

    // iterates through all possible square sizes
    for (int size = 1; size <= max_square; size++)
    {

        // iterate through the whole bitmap
        for (int i = 0; i <= img->rows - size; i++)
        {
            for (int j = 0; j <= img->cols - size; j++)
            {
                int valid = 1; // flag to track if the current square is valid

                // if top-left corner isnt 1, skip this position
                if (getColour(img, i, j) != 1)
                {
                    continue;
                }
                // if we find 1x1 square, we update coordinates and set biggestSquare and found1x1
                if (size == 1)
                {
                    updateCoords(&coords, i, j, i, j);
                    biggestSquare = 1;
                    found1x1 = 1;
                    break;
                }
                // validate the boundaries of the square
                for (int k = 0; k < size; k++)
                {
                    // check the top and botoom boundaries of the square, using getColour
                    if (getColour(img, i, j + k) != 1 ||
                        getColour(img, i + (size - 1), j + k) != 1)
                    {
                        valid = 0; // mark square as invalid
                        j += k;    // skip invalid coordinates in columns
                        break;     // exit the current loop
                    }
                    // check the left and right boundaries of the square
                    if (getColour(img, i + k, j) != 1 ||
                        getColour(img, i + k, j + (size - 1)) != 1)
                    {
                        valid = 0; // mark square as invalid
                        i += k;    // skip invalid coordinates in rows
                        break;     // exit the current loop
                    }
                }
                // if the current square is valid and larger than the previously largest square
                if (valid && size > biggestSquare)
                {
                    biggestSquare = size; // set biggestSquare to size
                    // using updateCoords, update coordinates to current square
                    updateCoords(&coords, i, j, i + size - 1, j + size - 1);
                }
            }
            // if 1x1 square has been found, exit the loop with size = 1
            if (size == 1 && found1x1)
                break;
        }
    }
    // if we find valid square, print his coordinates
    if (biggestSquare > 0)
    {
        printf("%d %d %d %d\n", coords.startRow, coords.startCol, coords.endRow, coords.endCol);
    }
    else
    {
        printf("Not found\n");
    }
}

// the function argHelp displays help informations for the user
void argHelp(void)
{
    printf("Cilem tohoto programu je vypis pocatecni a koncove souradnice nalezene usecky nebo ctverce, v zadane bitmape\n");
    printf("Program se spousti v nasledujici podobe:\n ./figsearch --help -> program vypise napovedu pouzivani programu\n");
    printf("./figsearch test SOUBOR -> zkontroluje, ze soubor dany druhym argumentem programu obsahuje radnou definici bitmapoveho obrazku");
    printf(" pokud format obrazku odpovida definici, vytiskne Valid, jinak vytiskne Invalid\n");
    printf("./figsearch hline SOUBOR -> vytiskne pocatecni a koncovou souradnici prvni nejdelsi horizontalni usecky v danem obrazku\n");
    printf("./figsearch vline SOUBOR -> vytiskne pocatecni a koncovou souradnici prvni nejdelsi vertikalni usecky v danem obrazku\n");
    printf("./figsearch square SOUBOR -> vytiskne pocatecni a koncovou souradnici prvniho nejvetsiho ctverce v danem obrazku\n");
    printf("SOUBOR reprezentuje jmeno souboru s obsahem bitmapy");
}

// the supporting function freeImafe deallocates the memory used by the Image structure
// function parameter is a pointer to the Image structure
void freeImage(Image *img)
{
    // iterate through each row and free memory allocated for it
    for (int i = 0; i < img->rows; i++)
    {
        free(img->data[i]);
    }
    free(img->data); // then free the array of row pointers
}

// the function argSwitch is used to processed the input arguments to determine which function
// should be called,using strcmp function
// it checks the argument passed by the user and calls the corresponding function
// function parameters are an array of pointers *arguments[],which represents the arguments
// provided by the user and pointer to the Image structure
int argSwitch(char *arguments[], Image *img)
{
    // if the user requested "test" operation
    if (strcmp(arguments[1], "test") == 0)
    {
        // if the bitmap doesnt match the specifications, print Invalid
        if (getImage(arguments[2], img) != 0 || argTest(img) != 0)
        {
            fprintf(stderr, "Invalid\n");
        }
        // if it does, print Valid and call freeImage
        else
        {
            printf("Valid");
            freeImage(img);
        }
    }
    // if the use requested "hline" operation
    else if (strcmp(arguments[1], "hline") == 0)
    {
        // if the bitmap doesnt match the specifications, print Invalid
        if (getImage(arguments[2], img) != 0 || argTest(img) != 0)
        {
            fprintf(stderr, "Invalid\n");
        }
        // if it does, call argHline, then freeImage
        else
        {
            argHline(img);
            freeImage(img);
        }
    }
    // if the use requested "vline" operation
    else if (strcmp(arguments[1], "vline") == 0)
    {
        // if the bitmap doesnt match the specifications, print Invalid
        if (getImage(arguments[2], img) != 0 || argTest(img) != 0)
        {
            fprintf(stderr, "Invalid\n");
        }
        // if it does, call argVline, then freeImage
        else
        {
            argVline(img);
            freeImage(img);
        }
    }
    // if the use requested "square" operation
    else if (strcmp(arguments[1], "square") == 0)
    {
        // if the bitmap doesnt match the specifications, print Invalid
        if (getImage(arguments[2], img) != 0 || argTest(img) != 0)
        {
            fprintf(stderr, "Invalid\n");
        }
        // if it does, call argSquare, then freeImage
        else
        {
            argSquare(img);
            freeImage(img);
        }
    }
    // if none of above operation is requested, user entered wrong input, return 1
    else
    {
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{

    Image img; // declare an Image structure to store the bitmap data
    // if user entered less than 2 arguments, printf error message and call argHelp function
    if (argc < 2 || argc > 3)
    {
        fprintf(stderr, "Invalid number of arguments\n");
        argHelp();
        return 1;
    }
    // if second argument is "--help", call argHelp function
    if (strcmp(argv[1], "--help") == 0 && argc < 3)
    {
        argHelp();
    }
    else
    {
        // if argSwitch doesnt return 0, print error message and call argHelp function
        // otherwise it will call corresponding function
        if (argSwitch(argv, &img) != 0)
        {
            fprintf(stderr, "Invalid arguments\n");
            argHelp();
            return 1;
        }
    }
    return 0;
}
