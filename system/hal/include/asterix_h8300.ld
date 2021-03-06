/*
 * 
 *  legOS - the independent LEGO Mindstorms OS
 *  h8300.rcx - linker command file
 *  (c) 1999 by Markus L. Noga <markus@noga.de>    
 * 
 */

/*
 *  Originally legOS linker command file changed to 
 *  suit the Asterix Framework
 *  
 *  Contributors(s):
 *           Andreas Engberg
 *           Anders Pettersson
 *
 *
 *  2000-06-12:
 *          No changes done yet. Future work.
 */

STARTUP(hal/asm/crt0.o)
OUTPUT_FORMAT("coff-h8300")
OUTPUT_ARCH(h8300)
ENTRY("_start_crt0")

MEMORY {
  rom (r)  : o = 0x0000, l = 0x8000

  ram      : o = 0x8000, l = 0x6f30
  lcddata  : o = 0xef30, l = 0x0020   /* display memory */
  ram2     : o = 0xef50, l = 0x00b0
  motor    : o = 0xf000, l = 0x0010   /* memory-mapped motor controller */
  ram3     : o = 0xf010, l = 0x0d70
  romdata  : o = 0xfd80, l = 0x0040   /* port shadows, interrupt vectors */
  ram4     : o = 0xfe00, l = 0x0100
    
  eight    : o = 0xff00, l = 0x0100
}

