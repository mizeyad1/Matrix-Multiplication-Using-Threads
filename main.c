#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
void* multiplyrow(void*);
void* multiplyelement(void*);
void printToFile();
int n1,n2,m1,m2;
int **outputMatrix;
int ** inputMatrix1;
int ** inputMatrix2;
int threadCount=0;
double timev1;
double timev2;

int main()
{
    //n1m1 inputMatrix1 n2m2inputMatrix2
        int i,j,q;
    FILE* filePointer;
    filePointer=fopen("input.txt","r");
    char c=fgetc(filePointer);
     n1=(int)c-48;
    printf("no. of rows of first matrix %d\n",n1);
    c=fgetc(filePointer);
    c=fgetc(filePointer);
     m1=(int)c-48;
    printf("no. of columns of first matrix %d\n",m1);
  //  int inputMatrix1 [n1][m1];
  inputMatrix1 = (int **)malloc(n1 * sizeof(int *));
    for (i=0; i<n1; i++)
         inputMatrix1[i] = (int *)malloc(m1 * sizeof(int));
   fgetc(filePointer);
    for(i=0;i<n1;i++)
    {

        for(j=0;j<m1;j++)
        {

            fscanf(filePointer, "%d ", &inputMatrix1[i][j]);


        }


    }
    for(i=0;i<n1;i++)
    {
        for(j=0;j<m1;j++)
        {
            printf("%d ",inputMatrix1[i][j]);
        }
        printf("\n");
    }
    c=fgetc(filePointer);
     n2=(int)c-48;
    fgetc(filePointer);
    c=fgetc(filePointer);
    m2=(int)c-48;
    printf("second matrix rows number is %d and no. of columns is %d\n",n2,m2);
  //  int inputMatrix2 [n1][m1];
    inputMatrix2 = (int **)malloc(n2 * sizeof(int *));
    for (i=0; i<n2; i++)
         inputMatrix2[i] = (int *)malloc(m2 * sizeof(int));
   fgetc(filePointer);
    for(i=0;i<n2;i++)
    {

        for(j=0;j<m2;j++)
        {

            fscanf(filePointer, "%d ", &inputMatrix2[i][j]);


        }


    }
    for(i=0;i<n2;i++)
    {
        for(j=0;j<m2;j++)
        {
            printf("%d ",inputMatrix2[i][j]);
        }
        printf("\n");
    }
   // int outputMatrix[n1][m2];
     outputMatrix = (int **)malloc(n1 * sizeof(int *));
    for (i=0; i<n1; i++)
         outputMatrix[i] = (int *)malloc(m2 * sizeof(int));




         for(i=0;i<n1;i++)
         {
             for(j=0;j<m2;j++)
             {
                 outputMatrix[i][j]=0;
             }
         }

    ////////////////////////////////////////////////////////////// First Variation ....each thread evaluates part of the program.
    pthread_t threadz[n1]; //each thread will evaluate one row
    void* p; // Argument to the multiplyrow function.
    clock_t startclock,endclock;

    startclock=clock();
    for(i=0;i<n1;i++)
    {

        pthread_create(&threadz[i],NULL,multiplyrow,p); //creation of threads it takes the thread pointer and null for default attribute then function which each thread executes then it takes argument
        printf("A thread has been created and now entering multiply row function\n");

    }

    for ( i = 0; i < n1; i++)   //joining all threads together and waiting for them to execute
    {

        pthread_join(threadz[i], NULL);
        printf("A thread has been joined now\n");
    }
    endclock=clock();
    timev1=((double)(endclock-startclock))/CLOCKS_PER_SEC;



  printToFile();
printf("The Output matrix is \n");
for(i=0;i<n1;i++)
    {
        for(j=0;j<m2;j++)
        {
            printf("%d ",outputMatrix[i][j]);
        }
        printf("\n");
    }
//////////////////////////////////////////////////////////////////////////Second Variation where each thread calculates single element

int numberOfThreads=n1*m2;
int countThreads=0;
pthread_t *threadzz=(pthread_t*)malloc(numberOfThreads*sizeof(pthread_t));
int* argumentData=NULL;
clock_t startclock2,endclock2;
startclock2=clock();
for(i=0;i<n1;i++)
{
    for(j=0;j<m2;j++)
    {

       argumentData=(int*)malloc((m1+n2+1)*sizeof(int));
       for(q=0;q<n1;q++)
       {
           argumentData[q+m1]=inputMatrix2[q][j];
       }
       for(q=0;q<m1;q++)
       {
           argumentData[q]=inputMatrix1[i][q];
       }


        pthread_create(&threadzz[countThreads++],NULL,multiplyelement,(void*)argumentData);
    }
}

printf("Matrix result after making each thread compute each element is\n");
FILE* fp;
    fp=fopen("output.txt","a");

for(i=0;i<numberOfThreads;i++)
{
    void* l;
    pthread_join(threadzz[i],&l);
    int* u=(int*)l;
    printf("%d ",*u);
    fprintf(fp,"%d ",*u);
    if ((i + 1) % m2 == 0){
          printf("\n");
          fprintf(fp,"\n");
    }
}
endclock2=clock();
timev2=((double)(endclock2-startclock2)/CLOCKS_PER_SEC);
fprintf(fp,"END2 %lf milliseconds\n",timev2);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    return 0;
}
void* multiplyrow(void* parameter)
{
int j,k;
int i=threadCount;
printf("A thread has entered and thread count is %d\n",threadCount);
    for (; i < (threadCount+ 1) ; i++)
      for ( j = 0; j < m2; j++)
          for ( k = 0; k < m1; k++)
               outputMatrix[i][j] += inputMatrix1[i][k] *inputMatrix2[k][j];


threadCount++;

}
void printToFile()
{
    int i,j;
    FILE* fp;
    fp=fopen("output.txt","a");
    for(i=0;i<n1;i++) {
    for(j=0;j<m2;j++) {
    fprintf(fp,"%d ",outputMatrix[i][j]);
}
fprintf(fp,"\n");
}
    fprintf(fp,"END1 %lf milliseconds\n",timev1);
}
void* multiplyelement(void* parameter)
{

    int* elements=(int*)parameter;
    int ans=0;
    int i=0;
    for(i=0;i<m1;i++)
    {
      ans+=elements[i]*elements[i+m1];
    }
    int* finalanswer=(int*)malloc(sizeof(int));
    *finalanswer=ans;
    pthread_exit(finalanswer);

}




