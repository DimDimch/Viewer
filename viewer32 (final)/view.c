#include "view.h"


/**
 * @brief Заполнение полей структуры начальными значениями
 *
 * @param view - структура, содержащая информацию об отображении
 *
 * @return void
 */
void InitView(view_info_t* view) {
    view->mode = MODE_MESSEGE;
    InitFont(&view->fontInfo);
    view->backColor = COLOR_GRAY;
    view->fontColor = COLOR_WHITE;
}


/**
 * @brief Создание шрифта, заполнение информации о размере символов
 *
 * @param font - структура, содержащая информацию о шрифте
 *
 * @return void
 */
void InitFont(font_info_t* font) {
    font->charWidth = CHAR_WIDTH;
    font->charHeight = CHAR_HEIGHT;
    font->font = CreateFont(
            CHAR_HEIGHT,                    //Высота в логических единицах символа шрифта
            CHAR_WIDTH,                     //Ширина в логических единицах символа шрифта
            0,                              //Угол
            0,                              //Угол
            FW_NORMAL,                      //Вес шрифта (жирность)
            FALSE,                          //Курсив
            FALSE,                          //Подчеркнутый
            FALSE,                          //Зачеркнутый
            DEFAULT_CHARSET,                //Набор символов
            OUT_DEFAULT_PRECIS,             //Точность вывода
            CLIP_DEFAULT_PRECIS,            //Точность отсечения
            ANTIALIASED_QUALITY,            //Качество вывода
            FIXED_PITCH,                    //Семейство шрифта
            "Courier New");                 //Имя шрифта
}


/**
 * @brief Изменение режима отображения на уровне смещения текста
 *
 * @param hwnd - дескриптор окна
 * @param mode - режим отображения
 *
 * @return void
 */
void ChangeViewMode(HWND hwnd, view_mode_t mode) {
    HDC hdc = GetDC(hwnd);

    switch (mode) {
        case MODE_MESSEGE:
            SetTextAlign(hdc, TA_CENTER | TA_LEFT);
            break;
        case MODE_LAYOUT:
        case MODE_USUAL:
            SetTextAlign(hdc, GetTextAlign(hdc) & (~TA_CENTER));
            break;
        default:
            break;
    }

    ReleaseDC(hwnd, hdc);
}


/**
 * @brief Изменение цвета шрифта
 *
 * @param hwnd - дескриптор окна
 * @param color - выбранный цвет
 *
 * @return void
 */
void ChangeTextColor(HWND hwnd, color_t color) {
    HDC hdc = GetDC(hwnd);

    switch (color) {
        case COLOR_WHITE:
            SetTextColor(hdc, RGB(255,255,255));
            break;
        case COLOR_BLACK:
            SetTextColor(hdc, RGB(0,0,0));
            break;
        case COLOR_GRAY:
            SetTextColor(hdc, RGB(71,71,71));
            break;
        default:
            break;
    }

    ReleaseDC(hwnd, hdc);
}


