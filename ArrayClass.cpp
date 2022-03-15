#include <iostream>
#include <cmath>

using namespace std;

class MyArrayParent {
protected:
    //сколько памяти выделено?
    int capacity;
    //количество элементов - сколько памяти используем
    int count;
    //массив
    double *ptr;
    const int maximum = 1000;
public:
    //конструкторы и деструктор
    MyArrayParent(int Dimension = 100) {
        cout << "\nMyArrayParent constructor";
        ptr = new double[Dimension];
        capacity = Dimension;
        count = 0;
    }

    //конструктор принимает существующий массив
    MyArrayParent(double *arr, int len) {
//        cout << "\nMyArrayParent constructor";
        capacity = (len > maximum) ? 2 * len : maximum;
        count = len;
        ptr = new double[capacity];
        for (int i = 0; i < len; ++i) {
            ptr[i] = arr[i];
        }
    }

    MyArrayParent(const MyArrayParent &P) {
        capacity = P.capacity;
        count = P.count;
        ptr = new double[capacity];
        for (int i = 0; i < count; i++) {
            ptr[i] = P.ptr[i];
        }
    }


    //деструктор
    ~MyArrayParent() {
        cout << "\nMyArrayParent destructor";
        if (ptr != NULL) {
            delete[] ptr;
            ptr = NULL;
        }
    }

    //обращение к полям
    int Capacity() { return capacity; }

    int Size() { return count; }

    double GetComponent(int index) {
        if (index >= 0 && index < count)
            return ptr[index];
        //сгенерировать исключение, если индекс неправильный
        return -1;
    }

    void SetComponent(int index, double value) {
        if (index >= 0 && index < count)
            ptr[index] = value;
        //сгенерировать исключение, если индекс неправильный
    }

    //добавление в конец нового значения
    void push(double value) {
        if (count < capacity) {
            ptr[count++] = value;
        }
    }

    //удаление элемента с конца
    void RemoveLastValue() {
        if (count > 0) {
            ptr[--count] = 0;
        }
        //что делаем, если пуст?
    }

    double &operator[](int index) {
        //перегрузка оператора []
    }

    MyArrayParent &operator=(const MyArrayParent &P) {
        if (capacity == P.capacity) {
            count = P.count;
            for (int i = 0; i < count; ++i) {
                ptr[i] = P.ptr[i];
            }
        }

        return *this;
    }

    void print() {
        cout << "\nMyArrParent, size: " << count << ", values: {";
        int i = 0;
        for (i = 0; i < count; i++) {
            cout << ptr[i];
            if (i != count - 1)
                cout << ", ";
        }
        cout << "}";
    }


};

class MyArrayChild : public MyArrayParent {
public:
    //используем конструктор родителя. Нужно ли что-то ещё?
    MyArrayChild(int Dimension = 100) : MyArrayParent(Dimension) { cout << "\nMyArrayChild constructor"; }

    MyArrayChild(double *arr, int len) : MyArrayParent(arr, len) { cout << "\nMyArrayChild constructor"; }

    ~MyArrayChild() { cout << "\nMyArrayChild destructor\n"; }

    //удаление элемента
    //void RemoveAt(int index = -1);

//    поиск элемента
    int IndexOf(double value, bool bFindFromStart = true) {
        if (bFindFromStart) {
            for (int i = 0; i < count; ++i) {
                if (ptr[i] == value) {
                    return i;
                }
            }
            return -1;
        } else {
            for (int i = count - 1; i >= 0; i--) {
                if (ptr[i] == value) {
                    return i;
                }
            }
            return -1;
        }
    }

    //вставка элемента
    void InsertAt(double value, int index = -1) {
        if (index < 0 || index > count) {
            push(value);
            return;
        }

        if (count < capacity) {
            for (int i = count; i >= index; i--) {
                ptr[i] = ptr[i - 1];
            }
            count++;
            ptr[index] = value;
        }
    }

    //выделение подпоследовательности
    //MyArrayChild SubSequence(int StartIndex = 0, int Length = -1)

    //добавление элемента в конец
    //operator + ?

};

class MySortedArray: public MyArrayChild{

protected:
    int BinSearch(double value, int left, int right){
        int middle = (left + right) / 2;
        if (fabs(ptr[middle] - value) < 0.00000001){
            return middle;
        }

        if (right - left <= 1){
            if (ptr[right] == value)
                return right;
            return -1;
        }

        if (ptr[middle] > value){
            return BinSearch(value, left, middle - 1);
        } else {
            return BinSearch(value, middle + 1, right);
        }
    }

    int BinSearch2(double value, int left, int right){
        int middle = (left + right) / 2;
        if (fabs(ptr[middle] - value) < 0.00000001){
            return middle;
        }

        if (right - left <= 1){
            if (ptr[right] == value)
                return right;
            if (ptr[left] > value){
                return left;
            }
            return right;
        }

        if (ptr[middle] > value){
            return BinSearch(value, left, middle - 1);
        } else {
            return BinSearch(value, middle + 1, right);
        }
    }

public:
    MySortedArray(int capacity = 1000): MyArrayChild(Capacity()) {}
    ~MySortedArray() {}

    int IndexOf(double value, bool bFindFromStart = true){
        return BinSearch(value, 0, count - 1); // тут может быть рассхождение
    }
};

int main() {
    //MyArrayParent arr;
    if (true) {
//        MyArrayChild arr;
//        int i = 0;
//        arr.RemoveLastValue();
//        for (i = 0; i < 10; i++) {
//            arr.push(i + 1);
//        }
//        arr.RemoveLastValue();
//        arr.InsertAt(8, 3);
//        arr.print();

        MySortedArray s_arr;
        for (int i = 0; i < 10; i++) {
            s_arr.push(i + 4);
        }

        cout << "bin_searh" << endl;
        cout << s_arr.IndexOf(5);

        cout << "\n";
        //cout << "\n" << sp << "\n";
    }
    char c;
    cin >> c;
    return 0;
}
