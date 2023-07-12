#include"cancion.h"
#include<eigen3/Eigen/Dense>
#include<vector>
#include<queue>
#include<thread>
#include<string>
#include<set>
#include<future>


using namespace std;


Eigen::VectorXf MayorEigenvector(const std::vector<Cancion*>& Canciones) {//Retorna el eigenvector mayor usando PCA 
    Eigen::MatrixXf matriz(Canciones.size(), 13);
    for (int i = 0; i < Canciones.size(); i++) {//pasar los puntos de vector de Canciones
        matriz.row(i) = Canciones[i]->punto.transpose();
    }

    Eigen::VectorXf matriz_media = matriz.colwise().mean();//determina la matriz media 
    matriz.rowwise() -= matriz_media.transpose();
    Eigen::MatrixXf matriz_covarianza = (matriz.transpose() * matriz) / (matriz.rows() - 1);//determin la matriz de covarianza

    Eigen::EigenSolver<Eigen::MatrixXf> solver(matriz_covarianza);
    Eigen::MatrixXf eigenvectors = solver.eigenvectors().real();
    return eigenvectors.col(0);//deveria retornar el mayor eigenvector eigenvalue
}


bool Comparar(Cancion* a, Cancion* b) {
    return a->proyFactor < b->proyFactor;
}

void Ordenar_canciones(std::vector<Cancion*>& Canciones) {//ordena las canciones dependieno de su valor de proyeccion
    Eigen::VectorXf eigenvect = MayorEigenvector(Canciones);
    for (auto Cancion : Canciones) {//determina el valor de proyecion para cada cancion
        Cancion->proyFactor = eigenvect.dot(Cancion->punto) / eigenvect.squaredNorm();
    }
    std::sort(Canciones.begin(), Canciones.end(), Comparar);//ordena las canciones dependiendo de valor de proyeccion
}

struct vecino{
    Cancion *cancion;
    float distancia; 
    vecino(Cancion *cancion, float distancia){
        this->cancion = cancion;
        this->distancia = distancia;
    }
    bool operator<(const vecino &elotro) const{
        return distancia < elotro.distancia;
    }
};

struct Node{
    int max_canciones;
    vector<Cancion*> canciones;
    bool es_hoja;
    Node *izq, *der;
    Eigen::VectorXf centro;
    float radio;
    
    Node(int max_canciones, vector<Cancion*> &canciones){
        this->max_canciones = max_canciones;
        this->canciones = canciones;
        if (canciones.size() > max_canciones){
            this->es_hoja = false;
        }
        else {
            this->es_hoja = true;}

        centro = Eigen::VectorXf::Zero(13);
        for (Cancion *temp: canciones){
            centro = centro + temp -> punto;
        }
        centro = centro/canciones.size();
        radio = 0.0;
        for (Cancion *temp: canciones){
            float dist = temp -> DistanciaC(centro);
            if (dist > radio) {
                radio = dist;
            }
        }
    }


    void Construccion_recursiva() {//Funcion para indexar el arbol despues del root
        int end = canciones.size();
        int medio = end / 2;

        if (es_hoja) {
            return;
        }
        vector<Cancion*> IzqC(canciones.begin(), canciones.begin() + medio);
        vector<Cancion*> DerC(canciones.begin() + medio, canciones.begin() + end);

        auto PCAizq = std::async(std::launch::async, [&]() {
            Ordenar_canciones(IzqC);
        });
        auto PCAder = std::async(std::launch::async, [&]() {
            Ordenar_canciones(DerC);
        });
        PCAizq.wait();
        PCAder.wait();
        izq = new Node(max_canciones, IzqC);
        der = new Node(max_canciones, DerC);
        auto LamdaIzq = std::async(std::launch::async, [&]() {
            izq->Construccion_recursiva();
        });
        auto LamdaDer = std::async(std::launch::async, [&]() {
            der->Construccion_recursiva();
        });
        LamdaIzq.wait();
        LamdaDer.wait();
    }

    float DistanciaF(Eigen::VectorXf &punto){
            return (centro-punto).norm()-radio;
        }
    void ConsultaKNN(priority_queue<vecino>& vecinos, int k,  Eigen::VectorXf& cent, float& rad) {
        if (DistanciaF(cent) > rad){
            return;
        }
        else if (es_hoja) {
            float dist, max_dist;
            for (Cancion* temp : canciones) {
                dist = temp -> DistanciaC(cent);
                    if (dist <= rad) {
                    vecinos.push(vecino(temp, dist));
                    if (vecinos.size() > k) {
                        vecinos.pop();
                        max_dist = vecinos.top().distancia;
                        rad = max_dist;
                    }
                }
            }
        }
        else {
            if (izq->DistanciaF(cent) <= rad){
                izq->ConsultaKNN(vecinos, k, cent, rad);
            }
            if (der->DistanciaF(cent) <= rad){
                der->ConsultaKNN(vecinos, k, cent, rad);
            }
        }
    }
};









