#include "menu.h"

#include <commdlg.h>
#include <stdlib.h>
#include <stdio.h>


/**
 * @brief �������� ���������� ����
 *
 * @return ��������� �� ��������� ����
 */
HMENU CreateStartMenu() {
    HMENU hMenu = CreateMenu();
    HMENU hMenuPopup;

    //������ � ������
    hMenuPopup = CreateMenu();

    AppendMenu(hMenuPopup, MF_STRING, IDM_FILE_OPEN, "&�������...\t(Ctrl + O)"); //������� ����� ���� ��� ���������
    AppendMenu(hMenuPopup, MF_SEPARATOR, 0, NULL);
    AppendMenu(hMenuPopup, MF_STRING, IDM_FILE_EXIT, "&������� ����"); //��������� ������ ��������� � �����

    AppendMenu(hMenu, MF_POPUP,(UINT_PTR) hMenuPopup, "&����");

    //������ ������
    hMenuPopup = CreateMenu();

    AppendMenu(hMenuPopup, MF_STRING, IDM_HELP_HELP, "&����������� �������..."); //��� �������� � ����������?
    AppendMenu(hMenuPopup, MF_STRING, IDM_HELP_ABOUT, "&� ���������..."); //���������� � ���������

    AppendMenu(hMenu, MF_POPUP,(UINT_PTR) hMenuPopup, "&�������");

    return hMenu;
}


/**
 * @brief ���������� � ���� ����� �������
 *
 * @param hMenu - ��������� �� ����
 *
 * @return TRUE - ���� ������ ���������, FALSE - ���� ������ ��� ����������
 */
BOOL UpgradeMenu(HMENU hMenu) {
    if (GetMenuItemCount(hMenu) == MENU_ITEM_COUNT) {
        return FALSE;
    }

    HMENU hMenuPopup;

    //�������� ������� ����� � �������
    HMENU hMenuHelp = GetSubMenu(hMenu, SUBMENU_NUM_HELP_START);
    RemoveMenu(hMenu, (UINT_PTR) hMenuHelp, MF_BYCOMMAND);

    //����� ������ ��������� �����
    hMenuPopup = CreateMenu();

    AppendMenu(hMenuPopup, MF_STRING | MF_CHECKED | MF_DISABLED, IDM_MODE_USUAL, "&��� �������"); //����� ��� �������
    AppendMenu(hMenuPopup, MF_STRING, IDM_MODE_LAYOUT, "&� ��������"); //����� � ��������

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hMenuPopup, "&�����");

    //����� ����� ������� ���� � ������
    hMenuPopup = CreateMenu();
    HMENU hMenuPopup1 = CreateMenu();
    HMENU hMenuPopup2 = CreateMenu();

    AppendMenu(hMenuPopup1, MF_STRING | MF_CHECKED | MF_DISABLED, IDM_VIEW_BACK_WHITE, "&�����");
    AppendMenu(hMenuPopup1, MF_STRING, IDM_VIEW_BACK_GRAY, "&�����");
    AppendMenu(hMenuPopup1, MF_STRING, IDM_VIEW_BACK_BLACK, "&������");

    AppendMenu(hMenuPopup2, MF_STRING, IDM_VIEW_TEXT_WHITE, "&�����");
    AppendMenu(hMenuPopup2, MF_STRING, IDM_VIEW_TEXT_GRAY, "&�����");
    AppendMenu(hMenuPopup2, MF_STRING | MF_CHECKED | MF_DISABLED, IDM_VIEW_TEXT_BLACK, "&������");


    AppendMenu(hMenuPopup, MF_POPUP, (UINT_PTR) hMenuPopup1, "&������ ���");
    AppendMenu(hMenuPopup, MF_POPUP, (UINT_PTR) hMenuPopup2, "&���� ������");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hMenuPopup, "&���");

    //���������� ������ � �����
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hMenuHelp, "&�������");

    return TRUE;
}


/**
 * @brief �������� ����������� ���� ������ �����
 *
 * @param hMenu - ��������� �� ����
 * @param buffer - ������ ��� ������ ���� � �����
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
    ofn.lpstrTitle = TEXT("����������, �������� ����");
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



