#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>

#define MATRIX_TOTAL_SIZE    9
#define ROW_SIZE             3


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
	matrixA = readMatrix("testingMatrix.dat");
	matrixB = readMatrix("testingMatrix2.dat");

	printf("BUFFERS No = ");	
	scanf("%d",&NUM_BUFFERS);
	result = malloc ((long)MATRIX_TOTAL_SIZE*(sizeof(long)));
	
	//CREATE THE ARRAY OF ARRAYS long **buffers
	buffers = (long**)malloc (ROW_SIZE*(sizeof(long*)));
	for(i=0;i<ROW_SIZE;i++){
		(buffers+i)=(long) malloc (ROW_SIZE*sizeof(long));
	}
		
	mutexes = malloc ((long)NUM_BUFFERS*sizeof(pthread_mutex_t));
	for(i=0;i<NUM_BUFFERS;i++){
		if(pthread_mutex_init(&mutexes[i],NULL)!=0){printf("\n Mutex init failed\n");return 1;}
	}
		
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
	for(i =0;i<ROW_SIZE;i++){free(buffers[i]);}
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
	long myMatrix = malloc (ROW_SIZE sizeof(long));
	for(i=0;i<ROW_SIZE;i++){
		myMatrix[i]=matrix[i*ROW_SIZE+col];	
		printf("COLUMNS: %d\n",myMatrix[i]);
	}
	return myMatrix;
}

long* getRow(int row, long *matrix){
	unsigned long i;
	unsigned long rmatrix =ROW_SIZE*row;
	long myMatrix = malloc (ROW_SIZE sizeof(long));
	for(i=0;i<ROW_SIZE;i++){
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
	for (i=0;i<ROW_SIZE;i++){
		temp=(vec1[i]*vec2[i]);
		//printf("temp: %d\n",temp);	
		suma+=temp;
	}
	return suma;
}
long *multiply(long *matA, long *matB){
	int i,visibleScope;
  	myStruct bufferThreads[ROW_SIZE];

	pthread_t tid [ROW_SIZE];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	if(pthread_attr_getscope(&attr,&visibleScope)!=0){fprintf(stderr,"UNABLE TO GET SCHEDULER SCOPE\n");}
	if(pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM)!=0){printf("Unable to set scheduling policy\n");}

	for(i=0;i<ROW_SIZE;i++){
		bufferThreads[i].row=i;
		bufferThreads[i].matrixA=matA;
		bufferThreads[i].matrixB=matB;

		pthread_create(&tid[i],&attr,runner,(void*)&bufferThreads[i]);
	}

	for(i =0; i<ROW_SIZE;i++){ pthread_join(tid[i],NULL);}
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
	       //Buffer Satatus
	myStruct *mR =param; //1
	int lockBuffer;      //2 
	lockBuffer = getLock(); //3

	while(-1==lockBuffer){lockBuffer = getLock();sleep(1);}      //Espera un buffer disponible
	
	for(i=0; i<ROW_SIZE;i++){
	//printf("EL OOOOOOOB");
	
	printf("DOT_PRODCUT : %ld\n", dotProduct(getRow(mR->row,mR->matrixA),getColumn(i,mR->matrixB)));
		buffers[mR->row][i] = dotProduct(getRow(mR->row,mR->matrixA),getColumn(i,mR->matrixB));
	}
	
	for(j=0; j<ROW_SIZE;j++){
		
		result[(mR->row*ROW_SIZE)+j]=buffers[mR->row][j];
	}
	releaseLock(lockBuffer);
	pthread_exit(0);
}
