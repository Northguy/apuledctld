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

void set_leds(uint8_t bm)
{
    char buf[2];

    buf[1]=0;
    //1
    if(LED1_ON(bm)) buf[0]='1';
    else buf[0]='0';
    set_led(APU_LED1,buf);
    //2
    if(LED2_ON(bm)) buf[0]='1';
    else buf[0]='0';
    set_led(APU_LED2,buf);
    //3
    if(LED3_ON(bm)) buf[0]='1';
    else buf[0]='0';
    set_led(APU_LED3,buf);
}
