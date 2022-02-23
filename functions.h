
void save_cursor_position(void)	    //This function saves cursor position in terminal's memory
   {
	printf("\033%d",7);
   }

void restore_cursor_position(void) // This function restores the cursors position
   {
	printf("\033%d",8);
   }

void move_cursor_to_xy(int x,int y)           //This function moves cursor to specific row and column i.e moves cursor to (x,y) position
	{
	 	printf("\x1b[H");	      //Move cursor to (0,0) position
		printf("\x1b[%dB",x); 	      //Move 'y' row down
		printf("\x1b[%dC",y); 	     //Move 'x' columns right
	}

void loading_bar_fun(int recent_val, int *t)	 //Loading bar function
{
        const char * red="\x1b[1;31m";
	const char * blue="\x1b[1;34m";
	const char * green="\x1b[1;32m";
	const char * end="\x1b[0m";
	restore_cursor_position();

	if (recent_val<0)
	{
	  for(int i=0;i<recent_val*(-1);i++)
	    {
	      if (*t>=0 && *t<=9)
		{system("setterm --cursor off");
		  printf("%s█%s",green,end);*t=*t+1;
		  fflush(stdout);
		  usleep(3000);
		}
	      else if (*t>=10 && *t<=20) {
		system("setterm --cursor off");
		printf("%s█%s",blue,end);
		*t=*t+1;
		fflush(stdout);
		usleep(3000);
	      }
	      else if (*t>20 && *t<=30){
		system("setterm --cursor off");
		printf("%s█%s",red,end);
		*t=*t+1;fflush(stdout);
		usleep(3000);
	      }

	    }

	}
	else
		for(int i=0;i<recent_val;i++)
	{
		system("setterm --cursor off");
		printf("░");printf("\x1b[2D");
		*t=*t-1;
		fflush(stdout);
		usleep(3000);
	}
	save_cursor_position();

}

void sig_handler()
{
	system("setterm --cursor on");
	system("stty echo && stty -cbreak");
	exit(0);
}

void check_permission_to_read_file()
{
#define NC "\e[0m"
#define RED "\e[0;31m"      //Red color defined in ANSCI escapes codes

 FILE *f_p=fopen("/proc/meminfo","r");
      if(f_p==NULL)
	{
	  fflush(f_p);
	  fprintf(stderr, RED "[ERROR]"
               NC  ": Couldnot open '/proc/meminfo' file! \n"
                   "Check if you have right permission to read file\n");
	  exit(1);
	}

}
