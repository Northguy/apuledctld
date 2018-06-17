#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/param.h>
#include <sys/linker.h>

#include "apuledctld.h"

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
