
#include <stdio.h>
#include <stdlib.h>    //For atof() function
#include <unistd.h>    //For sleep() fucntion
#include <signal.h>    //For signal handling like  CTRL+C i.e SIGINT 
#include "functions.h"


int bar_length=30;           //Changing bar length will cause problem as other parameters will have to be changed to
FILE *fp;

float get_integer_from_line(char *filename,int y);
int percentage(float total,float free);

int main()
{
  check_permission_to_read_file();           //Checks if it is possible to read file, if not,  program exits with code 1
  system ("stty -echo && stty cbreak");      //Prevents user from accidiently echoing the keypress while the program is running
  int tracker=0;
  int *p=&tracker;
  signal(SIGINT,sig_handler);       //Just control the kill signals and end the program properly
  signal(SIGQUIT,sig_handler);      //Control the quit signal
  printf("\ec");

  // Just some task for the bar
  move_cursor_to_xy(7,2);printf("|");save_cursor_position();
  for(int i=0;i<bar_length;i++)
    {
      printf("░");
    }
  printf("|");
  
  int backup=0;
  float total,free;
  printf("\x1b[H");

  while(1)
    {
      check_permission_to_read_file();
      int i;     
      for(i=0;i<=15;i++) {

	if (i==0) {
	  total=get_integer_from_line("/proc/meminfo",i);
	  printf("   Total memory:          %.2f GB            \n",get_integer_from_line("/proc/meminfo",i));
	}
	else if (i==1) {
	  free=get_integer_from_line("/proc/meminfo",i);
	  printf("   Free memory:           %.2f GB            \n",free);
	}

	else if (i==2) {
	  printf("   Available memory:      %.2f GB            \n",get_integer_from_line("/proc/meminfo",i));
	}
	  
	else if (i==3) {
	  printf("   Used memory:           %.2f GB            \n",total-free-get_integer_from_line("/proc/meminfo",4)-get_integer_from_line("/proc/meminfo",3));
	}
	  
	else if (i==14) {
	  printf("   Total Swap:            %.2f GB            \n",get_integer_from_line("/proc/meminfo",i));
	}

	else if (i==15) {
	  printf("   Free Swap:             %.2f GB            ",get_integer_from_line("/proc/meminfo",i));
	}

      }
      loading_bar_fun(backup-percentage(total,free),p);
      backup=percentage(total,free);
      printf("\x1b[H");                                //Move cursor to (0,0)
      sleep(1);
    }
  return 0;

}

float get_integer_from_line(char *filename,int y)                 //This function takes a "filename" and "line number" and returns integer int the line
{
        int x=0,i=0,return_val,random;char c,str[25];
        for(random=0;random<25;random++)                        //To clear the "str" array as this function will be called multiple times
        {
                str[random]='\0';
        }
        fp=fopen(filename,"r");
        while(x<=y)
        {
                c=getc(fp);return_val=c;
                if(c=='\n')
                {x++;}
                if (x==y && return_val>=48 && return_val<=57)
                {str[i]=c;i++;}
        }
        fclose(fp);
        x=0,i=0,return_val=0;
        return atof(str)/1048576;
}

int percentage(float total,float free)
{
        int per;
        per=(int)(bar_length*(total-free)/total);
        return per;
}
