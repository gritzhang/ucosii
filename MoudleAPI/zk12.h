#ifndef __zk12_H__
#define __zk12_H__

#include "AllHand.h"
// ------------------  ASCII��ģ�����ݱ� ------------------------ //
// �����0x20~0x7e                                                //
// �ֿ�: C:\Documents and Settings\lee\����\����LCD����\Asc6X12E.dat ����ȡģ���λ//
// -------------------------------------------------------------- //
unsigned char  ASC_MSK12[] =              // ASCII
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // - -
	0x00,0x00,0x00,0x00,//1

	0x00,0x10,0x10,0x10,0x10,0x10,0x00,0x00,  // -!-
	0x10,0x00,0x00,0x00,//2

	0x00,0x6C,0x48,0x48,0x00,0x00,0x00,0x00,  // -"-
	0x00,0x00,0x00,0x00,//3

	0x00,0x14,0x14,0x28,0x7C,0x28,0x7C,0x28,  // -#-
	0x50,0x50,0x00,0x00,//4

	0x00,0x10,0x38,0x40,0x40,0x38,0x48,0x70,  // -$-
	0x10,0x10,0x00,0x00,//5

	0x00,0x20,0x50,0x20,0x0C,0x70,0x08,0x14,  // -%-
	0x08,0x00,0x00,0x00,//6

	0x00,0x00,0x00,0x18,0x20,0x20,0x54,0x48,  // -&-
	0x34,0x00,0x00,0x00,//7

	0x00,0x10,0x10,0x10,0x10,0x00,0x00,0x00,  // -'-
	0x00,0x00,0x00,0x00,//8

	0x00,0x08,0x08,0x10,0x10,0x10,0x10,0x10,  // -(-
	0x10,0x08,0x08,0x00,//9

	0x00,0x20,0x20,0x10,0x10,0x10,0x10,0x10,  // -)-
	0x10,0x20,0x20,0x00,//10

	0x00,0x10,0x7C,0x10,0x28,0x28,0x00,0x00,  // -*-
	0x00,0x00,0x00,0x00,//11

	0x00,0x00,0x10,0x10,0x10,0xFC,0x10,0x10,  // -+-
	0x10,0x00,0x00,0x00,//12

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,  // -,-
	0x10,0x30,0x20,0x00,//13

	0x00,0x00,0x00,0x00,0x00,0x7C,0x00,0x00,  // ---
	0x00,0x00,0x00,0x00,//14

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,  // -.-
	0x30,0x00,0x00,0x00,//15

	0x00,0x04,0x04,0x08,0x08,0x10,0x10,0x20,  // -/-
	0x20,0x40,0x00,0x00,//16

	0x00,0x38,0x44,0x44,0x44,0x44,0x44,0x44,  // -0-
	0x38,0x00,0x00,0x00,//17

	0x00,0x30,0x10,0x10,0x10,0x10,0x10,0x10,  // -1-
	0x7C,0x00,0x00,0x00,//18

	0x00,0x38,0x44,0x04,0x08,0x10,0x20,0x44,  // -2-
	0x7C,0x00,0x00,0x00,//19

	0x00,0x38,0x44,0x04,0x18,0x04,0x04,0x44,  // -3-
	0x38,0x00,0x00,0x00,//20

	0x00,0x0C,0x14,0x14,0x24,0x44,0x7C,0x04,  // -4-
	0x0C,0x00,0x00,0x00,//21

	0x00,0x3C,0x20,0x20,0x38,0x04,0x04,0x44,  // -5-
	0x38,0x00,0x00,0x00,//22

	0x00,0x1C,0x20,0x40,0x78,0x44,0x44,0x44,  // -6-
	0x38,0x00,0x00,0x00,//23

	0x00,0x7C,0x44,0x04,0x08,0x08,0x08,0x10,  // -7-
	0x10,0x00,0x00,0x00,//24

	0x00,0x38,0x44,0x44,0x38,0x44,0x44,0x44,  // -8-
	0x38,0x00,0x00,0x00,//25

	0x00,0x38,0x44,0x44,0x44,0x3C,0x04,0x08,  // -9-
	0x70,0x00,0x00,0x00,//26

	0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x30,  // -:-
	0x30,0x00,0x00,0x00,//27

	0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x18,  // -;-
	0x30,0x20,0x00,0x00,//28

	0x00,0x00,0x0C,0x10,0x60,0x80,0x60,0x10,  // -<-
	0x0C,0x00,0x00,0x00,//29

	0x00,0x00,0x00,0x00,0x7C,0x00,0x7C,0x00,  // -=-
	0x00,0x00,0x00,0x00,//30

	0x00,0x00,0xC0,0x20,0x18,0x04,0x18,0x20,  // ->-
	0xC0,0x00,0x00,0x00,//31

	0x00,0x00,0x18,0x24,0x04,0x08,0x10,0x00,  // -?-
	0x30,0x00,0x00,0x00,//32

	0x38,0x44,0x44,0x4C,0x54,0x54,0x4C,0x40,  // -@-
	0x44,0x38,0x00,0x00,//33

	0x00,0x30,0x10,0x28,0x28,0x28,0x7C,0x44,  // -A-
	0xEC,0x00,0x00,0x00,

	0x00,0xF8,0x44,0x44,0x78,0x44,0x44,0x44,  // -B-
	0xF8,0x00,0x00,0x00,

	0x00,0x3C,0x44,0x40,0x40,0x40,0x40,0x44,  // -C-
	0x38,0x00,0x00,0x00,

	0x00,0xF0,0x48,0x44,0x44,0x44,0x44,0x48,  // -D-
	0xF0,0x00,0x00,0x00,

	0x00,0xFC,0x44,0x50,0x70,0x50,0x40,0x44,  // -E-
	0xFC,0x00,0x00,0x00,

	0x00,0x7C,0x20,0x28,0x38,0x28,0x20,0x20,  // -F-
	0x70,0x00,0x00,0x00,

	0x00,0x3C,0x44,0x40,0x40,0x4C,0x44,0x44,  // -G-
	0x38,0x00,0x00,0x00,

	0x00,0xEC,0x44,0x44,0x7C,0x44,0x44,0x44,  // -H-
	0xEC,0x00,0x00,0x00,

	0x00,0x7C,0x10,0x10,0x10,0x10,0x10,0x10,  // -I-
	0x7C,0x00,0x00,0x00,

	0x00,0x3C,0x08,0x08,0x08,0x48,0x48,0x48,  // -J-
	0x30,0x00,0x00,0x00,

	0x00,0xEC,0x44,0x48,0x50,0x70,0x48,0x44,  // -K-
	0xE4,0x00,0x00,0x00,

	0x00,0x70,0x20,0x20,0x20,0x20,0x24,0x24,  // -L-
	0x7C,0x00,0x00,0x00,

	0x00,0xEC,0x6C,0x6C,0x54,0x54,0x44,0x44,  // -M-
	0xEC,0x00,0x00,0x00,

	0x00,0xEC,0x64,0x64,0x54,0x54,0x54,0x4C,  // -N-
	0xEC,0x00,0x00,0x00,

	0x00,0x38,0x44,0x44,0x44,0x44,0x44,0x44,  // -O-
	0x38,0x00,0x00,0x00,

	0x00,0x78,0x24,0x24,0x24,0x38,0x20,0x20,  // -P-
	0x70,0x00,0x00,0x00,

	0x00,0x38,0x44,0x44,0x44,0x44,0x44,0x44,  // -Q-
	0x38,0x1C,0x00,0x00,

	0x00,0xF8,0x44,0x44,0x44,0x78,0x48,0x44,  // -R-
	0xE0,0x00,0x00,0x00,

	0x00,0x34,0x4C,0x40,0x38,0x04,0x04,0x64,  // -S-
	0x58,0x00,0x00,0x00,

	0x00,0xFC,0x90,0x10,0x10,0x10,0x10,0x10,  // -T-
	0x38,0x00,0x00,0x00,

	0x00,0xEC,0x44,0x44,0x44,0x44,0x44,0x44,  // -U-
	0x38,0x00,0x00,0x00,

	0x00,0xEC,0x44,0x44,0x28,0x28,0x28,0x10,  // -V-
	0x10,0x00,0x00,0x00,

	0x00,0xEC,0x44,0x44,0x54,0x54,0x54,0x54,  // -W-
	0x28,0x00,0x00,0x00,

	0x00,0xC4,0x44,0x28,0x10,0x10,0x28,0x44,  // -X-
	0xC4,0x00,0x00,0x00,

	0x00,0xEC,0x44,0x28,0x28,0x10,0x10,0x10,  // -Y-
	0x38,0x00,0x00,0x00,

	0x00,0x7C,0x44,0x08,0x10,0x10,0x20,0x44,  // -Z-
	0x7C,0x00,0x00,0x00,

	0x00,0x38,0x20,0x20,0x20,0x20,0x20,0x20,  // -[-
	0x20,0x20,0x38,0x00,

	0x00,0x40,0x20,0x20,0x20,0x10,0x10,0x08,  // -\-
	0x08,0x08,0x00,0x00,

	0x00,0x38,0x08,0x08,0x08,0x08,0x08,0x08,  // -]-
	0x08,0x08,0x38,0x00,

	0x00,0x10,0x10,0x28,0x44,0x00,0x00,0x00,  // -^-
	0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // -_-
	0x00,0x00,0x00,0xFC,

	0x00,0x10,0x08,0x00,0x00,0x00,0x00,0x00,  // -`-
	0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x38,0x44,0x3C,0x44,0x44,  // -a-
	0x3C,0x00,0x00,0x00,

	0x00,0xC0,0x40,0x58,0x64,0x44,0x44,0x44,  // -b-
	0xF8,0x00,0x00,0x00,

	0x00,0x00,0x00,0x3C,0x44,0x40,0x40,0x44,  // -c-
	0x38,0x00,0x00,0x00,

	0x00,0x0C,0x04,0x34,0x4C,0x44,0x44,0x44,  // -d-
	0x3C,0x00,0x00,0x00,

	0x00,0x00,0x00,0x38,0x44,0x7C,0x40,0x40,  // -e-
	0x3C,0x00,0x00,0x00,

	0x00,0x1C,0x20,0x7C,0x20,0x20,0x20,0x20,  // -f-
	0x7C,0x00,0x00,0x00,

	0x00,0x00,0x00,0x34,0x4C,0x44,0x44,0x44,  // -g-
	0x3C,0x04,0x38,0x00,

	0x00,0xC0,0x40,0x58,0x64,0x44,0x44,0x44,  // -h-
	0xEC,0x00,0x00,0x00,

	0x00,0x10,0x00,0x70,0x10,0x10,0x10,0x10,  // -i-
	0x7C,0x00,0x00,0x00,

	0x00,0x10,0x00,0x78,0x08,0x08,0x08,0x08,  // -j-
	0x08,0x08,0x70,0x00,

	0x00,0xC0,0x40,0x5C,0x48,0x70,0x50,0x48,  // -k-
	0xDC,0x00,0x00,0x00,

	0x00,0x30,0x10,0x10,0x10,0x10,0x10,0x10,  // -l-
	0x7C,0x00,0x00,0x00,

	0x00,0x00,0x00,0xE8,0x54,0x54,0x54,0x54,  // -m-
	0x54,0x00,0x00,0x00,

	0x00,0x00,0x00,0xD8,0x64,0x44,0x44,0x44,  // -n-
	0xEC,0x00,0x00,0x00,

	0x00,0x00,0x00,0x38,0x44,0x44,0x44,0x44,  // -o-
	0x38,0x00,0x00,0x00,

	0x00,0x00,0x00,0xD8,0x64,0x44,0x44,0x44,  // -p-
	0x78,0x40,0xE0,0x00,

	0x00,0x00,0x00,0x34,0x4C,0x44,0x44,0x44,  // -q-
	0x3C,0x04,0x0C,0x00,

	0x00,0x00,0x00,0x6C,0x30,0x20,0x20,0x20,  // -r-
	0x7C,0x00,0x00,0x00,

	0x00,0x00,0x00,0x3C,0x44,0x38,0x04,0x44,  // -s-
	0x78,0x00,0x00,0x00,

	0x00,0x00,0x20,0x7C,0x20,0x20,0x20,0x20,  // -t-
	0x1C,0x00,0x00,0x00,

	0x00,0x00,0x00,0xCC,0x44,0x44,0x44,0x4C,  // -u-
	0x34,0x00,0x00,0x00,

	0x00,0x00,0x00,0xEC,0x44,0x44,0x28,0x28,  // -v-
	0x10,0x00,0x00,0x00,

	0x00,0x00,0x00,0xEC,0x44,0x54,0x54,0x54,  // -w-
	0x28,0x00,0x00,0x00,

	0x00,0x00,0x00,0xCC,0x48,0x30,0x30,0x48,  // -x-
	0xCC,0x00,0x00,0x00,

	0x00,0x00,0x00,0xEC,0x44,0x24,0x28,0x18,  // -y-
	0x10,0x10,0x78,0x00,

	0x00,0x00,0x00,0x7C,0x48,0x10,0x20,0x44,  // -z-
	0x7C,0x00,0x00,0x00,

	0x00,0x08,0x10,0x10,0x10,0x10,0x20,0x10,  // -{-
	0x10,0x10,0x08,0x00,

	0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,  // -|-
	0x10,0x10,0x00,0x00,

	0x00,0x20,0x10,0x10,0x10,0x10,0x08,0x10,  // -}-
	0x10,0x10,0x20,0x00,

	0x00,0x00,0x00,0x00,0x00,0x24,0x58,0x00,  // -~-
	0x00,0x00,0x00,0x00,
};

