#include <bits\stdc++.h>
using namespace std;

class Exception: public exception
{
protected:
    //сообщение об ошибке
    char* str;
public:
    Exception(const char* s)
    {
        str = new char[strlen(s) + 1];
        strcpy_s(str, strlen(s) + 1, s);
    }

    Exception(char* s)
    {
        str = new char[strlen(s) + 1];
        strcpy_s(str, strlen(s) + 1, s);
    }

    Exception()
    {
        str = NULL;
    }

    Exception(const Exception& e)
    {
        str = new char[strlen(e.str) + 1];
        strcpy_s(str, strlen(e.str) + 1, e.str);
    }

    ~Exception()
    {
        delete[] str;
    }

    //функцию вывода можно будет переопределить в производных классах, когда будет ясна конкретика
    virtual void print()
    {
        cout << "Exception: " << str<< "; " << what();
    }
};


class UnequalSizeException: public Exception {
protected:
    int rows1, rows2, cols1, cols2;

public:
    UnequalSizeException(char* s, int rows1, int cols1, int rows2, int cols2): Exception(s) {
        this->rows1 = rows1; this->rows2 = rows2; this->cols1 = cols1; this->cols2 = cols2;

    }

    UnequalSizeException(const char* s, int rows1, int cols1, int rows2, int cols2): Exception(s) {
        this->rows1 = rows1; this->rows2 = rows2; this->cols1 = cols1; this->cols2 = cols2;

    }

    UnequalSizeException(const UnequalSizeException& e) {
        str = new char[strlen(e.str) + 1];
        strcpy_s(str, strlen(e.str) + 1, e.str);
        rows1 = e.rows1; rows2 = e.rows2; cols1 = e.cols1; cols2 = e.cols2;
    }

    virtual void print()
    {
        cout << "UnequalSizeException: " << str << "; " << what();
    }
};

class IndexOutOfBoundException: public Exception {
protected:
    int row_index; int col_index;

public:
    IndexOutOfBoundException(char* s, int row_index, int col_index): Exception(s) {
        this->row_index = row_index; this->col_index = col_index;

    }

    IndexOutOfBoundException(const char* s, int row_index, int col_index): Exception(s) {
        this->row_index = row_index; this->col_index = col_index;
    }

    IndexOutOfBoundException(const IndexOutOfBoundException& e) {
        str = new char[strlen(e.str) + 1];
        strcpy_s(str, strlen(e.str) + 1, e.str);
        row_index = e.row_index; col_index = e.col_index;
    }

    virtual void print()
    {
        cout << "UnequalSizeException: " << str << "; " << what();
    }
};


class BaseMatrix
{
protected:
    double** ptr;
    int height;
    int width;
public:
    BaseMatrix(int Height = 2, int Width = 2)
    {
        //конструктор
        height = Height;
        width = Width;
        ptr = new double* [height];
        for (int i = 0; i < height; i++)
            ptr[i] = new double[width];
    }

    //конструктор копий
    BaseMatrix(const BaseMatrix& M) {
        height = M.height;
        width = M.width;
        ptr = new double* [height];
        for (int i = 0; i < height; i++) {
            ptr[i] = new double[width];
            for (int j = 0; j < width; ++j) {
                ptr[i][j] = M.ptr[i][j];
            }
        }
    }

    BaseMatrix& operator=(BaseMatrix const& M) {
        if (height == M.height && width == M.width) {
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; ++j) {
                    ptr[i][j] = M.ptr[i][j];
                }
            }
        } else {
            throw UnequalSizeException("Unequal size of matrixes during operator = ",
                                       height, width, M.height, M.width);
        }
        return *this;
    }

    double* operator[](int index) {
        if (index < 0 || index >= height)
            throw IndexOutOfBoundException("Wrond index in operator[] ", height, -1);
        return ptr[index];
    }

    double& operator()(int index1, int index2) {
        if (index1 < 0 || index1 >= height || index2 < 0 || index2 >= height)
            throw IndexOutOfBoundException("Wrong index in operator()", index1, index2);
        return ptr[index1][index2];
    }

    BaseMatrix operator+() { // унарный = транспонирование
        BaseMatrix res(width, height);
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                double t = ptr[i][j];
                res.ptr[i][j] = ptr[j][i];
                res.ptr[j][i] = t;
            }
        }
        return res;
    }

    BaseMatrix operator+(double d) {
        BaseMatrix res(width, height);
        if (height == width) {
            for (int i = 0; i < height; ++i) {
                ptr[i][i] += d;
            }
        } else {
            throw UnequalSizeException("The matrix is not rectangular", height, width, 0, 0);
        }
        return res;
    }

    BaseMatrix operator+(BaseMatrix M) {
        BaseMatrix res(width, height);
        if (M.height == height && M.width == width) {
            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    res.ptr[i][j] = ptr[i][j] + M.ptr[i][j];
                }
            }
        } else {
            throw UnequalSizeException("Unequal size of summary of two Matrixes",
                                       height, width, M.height, M.width);
        }
        return res;
    }

    ~BaseMatrix()
    {
        //деструктор
        if (ptr != NULL)
        {
            for (int i = 0; i < height; i++)
                delete[] ptr[i];
            delete[] ptr;
            ptr = NULL;
        }
    }

    void print()
    {
        //вывод
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
                cout << ptr[i][j] << " ";
            cout << "\n";
        }
    }

    friend ostream& operator<<(ostream& s, BaseMatrix M) ;
    friend istream& operator>>(istream& s, BaseMatrix& M) ;

};

ostream &operator<<(ostream &s, BaseMatrix M) {
    if (typeid(s) == typeid(ofstream)) {
        s << M.height << " " << M.width << " ";
        for (int i = 0; i < M.height; ++i) {
            for (int j = 0; j < M.width; ++j) {
                s << M.ptr[i][j] << " ";
            }
        }
    } else {
    for (int i = 0; i < M.height; ++i) {
        for (int j = 0; j < M.width; ++j) {
            s << M.ptr[i][j] << " ";
        }
        s << endl;
    }
    return s;
    }
}

istream &operator>>(istream &s, BaseMatrix& M) {
    if (typeid(s) == typeid(ofstream)) {
        int h, w; s >> h >> w;
        if (w != M.width || h != M.height) {
            throw UnequalSizeException("Matrixes of unequal sizes while reading file",
                                       h, w, M.height, M.width);
        }
    }
    for (int i = 0; i < M.height; ++i) {
        for (int j = 0; j < M.width; ++j) {
            s >> M.ptr[i][j];
        }
    }
    return s;
}

ostream &mymanip(istream&) {
    // do what u want
}


int main()
{
    try {
        BaseMatrix M;
        M(0, 0) = 1;
        M(0, 1) = 1;
        M(1, 0) = 9;
        M(1, 1) = 1;


        ofstream fout("test.txt");
        if (fout) {
            fout << M;
            fout.close();
        }

        cout << M;

        BaseMatrix M2;
        ifstream fin;
        fin.open("test");
        if (fin) {
            fin >> M2;
            fin.close();
        }
        cout << M2;



//        M(23, 69) = 89;

//        BaseMatrix M3 = M + M2;


        char c; cin >> c;
    } catch (UnequalSizeException e) {
        e.print();
    } catch (IndexOutOfBoundException e) {
        e.print();
    }

    return 0;
}
