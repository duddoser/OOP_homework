#include <bits/stdc++.h>
using namespace std;

ostream& my_manip(ostream& s) {
    s.setf(ios::hex);
    return s;
}

//Шаблон классов
template<class T>
class Element {
protected:
    Element* next;
    Element* prev;
    T info;

public:
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

    Element<T>* getNext() {
        return next;
    }

    void setNext(Element* Next) {
        next = Next;
    }

    Element<T>* getPrev() {
        return prev;
    }

    void setPrev(Element* Prev) {
        prev = Prev;
    }

    T getInfo() {
        return info;
    }

    void setInfo(T Info) {
        info = Info;
    }

    template<class T1>
    friend ostream& operator<<(ostream& s, Element<T1>& el);
};

template<class T1>
ostream& operator<<(ostream& s, Element<T1>& el)
{
    s << "\n";
    my_manip(s);
    s << el.info;
    return s;
}

//Класс Односвязный список
template<class T>
class LinkedList{
protected:
    Element<T>* head;
    Element<T>* tail;
    int count;

public:

    LinkedList() {
        cout << "\nLinkedList class constructor";
        head = tail = NULL;
        count = 0;
    }

    LinkedList(T* arr, int len) {
        Element<T>* current = new Element<T>(arr[0]);
        head = current;
        for (int i = 1; i < len; i++)
        {
            current = current->getNext();
            current = new Element<T>(arr[i]);
        }
        tail = current;
        count = len;
    }

    virtual ~LinkedList() {
        cout << "\nLinkedList class destructor";
        if (head != NULL)
        {
            Element<T>* current = head;
            Element<T>* temp = head->getNext();
            for (; current != tail; current = temp, temp = temp->getNext())
                delete current;
        }
        head = NULL; tail = NULL;
    }

    virtual Element<T>* operator[](int index) = 0;
    virtual Element<T>* push(T value) = 0;
    virtual Element<T>* pop() = 0;
    virtual Element<T>* insert(T value, Element<T>* predecessor = NULL) = 0;
    virtual Element<T>* find(T value) = 0;
    virtual Element<T>* find(T value, Element<T>* el) = 0;
    virtual Element<T>* remove(T value) = 0;
    virtual void filter(bool (*cmp)(T), LinkedList<T>* dest) = 0;
    virtual void filter(bool (*cmp)(T), LinkedList<T>* dest, Element<T>* cur) = 0;
    virtual void filter(T, LinkedList<T>* dest, bool more) = 0;
    virtual void filter(T, LinkedList<T>* dest, Element<T>* cur, bool more) = 0;

    bool isEmpty() { return (LinkedList<T>::count == 0);}

    virtual void save(string fileName) {
        ofstream fout(fileName);
        if (fout.is_open()) {
            fout << count << "\n";
            Element<T>* pom = head;
            T info;
            for (int i = 0; i < count; i++)
            {
                info = pom->getInfo();
                fout << info << "\n";
                pom = pom->getNext();
            }
            fout.close();
        }
    }

    virtual void load(string fileName) {
        ifstream fin;
        fin.open(fileName);
        if (fin.is_open()) {
            int ct;
            fin >> ct;

            if (ct != count) {
                count = 0;
                if (head != NULL)
                {
                    Element<T>* current = head;
                    Element<T>* temp = head->getNext();
                    for (; current != tail; current = temp, temp = temp->getNext())
                        delete current;
                }
                head = NULL; tail = NULL;
                T val;
                for (int i = 0; i < ct; i++)
                {
                    fin >> val;
                    push(val);
                }
            } else {
                Element<T>* pom = head;
                T val;
                for (int i = 0; i < count; i++)
                {
                    fin >> val;
                    pom->setInfo(val);
                    pom = pom->getNext();
                }
            }
            fin.close();
        }
    }

    template<class T1>
    friend ostream& operator<<(ostream& s, LinkedList<T1>& el);
    template <class T1>
    friend istream& operator>>(istream& s, LinkedList<T1>& el);
};

template<class T1>
ostream& operator<<(ostream& s, LinkedList<T1>& el) {
    Element<T1>* current;
    s << "\n{";
    for (current = el.head; current->getNext() != NULL; current = current->getNext())
        s << *current << ", ";
    s << *current;
    s << "\n}";
    return s;
}

