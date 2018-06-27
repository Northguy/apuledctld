#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "apuledctld.h"

void reset_leds()
{
    char* Leds[]=
    {
	APU_LED1,
	APU_LED2,
	APU_LED3
    };
    FILE* f;
    
    
    for(int i=0;i<3;i++)
    {
	f=fopen(Leds[i],"r");
	if(!f) return;
	fprintf(f,"0");
	fclose(f);
    }
}

void set_led(char* name,char* buf)
{
    FILE* f;

    f=fopen(name,"a");
    if(!f) return;
    fprintf(f,"%s",buf);
    fclose(f);
}
