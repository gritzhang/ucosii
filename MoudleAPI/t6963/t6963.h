#ifndef	__T6963_H__
#define	__T6963_H__

#include "AllHand.h"

////LCDΩ”ø⁄≈‰÷√
//#define LCDRD       	(1ul << 18)                                                               
//#define LCDRDON()   	LPC_GPIO1->SET |= LCDRD;	LPC_GPIO1->DIR |= LCDRD                      /*                       */
//#define LCDRDOFF()  	LPC_GPIO1->CLR |= LCDRD;	LPC_GPIO1->DIR |= LCDRD   
//#define LCDWR       	(1ul << 19)                                                              
//#define LCDWRON()   	LPC_GPIO1->SET |= LCDWR;	LPC_GPIO1->DIR |= LCDWR                      /*                       */
//#define LCDWROFF()  	LPC_GPIO1->CLR |= LCDWR;	LPC_GPIO1->DIR |= LCDWR                      /*                       */
//#define LCDCS       	(1ul << 25)                                                               
//#define LCDCSON()   	LPC_GPIO1->SET |= LCDCS;	LPC_GPIO1->DIR |= LCDCS                      /*                       */
//#define LCDCSOFF()  	LPC_GPIO1->CLR |= LCDCS;	LPC_GPIO1->DIR |= LCDCS                      /*                       */
//#define LCDA0       	(1ul << 26)                                                              
//#define LCDA0ON()   	LPC_GPIO1->SET |= LCDA0;	LPC_GPIO1->DIR |= LCDA0                      /*                       */
//#define LCDA0OFF()  	LPC_GPIO1->CLR |= LCDA0;	LPC_GPIO1->DIR |= LCDA0  
//#define LCDRST      	(1ul << 29)                                                               
//#define LCDRSTON()  	//LPC_GPIO1->SET |= LCDRST;	LPC_GPIO1->DIR |= LCDRST                     /*                       */
//#define LCDRSTOFF() 	//LPC_GPIO1->CLR |= LCDRST;	LPC_GPIO1->DIR |= LCDRST  

//#define LCDBUS      	(uint8_t)(LPC_GPIO2->PIN & 0x000000ff)                                  			                     
//#define LCDBUSIN()  	LPC_GPIO2->DIR &= 0xffffff00
//#define LCDBUSOUT() 	LPC_GPIO2->DIR |= 0x000000ff         

void T6963_DisplayEng16(uint8_t y,uint8_t x,char *str);
void T6963_Line(uint8_t y00,uint8_t x0,uint8_t y11,uint8_t x1);
void T6963_DispPic(const unsigned char *pic);
void T6963_DispQR_Code(uint8_t x,uint8_t y,int version,const uint8_t ppixel[][27],uint8_t times);
void T6963_Init(void);
void T6963_DisplayCHN16nA(uint8_t y,uint8_t x,uint8_t n,uint8_t *string);
void T6963_DisplayCHN16n(uint8_t y,uint8_t x,uint8_t n,uint8_t *string);
void dspdatchg(uint8_t *dstr,uint8_t *sbuf,uint8_t len,uint8_t dotpst);
void MainDispClr(void);
void T6963_DisplayEng16A(uint8_t y,uint8_t x,char *str);
void T6963_DisplayCHN24n(uint8_t y,uint8_t x,uint8_t n,uint8_t *string);

#endif 

