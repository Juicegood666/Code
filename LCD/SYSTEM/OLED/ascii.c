#include "oled.h"

//横向取模 高位在前
const u8 ASCII_8_16[95][16]=
{
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",0*/

{0x00,0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,0x10,0x10,0x00,0x00},/*"!",1*/

{0x00,0x12,0x24,0x24,0x48,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*""",2*/

{0x00,0x00,0x00,0x12,0x12,0x12,0x7E,0x24,0x24,0x24,0x7E,0x24,0x24,0x24,0x00,0x00},/*"#",3*/

{0x00,0x00,0x08,0x3C,0x4A,0x4A,0x48,0x38,0x0C,0x0A,0x0A,0x4A,0x4A,0x3C,0x08,0x08},/*"$",4*/

{0x00,0x00,0x00,0x44,0xA4,0xA8,0xA8,0xB0,0x54,0x1A,0x2A,0x2A,0x4A,0x44,0x00,0x00},/*"%",5*/

{0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x50,0x6E,0xA4,0x94,0x98,0x89,0x76,0x00,0x00},/*"&",6*/

{0x00,0x60,0x20,0x20,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"'",7*/

{0x00,0x02,0x04,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x08,0x08,0x04,0x02,0x00},/*"(",8*/

{0x00,0x40,0x20,0x10,0x10,0x08,0x08,0x08,0x08,0x08,0x08,0x10,0x10,0x20,0x40,0x00},/*")",9*/

{0x00,0x00,0x00,0x00,0x10,0x10,0xD6,0x38,0x38,0xD6,0x10,0x10,0x00,0x00,0x00,0x00},/*"*",10*/

{0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x08,0x7F,0x08,0x08,0x08,0x00,0x00,0x00,0x00},/*"+",11*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x20,0x20,0x40},/*",",12*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"-",13*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x00,0x00},/*".",14*/

{0x00,0x00,0x02,0x04,0x04,0x04,0x08,0x08,0x10,0x10,0x10,0x20,0x20,0x40,0x40,0x00},/*"/",15*/

{0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00},/*"0",16*/

{0x00,0x00,0x00,0x08,0x38,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x3E,0x00,0x00},/*"1",17*/

{0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x02,0x04,0x08,0x10,0x20,0x42,0x7E,0x00,0x00},/*"2",18*/

{0x00,0x00,0x00,0x3C,0x42,0x42,0x02,0x04,0x18,0x04,0x02,0x42,0x42,0x3C,0x00,0x00},/*"3",19*/

{0x00,0x00,0x00,0x04,0x0C,0x0C,0x14,0x24,0x24,0x44,0x7F,0x04,0x04,0x1F,0x00,0x00},/*"4",20*/

{0x00,0x00,0x00,0x7E,0x40,0x40,0x40,0x78,0x44,0x02,0x02,0x42,0x44,0x38,0x00,0x00},/*"5",21*/

{0x00,0x00,0x00,0x18,0x24,0x40,0x40,0x5C,0x62,0x42,0x42,0x42,0x22,0x1C,0x00,0x00},/*"6",22*/

{0x00,0x00,0x00,0x7E,0x42,0x04,0x04,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x00,0x00},/*"7",23*/

{0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x24,0x18,0x24,0x42,0x42,0x42,0x3C,0x00,0x00},/*"8",24*/

{0x00,0x00,0x00,0x38,0x44,0x42,0x42,0x42,0x46,0x3A,0x02,0x02,0x24,0x18,0x00,0x00},/*"9",25*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00},/*":",26*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x10},/*";",27*/

{0x00,0x00,0x00,0x02,0x04,0x08,0x10,0x20,0x40,0x20,0x10,0x08,0x04,0x02,0x00,0x00},/*"<",28*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,0x00},/*"=",29*/

{0x00,0x00,0x00,0x40,0x20,0x10,0x08,0x04,0x02,0x04,0x08,0x10,0x20,0x40,0x00,0x00},/*">",30*/

{0x00,0x00,0x00,0x3C,0x42,0x42,0x62,0x04,0x08,0x08,0x08,0x00,0x18,0x18,0x00,0x00},/*"?",31*/

{0x00,0x00,0x00,0x38,0x44,0x5A,0xAA,0xAA,0xAA,0xAA,0xAA,0x5C,0x42,0x3C,0x00,0x00},/*"@",32*/

{0x00,0x00,0x00,0x10,0x10,0x18,0x28,0x28,0x24,0x3C,0x44,0x42,0x42,0xE7,0x00,0x00},/*"A",33*/

{0x00,0x00,0x00,0xF8,0x44,0x44,0x44,0x78,0x44,0x42,0x42,0x42,0x44,0xF8,0x00,0x00},/*"B",34*/

{0x00,0x00,0x00,0x3E,0x42,0x42,0x80,0x80,0x80,0x80,0x80,0x42,0x44,0x38,0x00,0x00},/*"C",35*/

{0x00,0x00,0x00,0xF8,0x44,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x44,0xF8,0x00,0x00},/*"D",36*/

{0x00,0x00,0x00,0xFC,0x42,0x48,0x48,0x78,0x48,0x48,0x40,0x42,0x42,0xFC,0x00,0x00},/*"E",37*/

{0x00,0x00,0x00,0xFC,0x42,0x48,0x48,0x78,0x48,0x48,0x40,0x40,0x40,0xE0,0x00,0x00},/*"F",38*/

{0x00,0x00,0x00,0x3C,0x44,0x44,0x80,0x80,0x80,0x8E,0x84,0x44,0x44,0x38,0x00,0x00},/*"G",39*/

{0x00,0x00,0x00,0xE7,0x42,0x42,0x42,0x42,0x7E,0x42,0x42,0x42,0x42,0xE7,0x00,0x00},/*"H",40*/

{0x00,0x00,0x00,0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00},/*"I",41*/

{0x00,0x00,0x00,0x3E,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x88,0xF0},/*"J",42*/

{0x00,0x00,0x00,0xEE,0x44,0x48,0x50,0x70,0x50,0x48,0x48,0x44,0x44,0xEE,0x00,0x00},/*"K",43*/

{0x00,0x00,0x00,0xE0,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x42,0xFE,0x00,0x00},/*"L",44*/

{0x00,0x00,0x00,0xEE,0x6C,0x6C,0x6C,0x6C,0x6C,0x54,0x54,0x54,0x54,0xD6,0x00,0x00},/*"M",45*/

{0x00,0x00,0x00,0xC7,0x62,0x62,0x52,0x52,0x4A,0x4A,0x4A,0x46,0x46,0xE2,0x00,0x00},/*"N",46*/

{0x00,0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x44,0x38,0x00,0x00},/*"O",47*/

{0x00,0x00,0x00,0xFC,0x42,0x42,0x42,0x42,0x7C,0x40,0x40,0x40,0x40,0xE0,0x00,0x00},/*"P",48*/

{0x00,0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0x82,0xB2,0x4C,0x38,0x06,0x00},/*"Q",49*/

{0x00,0x00,0x00,0xFC,0x42,0x42,0x42,0x7C,0x48,0x48,0x44,0x44,0x42,0xE3,0x00,0x00},/*"R",50*/

{0x00,0x00,0x00,0x3E,0x42,0x42,0x40,0x20,0x18,0x04,0x02,0x42,0x42,0x7C,0x00,0x00},/*"S",51*/

{0x00,0x00,0x00,0xFE,0x92,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x38,0x00,0x00},/*"T",52*/

{0x00,0x00,0x00,0xE7,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00},/*"U",53*/

{0x00,0x00,0x00,0xE7,0x42,0x42,0x44,0x24,0x24,0x28,0x28,0x18,0x10,0x10,0x00,0x00},/*"V",54*/

{0x00,0x00,0x00,0xD6,0x54,0x54,0x54,0x54,0x54,0x6C,0x28,0x28,0x28,0x28,0x00,0x00},/*"W",55*/

{0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x18,0x18,0x18,0x24,0x24,0x42,0xE7,0x00,0x00},/*"X",56*/

{0x00,0x00,0x00,0xEE,0x44,0x44,0x28,0x28,0x10,0x10,0x10,0x10,0x10,0x38,0x00,0x00},/*"Y",57*/

{0x00,0x00,0x00,0x7E,0x84,0x04,0x08,0x08,0x10,0x20,0x20,0x42,0x42,0xFC,0x00,0x00},/*"Z",58*/

{0x00,0x1E,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x1E,0x00},/*"[",59*/

{0x00,0x00,0x40,0x20,0x20,0x20,0x10,0x10,0x10,0x08,0x08,0x04,0x04,0x04,0x02,0x02},/*"\",60*/

{0x00,0x78,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x78,0x00},/*"]",61*/

{0x00,0x18,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"^",62*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF},/*"_",63*/

{0x00,0x60,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"`",64*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x44,0x0C,0x34,0x44,0x4C,0x36,0x00,0x00},/*"a",65*/

{0x00,0x00,0x00,0x00,0xC0,0x40,0x40,0x58,0x64,0x42,0x42,0x42,0x64,0x58,0x00,0x00},/*"b",66*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x22,0x40,0x40,0x40,0x22,0x1C,0x00,0x00},/*"c",67*/

{0x00,0x00,0x00,0x00,0x06,0x02,0x02,0x3E,0x42,0x42,0x42,0x42,0x46,0x3B,0x00,0x00},/*"d",68*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x42,0x7E,0x40,0x42,0x3C,0x00,0x00},/*"e",69*/

{0x00,0x00,0x00,0x00,0x0C,0x12,0x10,0x7C,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00},/*"f",70*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x44,0x44,0x38,0x40,0x3C,0x42,0x42,0x3C},/*"g",71*/

{0x00,0x00,0x00,0x00,0xC0,0x40,0x40,0x5C,0x62,0x42,0x42,0x42,0x42,0xE7,0x00,0x00},/*"h",72*/

{0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x70,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00},/*"i",73*/

{0x00,0x00,0x00,0x0C,0x0C,0x00,0x00,0x1C,0x04,0x04,0x04,0x04,0x04,0x04,0x44,0x78},/*"j",74*/

{0x00,0x00,0x00,0x00,0xC0,0x40,0x40,0x4E,0x48,0x50,0x70,0x48,0x44,0xEE,0x00,0x00},/*"k",75*/

{0x00,0x00,0x00,0x10,0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00},/*"l",76*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x49,0x49,0x49,0x49,0x49,0xED,0x00,0x00},/*"m",77*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDC,0x62,0x42,0x42,0x42,0x42,0xE7,0x00,0x00},/*"n",78*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00},/*"o",79*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD8,0x64,0x42,0x42,0x42,0x64,0x58,0x40,0xE0},/*"p",80*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1A,0x26,0x42,0x42,0x42,0x26,0x1A,0x02,0x07},/*"q",81*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEE,0x32,0x20,0x20,0x20,0x20,0xF8,0x00,0x00},/*"r",82*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x42,0x40,0x3C,0x02,0x42,0x7C,0x00,0x00},/*"s",83*/

{0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x7C,0x10,0x10,0x10,0x10,0x12,0x0C,0x00,0x00},/*"t",84*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC6,0x42,0x42,0x42,0x42,0x46,0x3B,0x00,0x00},/*"u",85*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEE,0x44,0x44,0x28,0x28,0x10,0x10,0x00,0x00},/*"v",86*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDB,0x89,0x4A,0x5A,0x54,0x24,0x24,0x00,0x00},/*"w",87*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x76,0x24,0x18,0x18,0x18,0x24,0x6E,0x00,0x00},/*"x",88*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x18,0x18,0x10,0x10,0x60},/*"y",89*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x44,0x08,0x10,0x10,0x22,0x7E,0x00,0x00},/*"z",90*/

{0x00,0x03,0x04,0x04,0x04,0x04,0x04,0x04,0x08,0x04,0x04,0x04,0x04,0x04,0x03,0x00},/*"{",91*/

{0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08},/*"|",92*/

{0x00,0xC0,0x20,0x20,0x20,0x20,0x20,0x20,0x10,0x20,0x20,0x20,0x20,0x20,0xC0,0x00},/*"}",93*/

{0x20,0x5A,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"~",94*/
};
