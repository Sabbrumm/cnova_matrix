#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>


#define KEY_ESC 27
#define KEY_CONTROL 224
#define KEY_BACKSPACE 8
#define KEY_ENTER 13
#define KEY_CONTROL_DOWN 80
#define KEY_CONTROL_UP 72

#define KEY_R_SMALL 114
#define KEY_R_BIG 82


#define KEY_N_SMALL 110
#define KEY_N_BIG 78
#define KEY_MINUS 45

#define KEY_CONTROL_RIGHT 77
#define KEY_CONTROL_LEFT 75

#define KEY_0 48
#define KEY_9 57

int ExitVar = 0;

int screen = 0;
int point = 1;

int Det = 0;
int lasterror = 0;

// 0 - нет ошибок
// 1 - матрица не задана нельзя сделать действие
// 2 - матрица не квадратная, нельзя сделать действие
// 3- неверные измерения, умножения не будет

#define ERR_INIT 1
#define ERR_SQUARE 2
#define ERR_DIM 3


int matrix1[100] = {0};
int matrix2[100] = {0};
int matrixres[100] = {0};
int Width1 = 0;
int Height1 = 0;
int Width2 = 0;
int Height2 = 0;
int WidthRes = 0;
int HeightRes = 0;

//0  1  2  3  4
//5  6  7  8  9
//10 11 12 13 14

int getvalue_ij(int i, int j, int mwid, int mhet, int* matrix){
    int neededvalue = j*mwid+i;
    if (mwid*mhet > neededvalue){
        return matrix[neededvalue];
    }
    return -11;
}
void setvalue_ij(int i, int j, int mwid, int mhet, int* matrix, int value){
    int neededvalue = j*mwid+i;
    if (mhet*mwid > neededvalue){
        matrix[neededvalue] = value;
    }
}

void erasematrix(int* matrix){
    for (int i = 0; i<100; i++){
        matrix[i] = 0;
    }
}
void fillrandommatrix(int* matrix){
    int r = 0;
    for (int i = 0; i<100; i++){
        r = rand()%22;
        if (r==21){
            r=0;
        }
        else if (r>10){
            r-=21;
        }

        matrix[i] = r;
    }

}


void remake_matrix(int i1, int j1, int i2, int j2, int*matrix){
    int buffer1[10][10] = {{0}};
    for (int i = 0; i<i1; i++){
        for (int j = 0; j<j1; j++){
            buffer1[i][j] = matrix[j*j1+i];
        }
    }
    for (int i = 0; i<i2; i++){
        for (int j = 0; j<j2; j++){
            matrix[j*j2+i] = buffer1[i][j];
        }
    }
    for (int i = i2; i<10; i++){
        for (int j = j2; j<10; j++){
            matrix[j*j2+i] = 0;
        }
    }
}

void Transpon(int *matrix, int *Width, int *Height){
    int buf = *Width;
    int buffer1[10][10] = {{0}};
    for (int i = 0; i<*Width; i++){
        for (int j = 0; j<*Height; j++){
            buffer1[i][j] = matrix[j* *Width+i];
        }
    }


    for (int i = 0; i<*Width; i++){
        for (int j = 0; j<*Height; j++){
            matrix[i**Height+j]=buffer1[i][j];
        }
    }
    *Width=*Height;
    *Height=buf;
}

void Minor(int *matrix, int im, int jm, int width, int height, int *output){
    int offi = 0;
    int offj = 0;

    for (int i = 0; i<width-1; i++){
        if (i == im){
            offi = 1;
        }
        offj = 0;
        for (int j = 0; j<height-1; j++){
            if (j==jm){
                offj=1;
            }
            output[j*(width-1)+i] = matrix[(j+offj)*width+i+offi];
        }
    }
}

int Determinator(int* matrix, int size){
    int det = 0;
    int degree = 1; //инициализация, степень по умолчанию


    if (size ==1){
        return matrix[0];
    }
    else if (size==2){
        return matrix[0]*matrix[3]-matrix[1]*matrix[2];
    }
    else{
        int* bufmatrix = malloc((size-1)*(size-1)*sizeof(int)); //выделили место под буфер
        for (int j=0; j<size; j++){
            Minor(matrix,0, j, size, size, bufmatrix);
            det=det+(degree*matrix[j*size])*Determinator(bufmatrix, size-1);
            degree=degree*(-1);
        }
        free(&bufmatrix[0]); //удалили буфер
    }
    return det;
}

