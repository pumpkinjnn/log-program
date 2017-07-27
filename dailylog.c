#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <libgen.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "dailylog.h"


typedef struct log_handle{
    char *log_file_path;
    char *log_file_name;
    int file_size;
    int file_num;
    int cur_index;
    FILE *cur_file;
    int print_time;
    int file_index;
    int denied_access;
}dailylog;

static void  print_time(FILE *file);
static char* generating_log_file_path(dailylog *handle, int index);
static void locate_start_file(dailylog *new);
static int  detect_cur_location(FILE *file);
static void find_next_available_file(dailylog *handle);
static void locate_place_to_write(dailylog *handle);
static void print_file_index_stamp(FILE *file, int index);
static int  read_file_index_stamp(FILE *file);
static void check_and_close_file(FILE *file);

//Print the current time in the form of "TIME:MON JAN 1 00:00 1997 "
void print_time(FILE *file){
    time_t current;
    fprintf(file,"TIME:");
    fprintf(file,"%s ",ctime(&current));
    return;
}

//Set the cur_file_path to path/NAME.log.index
char* generating_log_file_path(dailylog *handle, int index){
    int max_length = (strlen(handle->log_file_path)+strlen(handle->log_file_name)+10);

    char*  cur_file_path = malloc(max_length);
    snprintf(cur_file_path,max_length,"%s%s.log.%d",(const char*)handle->log_file_path, (const char*)handle->log_file_name, index);
    return cur_file_path;
}

void locate_start_file(dailylog *new) {
  int i;
  int max_index = 0;
  FILE *temp = NULL;

 for(i = 0 ; i < new->file_num; i++){
    char* cur_file_path = generating_log_file_path(new, i+1);
    temp = fopen(cur_file_path,"r+");

    if(NULL != temp){
      int current_stamp = read_file_index_stamp(temp);
      fclose(temp);
      if(current_stamp > max_index){
        max_index = current_stamp;
        new->cur_index = i;
        new->cur_file = temp;
      }
  }else{
      if( errno == EACCES){
       printf("FILE ACCESS DENIED FOR:%s\n",cur_file_path);
      }
    }
  free(cur_file_path);
  }

  new->file_index = max_index;

  if (NULL == new->cur_file) {
    new->cur_index = -1;
    find_next_available_file(new);
  }else{
    char *start_file_path = generating_log_file_path(new, new->cur_index+1);
    new->cur_file = fopen(start_file_path, "r+");
    free(start_file_path);
 }
}

//Detect the size of the current file, the file must exist
int detect_cur_location(FILE *file){
  fseek(file,0,SEEK_END);
  return ftell(file);
}

//Change the cur_file_path. CUR_FILE to the next file and increase the cur_index by 1
void  find_next_available_file(dailylog *handle) {
  int index = (++handle->cur_index)%handle->file_num+1;
  char *cur_file_path = generating_log_file_path(handle, index);
  handle->cur_file = fopen(cur_file_path,"w+");
  free(cur_file_path);
  if( NULL == handle->cur_file ){
  handle->denied_access++;
  if( handle->denied_access > handle->file_num ) return;
  handle->file_index++;
  find_next_available_file(handle);
  return;
  }
  print_file_index_stamp(handle->cur_file, ++handle->file_index);
  handle->denied_access = 0;
}

//Check if the currrent file size is okay, if not, create/open a new file to write
void locate_place_to_write(dailylog *handle){
//Check if the current file size has alsready reached the limit
  if (detect_cur_location(handle->cur_file) > handle->file_size) {
      check_and_close_file(handle->cur_file);
      find_next_available_file(handle);
  }
}

//Print the current logfile index at the beginning of the file
void print_file_index_stamp(FILE *file, int index) {
    fprintf(file, "%d\n", index);
}

//Read the current logfile index and return it
int read_file_index_stamp(FILE *file){
   int i;
   fscanf(file, "%d", &i);
   return i;
}

//Check if the pointer to file is NULL, if not, close the file and point the pointer to NULL
void check_and_close_file(FILE *file){
  if( NULL != file) fclose(file);
  file = NULL;
}

/*
Parameter:
	dir:the directory in which you want the log file to be
	name:the name of the log file
	num:the number of the log file, subject to user's change
	size:the size of each log file, in kb, subject to user's change，in kb
	time:a bool value decide if the user want the option of printing time
*/
dailylog* new_dailylog(const char *dir,const char *name,int num,int size,int b_need_time){

if ( NULL == dir || NULL == name){
 printf("ERROE AT EITHER LOG FILE DIRECTION OR NAME\n");
 return NULL;
}
//allocate a chunk of memory for the new dailylog
 dailylog *new = malloc(sizeof(dailylog));
 memset(new,0,sizeof(dailylog));

 //initialize the file size
 new->file_size = size*1024;

 //initialize the file number
 new->file_num = num;

 //Make a directory(if not exist)
 DIR *dir_status = opendir(dir);
 closedir(dir_status);
 if(NULL == dir_status){
 printf("INVALID FILE PATH : %s\n", dir);
 free_dailylog(new);
 return NULL;
 }

 //Set the current file index to 0
 new->cur_index = 0;

 //Set the bool time from user, if true, print time for log files
 new->print_time = b_need_time;

 //Set the file index to 0
 new->file_index = 0;

 //initialize log_file_path
 new->log_file_path = strdup(dir);

 //initialize log_file_name
 new->log_file_name = strdup(name);

 locate_start_file(new);

 //Check if the current file is full. if full. open a new one
 //locate_place_to_write(new);
 return new;
}

//Write log information passed by the user
void write_dailylog(dailylog *handle, const char *info) {
  if (  NULL == handle || NULL == handle->cur_file ) return;
  locate_place_to_write(handle);
  if( 1 == handle->print_time )  print_time(handle->cur_file);
  fprintf(handle->cur_file,"%s\n",info);
}

//Free the dailylog structure
void free_dailylog(dailylog *handle){
  if( NULL == handle ) return;
  if( NULL != handle->log_file_path ){
  free(handle->log_file_path);
  }
  if ( NULL != handle->log_file_name) {
      free(handle->log_file_name);
  }
  if ( NULL != handle->cur_file ) {
  fflush(handle->cur_file);
  fsync(fileno(handle->cur_file));
  fclose(handle->cur_file);
  }
  free(handle);
}
