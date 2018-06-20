#ifndef __APULEDCTLD_H_
#define __APULEDCTLD_H_

#define SYSCTL_APULED_DESC "dev.apuled.0.%desc"
#define APU_LED1 "/dev/led/led1"
#define APU_LED2 "/dev/led/led2"
#define APU_LED3 "/dev/led/led3"
#define APU_MODESW "/dev/modesw"

int check_apuled_module();
int check_apuled_devs();

#endif /*__APULEDCTLD_H_*/
