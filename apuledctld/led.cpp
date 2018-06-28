#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/select.h>

#include "apuledctld.h"

int __cs=-1; //Current blink scheme

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
	f=fopen(Leds[i],"a");
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

void blink_leds(blink_scheme* bs)
{
    blink* b;
    timeval tv;

    reset_leds();
    for(unsigned long i=0;;i++)
    {
	if(i>=bs->b.size()) i=0;
	b=bs->b[i];
	set_leds(b->leds);
	if(bs->b.size()==1) break;
	tv.tv_sec=0;
	tv.tv_usec=b->delay*1000;
	select(0,NULL,NULL,NULL,&tv);
    }
}

int get_blink_scheme(char* name)
{
    for(unsigned long i=0;i<cf.bs.size();i++)
    {
	if(!cf.bs[i]) return -1;
	if(!strcmp(cf.bs[i]->name,name)) return i;
    }
    return -1;
}
