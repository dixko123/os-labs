/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>

    int time[20],n,j,i,pos, temp,total;
    int Processtime[20],numberOfProcess[20];
    int tat[20];
    int averagetime=0, averagetat=0;
    int Priority[10];
 /* 
void RoundRobin(void){
  printf("Enter Total Process:\t "); 
  scanf("%d",&n); 
  remain=n; 
  for(count=0;count<n;count++) 
  { 
    printf("Enter Arrival Time and Burst Time for Process Process Number %d :",count+1); 
    scanf("%d",&at[count]); 
    scanf("%d",&bt[count]); 
    rt[count]=bt[count]; 
  } 
 
 
}
*/

//************************************SJS*******************************************
void PSA (void){
     printf("\nNumber of Process\n");
     scanf("%d",&n);
     
     printf("\nTape the priority of Process\n");
     
     for(i=0;i<n;i++)
    {
        printf("\nProcess%d \n",i+1);
        printf("Priority:");
        scanf("%d",&Priority[i]);
        numberOfProcess[i]=i+1;           //contains process number
    }
    for(i=0;i<n;i++)
    {
        pos=i;
        for(j=i+1;j<n;j++)
        {
            if(Priority[j]<Priority[pos]){
                pos=j;
            }
        }
        temp=Priority[i];
        Priority[i]=Priority[pos];
        Priority[pos]=temp;
        temp=Processtime[i];
        Processtime[i]=Processtime[pos];
        Processtime[pos]=temp;
        temp=numberOfProcess[i];
        numberOfProcess[i]=numberOfProcess[pos];
        numberOfProcess[pos]=temp;
    }
    
    for(i=0;i<n;i++)
    {
    
        printf("\nProcess %d ",numberOfProcess[i]);
    }
}
//************************************SJS*******************************************
void SJS(void){
     printf("\nNumber of Process\n");
     scanf("%d",&n);
     
     printf("Tape time for Process\n");
     
      for(i=0;i<n;i++){
         printf("PROCESS %d:",i+1);
         scanf("%d",&Processtime[i]);
         numberOfProcess[i]=i+1;
      }
         
    for(i=0;i<n;i++)
    {
        pos=i;
        for(j=i+1;j<n;j++)
        {
            if(Processtime[j]<Processtime[pos]){
                pos=j;
            }
        }
        temp=Processtime[i];
        Processtime[i]=Processtime[pos];
        Processtime[pos]=temp;
        temp= numberOfProcess[i];
        numberOfProcess[i]=numberOfProcess[pos];
        numberOfProcess[pos]=temp;
    }
    time[0]=0;
    
     for(i=1;i<n;i++)
    {
        time[i]=0;
        for(j=0;j<i;j++){
            time[i]+=Processtime[j];
            total+=time[i];
        }
        
    }
    total=0;
    
     for(i=0;i<n;i++)
    {
        
        tat[i]=Processtime[i]+time[i];     //calculate turnaround time
        total+=tat[i];
        printf("\nFor Process %d ",numberOfProcess[i]);
        printf("\n      Process Time =  %d ",Processtime[i]);
        printf("\n      Waiting Time =  %d ",time[i]);
        printf("\n      Response Time=  %d ",tat[i]);
    }
}
void FCFS(void){
  //************************************FCFS*******************************************
    printf("\nNumber of Process\n");  
    scanf("%d",&n);
    printf("Tape time for Process\n");
    for(i=0;i<n;i++){
         printf("PROCESS %d:",i+1);
         scanf("%d",&Processtime[i]);
         
    }
    time[0]=0;
    for(i=0;i<n;i++){
        time[i]=0;
        for(j=0;j<i;j++){
            time[i]+=Processtime[j];
        }
         
    }
    for(i=0;i<n;i++)
    {
        tat[i]=Processtime[i]+time[i];
        averagetime+=time[i];
        averagetat+=tat[i];
        printf("\nFor Process %d ",i+1);
        printf("\n      Process Time =  %d ",Processtime[i]);
        printf("\n      Waiting Time =  %d ",time[i]);
        printf("\n      Response Time=  %d ",tat[i]);
        
    }
}

int main()
{
    PSA();          //PRIORITY
   // SJS();        //Shortest Job First
    //FCFS();       //Fisrt Come Fist served
    return 0;
}
