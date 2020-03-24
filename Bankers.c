#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<unistd.h>
#include<pthread.h>

int nofresourses,nofprocesses,i,j;
int *resource;
int *safeseq;
int **allocated;
int **required;
int **need;

pthread_mutex_t lockResources;
pthread_cond_t condition;


int safeseqlist()
{
  int canallocate;
  int temp[nofresourses];
  for(i=0;i<nofresourses;i++)
  {
    temp[i]=resource[i];
  }
  int completed[nofprocesses];
  for(i=0;i<nofprocesses;i++)
  {
    completed[i]=0;
  }
  int nofcompleted=0;
  while(nofcompleted < nofprocesses)
  {
    int safe=0;
    for(i=0;i<nofprocesses;i++)
    {
      if(completed[i]==0)
      {
        canallocate =1;
      for(j=0;j<nofresourses;j++)
      {
        if(need[i][j] >temp[j])
        {
          canallocate =0;
          break;
        }
      }
      if(canallocate==1)
      {
        for(j=0;j<nofresourses;j++)
        {
          temp[j] =temp[j]+allocated[i][j];
        }
          safeseq[nofcompleted]=i;
          completed[i]=1;
          nofcompleted++;
          safe=1;
      }
      }
    }
    if(safe ==0)
    {
      for(int k=0;k<nofprocesses;k++)
      {
        safeseq[k] =-1;
      }
      return (0);
    }
  }
  return (1);
}

int main()
{
  
  {//GET NUMBER OD PROCESSES AND RESOURCES
  printf("\nEnter the no of processes ::");
  scanf("%d",&nofprocesses);
  printf("\nEnter the no of Resources ::");
  scanf("%d",&nofresourses);
  }
 
  {//MEMORYY ALLOCATION

  resource =(int *) malloc(nofresourses*sizeof(*resource));
  safeseq=(int *)malloc(nofprocesses*sizeof(*safeseq));
  required = (int **)malloc(nofprocesses* sizeof(*required));
  for(i=0;i<nofprocesses;i++)
  {
      required[i]=(int *)malloc(nofresourses*sizeof(**required));
  } 
  allocated = (int **)malloc(nofprocesses*sizeof(*allocated));
  for(i=0;i<nofprocesses;i++)
  {
      allocated[i]=(int *)malloc(nofresourses*sizeof(**allocated));
  }
   need = (int **)malloc(nofprocesses * sizeof(*need));
  for(int i=0; i<nofprocesses; i++)
  {
      need[i] = (int *)malloc(nofresourses* sizeof(**need));
  }
  
 }
 
  {//INPUT DATA
  printf("\nEnter the currently available resurces ::\n\n");
  for(i=0;i<nofresourses;i++)
  {
    printf("Enter the resource[%d] -->",i+1);
    scanf("%d",&resource[i]);
    printf("\n");
  }
  
  for(i=0;i<nofprocesses;i++)
  {
    printf("For process %d\n",i+1);
    for(j=0;j<nofresourses;j++)
    {
      printf("\tEnter the Total resources required in %d -->",j+1);
      scanf("%d",&required[i][j]);
      printf("\n");
    }
    printf("\n");
  }
 
  for(i=0;i<nofprocesses;i++)
  {
    printf("for process %d \n",i+1);
    for(j=0;j<nofresourses;j++)
    {
      printf("\tEnter the already allocated resources in %d -->",j+1);
      scanf("%d",&allocated[i][j]);
      printf("\n");
    }
    printf("\n");
  } 

  for(i=0;i<nofprocesses;i++)
  {
    for(j=0;j<nofresourses;j++)
    {
      need[i][j]=required[i][j]-allocated[i][j] ;
    }
  }   
    for(i=0;i<nofprocesses;i++)
    {
      safeseq[i] = -1;
    }
  
 }
  

  
  {//CHECK SAFE SEQUENCE
      if(safeseqlist() == 0)
      {
        printf("\n UNSAFE State--> No allocation can be done ");
        exit(0);
      }
      system("cls");
      printf("Safe sequence Found\n");
      for(i=0;i<nofprocesses;i++)
      {
          printf("\t %d \t",safeseq[i]+1);
      }
  }
  
}