template <class T1>
istream& operator>>(istream& s, LinkedList<T1>& el)
{
    cout << "\nList size: " << el.count << ". Enter " << el.count << " values: ";
    Element<T1>* pom = el.head;
    T1 val;
    for (int i = 0; i < el.count; i++)
    {
        s >> val;
        pom->setInfo(val);
        pom = pom->getNext();
    }
    return s;
}

// Класс Односвязный Стек
template<class T>
class Stack : public LinkedList<T>
{
public:
    Stack() : LinkedList<T>()
    {
        cout << "\nStack class constructor";
    }

    virtual ~Stack()
    {
        cout << "\nStack class destructor";
    }

    virtual Element<T>* operator[](int index)
    {
        Element<T>* current = LinkedList<T>::head;

        for (int i = 0;
             current != NULL && i < index;
             current = current->getNext(), i++);

        return current;
    }

    //Добавление элемента в конец списка
    virtual Element<T>* push(T value)
    {
        if (LinkedList<T>::head == NULL)
        {
            LinkedList<T>::tail = new Element<T>(value);
            LinkedList<T>::head = LinkedList<T>::tail;
        }
        else
        {
            LinkedList<T>::tail->setNext(new Element<T>(value));
            LinkedList<T>::tail = LinkedList<T>::tail->getNext();
        }
        LinkedList<T>::count++;
        return LinkedList<T>::tail;
    }

    //Удаление элемента из начала списка
    virtual Element<T>* pop()
    {
        if (LinkedList<T>::tail == NULL)
            return NULL;
        Element<T>* res = LinkedList<T>::tail;
        if (LinkedList<T>::head == LinkedList<T>::tail)
            LinkedList<T>::head = LinkedList<T>::tail = NULL;
        else
        {
            Element<T>* current;
            for (current = LinkedList<T>::head;
                 current->getNext() != LinkedList<T>::tail; current = current->getNext());
            LinkedList<T>::tail = current;
            LinkedList<T>::tail->setNext(NULL);
        }
        LinkedList<T>::count--;
        return res;
    }

    //Вставка нового элемента в произвольное место списка
    virtual Element<T>* insert(T value, Element<T>* predecessor = NULL)
    {
        if (LinkedList<T>::head == NULL)
        {
            return push(value);
        }
        if (predecessor == NULL)
        {
            Element<T>* newElem = new Element<T>(value);
            newElem->setNext(LinkedList<T>::head);
            LinkedList<T>::head = newElem;
            LinkedList<T>::count++;
            return newElem;
        }
        Element<T>* newElem = new Element<T>(value);
        Element<T>* successor = predecessor->getNext();
        predecessor->setNext(newElem);
        newElem->setNext(successor);
        if (predecessor == LinkedList<T>::tail)
            LinkedList<T>::tail = LinkedList<T>::tail->getNext();
        LinkedList<T>::count++;
        return newElem;
    }

    virtual Element<T>* find(T value)
    {
        Element<T>* p = LinkedList<T>::head;
        for (; p->getInfo() != value && p != NULL; p = p->getNext());
        return p;
    }

    virtual Element<T>* find(T value, Element<T>* el)
    {
        if (el->getInfo() == value || el == NULL)
            return el;
        return find(value, el->getNext());
    }

    //Удаление элемента из любого места списка
    virtual Element<T>* remove(T value)
    {
        Element<T>* p = LinkedList<T>::head;
        if (p == NULL) return p;
        if (p->getInfo() == value) {
            LinkedList<T>::head = p->getNext();
            p->setNext(NULL);
            LinkedList<T>::count--;
            return p;
        }
        for (; p->getNext() != NULL && p->getNext()->getInfo() != value; p = p->getNext());
        if (p->getNext()->getInfo() == value)
        {
            Element<T>* res = p->getNext();
            p->setNext(res->getNext());
            res->setNext(NULL);
            LinkedList<T>::count--;
            return res;
        }
        return NULL;
    }

    virtual void filter(bool (*cmp)(T), LinkedList<T>* dest) {
        for (Element<T>* cur = LinkedList<T>::head; cur != NULL; cur = cur->getNext())
            if (cmp(cur->getInfo()))
                dest->push(cur->getInfo());
    }

    virtual void filter(bool (*cmp)(T), LinkedList<T>* dest, Element<T>* cur) {
        if (cur == NULL) return;
        if (cmp(cur->getInfo()))
            dest->push(cur->getInfo());
        return filter(cmp, dest, cur->getNext());
    }

