#include<rcx_display.h>

/* the digits
   numbered right to left
   dots are associated with the digit to their left
   only digit 2,3,4 have dots
   digit 5 can only be used as a minus sign
*/

#define LCD_0_TOP   0xef44,0x4
#define LCD_0_MID   0xef44,0x5
#define LCD_0_BOT   0xef44,0x7
#define LCD_0_TOPR  0xef45,0x5
#define LCD_0_BOTR  0xef45,0x7
#define LCD_0_TOPL  0xef47,0x5
#define LCD_0_BOTL  0xef47,0x7

#define LCD_1_TOP   0xef43,0x4
#define LCD_1_MID   0xef43,0x5
#define LCD_1_BOT   0xef43,0x7
#define LCD_1_TOPR  0xef47,0x1
#define LCD_1_BOTR  0xef47,0x3
#define LCD_1_TOPL  0xef48,0x5
#define LCD_1_BOTL  0xef48,0x7

#define LCD_2_TOP   0xef44,0x0
#define LCD_2_MID   0xef44,0x1
#define LCD_2_BOT   0xef44,0x3
#define LCD_2_TOPR  0xef48,0x1
#define LCD_2_BOTR  0xef48,0x3
#define LCD_2_TOPL  0xef49,0x5
#define LCD_2_BOTL  0xef49,0x7
#define LCD_2_DOT   0xef48,0x6

#define LCD_3_TOP   0xef46,0x0
#define LCD_3_MID   0xef46,0x1
#define LCD_3_BOT   0xef46,0x3
#define LCD_3_TOPR  0xef4b,0x1
#define LCD_3_BOTR  0xef4b,0x3
#define LCD_3_TOPL  0xef4b,0x5
#define LCD_3_BOTL  0xef4b,0x7
#define LCD_3_DOT   0xef49,0x6

#define LCD_4_TOP   0xef46,0x4
#define LCD_4_MID   0xef46,0x5
#define LCD_4_BOT   0xef46,0x7
#define LCD_4_TOPR  0xef4a,0x1
#define LCD_4_BOTR  0xef4a,0x3
#define LCD_4_TOPL  0xef4a,0x5
#define LCD_4_BOTL  0xef4a,0x7
#define LCD_4_DOT   0xef4b,0x6


const char ascii_display_codes[] =
{
  0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,   // non-printables
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, // -> underscore
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, // except 0x00.
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,

  0x00,             // 32 ' '
   0x42,            // 33 ! 1
   0x0a,            // 34 "
   0x7b,            // 35 #
   0x6d,            // 36 $ 5 S Z
   0x13,            // 37 % /
   0x7d,            // 38 & 6
   0x08,            // 39 '  alt: `
   0x3c,            // 40 ( C [ {
   0x66,            // 41 ) ] }
   0x5b,            // 42 * H K X
   0x43,            // 43 +  alt: 0x19 (worse)
   0x10,            // 44 , .  alt: 0x40
   0x01,            // 45 -
   0x10,            // 46 . alt: 0x40
   0x13,            // 47 /

  0x7e,             // 48 0 0
   0x42,            // 49 1
   0x37,            // 50 2
   0x67,            // 51 3
    0x4b,           // 52 4
    0x6d,           // 53 5
    0x7d,           // 54 6
    0x46,           // 55 7
    0x7f,           // 56 8
    0x4f,           // 57 9 Q q

  0x21,             // 58 : ; = alt:0x5 (worse)
    0x21,           // 59 ;
    0x31,           // 60 < c alt:0xd (worse)
    0x21,           // 61 =
    0x61,           // 62 >   alt: 0x7 (worse)
    0x17,           // 63 ?
    0x3f,           // 64 @ alt: e

  0x5f,             // 65 A
    0x79,           // 66 b
    0x3c,           // 67 C
    0x73,           // 68 d
    0x3d,           // 69 E
    0x1d,           // 70 F
    0x7c,           // 71 G
    0x5b,           // 72 H
    0x42,           // 73 I 1
    0x62,           // 74 J
    0x5b,           // 75 K
    0x38,           // 76 L
    0x5e,           // 77 M N
    0x5e,           // 78 N
    0x7e,           // 79 O alt: o
    0x1f,           // 80 P
    0x4f,           // 81 Q
   0x11,            // 82 r
   0x6d,            // 83 S
   0x46,            // 84 T alt: t
   0x7a,            // 85 U V W
   0x7a,            // 86 V
   0x7a,            // 87 W
   0x5b,            // 88 X
   0x6b,            // 89 Y
   0x37,            // 90 Z

  0x3c,             // 91 [
   0x49,            // 92 '\\'
   0x66,            // 93 ]
   0x0e,            // 94 ^ ~
   0x20,            // 95 _
   0x02,            // 96 ` alt: '

  0x5f,             // 97 A R
   0x79,            // 98 b
   0x31,            // 99 c
   0x73,            // 100 d
   0x3d,            // 101 E
    0x1d,           // 102 F
    0x7c,           // 103 G
   0x59,            // 104 h
   0x42,            // 105 I 1
   0x62,            // 106 J alt 0x60 (worse)
   0x5b,            // 107 K alt h (worse?)
   0x38,            // 108 L
   0x51,            // 109 m n
   0x51,            // 110 n
    0x71,           // 111 o
   0x1f,            // 112 P
   0x4f,            // 113 q
   0x11,            // 114 r
   0x6d,            // 115 S
   0x39,            // 116 t
   0x70,            // 117 u v w
   0x70,            // 118 v
   0x70,            // 119 w
   0x5b,            // 120 X
   0x6b,            // 121 Y
   0x37,            // 122 Z

  0x3c,             // 123 {
   0x18,            // 124 | (left) alt: 1 (worse)
    0x66,           // 125 }
   0x0e,            // 126 ~
    0x00                // 127 "" 127 empty
};


