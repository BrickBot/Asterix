/*  \file   rcx_lib.c
 *   \brief  A hard ware dependent libary for the Lego Mindstorm RCX.
 *          Developed for the Asterix RTOS project.
 *  \date   3 Maj 2000
 *   \author Anders Davidsson, Joakim Lindgren.
 *
 *
 *   For more information or contact please visit:
 *   http://www.mds.mdh.se/~dtv96adn/asterix/
 */

#ifndef __SENSOR__
#define __SENSOR__

/* max and min value for light sensor */
#include <rcx_undef.h>


#define BLACK 0xdc00
#define WHITE 0x9c00


#define SENSOR_1 0x1000
#define SENSOR_2 0x1001
#define SENSOR_3 0x1002
#define SENSOR_4 0x1003     /* battery voltage */

/*
 * Init sensors (void)
 *
 * Sets port 6 bits 0, 1, and 2 to output.
 */

static inline void
initSensors (void)
{
     __rcall0 (0x1498);
}
/*
 * Set sensor active (short code)
 *
 * code=1000: Set sensor 0 active
 *   Call 3de0 (700a, 4) to turn on power to sensor 0
 * code=1001: Set sensor 1 active
 *   Call 3de0 (700a, 2) to turn on power to sensor 1
 * code=1002: Set sensor 2 active
 *   Call 3de0 (700a, 1) to turn on power to sensor 2
 */

void setSensorActive (short code);

/*
 * Set sensor passive (short code)
 *
 * code=1000: Set sensor 0 passive
 *   Call 3e9e (700a, 4) to turn off power to sensor 0
 * code=1001: Set sensor 1 passive
 *   Call 3e9e (700a, 2) to turn off power to sensor 1
 * code=1002: Set sensor 2 passive
 *   Call 3e9e (700a, 1) to turn off power to sensor 2
 */
void setSensorPassive (short code);


/*
    Name:   readSensor
    Input:   short code, which senor to sample/read
                short *raw, where to put the sampled value.
    Output: -
    Calls:  -
    Descr:      samplar angiven sensoringång och returnerar rådata.
                wcet for the conversion is 17 us
*/
void readSensor(short code, short *raw);

#endif
