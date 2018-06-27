#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "apuledctld.h"

struct conf cf;

int load_conf()
{
    FILE* f;
    char str[1024];
    char* p;
    
    cf.btn_wait=0;
    memset(cf.action,0,1024);
    f=fopen(CONF_FILE,"r");
    if(!f) return errno;
    for(;;)
    {
	if(!fgets(str,1024,f)) break;
	if(str[strlen(str)-1]=='\n') str[strlen(str)-1]=0;
	if(!strlen(str)) continue;
	p=strchr(str,'#');
	if(p) p[0]=0;
	if(!strlen(str)) continue;
	if(strstr(str,"blink"))
	{
	    //Blink scheme
	    if(load_scheme(str)) goto _err;
	}
	if(strstr(str,"action"))
	{
	    //Action
	    if(sscanf(str,"action:%d:%[^':']",&cf.btn_wait,cf.action)!=2)
	    {
_err:
		fclose(f);
		return EINVAL;
	    }
	}
    }
    fclose(f);
    if(!cf.btn_wait || !strlen(cf.action) || !cf.bs.size()) return EINVAL;
    return 0;
}

int load_scheme(char* str)
{
    char* p=NULL;
    blink_scheme* bs;
    blink* b;
    int bm;
    
    if(!str) return EINVAL;
    p=strtok(str,":");
    if(!p) return EINVAL;
    if(strcmp(p,"blink")) return EINVAL;
    p=strtok(NULL,":");
    if(!p) return EINVAL;
    bs=new blink_scheme;
    memset(bs->name,0,256);
    strcpy(bs->name,p);
    for(;;)
    {
	p=strtok(NULL,":");
	if(!p) break;
	b=new blink;
	memset(b,0,sizeof(blink));
	bm=atoi(p);
	if(bm<0) 
	{ 
_err:	
	    delete b;
	    return EINVAL; 
	}
	b->leds=(uint8_t)bm;
	//
	p=strtok(NULL,":");
	if(!p) goto _err;
	b->delay=atoi(p);
	if(!b->delay) goto _err;
	bs->b.push_back(b);
    }
    cf.bs.push_back(bs);
    return 0;
}
