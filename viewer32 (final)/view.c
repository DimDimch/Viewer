#include "view.h"


/**
 * @brief ���������� ����� ��������� ���������� ����������
 *
 * @param view - ���������, ���������� ���������� �� �����������
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
 * @brief �������� ������, ���������� ���������� � ������� ��������
 *
 * @param font - ���������, ���������� ���������� � ������
 *
 * @return void
 */
void InitFont(font_info_t* font) {
    font->charWidth = CHAR_WIDTH;
    font->charHeight = CHAR_HEIGHT;
    font->font = CreateFont(
            CHAR_HEIGHT,                    //������ � ���������� �������� ������� ������
            CHAR_WIDTH,                     //������ � ���������� �������� ������� ������
            0,                              //����
            0,                              //����
            FW_NORMAL,                      //��� ������ (��������)
            FALSE,                          //������
            FALSE,                          //������������
            FALSE,                          //�����������
            DEFAULT_CHARSET,                //����� ��������
            OUT_DEFAULT_PRECIS,             //�������� ������
            CLIP_DEFAULT_PRECIS,            //�������� ���������
            ANTIALIASED_QUALITY,            //�������� ������
            FIXED_PITCH,                    //��������� ������
            "Courier New");                 //��� ������
}


/**
 * @brief ��������� ������ ����������� �� ������ �������� ������
 *
 * @param hwnd - ���������� ����
 * @param mode - ����� �����������
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
 * @brief ��������� ����� ������
 *
 * @param hwnd - ���������� ����
 * @param color - ��������� ����
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