    virtual void filter(T check, LinkedList<T>* dest, bool more) {
        for (Element<T>* cur = LinkedList<T>::head; cur != NULL; cur = cur->getNext()) {
            if (cur->getInfo() > check && more)
                dest->push(cur->getInfo());
            if (cur->getInfo() < check && !more)
                dest->push(cur->getInfo());
        }
    }

    virtual void filter(T check, LinkedList<T>* dest, Element<T>* cur, bool more) {
        if (cur == NULL) return;
        if (cur->getInfo() > check && more)
            dest->push(cur->getInfo());
        if (cur->getInfo() < check && !more)
            dest->push(cur->getInfo());
        return filter(check, dest, cur->getNext(), more);
    }
};


//Класс Двусвязный Стек
template<class T>
class DoubleSidedStack : public Stack<T> {
public:
    DoubleSidedStack() : Stack<T>()
    {
        cout << "\nDoubleSidedStack class constructor";
    }
    virtual ~DoubleSidedStack()
    {
        cout << "\nDoubleSidedStack class destructor";
    }

    virtual Element<T>* operator[](int index)
    {
        Element<T>* current;
        if (index < LinkedList<T>::count / 2)
        {
            current = LinkedList<T>::head;
            for (int i = 0;
                 current != NULL && i < index;
                 current = current->getNext(), i++);
            return current;
        }
        else
        {
            current = LinkedList<T>::tail;
            for (int i = LinkedList<T>::count - 1;
                 current != NULL && i > index;
                 current = current->getPrev(), i--);
            return current;
        }
        return current;
    }

    //Добавление элемента в конец списка
    virtual Element<T>* push(T value)
    {
        Element<T>* tail_predecessor_push = LinkedList<T>::tail;
        Element<T>* res = Stack<T>::push(value);
        res->setPrev(tail_predecessor_push);
        return res;
    }

    //Удаление элемента из начала списка
    virtual Element<T>* pop()
    {
        if (LinkedList<T>::tail == LinkedList<T>::head)
            return Stack<T>::pop();
        Element<T>* res = LinkedList<T>::tail;
        LinkedList<T>::tail = LinkedList<T>::tail->getPrev();
        LinkedList<T>::tail->setNext(NULL);
        res->setPrev(NULL);
        LinkedList<T>::count--;
        return res;
    }

    //Вставка нового элемента в произвольное место списка
    virtual Element<T>* insert(T value, Element<T>* predecessor = NULL)
    {
        if (LinkedList<T>::head == NULL)
        {
            return push(value);
        }
        if (predecessor == NULL)
        {
            Stack<T>::insert(value);
            LinkedList<T>::head->getNext()->setPrev(LinkedList<T>::head);
            return LinkedList<T>::head;
        }
        Element<T>* successor = predecessor->getNext();
        Element<T>* inserted = Stack<T>::insert(value, predecessor);
        if (successor != NULL) successor->setPrev(inserted);
        inserted->setPrev(predecessor);
        return inserted;
    }

    //Удаление элемента из любого места списка
    virtual Element<T>* remove(T value)
    {
        Element<T>* p = LinkedList<T>::head;
        if (p == NULL) return p;
        if (p->getInfo() == value)
        {
            Stack<T>::remove(value);
            LinkedList<T>::head->setPrev(NULL);
            if (LinkedList<T>::count == 1)
                LinkedList<T>::tail->setPrev(NULL);
            return p;
        }
        for (; p->getNext() != NULL && p->getNext()->getInfo() != value; p = p->getNext());
        if (p->getNext()->getInfo() == value)
        {
            Element<T>* res = p->getNext();
            p->setNext(res->getNext());
            if (res->getNext() != NULL) res->getNext()->setPrev(p);
            if (res->getNext() == NULL) LinkedList<T>::tail = p;
            res->setNext(NULL);
            res->setPrev(NULL);
            LinkedList<T>::count--;
            return res;
        }
        return NULL;
    }

    // find и filter ищет по значению, поэтому нет смысла переопределять его для DoubleSidedStack
};

template <class T>
class PatientStack : protected DoubleSidedStack<T>
{
public:
    PatientStack() : DoubleSidedStack<T>()
    {
        cout << "\nHumanStack class constructor";
    }
    virtual ~PatientStack()
    {
        cout << "\nHumanStack class destructor";
    }

    virtual Element<T>* operator[](int index)
    {
        return DoubleSidedStack<T>::operator[](index);
    }

