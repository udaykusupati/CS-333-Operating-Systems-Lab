#include <stdio.h>
main()
{

    FILE *myFile;
    myFile = fopen("n.txt", "r");

    //read file into array
    int numberArray[16];
    int i = 0;
    // fscanf (file, "%d", &i); 
    while(!feof(myFile))
    {
        fscanf(myFile, "%d", &numberArray[i]);
        printf("Number is: %d\n\n", numberArray[i]);
        i++;
    }
    fclose(myFile);


}