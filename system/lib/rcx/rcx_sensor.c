#include <stdlib.h>
#include<rcx_sensor.h>



/* making a pointer to PORT6 */
char volatile *port6_ptr=(char*)0xffbb;




void setSensorActive(short code)
{
    switch(code)
    {
        case SENSOR_1:
            (*port6_ptr)|=0x04;
        break;

        case SENSOR_2:
            (*port6_ptr)|=0x02;
        break;

        case SENSOR_3:
            (*port6_ptr)|=0x01;
        break;
    }
}

void setSensorPassive(short code)
{
    switch(code)
    {
        case SENSOR_1:
            (*port6_ptr)&=~0x04;
        break;

        case SENSOR_2:
            (*port6_ptr)&=~0x02;
        break;

        case SENSOR_3:
            (*port6_ptr)&=~0x01;
        break;
    }
}


/*
    Name:       readSensor
    Input:   short code, which senor to sample/read
                short *raw, where to put the sampled value.
    Output: -
    Calls:  -
    Descr:      samplar angiven sensoringång och returnerar rådata.
                wcet for the conversion is 17 us
*/
void readSensor( short code, short *raw)
{
    volatile short *temp;
    volatile char *status    = (char*)0xffe8;
    volatile char *status2   = (char*)0xffe9;
    volatile char tmpstatus  = 0;

    /* nolla bit 5  i 0xffe8 */
    *status = (*status & 0xdf);

    /* nolla bit 7 i 0xffe8 */
    *status = (*status & 0x7f);

    /* nolla bit 6 i 0xffe8 */
    *status = (*status & 0xbf);

    /* nolla bit 4 i 0xffe8, singel mode*/
    *status = (*status & 0xef);

    /* disable interupt och choose sensor input port */
    *status &= 0xf0;
    switch(code){
    case SENSOR_1: *status|=0x02 ;break;
    case SENSOR_2: *status|=0x01 ;break;
    case SENSOR_3: *status|=0x00 ;break;
    case SENSOR_4: *status|=0x03 ;break;
    }

    /* nolla bit 7 i 0xffe9 */
    *status2 = (*status2 & 0x7f);

    /* sätta bit 5 i 0xffe8, starta sampling */
    *status = (*status & 0xdf) | 0x20;
    //*status= 32;

    tmpstatus = *status;
    while( !(tmpstatus & 0x20) )
        tmpstatus = *status;

    if(raw)
    {
        switch(code){
            case SENSOR_1: temp=(short*)0xffe4; break;
            case SENSOR_2: temp=(short*)0xffe2; break;
            case SENSOR_3: temp=(short*)0xffe0; break;
            case SENSOR_4: temp=(short*)0xffe6; break;
            default : temp = NULL;
        }
        *raw=*temp>>6;
    }

}
