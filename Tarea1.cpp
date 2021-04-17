#include <iostream>
#include <random>
#include <iomanip>
#include <chrono>

using namespace std;
void LlenarMatriz(int arr[][10]){
	random_device rd;  //Will be used to obtain a seed for the random number engine
	mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	uniform_real_distribution<> dis(1.0, 5.0);
	for (int i = 0 ; i < 10000; ++i) {
		for (int j = 0; j < 10; ++j) {
			arr[i][j] = dis(gen);
		}
	}
	cout<<"Llenado completo"<<endl;
}
void DistanciaEuclidiana(int arr[][10]) {
	
	for (int i = 0; i < 10000; ++i) {
		for (int j = i; j < 10000; ++j) {
			double flag = 0, len = 0;
			for (int k = 0; k < 10; ++k) {
				flag = flag + ((arr[i][k] - arr[j][k]) * (arr[i][k] - arr[j][k]));
			}
			len=sqrt(flag);
		}
	}
}
int main() {
	
	int arr[10000][10];
	LlenarMatriz(arr);
	chrono::time_point<chrono::high_resolution_clock> start, end;
	start = chrono::high_resolution_clock::now();
	
	DistanciaEuclidiana(arr);
	
	end = chrono::high_resolution_clock::now();
	
	int64_t duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count(); // duration is holding the elapsed time.
	cout << duration;
}
