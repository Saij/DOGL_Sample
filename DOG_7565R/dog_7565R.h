/*
 * Copyright (c) 2014 by ELECTRONIC ASSEMBLY <technik@lcd-module.de>
 * EA DOG Graphic (ST7565R) software library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef DOG_7565R_H
#define DOG_7565R_H


#define DOGM128 1
#define DOGL128 2
#define DOGM132 3

#define VIEW_BOTTOM 0xC0
#define VIEW_TOP 	0xC8

#define VIEW_INVERT 0xA7
#define VIEW_NORMAL 0xA6

class dog_7565R
{
  public:
    void initialize     (byte p_cs, byte p_si, byte p_clk, byte p_a0, byte p_res, byte type);
    void clear			(void);
    void contrast       (byte contr);
	void view			(byte direction);
	void string         (byte column, byte page, const byte *font_adress, const char *str);
	void rectangle		(byte start_column, byte start_page, byte end_column, byte end_page, byte pattern);
	void picture		(byte column, byte page, const byte *pic_adress);
    void invert         (byte invert);

  private:
    byte p_cs;
    byte p_si;
    byte p_clk;
    byte p_a0;
	byte type;
	boolean hardware;
    boolean top_view;
	
	void position   (byte column, byte page);
    void command	(byte dat);
    void data		(byte dat);
    
    void spi_initialize	(byte cs, byte si, byte clk);
    void spi_put_byte	(byte dat);
    void spi_put		(byte *dat, int len);
	void spi_out		(byte dat);
};

#endif
