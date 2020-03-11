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

#ifndef __DISPLAY__
#define __DISPLAY__

#include <rcx_undef.h>


#define LCD_STANDING         0x3006
#define LCD_WALKING          0x3007
#define LCD_SENSOR_0_VIEW    0x3008
#define LCD_SENSOR_0_ACTIVE  0x3009
#define LCD_SENSOR_1_VIEW    0x300a
#define LCD_SENSOR_1_ACTIVE  0x300b
#define LCD_SENSOR_2_VIEW    0x300c
#define LCD_SENSOR_2_ACTIVE  0x300d
#define LCD_MOTOR_0_VIEW     0x300e
#define LCD_MOTOR_0_REV      0x300f
#define LCD_MOTOR_0_FWD      0x3010
#define LCD_MOTOR_1_VIEW     0x3011
#define LCD_MOTOR_1_REV      0x3012
#define LCD_MOTOR_1_FWD      0x3013
#define LCD_MOTOR_2_VIEW     0x3014
#define LCD_MOTOR_2_REV      0x3015
#define LCD_MOTOR_2_FWD      0x3016
#define LCD_DATALOG          0x3018
#define LCD_DOWNLOAD         0x3019
#define LCD_UPLOAD           0x301a
#define LCD_BATTERY          0x301b
#define LCD_RANGE_SHORT      0x301c
#define LCD_RANGE_LONG       0x301d
#define LCD_ALL              0x3020



void clearDisplay(void);

/*
 * Set lcd segment (short code)
 *
 * code=3006: standing figure
 * code=3007: walking figure
 * code=3008: sensor 0 view selected
 * code=3009: sensor 0 active
 * code=300a: sensor 1 view selected
 * code=300b: sensor 1 active
 * code=300c: sensor 2 view selected
 * code=300d: sensor 2 active
 * code=300e: motor 0 view selected
 * code=300f: motor 0 backward arrow
 * code=3010: motor 0 forward arrow
 * code=3011: motor 1 view selected
 * code=3012: motor 1 backward arrow
 * code=3013: motor 1 forward arrow
 * code=3014: motor 2 view selected
 * code=3015: motor 2 backward arrow
 * code=3016: motor 2 forward arrow
 * code=3018: datalog indicator, multiple calls add 4 quarters clockwise
 * code=3019: download in progress, multiple calls adds up to 5 dots to right
 * code=301a: upload in progress, multiple calls removes up to 5 dots from left
 * code=301b: battery low
 * code=301c: short range indicator
 * code=301d: long range indicator
 * code=3020: all segments
 * All codes set bits in @ef43[10] array to affect display
 * Display must be refreshed for changes to become visible
 */

static inline void
setLcdSegment (short code)
{
     __rcall1 (0x1b62, code);
}

/* #define LCD_STANDING         0x3006 */
/* #define LCD_WALKING          0x3007 */
/* #define LCD_SENSOR_0_VIEW    0x3008 */
/* #define LCD_SENSOR_0_ACTIVE  0x3009 */
/* #define LCD_SENSOR_1_VIEW    0x300a */
/* #define LCD_SENSOR_1_ACTIVE  0x300b */
/* #define LCD_SENSOR_2_VIEW    0x300c */
/* #define LCD_SENSOR_2_ACTIVE  0x300d */
/* #define LCD_MOTOR_0_VIEW     0x300e */
/* #define LCD_MOTOR_0_REV      0x300f */
/* #define LCD_MOTOR_0_FWD      0x3010 */
/* #define LCD_MOTOR_1_VIEW     0x3011 */
/* #define LCD_MOTOR_1_REV      0x3012 */
/* #define LCD_MOTOR_1_FWD      0x3013 */
/* #define LCD_MOTOR_2_VIEW     0x3014 */
/* #define LCD_MOTOR_2_REV      0x3015 */
/* #define LCD_MOTOR_2_FWD      0x3016 */
/* #define LCD_DATALOG          0x3018 */
/* #define LCD_DOWNLOAD         0x3019 */
/* #define LCD_UPLOAD           0x301a */
/* #define LCD_BATTERY          0x301b */
/* #define LCD_RANGE_SHORT      0x301c */
/* #define LCD_RANGE_LONG       0x301d */

/*
 * Clear lcd segment (short code)
 *
 * code=3006: standing figure
 * code=3007: walking figure
 * code=3008: sensor 0 view selected
 * code=3009: sensor 0 active
 * code=300a: sensor 1 view selected
 * code=300b: sensor 1 active
 * code=300c: sensor 2 view selected
 * code=300d: sensor 2 active
 * code=300e: motor 0 view selected
 * code=300f: motor 0 backward arrow
 * code=3010: motor 0 forward arrow
 * code=3011: motor 1 view selected
 * code=3012: motor 1 backward arrow
 * code=3013: motor 1 forward arrow
 * code=3014: motor 2 view selected
 * code=3015: motor 2 backward arrow
 * code=3016: motor 2 forward arrow
 * code=3018: datalog indicator
 * code=3019: upload/download indicators
 * code=301a: upload/download indicators
 * code=301b: battery low
 * code=301c: range indicators
 * code=301d: range indicators
 * All codes clear bits in @ef43[10] array to affect display
 * Display must be refreshed for changes to become visible
 */

static inline void
clearLcdSegment (short code)
{
     __rcall1 (0x1e4a, code);
}

#define LCD_SIGNED           0x3001
#define LCD_DECIMAL_0        0x3002
#define LCD_DECIMAL_1        0x3003
#define LCD_DECIMAL_2        0x3004
#define LCD_DECIMAL_3        0x3005
#define LCD_PROGRAM          0x3017
#define LCD_UNSIGNED         0x301f

/*
 * Set lcd number (short code, short value, short pointcode)
 *
 * code=3001: Set lcd main number signed
 *   Set main number on display to signed value, with no leading zeros
 *   If value > 9999, displayed value is 9999
 *   If value < -9999, displayed value is -9999
 * code=3017: Set lcd program number
 *   Set program number on display to value, use pointcode=0
 *   If value < 0, no value is displayed
 *   If value > 0, no value is displayed
 *   Pointcode is ignored, no real need to set to zero
 * code=301f: Set lcd main number unsigned
 *   Set main number on display to unsigned value, with leading zeros
 *   Value is unsigned, so it is never less than 0
 * For 3001, 301f:
 *   pointcode=3002: no decimal point
 *   pointcode=3003: 000.0 format
 *   pointcode=3004: 00.00 format
 *   pointcode=3005: 0.000 format
 */

static inline void
setLcdNumber (short code, short value, short pointcode)
{
     __rcall3 (0x1ff2, code, value, pointcode);
}


/*
 * Clear display (void)
 *
 * Clears all bits in @ef43[10] array
 * Display must be refreshed for changes to become visible
 */
/*
static inline void
clearDisplay (void)
{
     __rcall0 (0x27ac);
}
moved to rcx_display.c*/



/*
 * Refresh display
 *
 * Set4s @ef3e[5] to { 7c, c8, 80, e0, 80 }
 * Calls 283c (ef3e) to have ef3e array sent to lcd using port 5 bits 5,6
 *
 * This function takes about 1.58 ms to run, in case that's important to you
 */

static inline void
refreshDisplay (void)
{
     __rcall0 (0x27c8);
}



/*
Name:       printHex
Input:   unsigned short number, a 2 bytes number
Output:     -
Calls:  cputc_native
Descr:   Prints the input number in the display on the RCX in hex format. Then
            the complete 2 byte number is fitted on the display(4 digits).
*/
extern void printHex(unsigned short number);


extern void outputLCD(short value);

#endif