void swap_matrix(int *leftm, int* width1, int* height1, int *rightm, int* width2, int* height2) {
    int buff[100] = {0};
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            buff[j*10+i] = leftm[j*10+i];
        }
    }
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            leftm[j*10+i] = rightm[j*10+i];
        }
    }
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            rightm[j*10+i] = buff[j*10+i];
        }
    }
    int bufw=*width1, bufh=*height1;

    *width1=*width2, *height1=*height2;
    *width2=bufw; *height2=bufh;



}
void BackwardsMatrix(int *matrix, int size, int *result, int*widthres, int*heightres){
    int det = Determinator(matrix, size);
    *widthres = size;
    *heightres = size;
    Transpon(matrix, &size, &size);

    if (det!=0){
        for (int i =0; i<size*size; i++){
            result[i] = (int)(
                        (double)matrix[i]*
                        ( (double)1 /(double)det )
                    );
        }
    }
}

void PlusMatrix(int* m1, int* m2, int width, int height, int* result, int*widthres, int*heightres){
    for (int i=0;i<width;i++){
        for (int j =0; j<height; j++){
            result[j*height+i]=m1[j*height+i]+m2[j*height+i];
        }
    }
    *widthres = width;
    *heightres = height;
}
void TimesConst(int* leftm, int width1, int height1,
                 int constant,
                 int* result, int*reswidth, int*resheight){
    int buff[100] = {0};
    for (int i =0; i<width1; i++){
        for (int j=0; j<height1;j++){
            buff[j* width1+i] += leftm[j*width1+i] * constant;
        }
    }
    *reswidth = width1;
    *resheight = height1;
    for (int i =0; i<width1; i++){
        for (int j =0; j<height1; j++){
            result[j*width1+i] = buff[j*width1+i];
        }
    }
}

void TimesMatrix(int* leftm, int width1, int height1,
                 int *rightm, int width2, int height2,
                 int* result, int*reswidth, int*resheight){
    int buff[100] = {0};
    for (int in =0; in<width2; in++){
        for (int jn=0; jn<height1;jn++){

            for (int k =0; k<width1; k++)
            buff[jn* width1+in] += leftm[k*height2+in] * rightm[jn* width1+k];
        }
    }
    *reswidth = width2;
    *resheight = height1;
    for (int i =0; i<width1; i++){
        for (int j =0; j<height1; j++){
            result[j*width1+i] = buff[j*width1+i];
        }
    }
}

void ConsoleColorBlank(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
}

void ConsoleColorChosen(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN);
}

void print_matrix(int chosenX, int chosenY, int mwid, int mhet, int *matrix){
    for (int j = 0; j<mhet; j++){
        if (j==0){
            for (int i = 0; i<mwid; i++){
                if (i==0){
                    printf("┏━━━━");
                }
                else if (i==mwid-1){
                    printf("┳━━━━┓");
                }
                else{
                    printf("┳━━━━");
                }
            }
            printf("\n");
            for (int i = 0; i<mwid; i++){



                printf("┃");

                if (i==chosenX && j==chosenY){
                    ConsoleColorChosen();
                }
                else {
                    ConsoleColorBlank();
                }
                printf("%4d", getvalue_ij(i, j,mwid, mhet, matrix));
                ConsoleColorBlank();

                if (i==mwid-1){
                    printf("┃");
                }
            }
        }

        else if (j==mhet-1){
            for (int i = 0; i<mwid; i++){
                if (i==0){
                    printf("┣━━━━");
                }
                else if (i==mwid-1){
                    printf("╋━━━━┫");
                }
                else{
                    printf("╋━━━━");
                }
            }
            printf("\n");
            for (int i = 0; i<mwid; i++){



                printf("┃");

                if (i==chosenX && j==chosenY){
                    ConsoleColorChosen();
                }
                else {
                    ConsoleColorBlank();
                }
                printf("%4d", getvalue_ij(i, j,mwid, mhet, matrix));
                ConsoleColorBlank();

                if (i==mwid-1){
                    printf("┃");
                }
            }
            printf("\n");
            for (int i = 0; i<mwid; i++){
                if (i==0){
                    printf("┗━━━━");
                }
                else if (i==mwid-1){
                    printf("┻━━━━┛");
                }
                else{
                    printf("┻━━━━");
                }
            }
        }
        else{
            for (int i = 0; i<mwid; i++){
                if (i==0){
                    printf("┣━━━━");
                }
                else if (i==mwid-1){
                    printf("╋━━━━┫");
                }
                else{
                    printf("╋━━━━");
                }
            }
            printf("\n");
            for (int i = 0; i<mwid; i++){



                printf("┃");

                if (i==chosenX && j==chosenY){
                    ConsoleColorChosen();
                }
                else {
                    ConsoleColorBlank();
                }
                printf("%4d", getvalue_ij(i, j,mwid, mhet, matrix));
                ConsoleColorBlank();

                if (i==mwid-1){
                    printf("┃");
                }
            }
        }
        printf("\n");
        ConsoleColorBlank();
    }

}

