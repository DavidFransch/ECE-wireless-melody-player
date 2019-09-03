#include "audio.h"

int main(void)
{

    // notes frequency chart: http://www.phy.mtu.edu/~suits/notefreqs.html

    buzzer_start();
    buzzer_set_freq(440);
    buzzer_set_volume(5);
    msleep(500);
    buzzer_set_freq(261);
    msleep(2500);
    buzzer_set_freq(261);
    msleep(2500);
    buzzer_set_freq(349);
    msleep(250);
    buzzer_set_freq(392);
    msleep(250);
    buzzer_set_freq(440);
    msleep(250);
    buzzer_set_freq(494);
    msleep(250);
    buzzer_beep(523, 200);
    msleep(250);


    buzzer_stop();
    return 0;
}