// ------------------  ASCII��ģ�����ݱ� ------------------------ //
// �����0x20~0x7e                                                //
// �ֿ�: F:\����������Ŀ����\Software\Zimo\lcd����ȡģ����1212\Asc8X16E.dat ����ȡģ���λ//
// -------------------------------------------------------------- //
unsigned char ASCII8x16[] =              // ASCII
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // - -
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x18,0x3C,0x3C,0x3C,0x18,0x18,  // -!-
	0x18,0x00,0x18,0x18,0x00,0x00,0x00,0x00,

	0x00,0x66,0x66,0x66,0x24,0x00,0x00,0x00,  // -"-
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x6C,0x6C,0xFE,0x6C,0x6C,  // -#-
	0x6C,0xFE,0x6C,0x6C,0x00,0x00,0x00,0x00,

	0x18,0x18,0x7C,0xC6,0xC2,0xC0,0x7C,0x06,  // -$-
	0x86,0xC6,0x7C,0x18,0x18,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0xC2,0xC6,0x0C,0x18,  // -%-
	0x30,0x60,0xC6,0x86,0x00,0x00,0x00,0x00,

	0x00,0x00,0x38,0x6C,0x6C,0x38,0x76,0xDC,  // -&-
	0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00,

	0x00,0x30,0x30,0x30,0x60,0x00,0x00,0x00,  // -'-
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x0C,0x18,0x30,0x30,0x30,0x30,  // -(-
	0x30,0x30,0x18,0x0C,0x00,0x00,0x00,0x00,

	0x00,0x00,0x30,0x18,0x0C,0x0C,0x0C,0x0C,  // -)-
	0x0C,0x0C,0x18,0x30,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x66,0x3C,0xFF,  // -*-
	0x3C,0x66,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x7E,  // -+-
	0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // -,-
	0x00,0x18,0x18,0x18,0x30,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,  // ---
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // -.-
	0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x02,0x06,0x0C,0x18,  // -/-
	0x30,0x60,0xC0,0x80,0x00,0x00,0x00,0x00,

	0x00,0x00,0x7C,0xC6,0xC6,0xCE,0xD6,0xD6,  // -0-
	0xE6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00,

	0x00,0x00,0x18,0x38,0x78,0x18,0x18,0x18,  // -1-
	0x18,0x18,0x18,0x7E,0x00,0x00,0x00,0x00,

	0x00,0x00,0x7C,0xC6,0x06,0x0C,0x18,0x30,  // -2-
	0x60,0xC0,0xC6,0xFE,0x00,0x00,0x00,0x00,

	0x00,0x00,0x7C,0xC6,0x06,0x06,0x3C,0x06,  // -3-
	0x06,0x06,0xC6,0x7C,0x00,0x00,0x00,0x00,

	0x00,0x00,0x0C,0x1C,0x3C,0x6C,0xCC,0xFE,  // -4-
	0x0C,0x0C,0x0C,0x1E,0x00,0x00,0x00,0x00,

	0x00,0x00,0xFE,0xC0,0xC0,0xC0,0xFC,0x0E,  // -5-
	0x06,0x06,0xC6,0x7C,0x00,0x00,0x00,0x00,

	0x00,0x00,0x38,0x60,0xC0,0xC0,0xFC,0xC6,  // -6-
	0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00,

	0x00,0x00,0xFE,0xC6,0x06,0x06,0x0C,0x18,  // -7-
	0x30,0x30,0x30,0x30,0x00,0x00,0x00,0x00,

	0x00,0x00,0x7C,0xC6,0xC6,0xC6,0x7C,0xC6,  // -8-
	0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00,

	0x00,0x00,0x7C,0xC6,0xC6,0xC6,0x7E,0x06,  // -9-
	0x06,0x06,0x0C,0x78,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,  // -:-
	0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,  // -;-
	0x00,0x18,0x18,0x30,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x06,0x0C,0x18,0x30,0x60,  // -<-
	0x30,0x18,0x0C,0x06,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,  // -=-
	0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x60,0x30,0x18,0x0C,0x06,  // ->-
	0x0C,0x18,0x30,0x60,0x00,0x00,0x00,0x00,

	0x00,0x00,0x7C,0xC6,0xC6,0x0C,0x18,0x18,  // -?-
	0x18,0x00,0x18,0x18,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x7C,0xC6,0xC6,0xDE,0xDE,  // -@-
	0xDE,0xDC,0xC0,0x7C,0x00,0x00,0x00,0x00,

	0x00,0x00,0x10,0x38,0x6C,0xC6,0xC6,0xFE,  // -A-
	0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00,

	0x00,0x00,0xFC,0x66,0x66,0x66,0x7C,0x66,  // -B-
	0x66,0x66,0x66,0xFC,0x00,0x00,0x00,0x00,

	0x00,0x00,0x3C,0x66,0xC2,0xC0,0xC0,0xC0,  // -C-
	0xC0,0xC2,0x66,0x3C,0x00,0x00,0x00,0x00,

	0x00,0x00,0xF8,0x6C,0x66,0x66,0x66,0x66,  // -D-
	0x66,0x66,0x6C,0xF8,0x00,0x00,0x00,0x00,

	0x00,0x00,0xFE,0x66,0x62,0x68,0x78,0x68,  // -E-
	0x60,0x62,0x66,0xFE,0x00,0x00,0x00,0x00,

	0x00,0x00,0xFE,0x66,0x62,0x68,0x78,0x68,  // -F-
	0x60,0x60,0x60,0xF0,0x00,0x00,0x00,0x00,

	0x00,0x00,0x3C,0x66,0xC2,0xC0,0xC0,0xDE,  // -G-
	0xC6,0xC6,0x66,0x3A,0x00,0x00,0x00,0x00,

	0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xFE,0xC6,  // -H-
	0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00,

	0x00,0x00,0x3C,0x18,0x18,0x18,0x18,0x18,  // -I-
	0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00,

	0x00,0x00,0x1E,0x0C,0x0C,0x0C,0x0C,0x0C,  // -J-
	0xCC,0xCC,0xCC,0x78,0x00,0x00,0x00,0x00,

	0x00,0x00,0xE6,0x66,0x6C,0x6C,0x78,0x78,  // -K-
	0x6C,0x66,0x66,0xE6,0x00,0x00,0x00,0x00,

	0x00,0x00,0xF0,0x60,0x60,0x60,0x60,0x60,  // -L-
	0x60,0x62,0x66,0xFE,0x00,0x00,0x00,0x00,

	0x00,0x00,0xC6,0xEE,0xFE,0xFE,0xD6,0xC6,  // -M-
	0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00,

	0x00,0x00,0xC6,0xE6,0xF6,0xFE,0xDE,0xCE,  // -N-
	0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00,

	0x00,0x00,0x38,0x6C,0xC6,0xC6,0xC6,0xC6,  // -O-
	0xC6,0xC6,0x6C,0x38,0x00,0x00,0x00,0x00,

	0x00,0x00,0xFC,0x66,0x66,0x66,0x7C,0x60,  // -P-
	0x60,0x60,0x60,0xF0,0x00,0x00,0x00,0x00,

	0x00,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,  // -Q-
	0xC6,0xD6,0xDE,0x7C,0x0C,0x0E,0x00,0x00,

	0x00,0x00,0xFC,0x66,0x66,0x66,0x7C,0x6C,  // -R-
	0x66,0x66,0x66,0xE6,0x00,0x00,0x00,0x00,

	0x00,0x00,0x7C,0xC6,0xC6,0x60,0x38,0x0C,  // -S-
	0x06,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00,

	0x00,0x00,0x7E,0x7E,0x5A,0x18,0x18,0x18,  // -T-
	0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00,

	0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,  // -U-
	0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00,

	0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,  // -V-
	0xC6,0x6C,0x38,0x10,0x00,0x00,0x00,0x00,

	0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xC6,0xD6,  // -W-
	0xD6,0xFE,0x6C,0x6C,0x00,0x00,0x00,0x00,

	0x00,0x00,0xC6,0xC6,0x6C,0x6C,0x38,0x38,  // -X-
	0x6C,0x6C,0xC6,0xC6,0x00,0x00,0x00,0x00,

	0x00,0x00,0x66,0x66,0x66,0x66,0x3C,0x18,  // -Y-
	0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00,

	0x00,0x00,0xFE,0xC6,0x86,0x0C,0x18,0x30,  // -Z-
	0x60,0xC2,0xC6,0xFE,0x00,0x00,0x00,0x00,

	0x00,0x00,0x3C,0x30,0x30,0x30,0x30,0x30,  // -[-
	0x30,0x30,0x30,0x3C,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x80,0xC0,0xE0,0x70,0x38,  // -\-
	0x1C,0x0E,0x06,0x02,0x00,0x00,0x00,0x00,

	0x00,0x00,0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,  // -]-
	0x0C,0x0C,0x0C,0x3C,0x00,0x00,0x00,0x00,

	0x10,0x38,0x6C,0xC6,0x00,0x00,0x00,0x00,  // -^-
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // -_-
	0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,

