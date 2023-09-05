// lab3D_5.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Kursach3D.h"

using namespace std;

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


struct POINTS_XYZ {
    double point[5] = { 0 };
};

POINTS_XYZ* Figure = NULL;
POINTS_XYZ* FigureRes = NULL;
int* v = NULL,* v_func = NULL;
int countArr = 0, sizeArr = 100, countArrRes = 0;
int sizeV = 0, countV = 0;
int sizeV_func = 0, countV_func = 0;
const int constSize = 100;
char buff[3000], res[10] = { 0 };
int countC = 0, pos = 2, n = 0, num_p = 0, arra = 0, count_v_func = 0;
double d = 0;
streampos pos_buff;
ifstream fcin("monkey.obj");
int fl_vid = 1, teta = 0, R = 300, fi = 0;
double x = 0, y = 0;
double k = 1, var = 1;
const double PI = 3.1415926535;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_KURSACH3D, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_KURSACH3D));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KURSACH3D));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_KURSACH3D);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//

void create(POINTS_XYZ*& arr) {
    arr = (POINTS_XYZ*)malloc(sizeArr * sizeof(POINTS_XYZ));
}

void add(POINTS_XYZ*& arr, int index, double num, int var, int& countArr, POINTS_XYZ*& arr1) {
    if (countArr >= sizeArr) {
        int swSize = sizeArr;
        sizeArr += constSize;
        POINTS_XYZ* swArr = arr;
        create(arr);
        create(arr1);
        for (int i = 0; i < swSize; i++) {
            for (int j = 0; j < 5; j++) {
                arr[i].point[j] = swArr[i].point[j];
            }
        }
        free(swArr);
    }
    arr[countArr].point[index] = num;
    if (var == 1) countArr++;

}

void createV(int*& v, int sizeV) {
    v = (int*)malloc(sizeV * sizeof(int));
}

void addPoligon(int*& v, int& sizeV, int& countV, int num) {
    if (countV >= sizeV) {
        int swSize = sizeV;
        int* swV = v;
        sizeV += 100;
        createV(v, sizeV);
        for (int i = 0; i < swSize; i++) {
            v[i] = swV[i];
        }
        free(swV);
    }
    v[countV] = num;
    countV++;

}

void Vid(POINTS_XYZ* matrix, double numO, double numF, const double PI, double R, POINTS_XYZ* mat) {
    double sum = 0;
    numO = numO * PI / 180;
    numF = numF * PI / 180;
    double arr[4] = { 0 };
    double swMatrix[4][4] = {
        {-sin(numO), -cos(numF) * cos(numO), -sin(numF) * cos(numO), 0},
        {cos(numO), -cos(numF) * sin(numO), -sin(numF) * sin(numO), 0},
        {0, sin(numF), -cos(numF), 0},
        {0, 0, R, 1},
    };
    for (int k = 0; k < countArr; k++) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                sum += matrix[k].point[j] * swMatrix[j][i];
            }
            arr[i] = sum;
            sum = 0;
        }
        for (int i = 0; i < 4; i++) {
            mat[k].point[i] = arr[i];
        }
    }
}

void Center(POINTS_XYZ* matrix, double R) {
    for (int i = 0; i < countArr; i++) {
        matrix[i].point[0] = matrix[i].point[0] / (1 + matrix[i].point[2] / 500);
        matrix[i].point[1] = matrix[i].point[1] / (1 + matrix[i].point[2] / 500);
    }
}

void Convert() {
    if (!fcin.is_open()) cout << "Error";
    else {
        while (!fcin.eof())
        {
            fcin.getline(buff, 3000);
            if (buff[0] == 'v' && buff[1] == ' ')
            {
                while (buff[pos] != '\0') {
                    if (buff[pos] != ' ') {
                        res[countC] = buff[pos];
                        countC++;
                    }
                    else {
                        countC = 0;
                        if (num_p == 0) add(Figure, 0, atof(res) * 50, 0, countArr, FigureRes);
                        if (num_p == 1) add(Figure, 1, atof(res) * 50, 0, countArr, FigureRes);
                        num_p++;
                        memset(res, 0, 10);
                    }
                    pos++;
                }
                if (num_p == 2) {
                    add(Figure, 2, atof(res)* 50, 0, countArr, FigureRes);
                    add(Figure, 3, 1, 1, countArr, FigureRes);
                    memset(res, 0, 10);
                }
                num_p = 0;
                countC = 0;
                pos = 2;
            }
            if (buff[0] == 'f' && buff[1] == ' ')
            {
                pos = 1;
                while (buff[pos] != '\0') {
                    if (buff[pos] == ' ') {
                        if (count_v_func == 1) {
                            addPoligon(v_func, sizeV_func, countV_func, 1);
                            count_v_func = 0;
                        }
                        else addPoligon(v_func, sizeV_func, countV_func, 0);
                        while (buff[pos] != '/') {
                            pos++;
                            res[countC] = buff[pos];
                            countC++;
                        }
                        countC = 0;
                        addPoligon(v, sizeV, countV, atof(res));
                        memset(res, 0, 10);
                    }
                    pos++;
                    if (buff[pos] == '\0') count_v_func = 1;
                }
            }
            memset(buff, 0, 200);
        }
    }
    fcin.close();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_KEYDOWN:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case VK_UP:
            y -= 100;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
            break;
        case VK_DOWN:
            y += 100;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
            break;
        case VK_LEFT:
            x -= 100;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
            break;
        case VK_RIGHT:
            x += 100;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
            break;
        case VK_NUMPAD4:
            teta += 5;
            fl_vid = 1;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
            break;
        case VK_NUMPAD6:
            teta -= 5;
            fl_vid = 1;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
            break;
        case VK_NUMPAD2:
            fi += 5;
            fl_vid = 1;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
            break;
        case VK_NUMPAD8:
            fi -= 5;
            fl_vid = 1;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
            break;
        case VK_NUMPAD7:
            R -= 20;
            fl_vid = 1;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
            break;
        case VK_NUMPAD9:
            R += 20;
            fl_vid = 1;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
            break;
        case VK_ESCAPE:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        if (arra == 0) {
            create(Figure);
            create(FigureRes);
            createV(v, sizeV);
            createV(v_func, sizeV_func);
            Convert();
            arra = 1;
        }

        if (fl_vid == 1) {
            Vid(Figure, teta, fi, PI, R, FigureRes);
            Center(FigureRes, R);
            fl_vid = 0;
        }
        
        for (int i = 0; i < countV; i++) {
            if (v_func[i] == 1 || i == 0) {
                count_v_func++;
                if (i != 0) LineTo(hdc, FigureRes[v[i - count_v_func] - 1].point[0] + x, FigureRes[v[i - count_v_func] - 1].point[1] + y);
                MoveToEx(hdc, FigureRes[v[i] - 1].point[0] + x, FigureRes[v[i] - 1].point[1] + y, NULL);
                count_v_func = 0;
            }
            else {
                LineTo(hdc, FigureRes[v[i] - 1].point[0] + x, FigureRes[v[i] - 1].point[1] + y);
                count_v_func++;
            }
        }

        ////LineTo(hdc, Square[0][0] + 200, Square[0][1] + 150);
        // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        free(v);
        free(v_func);
        free(Figure);
        free(FigureRes);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
