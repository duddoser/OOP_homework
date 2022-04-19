// LinkedListInherit21.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
using namespace std;

#include <iostream>

template<class T>
class Element
{
    //protected:
public:
    //переместить в protected
    Element* next;
    Element* prev;
    T info;

    Element(T data)
    {
        next = prev = NULL;
        info = data;
    }

    Element(Element* Next, Element* Prev, T data)
    {
        next = Next;
        prev = Prev;
        info = data;
    }

    Element(const Element& el)
    {
        next = el.next;
        prev = el.prev;
        info = el.info;
    }

    template<class T1>
    friend ostream& operator<<(ostream& s, Element<T1>& el);

};

template<class T1>
ostream& operator<<(ostream& s, Element<T1>& el)
{
    s << el.info;
    return s;
}

template<class T>
class LinkedList
{
    //protected:
public:
    //переместить в protected
    Element<T>* head;
    Element<T>* tail;
    int count;

    LinkedList()
    {
        head = tail = NULL;
        count = 0;
    }

    //удалить первый/последний элемент и вернуть удаленное значение
    virtual Element<T>* pop() = 0;
    //добавить элемент в список
    virtual Element<T>* push(T value) = 0;
    //получить первый/последний элемент списка
    //virtual Element<T>* front(T value) = 0;
    //virtual Element<T>* back(T value) = 0;

    //доступ по индексу
    //virtual Element<T>& operator[](int index) = 0;

    template<class T1>
    friend ostream& operator<<(ostream& s, LinkedList<T1>& el);

    //доработать деструктор
    virtual ~LinkedList() {
        Element<T>* p = head;
        Element<T>* q = head;
        for(; p != NULL; p = p->next) {
            q = p->next;
            delete p;
        }

    }
};

template<class T1>
ostream& operator<<(ostream& s, LinkedList<T1>& el){
    s << "(";
    Element<T1>* p = el.head;
    while (p != NULL ) {
        s << p->info << ", ";
        p = p -> next;
    }
    s << ")";
    return s;
}

template<class T>
class Stack : public LinkedList<T> {
public:
        Stack() : LinkedList<T>() {}

        virtual Element<T>* pop() {

            Element<T>* p = LinkedList<T>::head;
            while (p->next != LinkedList<T>::tail) {
                p = p -> next;
            }
            Element<T>* result = LinkedList<T>::tail;

            p -> next = NULL;
            LinkedList<T>::tail = p;
            LinkedList<T>::count--;

            return result;
        }

        virtual Element<T>* push(T value) {
            if (LinkedList<T>::tail == NULL) {
                LinkedList<T>::tail = new Element<T>(value);
                LinkedList<T>::head = LinkedList<T>::tail;
            } else {
                Element<T>* newElem = new Element<T>(value);
                LinkedList<T>::tail -> next = newElem;
                LinkedList<T>::tail = LinkedList<T>::tail->next;
            }
            LinkedList<T>::count++;
            return LinkedList<T>::tail;
        }

    };

int main() {

    return 0;
}