SECTIONS {

  .rom : {
    /* used rom functions */
    
    _rom_reset_vector = 0x0000;
        
    _reset        = 0x03ae ;
    lcd_show      = 0x1b62 ;
    lcd_hide      = 0x1e4a ;
    lcd_number    = 0x1ff2 ;
    lcd_clear     = 0x27ac ;
    lcd_refresh   = 0x27c8 ;
    power_init    = 0x2964 ;
    sound_system  = 0x299a ;
    power_off     = 0x2a62 ;
    sound_playing = 0x3ccc ;

    _rom_dummy_handler   = 0x046a ;
    _rom_ocia_handler    = 0x04cc ;
    _rom_ocia_return     = 0x04d4 ;
    
  } > rom
    
  .text :	{
    ___text = . ;
    *(.text) 	      /* must start with text for clean entry */			
    *(.rodata)
    *(.strings)
    *(.vectors)       /* vectors region (dummy) */
    ___text_end = . ;
    _etext = . ; 
  } > ram

  .tors : {
    ___ctors = . ;
    *(.ctors)
    ___ctors_end = . ;
    ___dtors = . ;
    *(.dtors)
    ___dtors_end = . ;
  } > ram

  .data : {
    ___data = . ;
    *(.data)
    *(.tiny)
    ___data_end = . ;
    __edata = . ;
  } > ram

  .bss : {
    ___bss     = . ;
    _bss_start = . ; 
    *(.bss)
    *(COMMON)
    ___bss_end = ALIGN(2) ;
    _mm_start  = ALIGN(2) ;    /* start memory management here */
    _end       = ALIGN(2) ;  
  } >ram

  .lcddata : {
    display_memory =  0xef3e - 0xef30 ; 
    
  } > lcddata
    
  .romdata : {
    /* shadow registers */
    
    _rom_port1_ddr = 0x00 ; 
    _rom_port2_ddr = 0x01 ; 
    _rom_port3_ddr = 0x02 ; 
    _rom_port4_ddr = 0x03 ; 
    _rom_port5_ddr = 0x04 ; 
    _rom_port6_ddr = 0x05 ; 
    _rom_port7_pin = 0x06 ; 
    
    /* interrupt vectors */
    
    _reset_vector = 0x10 ; 
    _nmi_vector   = 0x12 ; 
    _irq0_vector  = 0x14 ; 
    _irq1_vector  = 0x16 ; 
    _irq2_vector  = 0x18 ; 
    _icia_vector  = 0x1a ; 
    _icib_vector  = 0x1c ; 
    _icic_vector  = 0x1e ; 
    _icid_vector  = 0x20 ; 
    _ocia_vector  = 0x22 ; 
    _ocib_vector  = 0x24 ; 
    _fovi_vector  = 0x26 ; 
    _cmi0a_vector = 0x28 ; 
    _cmi0b_vector = 0x2a ; 
    _ovi0_vector  = 0x2c ; 
    _cmi1a_vector = 0x2e ; 
    _cmi1b_vector = 0x30 ; 
    _ovi1_vector  = 0x32 ; 
    _eri_vector   = 0x34 ; 
    _rxi_vector   = 0x36 ; 
    _txi_vector   = 0x38 ; 
    _tei_vector   = 0x3a ; 
    _ad_vector    = 0x3c ; 
    _wovf_vector  = 0x3e ; 
    
  } > romdata

  .motor : {
    _motor_controller = 0 ;
  } > motor
  
  .stack : {
    _stack = . ; 
    *(.stack)
  } > ram4

  .eight : {
    *(.eight)

    /* on-chip module registers (relative to 0xff00) */

    _T_IER     = 0x90 ;
    _T_CSR     = 0x91 ;
    _T_CNT     = 0x92 ;
    _T_OCRA    = 0x94 ;
    _T_OCRB    = 0x94 ;
    _T_CR      = 0x96 ;
    _T_OCR     = 0x97 ;
    _T_ICRA    = 0x98 ;
    _T_ICRB    = 0x9a ;
    _T_ICRC    = 0x9c ;
    _T_ICRD    = 0x9e ;
    _PORT1_PCR = 0xac ;
    _PORT2_PCR = 0xad ;
    _PORT3_PCR = 0xae ;
    _PORT1_DDR = 0xb0 ;
    _PORT2_DDR = 0xb1 ;
    _PORT1     = 0xb2 ;
    _PORT2     = 0xb3 ;
    _PORT3_DDR = 0xb4 ;
    _PORT4_DDR = 0xb5 ;
    _PORT3     = 0xb6 ;
    _PORT4     = 0xb7 ;
    _PORT5_DDR = 0xb8 ;
    _PORT6_DDR = 0xb9 ;
    _PORT5     = 0xba ;
    _PORT6     = 0xbb ;
    _PORT7     = 0xbe ;
    _STCR      = 0xc3 ;
    _SYSCR     = 0xc4 ;
    _T0_CR     = 0xc8 ;
    _T0_CSR    = 0xc9 ;
    _T0_CORA   = 0xca ;
    _T0_CORB   = 0xcb ;
    _T0_CNT    = 0xcc ;
    _T1_CR     = 0xd0 ;
    _T1_CSR    = 0xd1 ;
    _T1_CORA   = 0xd2 ;
    _T1_CORB   = 0xd3 ;
    _T1_CNT    = 0xd4 ;
    _S_MR      = 0xd8 ;
    _S_BRR     = 0xd9 ;
    _S_CR      = 0xda ;
    _S_TDR     = 0xdb ;
    _S_SR      = 0xdc ;
    _S_RDR     = 0xdd ;
    _AD_A      = 0xe0 ;
    _AD_A_H    = 0xe0 ;
    _AD_A_L    = 0xe1 ;
    _AD_B      = 0xe2 ;
    _AD_B_H    = 0xe2 ;
    _AD_B_L    = 0xe3 ;
    _AD_C      = 0xe4 ;
    _AD_C_H    = 0xe4 ;
    _AD_C_L    = 0xe5 ;
    _AD_D      = 0xe6 ;
    _AD_D_H    = 0xe6 ;
    _AD_D_L    = 0xe7 ;
    _AD_CSR    = 0xe8 ;
    _AD_CR     = 0xe9 ;
    
    /* end of on-chip module registers */

  } > eight

  .stab 0 (NOLOAD) : {
    [ .stab ]
  }

  .stabstr 0 (NOLOAD) : {
    [ .stabstr ]
  }

} /* SECTIONS */