/*	0x30,0x30,0x18,0x00,0x00,0x00,0x00,0x00,  // -`-
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
*/	
	0x00,0x10,0x38,0x54,0x92,0x10,0x10,0x10,  // -�Զ��� -
	0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x78,0x0C,0x7C,  // -a-
	0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00,

	0x00,0x00,0xE0,0x60,0x60,0x78,0x6C,0x66,  // -b-
	0x66,0x66,0x66,0xDC,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x7C,0xC6,0xC0,  // -c-
	0xC0,0xC0,0xC6,0x7C,0x00,0x00,0x00,0x00,

	0x00,0x00,0x1C,0x0C,0x0C,0x3C,0x6C,0xCC,  // -d-
	0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x7C,0xC6,0xFE,  // -e-
	0xC0,0xC0,0xC6,0x7C,0x00,0x00,0x00,0x00,

	0x00,0x00,0x38,0x6C,0x64,0x60,0xF0,0x60,  // -f-
	0x60,0x60,0x60,0xF0,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x76,0xCC,0xCC,  // -g-
	0xCC,0xCC,0xCC,0x7C,0x0C,0xCC,0x78,0x00,

	0x00,0x00,0xE0,0x60,0x60,0x6C,0x76,0x66,  // -h-
	0x66,0x66,0x66,0xE6,0x00,0x00,0x00,0x00,

	0x00,0x00,0x18,0x18,0x00,0x38,0x18,0x18,  // -i-
	0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00,

	0x00,0x00,0x06,0x06,0x00,0x0E,0x06,0x06,  // -j-
	0x06,0x06,0x06,0x06,0x66,0x66,0x3C,0x00,

	0x00,0x00,0xE0,0x60,0x60,0x66,0x6C,0x78,  // -k-
	0x78,0x6C,0x66,0xE6,0x00,0x00,0x00,0x00,

	0x00,0x00,0x38,0x18,0x18,0x18,0x18,0x18,  // -l-
	0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0xEC,0xFE,0xD6,  // -m-
	0xD6,0xD6,0xD6,0xD6,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0xDC,0x66,0x66,  // -n-
	0x66,0x66,0x66,0x66,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x7C,0xC6,0xC6,  // -o-
	0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0xDC,0x66,0x66,  // -p-
	0x66,0x66,0x66,0x7C,0x60,0x60,0xF0,0x00,

	0x00,0x00,0x00,0x00,0x00,0x76,0xCC,0xCC,  // -q-
	0xCC,0xCC,0xCC,0x7C,0x0C,0x0C,0x1E,0x00,

	0x00,0x00,0x00,0x00,0x00,0xDC,0x76,0x62,  // -r-
	0x60,0x60,0x60,0xF0,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x7C,0xC6,0x60,  // -s-
	0x38,0x0C,0xC6,0x7C,0x00,0x00,0x00,0x00,

	0x00,0x00,0x10,0x30,0x30,0xFC,0x30,0x30,  // -t-
	0x30,0x30,0x36,0x1C,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0xCC,0xCC,0xCC,  // -u-
	0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x66,0x66,0x66,  // -v-
	0x66,0x66,0x3C,0x18,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0xC6,0xC6,0xC6,  // -w-
	0xD6,0xD6,0xFE,0x6C,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0xC6,0x6C,0x38,  // -x-
	0x38,0x38,0x6C,0xC6,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0xC6,0xC6,0xC6,  // -y-
	0xC6,0xC6,0xC6,0x7E,0x06,0x0C,0xF8,0x00,

	0x00,0x00,0x00,0x00,0x00,0xFE,0xCC,0x18,  // -z-
	0x30,0x60,0xC6,0xFE,0x00,0x00,0x00,0x00,

	0x00,0x00,0x0E,0x18,0x18,0x18,0x70,0x18,  // -{-
	0x18,0x18,0x18,0x0E,0x00,0x00,0x00,0x00,

	0x00,0x00,0x18,0x18,0x18,0x18,0x00,0x18,  // -|-
	0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00,

	0x00,0x00,0x70,0x18,0x18,0x18,0x0E,0x18,  // -}-
	0x18,0x18,0x18,0x70,0x00,0x00,0x00,0x00,
	
	0x00,0x00,0x76,0xDC,0x00,0x00,0x00,0x00,  // -~-
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

#endif