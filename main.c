#include <stdio.h>
#include "dailylog.h"

int main(void){
 printf("Start program!\n");
 dailylog* new=new_dailylog("/home/nannan/","test",6,300,0);
 printf("Here OK?\n" );
 char* message="balabalabalabala\n";
 int i = 1;

 while (i<5) {
   write_dailylog(new,message);
   i++;
   printf("Writing message in main1!\n" );
  // printf("The current index is :%d\n",new->CUR_INDEX);
 }

 printf("program finished\n");
 free_dailylog(new);
/*
//Test some directory without permission
 new=new_dailylog("/home/lost+found/","test",6,300,0);

 while (i<10) {
   write_dailylog(new,message);
   i++;
   printf("Writing message in main!2\n" );
  // printf("The current index is :%d\n",new->CUR_INDEX);
 }
 
 free_dailylog(new);
 */
 return 1;
}
