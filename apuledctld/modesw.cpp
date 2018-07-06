#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>

#include "apuledctld.h"

void watch_modesw()
{
    timeval tv;
    int fd;
    unsigned char data;
    int bpt=0; //Button press timer
    int rn;
    int pn;

    fd=open(APU_MODESW,O_RDONLY);
    if(fd<0)
    {
	elog(1,"Can't open %s: %d (%s)\n",APU_MODESW,errno,strerror(errno));
	return;
    }
    rn=get_blink_scheme("running");
    if(rn<0)
    {
_serr:
	elog(1,"Can't get builtin scheme!\n");
	return;
    }
    pn=get_blink_scheme("press");
    if(pn<0) goto _serr;
    run=true;
    for(;;)
    {
	if(!run) break;
	if(__cs!=rn && !bpt) goto _wait; //Skip until startup finish
	if(read(fd,&data,1)!=1)
	{
	    elog(0,"Can't read from %s: %d (%s)\n",APU_MODESW,errno,strerror(errno));
	    break;
	}
	if(data!='0')
	{
	    //Button pressed
	    if(bpt<cf.btn_wait)
	    {
	        bpt++;
	        elog(1,"Button press detected, timer: %d\n",bpt);
	        if(__cs!=pn) change_scheme("press");
	    }
	    else
	    {
		//Run action
		elog(1,"Running action\n");
		change_scheme("running");
		system(cf.action);
		break;
	    }
	}
	else
	{
	    change_scheme("running");
	    bpt=0; //Reset press timer
	}
_wait:
	tv.tv_sec=1;
	tv.tv_usec=0;
	select(0,NULL,NULL,NULL,&tv);
    }
    close(fd);
}