void arrow(int boo){
    if (boo){
        printf("<-- \n");
    }
    else{
        printf("\n");
    }
}

void Screen_Operations(){
    printf("================================================================================\n"
           "Матрицы C-NOVA\n"
           "\n"
           "1) Ввести матрицу A");
    arrow(point==1);
    printf("2) Ввести матрицу B");
    arrow(point==2);
    printf("3) Найти определитель матрицы A");
    arrow(point==3);
    printf("4) Сложить матрицы");
    arrow(point==4);
    printf("5) Умножить матрицу A на B");
    arrow(point==5);
    printf("6) Умножить матрицу B на A");
    arrow(point==6);
    printf("7) Транспонировать матрицу A");
    arrow(point==7);
    printf("8) Вычислить матрицу, обратную A");
    arrow(point==8);
    printf("9) Поменять матрицы местами");
    arrow(point==9);
    printf("\nМатрица A\n");

    if (Width1==0 || Height1==0){
        printf("Матрица не определена\n");
    }
    else
        print_matrix(11, 11, Width1, Height1, matrix1);

    printf("\nМатрица B\n");
    if (Width2==0 || Height2==0){
        printf("Матрица не определена\n");
    }
    else
        print_matrix(11, 11, Width2, Height2, matrix2);

    printf("================================================================================\n");
    printf("\nРезультат:\n");
    if (WidthRes==0 || HeightRes==0){
        printf("Матрица не определена\n");
    }
    else
        print_matrix(11, 11, WidthRes, HeightRes, matrixres);

    printf("\n================================================================================\n");
}

void Screen_Det(int det){
    printf("================================================================================\n");
    printf("\n");
    printf("Определитель матрицы A: %d", det);
    printf("\n");
    printf("================================================================================\n");
}

void Screen_error(){
    printf("================================================================================\n"
           "\n"
           "ОШИБКА!"
           "\n");
    if (lasterror == 0){
        printf("Странно, вроде ошибок нет!");
    }
    else if (lasterror==ERR_INIT){
        printf("Вы не задали, как минимум, одну из матриц");
    }
    else if (lasterror==ERR_SQUARE){
        printf("Матрица А не квадратная, невозможно произвести операцию");
    }
    else if (lasterror==ERR_DIM){
        printf("Неправильные размеры матриц, невозможно произвести операцию");
    }
    printf("\n\nДля выхода, нажмите ESC, для продолжения нажмите Enter\n"
           "\n"
           "================================================================================\n");
}


void UI(){
    if (screen==0){
        Screen_Operations();
    }
    if (screen==1){
        Screen_Det(Det);
    }
    if (screen==2){
        Screen_error();
    }
}

void cls(){
    system("cls");
    //HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    //COORD coord = {0, 0};
    //SetConsoleCursorPosition(hcon, coord);
    //for (int i = 0; i<40;i++){
    //    printf("                                                                                \n");
    //}
    //SetConsoleCursorPosition(hcon, coord);
}

