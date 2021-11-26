#pragma once

#include <windows.h>

#define ID_ICON 23243

#define IDM_FILE_OPEN 11
#define IDM_FILE_EXIT 12

#define IDM_MODE_USUAL 21
#define IDM_MODE_LAYOUT 22

#define IDM_HELP_ABOUT 31
#define IDM_HELP_HELP 32

#define IDM_VIEW_BACK 410
#define IDM_VIEW_BACK_WHITE 411
#define IDM_VIEW_BACK_GRAY 412
#define IDM_VIEW_BACK_BLACK 413

#define IDM_VIEW_TEXT 420
#define IDM_VIEW_TEXT_WHITE 421
#define IDM_VIEW_TEXT_GRAY 422
#define IDM_VIEW_TEXT_BLACK 423

#define SUBMENU_NUM_HELP_START 1
#define MENU_ITEM_COUNT 4


HMENU CreateStartMenu();
BOOL UpgradeMenu(HMENU hMenu);

BOOL OpenFileDlg(HWND hwnd, PTCHAR buffer);
