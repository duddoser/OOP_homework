#include <bits/stdc++.h>

using namespace std;


class Vector
{
    private:

    vector<double> coord = {0, 0, 0};

    public:
    Vector(){
    }

    Vector(int x, int y, int z) {
        coord[0] = x;
        coord[1] = y;
        coord[2] = z;
    }

    vector<double> getCoord(){
        return coord;
    }

    void setCoord(int x, int y, int z){
        coord[0] = x;
        coord[1] = y;
        coord[2] = z;
    }

    Vector operator+(Vector v)
    {
        Vector new_v;
        new_v.setCoord(coord[0] + v.getCoord()[0], coord[1] + v.getCoord()[1], coord[2] + v.getCoord()[2]);
        return new_v;
    }

    void operator*(double a){ // добавить оператор []
        coord[0] *= a;
        coord[1] *= a;
        coord[2] *= a;
    }

    double makeScalarProduct(Vector v){
        return v.getCoord()[0] * coord[0] + v.getCoord()[1] * coord[1] + v.getCoord()[2] * coord[2];
    }

    void print()
    {
        cout<< "(" << coord[0] << ", " << coord[1] << ", " << coord[2] << ")" << endl;
    }

    friend void operator*(double a, Vector v);
};

void operator*(double a, Vector v) {
    v * a;
}


int main() {
    Vector v1(1, 2, 6);
    Vector v2(3, 4, 9);
    Vector vec_sum = v1 + v2;
    vec_sum.print();
    vec_sum * 8;
    cout << vec_sum.makeScalarProduct(v1) << endl;
    vec_sum.print();
    2.2 * vec_sum;
    vec_sum.print();
    return 0;
}