#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "apuledctld.h"

bool run=false;

int main(int argc,char** argv)
{
    int r;

    if(check_apuled_module()) return 0; //No module loaded
    if(check_apuled_devs()) return 0; //No devices present
    r=load_conf();
    if(r)
    {
	printf("Can't load config: %s\n",strerror(r));
	return 1;
    }
    if(argc>1)
    {
	for(int i=1;i<argc;i++)
	{
	    if(!strcmp(argv[i],"--startup"))
	    {
		r=get_blink_scheme("startup");
		if(r<0)
		{
		    printf("Can't get 'startup' blink scheme\n");
		    clear_conf();
		    return 1;
		}
		__cs=r;
	    }
	    if(!strcmp(argv[i],"--debug"))
	    {
		if(argc<=(i+1))
		{
_idl:
		    printf("Invalid debug level\n");
		    clear_conf();
		    return 1;
		}
		r=atoi(argv[i+1]);
		if(r<=0) goto _idl;
		__log_level_=r;
		i++;
		printf("%s: Debug logging enabled\n",argv[0]);
	    }
	}
    }

    if(daemon(0,1)<0)
    {
	printf("Can't fork to daemon");
	clear_conf();
	return 1;
    }

    if(signal(SIGINT,sigproc)==SIG_ERR)
    {
_serr:
	elog(1,"Can't set signal handler: %s\n",strerror(errno));
	clear_conf();
	return 1;
    }
    if(signal(SIGHUP,sigproc)==SIG_ERR) goto _serr;
    if(signal(SIGTERM,sigproc)==SIG_ERR) goto _serr;
    elog(1,"apuledctld started.\n");
    r=run_leds();
    if(r)
    {
	elog(1,"Can't run leds: %s\n",strerror(errno));
	clear_conf();
	return 1;
    }
    watch_modesw();
    elog(1,"Exiting...\n");
    return 0;
}
