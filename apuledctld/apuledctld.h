#ifndef __APULEDCTLD_H_
#define __APULEDCTLD_H_

#include <vector>

#define SYSCTL_APULED_DESC "dev.apuled.0.%desc"
#define APU_LED1 "/dev/led/led1"
#define APU_LED2 "/dev/led/led2"
#define APU_LED3 "/dev/led/led3"
#define APU_MODESW "/dev/modesw"

#define BTN_WAIT 5 //In seconds, wait time before run action when button pressed

#define LOG_FILE "/var/log/apuledctld.log"
#define CONF_FILE "/usr/local/etc/apuledctld.conf"

using namespace std;

struct blink
{
    uint8_t leds; //LEDs bitmap
    int delay;
};

struct blink_scheme
{
    char name[256];
    vector <blink*> b;
};

struct conf
{
    int btn_wait;
    char action[1024];
    vector <blink_scheme*> bs;
};

extern bool run;

//common
int check_apuled_module();
int check_apuled_devs();

//log
void log(int lev,char* args,...);
void elog(int lev,char* args,...);

//modesw
void watch_modesw();

//conf
int load_conf();
int load_scheme(char* str);

#endif /*__APULEDCTLD_H_*/