void InputMatrix(int *matrix, int* Width, int* Height){
    int key = 0;
    int chosenX = 0;
    int chosenY = 0;
    int Error = 0;
    int prevwidth = *Width;
    int prevheight = *Height;


    point = 1;
    while (key != KEY_ENTER && key!= KEY_ESC){
        cls();
        printf("================================================================================\n"
               "\n"
               "Пожалуйста, введите длину и ширину матрицы!\n"
               "\n"
               "Обратите внимание, что значения длины и ширины варьируются от 1 до 10.\n"
               "\n"
               "Если вы поменяли значения длины или ширины, для существующей матрицы переформатируется.\n"
               "(Если поставили больше - заполнится нулями, если меньше - обрежется)\n"
               "\n"
               "Для продолжения нажмите Enter, для выхода нажмите ESC\n"
               "\n");
        printf("1) Ширина матрицы: %d", *Width);
        arrow(point==1);
        printf("2) Высота матрицы: %d", *Height);
        arrow(point==2);
        printf("\n================================================================================");


        if (Error){
            printf("\n\nОШИБКА! Неверные значения длины и ширины!\n"
                   "\n"
                   "================================================================================");
        }
        key = getch();
        if (key == KEY_CONTROL){
            key = getch();
            if (key == KEY_CONTROL_DOWN){
                if (screen==0){
                    point++;
                    if (point>2){
                        point = 1;
                    }
                }
            }
            if (key == KEY_CONTROL_UP){
                if (screen==0){
                    point--;
                    if (point<1){
                        point=2;
                    }
                }
            }
        }
        if (key >= KEY_0 && key <= KEY_9){
            if (point == 1){
                if (*Width*10+key-48<=10){
                    *Width=*Width*10+key-48;
                }
            }
            if (point == 2){
                if (*Height*10+key-48<=10){
                    *Height=*Height*10+key-48;
                }
            }
        }
        if (key == KEY_BACKSPACE){
            if (point == 1){
                *Width/=10;
            }
            if (point == 2){
                *Height/=10;
            }
        }
        if (key == KEY_ENTER){
            if (*Width <1 || *Height<1){
                Error = 1;
                key = 0;
            }
        }
    }
    if (key!=KEY_ESC){
        key = 0;
        if (prevheight!=*Height || prevwidth!=*Width){
            remake_matrix(prevwidth, prevheight, *Width, *Height, matrix);
            //printf("Прокнуло\n");
            //printf("%d, %d\n", prevwidth, prevheight);
        }
        while (key != KEY_ENTER && key!= KEY_ESC){
            cls();
            printf("================================================================================\n"
                   "\n"
                   "Введите матрицу, переключаясь между ячейками с помощью стрелок\n"
                   "\n"
                   "Для инверсии числа, нажмите -\n"
                   "Для рандомной генерации матрицы, нажмите R\n"
                   "Для обнуления матрицы, нажмите N\n"
                   "Для продолжения нажмите Enter или ESC\n"
                   "\n");
            print_matrix(chosenX, chosenY, *Width, *Height, matrix);
            printf("\n================================================================================\n");
            key = getch();

            if (key == KEY_CONTROL){
                key = getch();
                if (key == KEY_CONTROL_UP){
                    if (chosenY>0){
                        chosenY--;
                        continue;
                    }
                }
                if (key == KEY_CONTROL_DOWN){
                    if (chosenY<*Height-1){
                        chosenY++;
                        continue;
                    }
                }
                if (key == KEY_CONTROL_LEFT){
                    if (chosenX>0){
                        chosenX--;
                        continue;
                    }
                }
                if (key == KEY_CONTROL_RIGHT) {
                    if (chosenX < *Width - 1) {
                        chosenX++;
                        continue;
                    }
                }
            }
            if (key >= KEY_0 && key <= KEY_9){
                int value = getvalue_ij(chosenX, chosenY, *Width, *Height, matrix);
                if (value>=0){
                    if (value*10+key-48<=10) {
                        setvalue_ij(chosenX, chosenY, *Width, *Height, matrix, value*10+key-48);
                    }
                }
                else{
                    if (value*10+key-48>=-10) {
                        setvalue_ij(chosenX, chosenY, *Width, *Height, matrix, value*10-key+48);
                    }
                }
            }
            if (key == KEY_BACKSPACE){
                int value = getvalue_ij(chosenX, chosenY, *Width, *Height, matrix);
                setvalue_ij(chosenX, chosenY, *Width, *Height, matrix, value/10);
            }
            if (key == KEY_R_BIG || key == KEY_R_SMALL){
                fillrandommatrix(matrix);
            }
            if (key == KEY_MINUS){
                int value = getvalue_ij(chosenX, chosenY, *Width, *Height, matrix);
                setvalue_ij(chosenX, chosenY, *Width, *Height, matrix, -1*value);
            }
            if (key == KEY_N_BIG || key == KEY_N_SMALL){
                erasematrix(matrix);
            }

        }
    }
    else{
        remake_matrix(prevwidth, prevheight, *Width, *Height, matrix);
    }



    point = 1;
}


