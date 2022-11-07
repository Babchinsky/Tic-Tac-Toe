#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include <time.h>

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
    // создаём главное окно приложения на основе модального диалога
    return  DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

TCHAR str[60];          // строка для записи
HICON hIcon;            // иконка 
HWND hBut[9];          // кнопки (поле), 10я - таймер
unsigned int u_p[9];    // массив номера картинки
unsigned int u_end = 0; // количество незаполненных клеток
HBITMAP hBmp[3];        // крестик - нолик - пустота 
BOOL b_var = true;      // крестик или нолик




BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wp, LPARAM lp)
{

    switch (message)
    {
    case WM_INITDIALOG:
    {
        SetTimer(hWnd, 1, 1000, NULL);
        hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));                // програмная загрузка иконки окна 
        SendMessage(hWnd, WM_SETICON, 1, LPARAM(hIcon));                                    // посылка сообщения на установку иконки окна

        for (int i = 0; i < 9; i++)
        {
            hBut[i] = GetDlgItem(hWnd, IDC_BUTTON1 + i);                       //  привязка кнопок
            u_p[i] = 3;
        }
        //hBut[9] = GetDlgItem(hWnd, IDC_BUTTON1);                               // подгружаем кнопку таймера
        for (int i = 0; i < 3; i++)
            hBmp[i] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1 + i));  // програмная загрузка крестика-нолика
        return TRUE;
    }
    case WM_COMMAND:
    {
        u_end = 0;
        for (int i = 0; i < 10; i++) // сама игра 
        {
            // какая кнопка нажата? 
            if (wp == IDC_BUTTON1 + i)
            {
                // ставит крестик
                if (b_var)
                {
                    wsprintf(str, TEXT("Ход ноликов"));
                    SetWindowText(hWnd, str);	// строка выводится в заголовок окна
                    SendMessage(hBut[i], BM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(hBmp[0]));
                    EnableWindow(hBut[i], FALSE);
                    u_p[i] = 0;
                    b_var = false;
                }
                // ставит нолик
                else
                {
                    wsprintf(str, TEXT("Ход крестиков"));
                    SetWindowText(hWnd, str);	// строка выводится в заголовок окна
                    SendMessage(hBut[i], BM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(hBmp[1]));
                    EnableWindow(hBut[i], FALSE);
                    u_p[i] = 1;
                    b_var = true;
                }

                // проверка на комбинации
                {
                    if (u_p[0] == 0 && u_p[1] == 0 && u_p[2] == 0 ||    // гориз верхняя линия
                        u_p[3] == 0 && u_p[4] == 0 && u_p[5] == 0 ||    // гориз средняя линия
                        u_p[6] == 0 && u_p[7] == 0 && u_p[8] == 0 ||    // гориз нижняя линия

                        u_p[0] == 0 && u_p[3] == 0 && u_p[6] == 0 ||    // вертик левая линия
                        u_p[1] == 0 && u_p[4] == 0 && u_p[7] == 0 ||    // вертик средняя линия
                        u_p[2] == 0 && u_p[5] == 0 && u_p[8] == 0 ||    // вертик правая линия 

                        u_p[0] == 0 && u_p[4] == 0 && u_p[8] == 0 ||    // диагональ 1
                        u_p[2] == 0 && u_p[4] == 0 && u_p[6] == 0)      // диагональ 2
                    {
                        int res = MessageBox(hWnd, TEXT("Крестики победили\nНачать сначала?"), TEXT("КОНЕЦ ИГРЫ"), MB_YESNO);
                        if (res == IDYES)
                        {
                            for (int i = 0; i < 9; i++)
                            {
                                SendMessage(hBut[i], BM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(hBmp[2]));   // картинки очищаются
                                EnableWindow(hBut[i], true);     // клавиши становятся активными 
                                u_p[i] = 3;                                 // номер картинки меняется на 3ки

                            }
                        }
                        else
                        {
                            KillTimer(hWnd, 1);                     // уничтожаем таймер 
                            EndDialog(hWnd, 0);                 // закрываем модальный диалог
                            return TRUE;
                        }
                    }

                    else if (u_p[0] == 1 && u_p[1] == 1 && u_p[2] == 1 ||   // гориз верхняя линия
                        u_p[3] == 1 && u_p[4] == 1 && u_p[5] == 1 ||        // гориз средняя линия
                        u_p[6] == 1 && u_p[7] == 1 && u_p[8] == 1 ||        // гориз нижняя линия

                        u_p[0] == 1 && u_p[3] == 1 && u_p[6] == 1 ||        // вертик левая линия
                        u_p[1] == 1 && u_p[4] == 1 && u_p[7] == 1 ||        // вертик средняя линия
                        u_p[2] == 1 && u_p[5] == 1 && u_p[8] == 1 ||        // вертик правая линия 

                        u_p[0] == 1 && u_p[4] == 1 && u_p[8] == 1 ||        // диагональ 1
                        u_p[2] == 1 && u_p[4] == 1 && u_p[6] == 1)          // диагональ 2
                    {
                        int res = MessageBox(hWnd, TEXT("Нолики победили\nНачать сначала?"), TEXT("КОНЕЦ ИГРЫ"), MB_YESNO);
                        if (res == IDYES)
                        {
                            for (int i = 0; i < 9; i++)
                            {
                                SendMessage(hBut[i], BM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(hBmp[2]));
                                EnableWindow(hBut[i], true);
                                u_p[i] = 3;
                            }
                        }
                        else
                        {
                            KillTimer(hWnd, 1);
                            EndDialog(hWnd, 0); // закрываем модальный диалог
                            return TRUE;
                        }
                        break;
                    }

                    for (int i = 0; i < 9; i++)                             // проверка на ничью
                    {
                        if (u_p[i] == 3)
                        {
                            u_end++;
                            break;
                        }
                    }
                    if (u_end == 0) // если есть хоть одна тройка 
                    {
                        int res = MessageBox(hWnd, TEXT("Ничья\nНачать сначала?"), TEXT("КОНЕЦ ИГРЫ"), MB_YESNO);
                        if (res == IDYES)
                        {
                            for (int i = 0; i < 9; i++)
                            {
                                SendMessage(hBut[i], BM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(hBmp[2]));
                                EnableWindow(hBut[i], true);
                                u_p[i] = 3;
                            }
                        }
                        else
                        {
                            KillTimer(hWnd, 1);
                            EndDialog(hWnd, 0); // закрываем модальный диалог
                            return TRUE;
                        }
                    }
                }
            }
        }

    }
    break;
    case WM_CLOSE:
        EndDialog(hWnd, 0); // закрываем модальный диалог
        return TRUE;
    }
    return FALSE;
}