    //Добавление в начало
    virtual Element<T>* push(T value)
    {
        Element<T>* pom = new Element<T>(value);
        if (LinkedList<T>::count == 0)
        {
            LinkedList<T>::head = LinkedList<T>::tail = pom;
            LinkedList<T>::count++;
            return pom;
        }
        pom->setNext(LinkedList<T>::head);
        if (pom->getNext() != NULL) LinkedList<T>::head->setPrev(pom);
        LinkedList<T>::head = pom;
        LinkedList<T>::count++;
        return pom;
    }

    //Удаление с конца
    virtual Element<T>* pop()
    {
        if (LinkedList<T>::count == 0) return NULL;
        Element<T>* res = LinkedList<T>::tail;
        if (LinkedList<T>::count == 1)
        {
            LinkedList<T>::head = LinkedList<T>::tail = NULL;
        }
        res->getPrev()->setNext(NULL);
        LinkedList<T>::tail = res->getPrev();
        res->setPrev(NULL);
        LinkedList<T>::count--;
        return res;
    }

    //Вставка нового элемента в произвольное место списка
    virtual Element<T>* insert(T value, Element<T>* predecessor = NULL)
    {
        return DoubleSidedStack<T>::insert(value, predecessor);
    }

    //Поиск по фамилии
    virtual Element<T>* find(T value) {
        Element<T>* p = LinkedList<T>::head;
        for (; p->getInfo().last_name != value.last_name && p != NULL; p = p->getNext());
        return p;
    }

    virtual Element<T>* find(T value, Element<T>* el)
    {
        if (el->getInfo().last_name == value.last_name || el == NULL)
            return el;
        return find(value, el->getNext());
    }

    Element<T>* find(string last_name)
    {
        Element<T>* p = LinkedList<T>::head;
        for (; p->getInfo().last_name != last_name && p != NULL; p = p->getNext());
        return p;
    }

    Element<T>* find(string last_name, Element<T>* el)
    {
        if (el->getInfo().last_name == last_name || el == NULL)
            return el;
        return find(last_name, el->getNext());
    }

    //Удаление элемента из любого места списка
    virtual Element<T>* remove(T value)
    {
        return DoubleSidedStack<T>::remove(value);
    }

    //Фильтр по группе крови
    virtual void filter(bool (*cmp)(T), LinkedList<T>* dest)
    {
        for (Element<T>* cur = LinkedList<T>::head; cur != NULL; cur = cur->getNext())
            if (cmp(cur->getInfo()))
                dest->push(cur->getInfo());
    }

    virtual void filter(bool (*cmp)(T), LinkedList<T>* dest, Element<T>* cur)
    {
        if (cur == NULL) return;
        if (cmp(cur->getInfo()))
            dest->push(cur->getInfo());
        return filter(cmp, dest, cur->getNext());
    }

    virtual void filter(T check, LinkedList<T>* dest, bool more = false)
    {
        for (Element<T>* cur = LinkedList<T>::head; cur != NULL; cur = cur->getNext())
        {
            if (cur->getInfo().blood_type > check.blood_type && more)
                dest->push(cur->getInfo());
            if (cur->getInfo().blood_type < check.blood_type && !more)
                dest->push(cur->getInfo());
        }
    }

    virtual void filter(T check, LinkedList<T>* dest,
                        Element<T>* cur, bool more = false)
    {
        if (cur == NULL) return;
        if (cur->getInfo().blood_type > check.blood_type && more)
            dest->push(cur->getInfo());
        if (cur->getInfo().blood_type < check.blood_type && !more)
            dest->push(cur->getInfo());
        return filter(check, dest, cur->getNext(), more);
    }

    virtual void save(string fileName)
    {
        ofstream fout(fileName);
        if (fout.is_open())
        {
            fout << LinkedList<T>::count << "\n";
            Element<T>* pom = LinkedList<T>::head;
            T info;
            for (int i = 0; i < LinkedList<T>::count; i++)
            {
                info = pom->getInfo();
                fout << info << "\n";
                pom = pom->getNext();
            }
            fout.close();
        }
    }
    virtual void load(string fileName)
    {
        ifstream fin;
        fin.open(fileName);
        if (fin.is_open())
        {
            int ct;
            fin >> ct;

            if (ct != LinkedList<T>::count)
            {
                LinkedList<T>::count = 0;
                if (LinkedList<T>::head != NULL)
                {
                    Element<T>* current = LinkedList<T>::head;
                    Element<T>* temp = LinkedList<T>::head->getNext();
                    for (; current != LinkedList<T>::tail; current = temp, temp = temp->getNext())
                        delete current;
                }
                LinkedList<T>::head = NULL; LinkedList<T>::tail = NULL;
                T val;
                for (int i = 0; i < ct; i++)
                {
                    fin >> val;
                    push(val);
                }
            }
            else {
                Element<T>* pom = LinkedList<T>::head;
                T val;
                for (int i = 0; i < LinkedList<T>::count; i++)
                {
                    fin >> val;
                    pom->setInfo(val);
                    pom = pom->getNext();
                }
            }
            fin.close();
        }
    }

