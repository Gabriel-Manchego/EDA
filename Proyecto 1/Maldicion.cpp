#include <iostream>
#include <vector>
#include <random>
#include <fstream>


using namespace std;

float disteu(float punto1[], float punto2[], int ndim){
	float dist=0;
	for(int i=0;i<ndim;++i){
		dist = dist + pow(punto2[i] - punto1[i],2);
	}
	return sqrt(dist);
}

int main() {
	random_device rd;
	mt19937 gen (rd());
	uniform_real_distribution<> dis (0.0, 1.0);
	const int ndim = 5000;
	const int npts = 100; 
	
	float vec[npts][ndim];
	for (int i=0; i<npts  ;++i){
		for(int j=0 ; j<ndim; ++j){
			vec[i][j] = dis (gen);;	
		}
	}
	ofstream myfile ("example.txt");
	for(int i=0 ; i<npts-1 ; ++i){
		for(int j=i+1; j<npts;++j){
			myfile << disteu(vec[i],vec[j], ndim)<<endl;
		}
	}
	return 0;
}