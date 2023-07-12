#include"nodo.h"
#include<eigen3/Eigen/Dense>
#include<vector>
#include<fstream>
#include<sstream>
#include<thread>
#include<string>
#include<chrono>
#include<climits>
#include<iostream>


class BallStarTree{
private:
    std::vector<Cancion*> Canciones;
    int LimMaximo;
    string lista;
    Node *root;
public:
    BallStarTree(string lista, int LimMaximo){
        this->lista= lista;
        this->LimMaximo = LimMaximo;
    }


    void PasarCanciones(){//funcion para cargar todas las canciones del .csv
        ifstream myFileStream(lista);
        string line, temp, coordenada, nombre;
        std::vector<string> titulos(13+1);
        if(!myFileStream.is_open()){
            cout<<"Problema al abrir archivo"<<endl;
            return;
        }
        getline(myFileStream, line);
        stringstream ss(line);
        for (int i=0; i<13+1; i++){
            getline(ss, temp, ',');
            titulos[i]=temp;
        }
        int id = 1;
        while (getline(myFileStream, line)){
            stringstream ss(line);
            Eigen::VectorXf point(13);
            for (int i=0 ; i<13 ; i++){
                getline(ss, coordenada, ',');
                point[i] = stof(coordenada);
            }
            getline(ss, nombre);
            Cancion *cancion = new Cancion(id, nombre, point);
            Canciones.push_back(cancion);
            id++;
        }
        myFileStream.close();
    }


    void Indexar(){
        Ordenar_canciones(Canciones);//pca, eigenvector y ordnar dependiendo del Hyperplano
        root = new Node(LimMaximo, Canciones);//nodo root
        root -> Construccion_recursiva();//recurccion del resto del arbol
    }
    

vector<string> KNN(int id, int k){
    Eigen::VectorXf centro;
    k=k+1;
    for (Cancion *temp: Canciones){//para seleccionar el centro como punto de la cancion con el id recibido 
        if (temp->id == id){
            centro = temp->punto;
        }
    }
    float radio = std::numeric_limits<float>::max();;
    priority_queue<vecino> vecinos;
    root->ConsultaKNN(vecinos, k, centro, radio);
    vector<string> knns;
    while (!vecinos.empty()) {
        knns.push_back(vecinos.top().cancion->nombre);
        vecinos.pop();
    }
    return knns;
}

};  