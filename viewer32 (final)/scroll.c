#include "scroll.h"


/**
 * @brief »нициализаци€ полей скролла нулевыми значени€ми и возврат в начальное положение
 *
 * @param hwnd - дескриптор окна
 * @param scroll - структура, содержаща€ информацию о скролле
 *
 * @return void
 */
void InitScroll(HWND hwnd, scroll_info_t* scroll) {
    scroll->posVert = 0;
    scroll->posHorz = 0;
    scroll->shiftX = 0;
    scroll->shiftY = 0;
    SetScrollPos(hwnd, SB_HORZ, 0, TRUE);
    SetScrollPos(hwnd, SB_VERT, 0, TRUE);
}


/**
 * @brief ќбновление текущего номера строки по номеру первого элемента (при изменении режима отображени€)
 *
 * @param model - структура, содержаща€ информацию о модели
 * @param scroll - структура, содержаща€ информацию о скролле
 * @param symbolNum - позици€ первого символа из искомой строки
 *
 * @return void
 */
void UpdateScrollShift(model_info_t* model, scroll_info_t* scroll, unsigned long symbolNum) {
    for (int i = 0; i < model->numLines; i++) {
        if (symbolNum >= model->shift[i] && symbolNum < model->shift[i + 1]) {
            scroll->shiftY = i;
            break;
        }
    }
}


/**
 * @brief ѕоиск смещени€ при изменении позиции скролла
 *
 * @param model - структура, содержаща€ информацию о модели
 * @param scroll - структура, содержаща€ информацию о скролле
 *
 * @return void
 */
void FindScrollShift(model_info_t* model, scroll_info_t* scroll) {
    scroll->shiftX = (int)(((float)scroll->posHorz / (float)MAX_SCROLL_RANGE) * (float)model->maxLength);
    scroll->shiftY = (int)(((float)scroll->posVert / (float)MAX_SCROLL_RANGE) * (float)model->numLines);
}


/**
 * @brief ѕоиск позиции скролла при изменении смещени€
 *
 * @param model - структура, содержаща€ информацию о модели
 * @param scroll - структура, содержаща€ информацию о скролле
 *
 * @return void
 */
void FindScrollPos(model_info_t* model, scroll_info_t* scroll) {
    scroll->posHorz = (int)(((float)scroll->shiftX / (float)model->maxLength) * (float)MAX_SCROLL_RANGE);
    scroll->posVert = (int)(((float)scroll->shiftY / (float)model->numLines) * (float)MAX_SCROLL_RANGE);
}
