#ifndef __APULEDCTLD_H_
#define __APULEDCTLD_H_

#include <vector>
#include <pthread.h>

#define UNUSED(x) (void)(x)

#define SYSCTL_APULED_DESC "dev.apuled.0.%desc"
#define APU_LED1 "/dev/led/led1"
#define APU_LED2 "/dev/led/led2"
#define APU_LED3 "/dev/led/led3"
#define APU_MODESW "/dev/modesw"

#define LED1 0x1
#define LED2 0x2
#define LED3 0x4

#define LED1_ON(bm) ((bm) & LED1)
#define LED2_ON(bm) ((bm) & LED2)
#define LED3_ON(bm) ((bm) & LED3)

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
extern conf cf;
extern blink_scheme* __cs; //Current blink scheme
extern pthread_t __led_tid;

//common
int check_apuled_module();
int check_apuled_devs();
int run_leds();
void* led_thread(void* ptr);

//log
void log(int lev,char* args,...);
void elog(int lev,char* args,...);

//modesw
void watch_modesw();

//conf
int load_conf();
int load_scheme(char* str);
void clear_scheme(blink_scheme* bs);
void clear_conf();

//led
void reset_leds();
void set_led(char* name,char* buf);
void set_leds(uint8_t bm);
void blink_leds(blink_scheme* bs);
blink_scheme* get_blink_scheme(char* name);

#endif /*__APULEDCTLD_H_*/
