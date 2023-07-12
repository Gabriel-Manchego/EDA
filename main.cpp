#include<iostream>
#include"source/ballstartree.h"

using namespace std;

int main()
{
  int id = 1;
  int k = 10;
  BallStarTree BALL("songs_final.csv", 400);
  cout<<"BallStar Tree"<<endl;
  BALL.PasarCanciones();
  auto start = chrono::steady_clock::now();
  BALL.Indexar();
  auto end = chrono::steady_clock::now();
  cout<<"Tiempo de indexado: "<<chrono::duration_cast<chrono::milliseconds>(end - start).count()<<" ms"<<endl;

  auto start2 = chrono::steady_clock::now();
  vector<string> vec= BALL.KNN(id, k);
  auto end2 = chrono::steady_clock::now();
  cout<<"Tiempo del knn: "<<chrono::duration_cast<chrono::milliseconds>(end2 - start2).count()<<" ms"<<endl;
  cout << "Para la cancion : "<< vec[k] << " los " << k << " vecinos mas cercanos son: "<<endl; 
  for (int j=vec.size()-2; j>=0; j--){ 
    cout << vec[j] << endl;
  }
}