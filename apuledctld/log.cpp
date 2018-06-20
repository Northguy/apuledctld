#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <time.h>

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

void elog(int lev,char* args,...)
{
    FILE* f;
    va_list vlist;
    char str[1024];
    time_t t;
    tm* tt;

    if(lev>=0 && __log_level_<lev) return;
    t=time(NULL);
    tt=localtime(&t);
    if(!tt) return;
    if(lev<0) f=stdout;
    else f=fopen(LOG_FILE,"a");
    if(!f) return;
    va_start(vlist,args);
    sprintf(str,"%02d.%02d.%d [%02d:%02d:%02d] %s",tt->tm_mday,tt->tm_mon+1,
    tt->tm_year+1900,tt->tm_hour,tt->tm_min,tt->tm_sec,args);
    vfprintf(f,str,vlist);
    va_end(vlist);
    if(f!=stdout) fclose(f);
}
