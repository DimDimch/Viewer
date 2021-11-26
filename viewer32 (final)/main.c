#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include "menu.h"
#include "view.h"
#include "model.h"
#include "textPainting.h"
#include "components.h"
#include "scroll.h"

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow) {
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;
    TCHAR szClassName[ ] = _T("Viewer");

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_VREDRAW | CS_HREDRAW | CS_CLASSDC;
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(ID_ICON));
    wincl.hIconSm = wincl.hIcon;
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.lpszClassName = szClassName;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    if (!RegisterClassEx (&wincl))
        return 0;

    hwnd = CreateWindowEx (
            0,
            szClassName,
            _T("Viewer"),
            WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            900,
            550,
            HWND_DESKTOP,
            NULL,
            hThisInstance,
            lpszArgument
    );

    ShowWindow (hwnd, nCmdShow);

    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return messages.wParam;
}

void CheckError(HWND hwnd, error_t* error){
    switch(*error){
        case ERROR_NO:
            break;
        case ERROR_FILE:
            MessageBox(hwnd, STRING_ERROR_FILE, _T("Error"), MB_OK | MB_ICONERROR);
            SendMessage(hwnd, WM_CLOSE, 0, 0L);
            break;
        case ERROR_MEMORY:
            MessageBox(hwnd, STRING_ERROR_MEMORY, _T("Error"), MB_OK | MB_ICONERROR);
            SendMessage(hwnd, WM_CLOSE, 0, 0L);
            break;
    }
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

    static model_info_t modelInfo;
    static view_info_t viewInfo;
    static scroll_info_t scrollInfo;
    static error_t error = ERROR_NO;

    static HMENU hMenu;
    static PAINTSTRUCT ps;

    HDC hdc;

    switch (message) {
        case WM_COMMAND:
            hMenu = GetMenu(hwnd);
            switch(LOWORD(wParam)) {
                case IDM_FILE_OPEN: {
                    //вызываем диалог для получения абсолютного пути к файлу
                    char buffer[MAX_PATH];
                    ZeroMemory(buffer, sizeof(TCHAR)*MAX_PATH);
                    OpenFileDlg(hwnd, buffer);

                    //проверяем, выбран ли файл
                    if (buffer[0] != 0) {
                        //если первый раз после открытия, то обновляем меню
                        if(UpgradeMenu(hMenu)) {
                            DrawMenuBar(hwnd);
                        }
                        //вызываем функцию заполнения всех структур
                        FillModel(&modelInfo, buffer, &error);
                        CheckError(hwnd, &error);

                        //заменяем режим с преветствия на отображение текста
                        viewInfo.mode = MODE_USUAL;
                        CheckMenuItem(hMenu, IDM_MODE_LAYOUT, MF_UNCHECKED);
                        EnableMenuItem(hMenu, IDM_MODE_LAYOUT, MF_ENABLED);
                        CheckMenuItem(hMenu, IDM_MODE_USUAL, MF_CHECKED);
                        EnableMenuItem(hMenu, IDM_MODE_USUAL, MF_DISABLED);
                        ShowScrollBar(hwnd, SB_HORZ, TRUE);
                        ChangeViewMode(hwnd, MODE_USUAL);

                        //меняем фон и цвет шрифта (по умолчанию для данного режима)
                        viewInfo.fontColor = COLOR_BLACK;
                        ChangeTextColor(hwnd, viewInfo.fontColor);
                        viewInfo.backColor = COLOR_WHITE;

                        //устанавливем величину скролла
                        InitScroll(hwnd, &scrollInfo);
                        SetScrollRange(hwnd, SB_VERT, 0, MAX_SCROLL_RANGE , TRUE);
                        SetScrollRange(hwnd, SB_HORZ, 0, MAX_SCROLL_RANGE, TRUE);

                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                    break;
                }
                case IDM_FILE_EXIT:
                    SendMessage(hwnd, WM_CLOSE, 0, 0);
                    break;
                case IDM_MODE_LAYOUT: {
                    CheckMenuItem(hMenu, IDM_MODE_USUAL, MF_UNCHECKED);
                    EnableMenuItem(hMenu, IDM_MODE_USUAL, MF_ENABLED);
                    CheckMenuItem(hMenu, IDM_MODE_LAYOUT, MF_CHECKED);
                    EnableMenuItem(hMenu, IDM_MODE_LAYOUT, MF_DISABLED);
                    ShowScrollBar(hwnd, SB_HORZ, FALSE);
                    scrollInfo.shiftX = 0;
                    viewInfo.mode = MODE_LAYOUT;

                    //получаем номер первого символа в отображаемой строке
                    unsigned long numLine = modelInfo.shift[scrollInfo.shiftY];
                    //пересчитываем массив шифтов для нового режима
                    UpdateModel(&modelInfo, viewInfo.size.width / viewInfo.fontInfo.charWidth, &error);
                    CheckError(hwnd, &error);
                    //вычисляем новый номер строки по номеру символа
                    UpdateScrollShift(&modelInfo, &scrollInfo, numLine);
                    //обновляем позицию скролла
                    FindScrollPos(&modelInfo, &scrollInfo);
                    SetScrollPos(hwnd, SB_VERT, (int)scrollInfo.posVert, TRUE);

                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                }
                case IDM_MODE_USUAL: {
                    CheckMenuItem(hMenu, IDM_MODE_LAYOUT, MF_UNCHECKED);
                    EnableMenuItem(hMenu, IDM_MODE_LAYOUT, MF_ENABLED);
                    CheckMenuItem(hMenu, IDM_MODE_USUAL, MF_CHECKED);
                    EnableMenuItem(hMenu, IDM_MODE_USUAL, MF_DISABLED);
                    ShowScrollBar(hwnd, SB_HORZ, TRUE);
                    viewInfo.mode = MODE_USUAL;

                    //получаем номер первого символа в отображаемой строке
                    unsigned long numLine = modelInfo.shift[scrollInfo.shiftY];
                    //пересчитываем массив шифтов для нового режима
                    UpdateModel(&modelInfo, UINT_MAX, &error);
                    CheckError(hwnd, &error);
                    //вычисляем новый номер строки по номеру символа
                    UpdateScrollShift(&modelInfo, &scrollInfo, numLine);
                    //обновляем позицию скролла
                    FindScrollPos(&modelInfo, &scrollInfo);
                    SetScrollPos(hwnd, SB_VERT, (int)scrollInfo.posVert, TRUE);

                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                }
                case IDM_VIEW_BACK_WHITE :
                case IDM_VIEW_BACK_GRAY :
                case IDM_VIEW_BACK_BLACK :
                    CheckMenuItem(hMenu, viewInfo.backColor + IDM_VIEW_BACK, MF_UNCHECKED);
                    EnableMenuItem(hMenu, viewInfo.backColor + IDM_VIEW_BACK, MF_ENABLED);
                    viewInfo.backColor = LOWORD(wParam) - IDM_VIEW_BACK;
                    CheckMenuItem(hMenu, viewInfo.backColor + IDM_VIEW_BACK, MF_CHECKED);
                    EnableMenuItem(hMenu, viewInfo.backColor + IDM_VIEW_BACK, MF_DISABLED);
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case IDM_VIEW_TEXT_WHITE:
                case IDM_VIEW_TEXT_GRAY:
                case IDM_VIEW_TEXT_BLACK:
                    CheckMenuItem(hMenu, viewInfo.fontColor + IDM_VIEW_TEXT, MF_UNCHECKED);
                    EnableMenuItem(hMenu, viewInfo.fontColor + IDM_VIEW_TEXT, MF_ENABLED);
                    viewInfo.fontColor = LOWORD(wParam) - IDM_VIEW_TEXT;
                    CheckMenuItem(hMenu, viewInfo.fontColor + IDM_VIEW_TEXT, MF_CHECKED);
                    EnableMenuItem(hMenu, viewInfo.fontColor + IDM_VIEW_TEXT, MF_DISABLED);
                    ChangeTextColor(hwnd, viewInfo.fontColor);
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case IDM_HELP_ABOUT:
                    MessageBox(hwnd, STRING_MSG_ABOUT, _T("About program"), MB_ICONINFORMATION | MB_OK);
                    break;
                case IDM_HELP_HELP:
                    MessageBox(hwnd, STRING_MSG_HELP, _T("Help"), MB_OK | MB_ICONQUESTION);
                    break;
            }
            break;
        case WM_CREATE: {
            //задаем начальное состояние отображения, модели и скрулла
            InitModel(&modelInfo);
            InitView(&viewInfo);
            InitScroll(hwnd, &scrollInfo);

            //устанавливаем режим отображения и цвет текста
            ChangeViewMode(hwnd, viewInfo.mode);
            ChangeTextColor(hwnd, viewInfo.fontColor);
            setlocale(LC_ALL, "Rus");

            //устанавливаем шрифт
            hdc = GetDC(hwnd);
            SelectObject(hdc, viewInfo.fontInfo.font);
            SetBkMode(hdc, TRANSPARENT);
            ReleaseDC(hwnd, hdc);

            //создаем и прикрепляем начальное меню
            hMenu = CreateStartMenu();
            SetMenu(hwnd, hMenu);
            DrawMenuBar(hwnd);

            //убираем сколлы
            ShowScrollBar(hwnd, SB_HORZ, FALSE);
            ShowScrollBar(hwnd, SB_VERT, FALSE);
            break;
        }
        case WM_PAINT: {
            hdc = BeginPaint(hwnd, &ps);
            PaintBackground(hdc, &viewInfo, &ps);
            PaintText(hdc, &modelInfo, &viewInfo, &scrollInfo, &ps);
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_SIZE: {
            viewInfo.size.width = LOWORD(lParam);
            viewInfo.size.height = HIWORD(lParam);
            if (viewInfo.mode == MODE_LAYOUT) {
                //получаем номер первого символа в отображаемой строке
                unsigned long numLine = modelInfo.shift[scrollInfo.shiftY];
                //пересчитываем массив шифтов для нового размера окна
                UpdateModel(&modelInfo, viewInfo.size.width / viewInfo.fontInfo.charWidth, &error);
                CheckError(hwnd, &error);
                //вычисляем новый номер строки по номеру символа
                UpdateScrollShift(&modelInfo, &scrollInfo, numLine);
                //обновляем позицию скролла
                FindScrollPos(&modelInfo, &scrollInfo);
                SetScrollPos(hwnd, SB_VERT, (int)scrollInfo.posVert, TRUE);
            }
            break;
        }
        case WM_VSCROLL: {
            switch (LOWORD(wParam)) {
                case SB_TOP:
                    scrollInfo.shiftY = 0;
                    scrollInfo.posVert = 0;
                    break;
                case SB_BOTTOM:
                    scrollInfo.shiftY = modelInfo.numLines - viewInfo.size.height / viewInfo.fontInfo.charHeight;
                    scrollInfo.posVert = MAX_SCROLL_RANGE;
                    break;
                case SB_LINEUP:
                    scrollInfo.shiftY -= 1;
                    FindScrollPos(&modelInfo, &scrollInfo);
                    break;
                case SB_LINEDOWN:
                    scrollInfo.shiftY += 1;
                    FindScrollPos(&modelInfo, &scrollInfo);
                    break;
                case SB_PAGEUP:
                    scrollInfo.shiftY -= viewInfo.size.height / viewInfo.fontInfo.charHeight;
                    FindScrollPos(&modelInfo, &scrollInfo);
                    break;
                case SB_PAGEDOWN:
                    scrollInfo.shiftY += viewInfo.size.height / viewInfo.fontInfo.charHeight;
                    FindScrollPos(&modelInfo, &scrollInfo);
                    break;
                case SB_THUMBTRACK:
                    scrollInfo.posVert = HIWORD(wParam);
                    FindScrollShift(&modelInfo, &scrollInfo);
                    break;
            }
            if (scrollInfo.shiftY < 0) {
                scrollInfo.shiftY = 0;
                scrollInfo.posVert = 0;
            }
            if (scrollInfo.shiftY > modelInfo.numLines - viewInfo.size.height / viewInfo.fontInfo.charHeight) {
                scrollInfo.shiftY = modelInfo.numLines - viewInfo.size.height / viewInfo.fontInfo.charHeight;
                scrollInfo.posVert = MAX_SCROLL_RANGE;
            }
            SetScrollPos(hwnd, SB_VERT, (int)scrollInfo.posVert, TRUE);
            InvalidateRect(hwnd, NULL, TRUE);
            UpdateWindow(hwnd);
            break;
        }
        case WM_HSCROLL: {
            switch (LOWORD(wParam)) {
                case SB_LINERIGHT:
                    scrollInfo.shiftX += 1;
                    FindScrollPos(&modelInfo, &scrollInfo);
                    break;
                case SB_LINELEFT:
                    scrollInfo.shiftX -= 1;
                    FindScrollPos(&modelInfo, &scrollInfo);
                    break;
                case SB_PAGERIGHT:
                    scrollInfo.shiftX += viewInfo.size.width / viewInfo.fontInfo.charWidth;
                    FindScrollPos(&modelInfo, &scrollInfo);
                    break;
                case SB_PAGELEFT:
                    scrollInfo.shiftX -= viewInfo.size.width / viewInfo.fontInfo.charWidth;
                    FindScrollPos(&modelInfo, &scrollInfo);
                    break;
                case SB_THUMBTRACK:
                    scrollInfo.posHorz = HIWORD(wParam);
                    FindScrollShift(&modelInfo, &scrollInfo);
                    break;
            }
            if (scrollInfo.shiftX < 0) {
                scrollInfo.shiftX = 0;
                scrollInfo.posHorz = 0;
            }
            if (scrollInfo.shiftX > modelInfo.maxLength) {
                scrollInfo.shiftX = modelInfo.maxLength;
                scrollInfo.posHorz = MAX_SCROLL_RANGE;
            }
            SetScrollPos(hwnd, SB_HORZ, scrollInfo.posHorz, TRUE);
            InvalidateRect(hwnd, NULL, TRUE);
            UpdateWindow(hwnd);
            break;
        }
        case WM_CHAR:
            switch (wParam) {
                case 0x01 + ('O' - 'A'):
                    SendMessage(hwnd, WM_COMMAND, IDM_FILE_OPEN, 0);
                    break;
                case 0x01 + ('U' - 'A'):
                    if (viewInfo.mode == MODE_LAYOUT)
                        SendMessage(hwnd, WM_COMMAND, IDM_MODE_USUAL, 0);
                    break;
                case 0x01 + ('L' - 'A'):
                    if (viewInfo.mode == MODE_USUAL)
                        SendMessage(hwnd, WM_COMMAND, IDM_MODE_LAYOUT, 0);
                    break;
            }
        case WM_KEYDOWN: {
            switch (wParam) {
                case VK_LEFT:
                    SendMessage(hwnd, WM_HSCROLL, SB_LINELEFT, 0);
                    break;
                case VK_RIGHT:
                    SendMessage(hwnd, WM_HSCROLL, SB_LINERIGHT, 0);
                    break;
                case VK_UP:
                    SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0);
                    break;
                case VK_DOWN:
                    SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0);
                    break;
                case VK_PRIOR:
                    SendMessage(hwnd, WM_VSCROLL, SB_PAGEUP, 0);
                    break;
                case VK_NEXT:
                    SendMessage(hwnd, WM_VSCROLL, SB_PAGEDOWN, 0);
                    break;
                case VK_HOME:
                    SendMessage(hwnd, WM_VSCROLL, SB_TOP, 0);
                    break;
                case VK_END:
                    SendMessage(hwnd, WM_VSCROLL, SB_BOTTOM, 0);
                    break;
            }
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            DestroyMenu(hMenu);
            if (modelInfo.data != NULL)
                free(modelInfo.data);
            if (modelInfo.shift != NULL)
                free(modelInfo.shift);
            break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}
