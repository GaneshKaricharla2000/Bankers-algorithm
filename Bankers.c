#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

int nofresourses,nofprocesses,i,j;
int *resource;
int *safeseq;
int **allocated;
int **required;
int **need;
int rank =0;

pthread_mutex_t lockResources;
pthread_cond_t condition;

void release()
{//releasing the memory
    for(i=0;i<nofprocesses;i++)
    {
      free(allocated[i]);
      free(required[i]);
      free(need[i]);
    }
    free(resource);
    free(safeseq);
    free(allocated);    
    free(need);
}
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

void* process(void *arg)
{
  int a;
  {//CONDITION CHECKING
    a =*((int *)arg);
    pthread_mutex_lock(&lockResources);
    while(a !=safeseq[rank])
    {
      pthread_cond_wait(&condition,&lockResources);
    }
  }
  {//Running Process
    printf("\n\n\n\n");
    printf("==============================");
    printf("\n Process --> %d\n",a+1);
    printf("==============================");
    printf("\n\n\tALLOCATED::");
    for(i=0;i<nofresourses;i++)
    {printf("   %d   ",allocated[a][i]);}
    printf("\n\n\tNEED::");
    for(i=0;i<nofresourses;i++)
    {printf("   %d   ",need[a][i]);}
    printf("\n\n\tAVAILABLE ::");
    for(i=0;i<nofresourses;i++)
    {printf("   %d   ",resource[i]);}
    
    printf("\nResource Allocated");
    printf("\nResources released by Process");
    printf("\nResources available-->");
    for(i=0;i<nofresourses;i++)
    {
      resource[i]=resource[i]+allocated[a][i];
      printf("   %d  ",resource[i]);   
    }
  }
  {//Moving to nxt process
    rank++;
    pthread_cond_broadcast(&condition);
    pthread_mutex_unlock(&lockResources);
    pthread_exit(NULL);
  }

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
        release();
        exit(0);
      }
      system("cls");
      printf("Safe sequence Found\n");
      for(i=0;i<nofprocesses;i++)
      {
          int t=safeseq[i];
          printf("%d  ",t+1);
      }
      printf("Safe sequence Aquired");
  }
  {//Process Occurs
      pthread_t processes[nofprocesses];
      pthread_attr_t attr;
      pthread_attr_init(&attr);
      
      int processid[nofprocesses];
      for(i=0;i<nofprocesses;i++)
      {
        processid[i]=safeseq[i];
      }
      for(i=0;i<nofprocesses;i++)
      {
        pthread_create(&processes[i],&attr,process,(void *)(&processid[i]));
      }
      for(i=0;i<nofprocesses;i++)
      {
        pthread_join(processes[i],NULL);
      }
      printf("/n/n/nFinished");
      release();
  }
}


