#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>


#define MATRIX_TOTAL_SIZE    9
#define ROWS             3


int NUM_BUFFERS;
long **buffers;
pthread_mutex_t *mutexes;
long* result;

typedef struct{

	long *matrixA;
	long *matrixB;
	long row;

}myStruct;

long* readMatrix (char *filename);
long* getColumn (int col,long *matrix);
long* getRow(int row, long *matrix);
int getLock();
int releaseLock(int lock);
long dotProduct(long *vec1, long *vec2);
long *multiply(long *matA, long*matB);
int saveResultMatrix(long *result);

void *runner(void *param);

int main(void){

	int i;
	long *matrixA,*matrixB,*col,*fila;
	matrixA = readMatrix("testingMatrix.dat");               //CHANGE THIS NAME FILE TO USE YOUR MATRIX 
	matrixB = readMatrix("testingMatrix2.dat");              //CHANGE THIS NAME FILE TO USE YOUR MATRIX 
	result = malloc ((long)MATRIX_TOTAL_SIZE*(sizeof(long)));
	
	printf("BUFFERS No = ");	
	scanf("%d",&NUM_BUFFERS);
	
	
	//CREATE THE ARRAY OF ARRAYS long **buffers
	buffers = (long**)malloc (NUM_BUFFERS*(sizeof(long*)));
	for(i=0;i<NUM_BUFFERS;i++){
		(buffers+i)=(long) malloc (ROWS*sizeof(long));
	}
		
	mutexes = malloc ((long)NUM_BUFFERS*sizeof(pthread_mutex_t));
	for(i=0;i<NUM_BUFFERS;i++){
		if(pthread_mutex_init(&mutexes[i],NULL)!=0){printf("\n Mutex init failed\n");return 1;}
	}
	/*
	for(i=0;i<NUM_BUFFERS;i++){
		(buffers+i)=(long) malloc (ROWS*sizeof(long));
	}
		
	mutexes = malloc ((long)NUM_BUFFERS*sizeof(pthread_mutex_t));
	for(i=0;i<NUM_BUFFERS;i++){
		if(pthread_mutex_init(&mutexes[i],NULL)!=0){printf("\n Mutex init failed\n");return 1;}
	}
	
	*/	
	multiply(matrixA,matrixB);

	if(saveResultMatrix(result)!=0){fprintf(stderr,"Unable to create new file.\n");}

	free(matrixA);
	free(matrixB);

	/* 
	mutexes = malloc ((long)NUM_BUFFERS*sizeof(pthread_mutex_t));
	for(i=0;i<NUM_BUFFERS;i++){
		if(pthread_mutex_init(&mutexes[i],NULL)!=0){printf("\n Mutex init failed\n");return 1;}
	}
		
	multiply(matrixA,matrixB);

	if(saveResultMatrix(result)!=0){fprintf(stderr,"Unable to create new file.\n");}
	*/
	for(i=0; i<NUM_BUFFERS;i++){pthread_mutex_destroy(&mutexes[i]);}
	for(i =0;i<ROWS;i++){free(buffers[i]);}
	free(buffers);
	return 0;
		
}

long* readMatrix(char *filename){
	char buffer[10];
	char *eptr;
	FILE *myFile;
	myFile = fopen(filename,"r");
	long temp;
	long myMatrix =(long *) malloc (MATRIX_TOTAL_SIZE sizeof(long));
	int i=0;
	if(myFile){
		while(fscanf(myFile,"%s",buffer)!=EOF){
			//printf("%s\n",buffer);
			temp = strtoll(buffer,&eptr,10);
			//printf("%d\n",temp);
			myMatrix[i++]=temp;
			//printf("%d\n",myMatrix[i-1]);
		}
		fclose(myFile);
	}
	return myMatrix;
}

long* getColumn (int col,long *matrix){

	unsigned long i;
	long myMatrix = malloc (ROWS sizeof(long));
	for(i=0;i<ROWS;i++){
		myMatrix[i]=matrix[i*ROWS+col];	
		printf("COLUMNS: %d\n",myMatrix[i]);
	}
	return myMatrix;
}

long* getRow(int row, long *matrix){
	unsigned long i;
	unsigned long rmatrix =ROWS*row;
	long myMatrix = malloc (ROWS sizeof(long));
	for(i=0;i<ROWS;i++){
		myMatrix[i]=matrix[rmatrix++];
		printf("ROWS : %d\n",myMatrix[i]);
	} 
	return myMatrix;
}
int getLock(){
	int i=0;
	do{
		if(0==pthread_mutex_trylock(&mutexes[i])){
			return i;
		}
	}while(i++<NUM_BUFFERS-1);
	return -1;
	/*int i;
	for (i=0;i<NUM_BUFFERS;i++){   	
		
	}
	return -1;*/
}
int releaseLock(int lock){
	
    int value;
	if(pthread_mutex_unlock(&mutexes[lock])){
		value=0;
	}
	else{
		value=-1;
	}
 	return value;
}
long dotProduct(long *vec1, long *vec2){
	int i;	
	long temp;
	long suma=0;
	for (i=0;i<ROWS;i++){
		temp=(vec1[i]*vec2[i]);
		//printf("temp: %d\n",temp);	
		suma+=temp;
	}
	return suma;
}
long *multiply(long *matA, long *matB){
	int i,
	    visibleScope,
	    rmatrix;
  	myStruct bufferThreads[ROWS];

	pthread_t tid [ROWS];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	for(i=0;i<ROWS;i++){
		bufferThreads[i].row=i;
		bufferThreads[i].matrixA=matA;
		bufferThreads[i].matrixB=matB;
		pthread_create(&tid[i],&attr,runner,(void*)&bufferThreads[i]);
	}
	/*

	for(i=0;i<ROWS;i++){
		bufferThreads[i].row=i;
		bufferThreads[i].matrixA=matA;
		bufferThreads[i].matrixB=matB;
		pthread_create(&tid[i],&attr,runner,(void*)&bufferThreads[i]);
	}

	*/

	

	for(i =0; i<ROWS;i++){ pthread_join(tid[i],NULL);}
	return 0;
}

int saveResultMatrix(long *result){
	FILE *myFile;
	myFile=fopen("resultado.dat", "w");
	int successful;
	int i;
	
	for(i=0;i<9;i++){
		fprintf(myFile,"%ld\n",result[i]);
	}
	fclose(myFile);
	return 0;
}

void *runner(void *param)
{	
	int i;
	int j;
	int availableBuffer; 
	int count_rows;
	       //Buffer Satatus
	myStruct *mR =param; 
	int lockBuffer;      
	lockBuffer = getLock(); 

	while(-1==lockBuffer){lockBuffer = getLock();sleep(1);}      //Espera un buffer disponible
	
	for(i=0; i<ROWS;i++){
	//printf("EL OOOOOOOB");
	//count_row++;
	//printf("DOT_PRODCUT : %ld\n", dotProduct(getRow(mR->row,mR->matrixA),getColumn(i,mR->matrixB)));
	//printf("DOT_PRODCUT : %ld\n", dotProduct(getRow(mR->row,mR->matrixA),getColumn(i,mR->matrixB)));
		buffers[mR->row][i] = dotProduct(getRow(mR->row,mR->matrixA),getColumn(i,mR->matrixB));
	//buffers[count_row++][i] = dotProduct(getRow(count_row++,mR->matrixA),getColumn(i,mR->matrixB));	
	}
	
	for(j=0; j<ROWS;j++){result[(mR->row*ROWS)+j]=buffers[mR->row][j];}
	releaseLock(lockBuffer);
	pthread_exit(0);
}
