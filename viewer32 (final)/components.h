#pragma once

#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <locale.h>

#define STRING_START "�������� ���� ��� ��������� �����������"
#define STRING_ERROR_MEMORY "�������� ��������� ��� ��������� ������.\n� ���������, ���������� ��������� ���� ������"
#define STRING_ERROR_FILE "�������� ��������� ��� ������ � ������.\n� ���������, ���������� ��������� ���� ������"
#define STRING_MSG_ABOUT "Thank you for using this program\n\n(�) Serdyukov Dmitriy\n2021"
#define STRING_MSG_HELP "��� ��������� ��� ��������� ������. �� ������ ������� ������ ���� � ����������� ������ ��������� �����: � �������� � ������� (��� �������)"

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 20

#define REALLOC_STEP 100000


/** @brief ���� ������, ����������� ��� ������ ����������
 *
 * ERROR_MEMORY - ������ ��� ������ � �������
 * ERROR_FILE - ������ ��� ������ � �������
 * ERROR_NO - ���������� ������
 *
 */
typedef enum error {
    ERROR_MEMORY,
    ERROR_FILE,
    ERROR_NO
}error_t;


/** @brief �����
 *
 * COLOR_WHITE - ����� ����
 * COLOR_GRAY - ����� ����
 * COLOR_BLACK - ������ ����
 *
 */
typedef enum color {
    COLOR_WHITE = 1,
    COLOR_GRAY,
    COLOR_BLACK
}color_t;


/** @brief ������ ����������� ������
 *
 * MODE_USUAL - ����� ��� �������
 * MODE_LAYOUT - ����� � ��������
 * MODE_MESSEGE - ����� ��������� (��������� �����, ����� �� ������)
 *
 */
typedef enum view_mode {
    MODE_USUAL = 1,
    MODE_LAYOUT,
    MODE_MESSEGE
}view_mode_t;


/** @brief ������� ����
 *
 * height - ������
 * width - ������
 *
 */
typedef struct window_size {
    unsigned int height;
    unsigned int width;
}window_size_t;


/** @brief ��������� ������
 *
 * font - ��������� �����
 * charWidth - ������ �������
 * charHeight - ������ �������
 *
 */
typedef struct font_info {
    HFONT font;
    int charWidth;
    int charHeight;
}font_info_t;


/** @brief ��������� ������ ���������
 *
 * posVert - ������� ������������� ������
 * posHorz - ������� ��������������� ������
 * shiftX - �������������� �������� ������
 * shiftY - ������������ �������� ������
 *
 */
typedef struct scroll_info {
    unsigned int posVert;
    unsigned int posHorz;
    int shiftX;
    int shiftY;
}scroll_info_t;


/** @brief ��������� ������
 *
 * data - ��������� �� �����
 * numSymbols - ���-�� �������� � ������
 * numLines - ���-�� ����� � ������
 * maxLength - ������������ ����� ������
 * shift - ��������� �� ������ �������� �����
 *
 */
typedef struct model_info {
    char* data;
    unsigned long numSymbols;
    unsigned int numLines;
    unsigned int maxLength;
    unsigned long* shift;
}model_info_t;


/** @brief ��������� �������������
 *
 * mode - ��������� ����� �����������
 * fontInfo - ���������� � ������
 * backColor - ���� ������� ����
 * fontColor - ���� ������
 * size - ������� ����
 *
 */
typedef struct view_info {
    view_mode_t mode;
    font_info_t fontInfo;
    color_t backColor;
    color_t fontColor;
    window_size_t size;
}view_info_t;

