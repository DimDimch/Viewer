#pragma once

#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <locale.h>

#define STRING_START "Откройте файл для просмотра содержимого"
#define STRING_ERROR_MEMORY "Возникли трудности при выделении памяти.\nК сожалению, приложение завершает свою работу"
#define STRING_ERROR_FILE "Возникли трудности при работе с файлом.\nК сожалению, приложение завершает свою работу"
#define STRING_MSG_ABOUT "Thank you for using this program\n\n(с) Serdyukov Dmitriy\n2021"
#define STRING_MSG_HELP "Это программа для просмотра файлов. Вы можете выбрать нужный файл и переключать режимы просмотра файла: с версткой и обычный (без верстки)"

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 20

#define REALLOC_STEP 100000


/** @brief Типы ошибок, возникающих при работе приложения
 *
 * ERROR_MEMORY - ошибки при работе с памятью
 * ERROR_FILE - ошибки при работе с файлами
 * ERROR_NO - отсутствие ошибок
 *
 */
typedef enum error {
    ERROR_MEMORY,
    ERROR_FILE,
    ERROR_NO
}error_t;


/** @brief Цвета
 *
 * COLOR_WHITE - белый цвет
 * COLOR_GRAY - серый цвет
 * COLOR_BLACK - черный цвет
 *
 */
typedef enum color {
    COLOR_WHITE = 1,
    COLOR_GRAY,
    COLOR_BLACK
}color_t;


/** @brief Режимы отображения текста
 *
 * MODE_USUAL - режим без верстки
 * MODE_LAYOUT - режим с версткой
 * MODE_MESSEGE - режим сообщения (начальный режим, текст по центру)
 *
 */
typedef enum view_mode {
    MODE_USUAL = 1,
    MODE_LAYOUT,
    MODE_MESSEGE
}view_mode_t;


/** @brief Размеры окна
 *
 * height - высота
 * width - ширина
 *
 */
typedef struct window_size {
    unsigned int height;
    unsigned int width;
}window_size_t;


/** @brief Параметры шрифта
 *
 * font - созданный шрифт
 * charWidth - ширина символа
 * charHeight - высота символа
 *
 */
typedef struct font_info {
    HFONT font;
    int charWidth;
    int charHeight;
}font_info_t;


/** @brief Параметры полосы прокрутки
 *
 * posVert - позиция вертикального сколла
 * posHorz - позиция горизонтального сколла
 * shiftX - горизонтальное смещение текста
 * shiftY - вертикальное смещение текста
 *
 */
typedef struct scroll_info {
    unsigned int posVert;
    unsigned int posHorz;
    int shiftX;
    int shiftY;
}scroll_info_t;


/** @brief Параметры модели
 *
 * data - указатель на текст
 * numSymbols - кол-во символов в тексте
 * numLines - кол-во строк в тексте
 * maxLength - максимальная длина строки
 * shift - указатель на массив перевода строк
 *
 */
typedef struct model_info {
    char* data;
    unsigned long numSymbols;
    unsigned int numLines;
    unsigned int maxLength;
    unsigned long* shift;
}model_info_t;


/** @brief Параметры представления
 *
 * mode - выбранный режим отображения
 * fontInfo - информация о шрифте
 * backColor - цвет заднего фона
 * fontColor - цвет шрифта
 * size - размеры окна
 *
 */
typedef struct view_info {
    view_mode_t mode;
    font_info_t fontInfo;
    color_t backColor;
    color_t fontColor;
    window_size_t size;
}view_info_t;

