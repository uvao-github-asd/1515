#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define SIZE  4
#define NUMBS SIZE*SIZE
#define UP    0
#define DOWN  1
#define LEFT  2
#define RIGHT 3

int board[SIZE][SIZE];

void FillBoard(void);
int  HaveSolution(void);
int  EndOfGame(void);
void PrintBoard(void);
void Move(int);


int main(void)
{
    HANDLE cInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD dr;
    INPUT_RECORD  rec;
    //init
    system("title 15 P U Z Z L E");
    FillBoard();
    PrintBoard();
    //
    while (!EndOfGame())
    {

        //Sleep(50);
        ReadConsoleInput(cInput, &rec, sizeof(INPUT_RECORD), &dr);
        FlushConsoleInputBuffer(cInput);
        if (rec.EventType == KEY_EVENT && rec.Event.KeyEvent.bKeyDown)
        {
            switch (rec.Event.KeyEvent.wVirtualKeyCode)
            {
            case VK_LEFT:
                Move(LEFT);
                break;
            case VK_RIGHT:
                Move(RIGHT);
                break;
            case VK_UP:
                Move(UP);
                break;
            case VK_DOWN:
                Move(DOWN);
                break;
            }
            PrintBoard();
        }
    }
    return 0;
}
/*Заполняем доску пока не найдем решаемую комбинацию

*/
void FillBoard(void)
{
    do
    {
        srand(time(NULL));
        int numbers[NUMBS] = { 0 };
        int i;
        int nextNum;
        for (i = 0; i < NUMBS - 1; i++)
            while (1)
            {
                nextNum = rand() % (NUMBS - 1) + 1;
                if (numbers[nextNum] == 0)
                {
                    numbers[nextNum] = 1;
                    board[i / SIZE][i % SIZE] = nextNum;
                    break;
                }
            }
    } while (!HaveSolution());
}
/*Проверка присутствия решения в доске
то есть сумма числа пар костяшек, в которых костяшка с большим номером
идёт перед костяшкой с меньшим номером, и номера ряда пустой клетки)
нечётна, то решения головоломки не существует
*/
int HaveSolution(void)
{
    int i, j;
    int inv = 0;
    for (i = 0; i < NUMBS; i++)
    {
        if (board[i / SIZE][i % SIZE] == 0)
            inv += i + 1;
        for (j = 0; j < i; j++)
            if (board[j / SIZE][j % SIZE] > board[i / SIZE][i % SIZE])
                inv++;
    }
    return (inv % 2 ? 1 : 0);
}
/*Проверка собранности доски
Проходим по всем позициям доски в цикле и сравниваем число в клетке с ожидаемым
Если не совпало - возвращаем ноль
Если все в порядке - доска решена, возвращаем 1
*/
int EndOfGame(void)
{
    int i;
    for (i = 0; i < NUMBS - 1; i++)
        if (board[i / SIZE][i % SIZE] != i + 1)
            return 0;
    return 1;
}

void PrintBoard(void)
{
    // system("cls");
    // system("color 0F");
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);//Для настройки вывода текста с другим цветом
    COORD cPos;
    unsigned char BL = 219; //заполненный прямоугольник
    SetConsoleTextAttribute(hCon, BACKGROUND_RED | BACKGROUND_RED | BACKGROUND_INTENSITY);//
    printf("15 15 ! 15 15");
    SetConsoleTextAttribute(hCon, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_RED | BACKGROUND_BLUE);//
    int i, j;
    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            cPos.X = j * 3;
            cPos.Y = i * 2 + 1;
            //maybe at five or maybe at six

            SetConsoleCursorPosition(hCon, cPos);//
            printf("%c%c%c%c", BL, BL, BL, BL);
            cPos.Y = i * 2 + 2;
            cPos.X = j * 3;
            SetConsoleCursorPosition(hCon, cPos);
            if (board[i][j])
                printf("%c%2i%c", BL, board[i][j], BL);
            else
                printf("%c  %c", BL, BL);
            cPos.Y = i * 2 + 3;
            cPos.X = j * 3;
            SetConsoleCursorPosition(hCon, cPos);
            printf("%c%c%c%c", BL, BL, BL, BL);
        }
    }
    SetConsoleTextAttribute(hCon, BACKGROUND_RED | BACKGROUND_RED | BACKGROUND_INTENSITY);
    printf("\ni  U  v  a  o\n");
}


void Move(int direction)
{
    int dy[] = { 1,-1, 0, 0 }; //векторы направления взависимости от 
    int dx[] = { 0, 0, 1,-1 }; //нажатой кнопки up down left rigth
    int i, j;
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
            if (board[i][j] == 0)
            {
                int newy = i + dy[direction];
                int newx = j + dx[direction];
                //Проверка на допустимость хода
                if (newy >= 0 && newy < SIZE && newx >= 0 && newx < SIZE)
                //Прошли проверку - меняем пустую ячейку с выбранной
                {
                    board[i][j] = board[newy][newx]; //в пустую ячейку ставим новое значение
                    board[newy][newx] = 0; // в ячейку выбранную ставим ноль
                }
                return;
            }
}