void Intercept(){
    int key = _getch();
    if (key == KEY_CONTROL){
        key = _getch();
        if (key == KEY_CONTROL_DOWN){
            if (screen==0){
                point++;
                if (point>9){
                    point = 1;
                }
            }
        }
        if (key == KEY_CONTROL_UP){
            if (screen==0){
                point--;
                if (point<1){
                    point=9;
                }
            }
        }
    }
    if (key == KEY_ENTER) {
        if (screen == 0) {
            if (point == 1) {
                InputMatrix(matrix1, &Width1, &Height1);
            }
            if (point == 2) {
                InputMatrix(matrix2, &Width2, &Height2);
            }
            if (point == 3) {
                if (Width1==Height1){
                    if (Width1!=0){
                        Det=Determinator(matrix1, Width1);
                        screen=1;
                    }
                    else{
                        lasterror=ERR_INIT;
                        screen=2;
                    }
                }
                else{
                    lasterror=ERR_SQUARE;
                    screen=2;
                }
            }
            if (point == 4) {
                if (Width1==Width2 && Height1==Height2){
                    if (Width1!=0 && Height1!=0){
                        PlusMatrix(matrix1, matrix2, Width1, Height1,
                                   matrixres, &WidthRes, &HeightRes);
                    }
                    else{
                        lasterror=ERR_INIT;
                        screen=2;
                    }
                }
                else{
                    lasterror=ERR_DIM;
                    screen=2;
                }
            }
            if (point == 5) {
                if (Width1==Height2){
                    if (Width1!=0 && Height1!=0 && Width2!=0){
                        TimesMatrix(matrix1, Width1, Height1,
                                    matrix2, Width2, Height2,
                                    matrixres, &WidthRes, &HeightRes);

                    }
                    else{
                        lasterror=ERR_INIT;
                        screen=2;
                    }
                }
                else if ((Width1==1 && Height1==1) ^ (Width2==1 && Height2==1)) {
                    if (Width1!=0 && Height1!=0 && Width2!=0){
                        if (Width1==1){
                            TimesConst(matrix2, Width2, Height2,
                                       matrix1[0],
                                       matrixres, &WidthRes, &HeightRes);
                        }
                        else {
                            TimesConst(matrix1, Width1, Height1,
                                       matrix2[0],
                                       matrixres, &WidthRes, &HeightRes);
                        }


                    }
                    else{
                        lasterror=ERR_INIT;
                        screen=2;
                    }
                }
                else{
                    lasterror=ERR_DIM;
                    screen=2;
                }
            }

            if (point == 6) {
                if (Width2==Height1){
                    if (Width1!=0 && Height1!=0 && Height2!=0){
                        TimesMatrix(
                                    matrix2, Width2, Height2,
                                    matrix1, Width1, Height1,
                                    matrixres, &WidthRes, &HeightRes);
                    }

                    else{
                        lasterror=ERR_INIT;
                        screen=2;
                    }
                }
                else if ((Width1==1 && Height1==1) ^ (Width2==1 && Height2==1)) {
                    if (Width1!=0 && Height1!=0 && Width2!=0){
                        if (Width1==1){
                            TimesConst(matrix2, Width2, Height2,
                                       matrix1[0],
                                       matrixres, &WidthRes, &HeightRes);
                        }
                        else {
                            TimesConst(matrix1, Width1, Height1,
                                       matrix2[0],
                                       matrixres, &WidthRes, &HeightRes);
                        }


                    }
                    else{
                        lasterror=ERR_INIT;
                        screen=2;
                    }
                }
                else{
                    lasterror=ERR_DIM;
                    screen=2;
                }
            }
            if (point == 7) {
                if (Width1 != 0 && Height1 != 0 )
                    Transpon(matrix1, &Width1, &Height1);
                else {
                    screen = 2;
                    lasterror = ERR_INIT;
                }
            }
            if (point == 8) {
                if (Width1==Height1){
                    if (Width1!=0){
                        BackwardsMatrix(matrix1, Width1, matrixres,
                                        &WidthRes, &HeightRes);
                    }
                    else{
                        lasterror=ERR_INIT;
                        screen=2;
                    }
                }
                else{
                    lasterror=ERR_SQUARE;
                    screen=2;
                }
            }
            if (point == 9) {
                if (Width1 != 0 && Width2 != 0 && Height1 != 0 && Height2 != 0)
                    swap_matrix(matrix1, &Width1, &Height1, matrix2, &Width2, &Height2);
                else {
                    screen = 2;
                    lasterror = ERR_INIT;
                }

            }

        }
        else if (screen == 2) {
            screen = 0;
        }
        else if (screen == 1) {
            screen = 0;
        }
    }
    if (key == KEY_ESC){
        if (screen==0){
            ExitVar = 1;
        }
        else if (screen==2){
            screen=0;
        }
        else if (screen == 1) {
            screen = 0;
        }
    }
}



int main() {
    srand(time(NULL));
    SetConsoleOutputCP(CP_UTF8);



    while (!ExitVar){
        cls();
        UI();
        Intercept();
    }
}