#define dlcd_store(a)   bit_store(BYTE_OF(a),BIT_OF(a))

//! helper macros
//
#define BYTE_OF(a,b)    a
#define BIT_OF(a,b) b

#define bit_store(byte,bit) __asm__ ( "bst %0,@%1\n" : : "i" (bit),"r" (byte))


//! load a single bit from a mask to the carry flag
/*! carry=mask & (1<<bit)
*/
#define bit_load(mask,bit) __asm__ ( "bld %0,%1l\n" : : "i" (bit),"r" (mask):"cc")


//! display native mode segment mask at display position 0
/*! \param mask the mask to display
 */
void cputc_native_0(char mask)
{
  // gcc is stupid
  // doesn't re-use constant values in registers.
  // re-ordered stores to help him.

  bit_load(mask, 0x2);
  dlcd_store(LCD_0_TOP);
  bit_load(mask, 0x0);
  dlcd_store(LCD_0_MID);
  bit_load(mask, 0x5);
  dlcd_store(LCD_0_BOT);
  bit_load(mask, 0x1);
  dlcd_store(LCD_0_TOPR);
  bit_load(mask, 0x6);
  dlcd_store(LCD_0_BOTR);
  bit_load(mask, 0x3);
  dlcd_store(LCD_0_TOPL);
  bit_load(mask, 0x4);
  dlcd_store(LCD_0_BOTL);
}

//! display native mode segment mask at display position 1
/*! \param mask the mask to display
 */
void cputc_native_1(char mask)
{
  bit_load(mask, 0x2);
  dlcd_store(LCD_1_TOP);
  bit_load(mask, 0x0);
  dlcd_store(LCD_1_MID);
  bit_load(mask, 0x5);
  dlcd_store(LCD_1_BOT);
  bit_load(mask, 0x1);
  dlcd_store(LCD_1_TOPR);
  bit_load(mask, 0x6);
  dlcd_store(LCD_1_BOTR);
  bit_load(mask, 0x3);
  dlcd_store(LCD_1_TOPL);
  bit_load(mask, 0x4);
  dlcd_store(LCD_1_BOTL);
}

//! display native mode segment mask at display position 2
/*! \param mask the mask to display
 */
void cputc_native_2(char mask)
{
  bit_load(mask, 0x2);
  dlcd_store(LCD_2_TOP);
  bit_load(mask, 0x0);
  dlcd_store(LCD_2_MID);
  bit_load(mask, 0x5);
  dlcd_store(LCD_2_BOT);
  bit_load(mask, 0x1);
  dlcd_store(LCD_2_TOPR);
  bit_load(mask, 0x6);
  dlcd_store(LCD_2_BOTR);
  bit_load(mask, 0x3);
  dlcd_store(LCD_2_TOPL);
  bit_load(mask, 0x4);
  dlcd_store(LCD_2_BOTL);
}

