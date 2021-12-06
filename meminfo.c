#include <stdio.h>
#include <stdlib.h>										 //For atof() function
#include <unistd.h>									    //For sleep() fucntion
#include <signal.h>									   //For signal handling like  CTRL+C i.e SIGINT 

//Global variables
int bar_length=30;
//End of Global variables


void save(void)									//This is just simple fuction that saves cursor current position in terminal memory
{
	printf("\033%d",7);
}
void restore(void)							// This function restores the cursors position 		
{
	printf("\033%d",8);
}

void move(int x,int y)					 //This function moves cursor to specific row and column
	{
	 	printf("\x1b[H");		        //Move cursor to (0,0) position
		printf("\x1b[%dB",x); 	       //Move 'y' row down
		printf("\x1b[%dC",y); 	      //Move 'x' columns right
	}

FILE *fp;

float line_int(char *filename,int y)					  //This function takes a "filename" and "line number" as arguments and returns integer from that line
{
	int x=0,i=0,return_val,random;char c,str[25];
	for(random=0;random<25;random++)					//To clear the "str" array as this function will be called multiple times previous value will have to be erased
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

	void loading(int recent_val, int *t)								//Loading bar function
{
	char red[]="\x1b[1;31m";
	char blue[]="\x1b[1;34m";
	char green[]="\x1b[1;32m";
	char end[]="\x1b[0m";
	restore();
	if (recent_val<0)
	{
	for(int i=0;i<recent_val*(-1);i++)
	{
			if (*t>=0 && *t<=9)
			{system("setterm --cursor off");printf("%s█%s",green,end);*t=*t+1;fflush(stdout);usleep(3000);}
			else if (*t>=10 && *t<=20)
			{system("setterm --cursor off");printf("%s█%s",blue,end);*t=*t+1;fflush(stdout);usleep(3000);}
			else if (*t>20 && *t<=30)
			{system("setterm --cursor off");printf("%s█%s",red,end);*t=*t+1;fflush(stdout);usleep(3000);}

	}
	}
	else
		for(int i=0;i<recent_val;i++)
	{
		system("setterm --cursor off");printf("░");printf("\x1b[2D");*t=*t-1;fflush(stdout);usleep(3000);
	}
	save();

}

int percentage(float total,float free)
{
	int per;
	per=(int)(bar_length*(total-free+0.5)/total);        //+1.5 cause when the float eg, 1.5 gets converted to integer it coverts into 
							    // 1 which causes the loading bar to be inaccurate. This might create some bugs so if any eroors seen in loading bar remove +1.5
	return per;
}

void sig_handler()
{
	system("setterm --cursor on");
	system("stty echo && stty -cbreak");
	exit(0);
}
int main()

{
	system("stty -echo");
	system("stty cbreak");

	int tracker=0;
	int *p=&tracker;
	signal(SIGINT,sig_handler);
	printf("\ec");
	move(7,2);printf("|");save();
	for(int i=0;i<bar_length;i++)
	{
		printf("░");
	}
	printf("|");
	int backup=0;float total,free;
	printf("\x1b[H");
	while(1)
	{
	int i;
		for(i=0;i<=15;i++)
	{
	if      (i==0)  {total=line_int("/proc/meminfo",i);printf("   Total memory:          %.2f GB            \n",line_int("/proc/meminfo",i));}
	else if (i==1)  {free=line_int("/proc/meminfo",i);printf("   Free memory:           %.2f GB            \n",free);}
	else if (i==2)  {printf("   Available memory:      %.2f GB            \n",line_int("/proc/meminfo",i));}	
	else if (i==3)  {printf("   Used memory:           %.2f GB            \n",total-free-line_int("/proc/meminfo",4)-line_int("/proc/meminfo",3));}
	else if (i==14) {printf("   Total Swap:            %.2f GB            \n",line_int("/proc/meminfo",i));}
	else if (i==15) {printf("   Free Swap:             %.2f GB            ",line_int("/proc/meminfo",i));}
	}
	loading(backup-percentage(total,free),p);
	backup=percentage(total,free);
	printf("\x1b[H");
	//printf("\ec");
	sleep(1);
	}
	return 0;

}
