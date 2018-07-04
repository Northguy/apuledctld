#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/param.h>
#include <sys/linker.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sysctl.h>

#include "apuledctld.h"

pthread_t __led_tid=NULL;

int check_apuled_module()
{
    int fid; //File id
    kld_file_stat st;
    
    fid=kldfind("apuled");
    if(fid<0) return -1;
    memset(&st,0,sizeof(kld_file_stat));
    st.version=sizeof(kld_file_stat);
    if(kldstat(fid,&st)) return -1;
    return 0; //Module loaded
}

int check_apuled_devs()
{
    struct stat st;
    char* desc;
    size_t dsz=0;

    //Get buffer size first
    sysctlbyname(SYSCTL_APULED_DESC,NULL,&dsz,NULL,0);
    if(dsz<=0) return -1;
    desc=new char[dsz];
    if(sysctlbyname(SYSCTL_APULED_DESC,desc,&dsz,NULL,0))
    {
_serr:
	delete []desc;
	return -1;
    }
    if(strstr(desc,"APU")!=desc) goto _serr; //Not APU<x>
    delete []desc;
    if(stat(APU_LED1,&st)) return -1;
    if(!S_ISCHR(st.st_mode)) return -1;
    if(stat(APU_LED2,&st)) return -1;
    if(!S_ISCHR(st.st_mode)) return -1;
    if(stat(APU_LED3,&st)) return -1;
    if(!S_ISCHR(st.st_mode)) return -1;
    if(stat(APU_MODESW,&st)) return -1;
    if(!S_ISCHR(st.st_mode)) return -1;
    return 0;
}

int run_leds()
{
    pthread_t tid;
    int r;

    if((int)__cs<0 || __cs>(int)(cf.bs.size()-1)) return EINVAL;
    if(cf.bs[__cs]->b.size()==1)
    {
	//Static scheme
	__led_tid=NULL;
	blink_leds(cf.bs[__cs]);
	return 0;
    }
    //Dynamic scheme
    r=pthread_create(&tid,NULL,led_thread,NULL);
    if(r)
    {
	__led_tid=NULL;
	return r;
    }
    __led_tid=tid;
    return 0;
}

void* led_thread(void* ptr)
{
    UNUSED(ptr);
    blink_leds(cf.bs[__cs]);
    return NULL;
}

void sigproc(int sig)
{
    switch(sig)
    {
	case SIGHUP:
	elog(1,"Received SIGHUP, reading scheme name from file...\n");
	switch_scheme();
	break;

	case SIGTERM:
	case SIGINT:
	elog(1,"Received ");
	if(sig == SIGINT) log(1," SIGINT.\n");
	else log(1," SIGTERM.\n");
	run=false;
	break;

	case SIGUSR1:

	break;

	case SIGUSR2:
	break;

	default:
	break;
    }
}

void switch_scheme()
{
    __cs++;
    if(__cs>(int)(cf.bs.size()-1)) __cs=0;
    elog(1,"Switching to scheme '%s'\n",cf.bs[__cs]->name);
    if(__led_tid)
    {
	if(pthread_cancel(__led_tid)) return;
	__led_tid=0;
    }
    run_leds();
}

void change_scheme(char* name)
{
    int r;

    r=get_blink_scheme(name);
    if(r<0)
    {
	elog(1,"Can't change scheme to '%s', not found\n",name);
	return;
    }
    if(__led_tid)
    {
	if(pthread_cancel(__led_tid)) return;
	__led_tid=NULL;
    }
    __cs=r;
    run_leds();
}