//! display native mode segment mask at display position 3
/*! \param mask the mask to display
 */
void cputc_native_3(char mask)
{
  bit_load(mask, 0x2);
  dlcd_store(LCD_3_TOP);
  bit_load(mask, 0x0);
  dlcd_store(LCD_3_MID);
  bit_load(mask, 0x5);
  dlcd_store(LCD_3_BOT);
  bit_load(mask, 0x1);
  dlcd_store(LCD_3_TOPR);
  bit_load(mask, 0x6);
  dlcd_store(LCD_3_BOTR);
  bit_load(mask, 0x3);
  dlcd_store(LCD_3_TOPL);
  bit_load(mask, 0x4);
  dlcd_store(LCD_3_BOTL);
}

//! display native mode segment mask at display position 4
/*! \param mask the mask to display
 */
void cputc_native_4(char mask)
{
  bit_load(mask, 0x2);
  dlcd_store(LCD_4_TOP);
  bit_load(mask, 0x0);
  dlcd_store(LCD_4_MID);
  bit_load(mask, 0x5);
  dlcd_store(LCD_4_BOT);
  bit_load(mask, 0x1);
  dlcd_store(LCD_4_TOPR);
  bit_load(mask, 0x6);
  dlcd_store(LCD_4_BOTR);
  bit_load(mask, 0x3);
  dlcd_store(LCD_4_TOPL);
  bit_load(mask, 0x4);
  dlcd_store(LCD_4_BOTL);
}

//! display native mode segment mask
/*! \param mask the segment mask.
    \param pos  the desired display position. 0-4

    this is a dispatcher for the fixed position routines.
 */
void cputc_native(char mask, int pos)
{
  switch (pos) {
  case 0:
     cputc_native_0(mask);
     break;
  case 1:
     cputc_native_1(mask);
     break;
  case 2:
     cputc_native_2(mask);
     break;
  case 3:
     cputc_native_3(mask);
     break;
  case 4:
     cputc_native_4(mask);
     break;
  }
}

/*
Name:       printHex
Input:   unsigned short number, a 2 bytes number
Output:     -
Calls:  cputc_native
Descr:   Prints the input number in the display on the RCX in hex format. Then
            the complete 2 byte number is fitted on the display(4 digits).

*/
void printHex(unsigned short number)
{
    unsigned short temp, tal;   /* temp is used to tell witch ascii character we print*/
    int i;

    for(i=1;i<5;i++)
    {
        tal = number % 16;      /* get lowest 4 bits in number*/
        number= number /16;
        if(tal>9)
            temp=(tal-10)+65;       /* A-F */
        else
            temp=tal+48;     /* 0-9 */
        cputc_native(ascii_display_codes[temp], i);
    }

}


/* old code
    tal = number % 16;
    number= number /16;
    if(tal>9)
        temp=(tal-10)+65;
    else
        temp=tal+48;
    cputc_native(ascii_display_codes[temp], 1);


    tal = number % 16;
    number= number /16;
    if(tal>9)
        temp=(tal-10)+65;
    else
        temp=tal+48;
    cputc_native(ascii_display_codes[temp], 2);


    tal = number % 16;
    number= number /16;
    if(tal>9)
        temp=(tal-10)+65;
    else
        temp=tal+48;
    cputc_native(ascii_display_codes[temp], 3);


    tal = number % 16;
    number= number /16;
    if(tal>9)
        temp=(tal-10)+65;
    else
        temp=tal+48;
    cputc_native(ascii_display_codes[temp], 4);

}
*/




void outputLCD(short value)
{
    clearDisplay();
    setLcdNumber(LCD_SIGNED, value,LCD_DECIMAL_0);
    refreshDisplay();
}



/*
 * Clear display (void)
 *
 * Clears all bits in @ef43[10] array
 * Display must be refreshed for changes to become visible
 */

void clearDisplay (void)
{
     __rcall0 (0x27ac);
}
