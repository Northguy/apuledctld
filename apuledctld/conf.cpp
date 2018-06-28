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
    int r;
    
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
		clear_conf();
		return EINVAL;
	    }
	}
    }
    fclose(f);
    if(!cf.btn_wait || !strlen(cf.action) || cf.bs.size()<2) goto _err;
    if(get_blink_scheme("startup")<0) goto _err;
    r=get_blink_scheme("running");
    if(r<0) goto _err;
    __cs=r;
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
	    delete bs;
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

void clear_scheme(blink_scheme* bs)
{
    for(unsigned long i=0;i<bs->b.size();i++)
    {
     if(!bs->b[i]) break;
     delete bs->b[i];
    }
    bs->b.clear();
}

void clear_conf()
{
    for(unsigned long i=0;i<cf.bs.size();i++)
    {
     if(!cf.bs[i]) break;
     clear_scheme(cf.bs[i]);
    }
    cf.bs.clear();
}
