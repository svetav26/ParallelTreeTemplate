#include <iostream>

#include <fstream>

#include "Header.h"
using namespace std;
//
const int n = 100;


void delete_matrix(int**& a, int N) {
    for (int i = 0; i < N; i++)
        delete[] a[i];
    delete[]a;
}

void pechat(int N,int *peak)
{
    for (int i = 0; i < N; i++) {
        cout << peak[i] + 1 << ' ';
    }
    cout << peak[0] + 1;
    cout << endl;
}

int gamilton(int k, int** a, int v0, int N,int *peak,int *c)
{
    peak[0] = v0;
    c[v0] = v0;
    int v=0;
    int flag = 0;

    
    for (v = 0; v < N && flag != 1; v++)
    {
        if ((a[v][peak[k - 1]] || a[peak[k - 1]][v]) != 0)  
        {

            if (k == N && v == v0) {

                flag = 1;

            }
            else if (c[v] == -100)
            {

                c[v] = k;
                peak[k] = v;
                flag = gamilton(k + 1, a, v0, N,peak,c);
                if (flag != 1)
                    c[v] = -100;
            }
            else continue;
        }
    }
 
    return flag;
}
int Gam(int v0, int N, int** a,int *peak,int *c) {
    int K = 0;
    while (v0 < N && K != 1) {
        K = gamilton(1, a, v0, N,peak,c);
        v0++;
    }
    return K;
}
int main()
{
    
    setlocale(0, "");
    int j;
    int N = 0;
    ifstream fin;
    fin.open("test.txt");
    fin >> N;
    int** a = new int* [N];
    for (int i = 0; i < N; i++)
    {
        a[i] = new int[N];
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            fin >> a[i][j];
            cout << a[i][j];
        }
        cout << endl;
    }
    fin.close();
    /////////////
    cout << "Первый найденный Гамильтонов цикл : " << endl; 
    int c[n], peak[n];
    for (j = 0; j < N; j++) {
        c[j] = -100;
    }
    if (Gam(0, N, a,peak,c)==1)
        pechat(N,peak);
    else 
        cout << "Гамильтонова цикла в графе нет" << endl;
    delete_matrix(a, N);

    return 0;
}

