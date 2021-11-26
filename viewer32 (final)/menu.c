#include "menu.h"

#include <commdlg.h>
#include <stdlib.h>
#include <stdio.h>


/**
 * @brief Создание стартового меню
 *
 * @return указатель на созданное меню
 */
HMENU CreateStartMenu() {
    HMENU hMenu = CreateMenu();
    HMENU hMenuPopup;

    //Работа с файлом
    hMenuPopup = CreateMenu();

    AppendMenu(hMenuPopup, MF_STRING, IDM_FILE_OPEN, "&Открыть...\t(Ctrl + O)"); //Открыть новый файл для просмотра
    AppendMenu(hMenuPopup, MF_SEPARATOR, 0, NULL);
    AppendMenu(hMenuPopup, MF_STRING, IDM_FILE_EXIT, "&Закрыть окно"); //Завершить работу программы и выйти

    AppendMenu(hMenu, MF_POPUP,(UINT_PTR) hMenuPopup, "&Файл");

    //Раздел помощи
    hMenuPopup = CreateMenu();

    AppendMenu(hMenuPopup, MF_STRING, IDM_HELP_HELP, "&Просмотреть справку..."); //Как работать с программой?
    AppendMenu(hMenuPopup, MF_STRING, IDM_HELP_ABOUT, "&О программе..."); //Информация о программе

    AppendMenu(hMenu, MF_POPUP,(UINT_PTR) hMenuPopup, "&Справка");

    return hMenu;
}


/**
 * @brief Добавление в меню новых пунктов
 *
 * @param hMenu - указатель на меню
 *
 * @return TRUE - если пункты добавлены, FALSE - если пункты уже существуют
 */
BOOL UpgradeMenu(HMENU hMenu) {
    if (GetMenuItemCount(hMenu) == MENU_ITEM_COUNT) {
        return FALSE;
    }

    HMENU hMenuPopup;

    //Временно убираем пункт с помощью
    HMENU hMenuHelp = GetSubMenu(hMenu, SUBMENU_NUM_HELP_START);
    RemoveMenu(hMenu, (UINT_PTR) hMenuHelp, MF_BYCOMMAND);

    //Выбор режима просмотра файла
    hMenuPopup = CreateMenu();

    AppendMenu(hMenuPopup, MF_STRING | MF_CHECKED | MF_DISABLED, IDM_MODE_USUAL, "&Без верстки"); //Режим без верстки
    AppendMenu(hMenuPopup, MF_STRING, IDM_MODE_LAYOUT, "&С версткой"); //Режим с версткой

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hMenuPopup, "&Режим");

    //Выбор цвета заднего фона и текста
    hMenuPopup = CreateMenu();
    HMENU hMenuPopup1 = CreateMenu();
    HMENU hMenuPopup2 = CreateMenu();

    AppendMenu(hMenuPopup1, MF_STRING | MF_CHECKED | MF_DISABLED, IDM_VIEW_BACK_WHITE, "&Белый");
    AppendMenu(hMenuPopup1, MF_STRING, IDM_VIEW_BACK_GRAY, "&Серый");
    AppendMenu(hMenuPopup1, MF_STRING, IDM_VIEW_BACK_BLACK, "&Черный");

    AppendMenu(hMenuPopup2, MF_STRING, IDM_VIEW_TEXT_WHITE, "&Белый");
    AppendMenu(hMenuPopup2, MF_STRING, IDM_VIEW_TEXT_GRAY, "&Серый");
    AppendMenu(hMenuPopup2, MF_STRING | MF_CHECKED | MF_DISABLED, IDM_VIEW_TEXT_BLACK, "&Черный");


    AppendMenu(hMenuPopup, MF_POPUP, (UINT_PTR) hMenuPopup1, "&Задний фон");
    AppendMenu(hMenuPopup, MF_POPUP, (UINT_PTR) hMenuPopup2, "&Цвет текста");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hMenuPopup, "&Вид");

    //Возвращаем помощь в конец
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hMenuHelp, "&Справка");

    return TRUE;
}


/**
 * @brief Открытие диалогового окна выбора файла
 *
 * @param hMenu - указатель на меню
 * @param buffer - буффер для записи пути к файлу
 *
 * @return BOOL
 */
 BOOL OpenFileDlg(HWND hwnd, PTCHAR name){
    static OPENFILENAME ofn;
    static char szFilter[] = "Text Files(*.TXT)\0*.txt\0";

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = name;
    ofn.nMaxFile = _MAX_PATH;
    ofn.lpstrFilter = szFilter;
    ofn.nFilterIndex = 1;
    ofn.lpstrTitle = TEXT("Пожалуйста, выберите файл");
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter = 0;
    ofn.nFileOffset = 0;
    ofn.nFileExtension = 0;
    ofn.lpstrDefExt = "txt";
    ofn.lCustData = 0;
    ofn.lpfnHook = NULL;
    ofn.lpTemplateName = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    return GetOpenFileName(&ofn);
}



