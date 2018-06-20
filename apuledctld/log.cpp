#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

#include "apuledctld.h"

int __log_level_=0;

void log(int lev,char* args,...)
{
    FILE* f;
    va_list vlist;
    
    if(lev>=0 && __log_level_<lev) return;
    if(lev<0) f=stdout;
    else f=fopen(LOG_FILE,"a");
    if(!f) return;
    va_start(vlist,args);
    vfprintf(f,args,vlist);
    va_end(vlist);
    if(f!=stdout) fclose(f);
}
