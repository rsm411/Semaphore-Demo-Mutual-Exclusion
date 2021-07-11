// for compiling use gcc code5b.c -o code5b -lpthread
// for executing use ./code5b
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
# define BufferSize 10
void *writer();
void *reader();
pthread_t tid_writer[20],tid_reader[20];
int reader_count=0;
int r=0,w=0;
int writer_data=0;
pthread_mutex_t mvar=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t wrt=PTHREAD_MUTEX_INITIALIZER;
int main()
{
  int i=0;
  printf("\nEnter No of writers: ");
  scanf("%d",&w);
  printf("\nEnter No of readers: ");
  scanf("%d",&r);
  for(i=0;i<w;i++)
  {
     pthread_create(&tid_writer[i],NULL,writer,NULL);
  }
  for(i=0;i<r;i++)
  {
     pthread_create(&tid_reader[i],NULL,reader,NULL);
  }
  for(i=0;i<w;i++)
  {
     pthread_join(tid_writer[i],NULL);
  }
  for(i=0;i<r;i++)
  {
     pthread_join(tid_reader[i],NULL);
  }
  return 0;
}
void *writer()
{
  int i;
  pthread_mutex_lock(&wrt);
  writer_data++;
  printf("Writer is writing data: %d\n",writer_data);
  pthread_mutex_unlock(&wrt);
}
void *reader()
{
pthread_mutex_lock(&mvar);
reader_count++;
if(reader_count==1)
{
pthread_mutex_lock(&wrt);
}
pthread_mutex_unlock(&mvar);
printf("Reader is reading  data: %d\n",writer_data);
pthread_mutex_lock(&mvar);
reader_count--;
if(reader_count==0)
{
pthread_mutex_unlock(&wrt);
}
pthread_mutex_unlock(&mvar);
}