    template<class T1>
    friend ostream& operator<<(ostream& s, PatientStack<T1>& el);
    template <class T1>
    friend istream& operator>>(istream& s, PatientStack<T1>& el);
};


template<class T1>
ostream& operator<<(ostream& s, PatientStack<T1>& el)
{
    Element<T1>* current;
    s << "\n{";
    for (current = el.head; current->getNext() != NULL; current = current->getNext())
        s << *current << ", ";
    s << *current;
    s << "\n}";
    return s;
}


template <class T1>
istream& operator>>(istream& s, PatientStack<T1>& el)
{
    cout << "\nList size is: " << el.count << ". Enter " << el.count << " values: ";
    Element<T1>* pom = el.head;
    T1 val;
    for (int i = 0; i < el.count; i++)
    {
        s >> val;
        pom->setInfo(val);
        pom = pom->getNext();
    }
    return s;
}


class Patient {
public:
    string last_name, name, birth_date, address;
    int64_t phone_number, card_number, blood_type;

    Patient(string last_name, string name, string birth_date, string address,
            int64_t phone_number, int64_t card_number, int blood_type) {
        this->last_name = last_name;
        this->name = name;
        this->birth_date = birth_date;
        this->address = address;
        this->phone_number = phone_number;
        this->card_number = card_number;
        this->blood_type = blood_type;
    }

    void fillRandom() {
        last_name = makeRandomString();
        name = makeRandomString();
        birth_date = makeRandomString();
        address = makeRandomString();

        phone_number = 70000000000 + (rand() * rand()) % 10000000000;
        card_number = 2000000000000000 + (rand() * rand()) % 10000000000000;
        blood_type = rand() % 5;
    }

    string makeRandomString() {
        int n = rand() % 100;
        string res = "";
        for (int i = 0; i < n; ++i) {
            res += char(65 + rand() % 110);
        }
        return res;
    }

    bool operator==(Patient cus)
    {
        return last_name == cus.last_name &&
               name == cus.name &&
               birth_date == cus.birth_date &&
               address == cus.address &&
               card_number == cus.card_number &&
               blood_type == cus.blood_type &&
               phone_number == cus.phone_number;
    }

    bool operator!=(Patient cus)
    {
        return last_name != cus.last_name &&
               name != cus.name &&
               birth_date != cus.birth_date &&
               address != cus.address &&
               card_number != cus.card_number &&
               blood_type != cus.blood_type &&
               phone_number != cus.phone_number;
    }

    bool operator<(Patient cus)
    {
        return blood_type < cus.blood_type;
    }

    bool operator>(Patient cus) {
        return blood_type > cus.blood_type;
    }

    friend ostream& operator<<(ostream& s, Patient& value);
    friend istream& operator>>(istream& s, Patient& value);
};

ostream& operator<<(ostream& s, Patient& value) {
    if (typeid(s) == typeid(ofstream))
    {
        s << value.last_name << " "
          << value.name << " "
          << value.birth_date << " "
          << value.address << " "
          << value.phone_number << " "
          << value.card_number << " "
          << value.blood_type << "\n";
    } else {
        s << "(" << value.last_name << ", " << value.name << ", "
          << value.birth_date << ", " << value.address << ", " << value.phone_number << ", "
          << value.card_number << ", " << value.blood_type << ")";
    }
    return s;
}

istream& operator>>(istream& s, Patient& value)
{
    if (typeid(s) == typeid(ifstream)) {
        s >> value.last_name
          >> value.name
          >> value.birth_date
          >> value.address
          >> value.phone_number
          >> value.card_number
          >> value.blood_type;
    } else {
        cout << "\nEnter fields: ";
        cout << "\nLastname (string): ";
        s >> value.last_name;
        cout << "\nName (string): ";
        s >> value.name;
        cout << "\nBirth date (string): ";
        s >> value.birth_date;
        cout << "\nAddress (string): ";
        s >> value.address;
        cout << "\nPhone number (int): ";
        s >> value.phone_number;
        cout << "\nCard number (int): ";
        s >> value.card_number;
        cout << "\nBlood type (int): ";
        s >> value.blood_type;
    }
    return s;
}


