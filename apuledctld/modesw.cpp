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

    fd=open(APU_MODESW,O_RDONLY);
    if(fd<0)
    {
	elog(1,"Can't open %s: %d (%s)\n",APU_MODESW,errno,strerror(errno));
	return;
    }
    run=true;
    for(;;)
    {
	if(!run) break;
	if(read(fd,&data,1)!=1)
	{
	    elog(0,"Can't read from %s: %d (%s)\n",APU_MODESW,errno,strerror(errno));
	    break;
	}
	if(data)
	{
	    //Button pressed
	    if(bpt<BTN_WAIT)
	    {
	        bpt++;
	        elog(1,"Button press detected, timer: %d\n",bpt);
	    }
	    else
	    {
		//Run action
		elog(1,"Running action\n");
	    }
	}
	else bpt=0; //Reset press timer
	tv.tv_sec=1;
	tv.tv_usec=0;
	select(0,NULL,NULL,NULL,&tv);
    }
    close(fd);
}
