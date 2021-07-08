#include <iostream>
#include <fstream>

using namespace std;

const int n = 100;
int peak[n];

void delete_matrix(int**& a, int N) {
	for (int i = 0; i < N; i++)
		delete[] a[i];
	delete[]a;
}
bool gamilton(int v0, int** a,int N,int *visit,int *jump)
{
	int start = v0;
	int next = 0;
	peak[next++] = start;
	bool flag;
	while (true)
	{
		flag = false;
		visit[start] = 1;
		for (int i = jump[start] + 1; i < N; i++) {
			if ((i != start && (a[i][start] || a[start][i]) && visit[i]==0))           //идем только по 1 в таблице смежности,в 0 не заходим
			{
				
				flag = true;
				jump[start] = i;
				start = i;
				peak[next++] =start;
				break;
			}
		}
		if (!flag)
		{
			if (next == 1) 
				return false;
			jump[start] = -1;
			visit[start] = 0;
			start = peak[--next - 1];
		}
		else {
			if (next == N && (a[start][v0] || a[v0][start]))
				return true;
		}
	}
}
int Gam(int v0, int N, int** a, int *jump, int* visit) {              //проверить в цикле все вершины,пока не найдем гамильтонов цикл
	int K = 0;
	while (v0 < N && K != 1) {
		K = gamilton(v0,a,N,visit,jump);
		v0++;
	}
	return K;
}

int main()
{

	setlocale(0, "");
	//из файла загружаю матрицу
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
	///
	int visit[n];
	int  jump[n];
	for (int i = 0; i < N; i++)
	{
		visit[i] = 0;
		jump[i] = -1;
	}
	if (Gam(0, N, a, jump, visit))
	{
		cout << "Первый найденный Гамильтонов цикл: " << endl;
		for (int i = 0; i < N; i++)
			cout << peak[i]+1<< ' ';
		cout << peak[0]+1 << endl;
	
	}
	else
		cout << "Гамильтонова цикла в графе нет" << endl;
	delete_matrix(a, N);
	
	return 0;
}