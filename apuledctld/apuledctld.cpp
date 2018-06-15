#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "apuledctld.h"

int main(int argc,char** argv)
{
 if(argc>1)
 {
  //Check argv
  argv+=0;
 }
 if(check_apuled_module()) return 0; //No module loaded
 printf("Module loaded\n");
}
