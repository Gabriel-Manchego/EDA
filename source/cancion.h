#include <eigen3/Eigen/Dense>
#include <string>

using namespace std;

struct Cancion{
    int id;
    string nombre;
    Eigen::VectorXf punto;
    float proyFactor;

    Cancion(int id, string nombre, Eigen::VectorXf punto){
        this -> id = id;
        this -> nombre = nombre;
        this -> punto = punto;
    }
    float DistanciaC(Eigen::VectorXf &elotro) const {
        return (punto - elotro).norm();
    }
    
};
