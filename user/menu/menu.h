#ifndef __MENU_H__
#define __MENU_H__
#include "main.h"
#include <string.h>
#include "stdio.h"
#include "font_show.h"
struct MenuInit
{
  uint8_t         			*DisplaySrting;
  void                  (*function)();
  struct                MenuInit* ChildrenMenu;
  struct                MenuInit* ParentMenu;
};

void refresh_show(void);
void menu_init(void);
void menu(void);
void duocixianshi(int x, int y, int weight, int height, uint32_t addr, int number, int last);
#endif
