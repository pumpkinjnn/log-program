/*
This is the header file for the log program
2017.7.18
*/

#ifndef DAILY_LOG_H
#define DAILY_LOG_H

#ifdef __cplusplus
extern "C" {
#endif


struct log_handle;
typedef struct log_handle dailylog;

//A struct that points to four files that store the log info
/*  dir: A string of the log file directory
    name:name of the log file
    num:number of the log file, at least two
    size:the maximum size of each log file，in kb
    b_need_time:whether to print time in the log file, 1 for printing and 0 for not printing
    */
dailylog* new_dailylog(const char *dir,const char *name,int num,int size,int b_need_time);

//Write the info into the file
void  write_dailylog(dailylog *handle, const char *info);

//Free the dailylog structure;
void free_dailylog(dailylog *handle);

#ifdef __cplusplus
}
#endif

#endif //DAILYLOG_H