bool f(double d) { return d > 10; }


bool g(Patient c) { return c.address == "Moscow Gurianova 4,1,73"; }


int main() {
    LinkedList<double>* list;
    cout << "------------------STACK WORK--------------------";
    list = new Stack<double>();
    for (int i = 0; i < 10; i++)
        list->push(i * 2);

    cout << endl << "operator[](4): " << *list->operator[](4);
    cout << endl << "pop(): " << *list->pop();
    Element<double>* el = list->find(4);
    cout << endl << "el = find(4): " << *el;
    cout << endl << "find(14, el): " << *list->find(14, el);
    cout << *list << " list";
    list->insert(77, list->find(10));
    cout << *list << " list after insert(77, find(10))";
    list->remove(12);
    cout << *list << " list after remove(12)";

    LinkedList<double>* filteredList = new Stack<double>();

    list->filter(f, filteredList);
    cout << *filteredList << " filtered list by double > 10";

    list->filter(f, filteredList, el);
    cout << *filteredList << " filtered list + by double > 10 recursion after el";

    delete list;
    delete filteredList;

    cout << "\n---------------------DOUBLE SIDED STACK WORK------------------------";
    list = new DoubleSidedStack<double>();
    for (int i = 0; i < 10; i++)
        list->push(i * 2 - 1);

    cout << endl << "operator[](3): " << *list->operator[](3);
    cout << endl << "operator[](9): " << *list->operator[](9);
    cout << endl << "pop(): " << *list->pop();
    el = list->find(7);
    cout << endl << "el = find(7): " << *el;
    cout << endl << "find(15, el): " << *list->find(15, el);
    cout << *list << " list";
    list->insert(77, list->find(11));
    cout << *list << " list after insert(77, find(11))";
    list->remove(3);
    cout << *list << " list after remove(3)";

    filteredList = new DoubleSidedStack<double>();

    list->filter(f, filteredList);
    cout << *filteredList << " filtered list by double > 10";

    list->filter(f, filteredList, el);
    cout << *filteredList << " filtered list + by double > 10 recursion after el";

    cout << "\n---------------------DYNAMIC_CAST--------------------------";

    DoubleSidedStack<double>* castedList = dynamic_cast<DoubleSidedStack<double>*>(list);
    cout << *castedList;

    delete list;
    delete filteredList;

    cout << "\n------------------------MY ClASS WORK------------------------------";
    PatientStack<Patient> patient;
    Patient patient1("Kotik", "Bobby", "16.11.1963", "America, Activision", 56784567121, 123, 1);
    Patient patient2("Gold", "Asmon", "14.05.1990", "America, Texas", 54783018392, 129, 3);
    Patient patient3("Rawr", "Zack", "13.07.1991", "America, Texas, Austin" , 59284718023, 58, 2);
    Patient patient4("QC", "X", "14.06.2015", "America, California", 48734650192, 333, 2);
    Patient patient5("Tsal", "Vitaliy", "01.06.1987", "Ukraine, Vinitsa", 73450192342, 777, 4);
    Patient patients[] = {patient1, patient2, patient3, patient4, patient5};
    for (int i = 0; i < 4; i++)
        patient.push(patients[i]);

    cout << patient << " patient";
//    cout << endl << "operator[](2): " << *human.operator[](2);
//    cout << endl << "operator[](0): " << *human.operator[](0);
//    cout << endl << "pop(): " << *human.pop();
//    Element<Human>* humanEl = human.find(h4);
//    cout << endl << "humanEl = find(h4): " << *humanEl;
//    cout << endl << "find(h2, h4): " << *human.find(h2, humanEl);
//    cout << human << " human";
//    human.insert(h5, human.find(h4));
//    cout << human << " human after insert(h5, find(h4))";
//    human.remove(h5);
//    cout << h5;
//    cout << human << " human after remove(h5)";
//
//    LinkedList<Human>* filteredHumanList = new DoubleSidedStack<Human>();
//
//    human.filter(g, filteredHumanList);
//    cout << *filteredHumanList << " filtered human list by my address";
//
//    cout << "\n------------------------FILE SAVE/LOAD--------------------------";
//
//    human.save("list.txt");
//    filteredHumanList->load("list.txt");
//    cout << *filteredHumanList;
//
//    delete filteredHumanList;
//
//    return 0;
}