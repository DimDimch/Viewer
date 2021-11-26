#include "textPainting.h"


/**
 * @brief ����������� ������ ��� ���������� ������ "���������" (����� �� ������ ����)
 *
 * @param hdc - �������� ����������
 * @param model - ���������, ���������� ���������� � ������
 * @param view - ���������, ���������� ���������� �� �����������
 * @param scroll - ���������, ���������� ���������� � ������
 * @param ps - ���������, ����������� ���������� ��� ���������
 *
 * @return void
 */
void MessageTextOut(HDC hdc, model_info_t* model, view_info_t* view, PAINTSTRUCT* ps) {
    TextOut(hdc, view->size.width / 2,  view->size.height / 2 - view->fontInfo.charHeight / 2,  STRING_START, strlen(STRING_START));
}


/**
 * @brief ����������� ������ ��� ������ � �������� � ��� �������
 *
 * @param hdc - �������� ����������
 * @param model - ���������, ���������� ���������� � ������
 * @param view - ���������, ���������� ���������� �� �����������
 * @param scroll - ���������, ���������� ���������� � ������
 * @param ps - ���������, ����������� ���������� ��� ���������
 *
 * @return void
 */
void UsualTextOut(HDC hdc, model_info_t* model, view_info_t* view, scroll_info_t* scroll, PAINTSTRUCT* ps) {
    int currentLine = scroll->shiftY;

    int paintBegin = ps->rcPaint.top / view->fontInfo.charHeight;
    int paintEnd = min(
            ps->rcPaint.bottom / view->fontInfo.charHeight,
            min(view->size.height / view->fontInfo.charHeight, model->numLines - 1 - currentLine)
            ) + 1;

    int curLineLength;
    int countNum;
    char *textForOut;

    for (int i = paintBegin; i < paintEnd; i++) {
        curLineLength = (int)(model->shift[currentLine + i + 1] - model->shift[currentLine + i]);
        textForOut = model->data + model->shift[currentLine + i] + scroll->shiftX;
        countNum = min((int)(view->size.width / view->fontInfo.charWidth + 1), (int)(curLineLength - scroll->shiftX));

        while ((textForOut[countNum - 1] == '\n' || textForOut[countNum - 1] == '\r' || textForOut[countNum - 1] == '\0') && countNum > 0) {
            countNum--;
        }

        TextOut(hdc, 0,  i * view->fontInfo.charHeight, textForOut, max(0, countNum));
    }
}


/**
 * @brief ����������� ������ ��� ������ �������
 *
 * @param hdc - �������� ����������
 * @param model - ���������, ���������� ���������� � ������
 * @param view - ���������, ���������� ���������� �� �����������
 * @param scroll - ���������, ���������� ���������� � ������
 * @param ps - ���������, ����������� ���������� ��� ���������
 *
 * @return void
 */
void PaintText(HDC hdc, model_info_t* model, view_info_t* view, scroll_info_t* scroll, PAINTSTRUCT* ps) {
    switch (view->mode) {
        case MODE_USUAL:
        case MODE_LAYOUT:
            UsualTextOut(hdc, model, view, scroll, ps);
            break;
        case MODE_MESSEGE:
            MessageTextOut(hdc, model, view, ps);
        default:
            break;
    }
}


/**
 * @brief ��������� ������� ����
 *
 * @param hdc - �������� ����������
 * @param view - ���������, ���������� ���������� �� �����������
 * @param ps - ���������, ����������� ���������� ��� ���������
 *
 * @return void
 */
void PaintBackground(HDC hdc, view_info_t* view, PAINTSTRUCT* ps) {
    switch (view->backColor) {
        case COLOR_BLACK:
            FillRect(hdc, &ps->rcPaint, (HBRUSH)GetStockObject(BLACK_BRUSH));
            break;
        case COLOR_WHITE:
            FillRect(hdc, &ps->rcPaint, (HBRUSH)GetStockObject(WHITE_BRUSH));
            break;
        case COLOR_GRAY:
            FillRect(hdc, &ps->rcPaint, (HBRUSH)GetStockObject(GRAY_BRUSH));
            break;
        default:
            break;
    }
}
