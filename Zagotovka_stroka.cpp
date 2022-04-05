#include <iostream>
using namespace std;

class BaseString
{
protected:
	char* p;
	int len;
	int capacity;
public:
	BaseString(char* ptr)
	{
		int i = 0;
		while (ptr[i] != '\0')
			i++;
		len = i;
		capacity = i * 4;
		p = new char[capacity];
		for (i = 0; i < len; i++)
		{
			p[i] = ptr[i];
		}
		p[i] = '\0';
	}

	BaseString(const char* ptr)
	{
		int i = 0;
		while (ptr[i] != '\0')
			i++;
		len = i;
		capacity = i * 4;
		p = new char[capacity];
		for (i = 0; i < len; i++)
		{
			p[i] = ptr[i];
		}
		p[i] = '\0';
	}

	BaseString(const BaseString& s)
	{
		capacity = s.capacity;
		p = new char[capacity];
		len = s.len;
		for (int i = 0; i < len; i++)
			p[i] = s.p[i];
		p[len] = '\0';
	}

	BaseString(int Capacity = 256)
	{
		cout << "\nBase Constructor 0\n";
		capacity = Capacity;
		p = new char[capacity];
		len = 0;
	}

	~BaseString()
	{
		cout << "\nBase Destructor\n";
		if (p != NULL)
			delete[] p;
		len = 0;
	}

	int Length() { return len; }
	int Capacity() { return capacity; }
	//char* get() {return p;}
	char& operator[](int i) { return p[i]; }


	BaseString& operator=(const BaseString& s) {
		capacity = s.capacity;
		p = new char[capacity];
		len = s.len;
		for (int i = 0; i < len; i++) {
			p[i] = s.p[i];
		}
		p[len] = '\0';
		return *this;
	}

	BaseString(BaseString& s)
	{
		cout << "\nBase Copy Constructor\n";

	}

	virtual void print()
	{
		int i = 0;
		while (p[i] != '\0')
		{
			cout << p[i];
			i++;
		}
	}
};

class String : public BaseString
{
public:
	String(int Capacity = 256) : BaseString(Capacity) {}
	String(char* ptr) : BaseString(ptr) {}
	String(const char* ptr) : BaseString(ptr) {}
	String(const String& ptr)
	{
		delete[] p;
		capacity = ptr.capacity;
		p = new char[capacity];
		len = ptr.len;
		for (int i = 0; i < len; i++)
			p[i] = ptr.p[i];
		p[len] = '\0';
	}
	~String() { cout << "\nString destructor"; }

	String operator+(String& s)
	{
		String res((capacity +s.capacity)>=256 ? capacity+s.capacity :256);
		res.len = len + s.len;
		for (int i = 0; i < len; i++)
		{
			res.p[i] = p[i];
		}
		for (int i = len; i < res.len; i++)
		{
			res.p[i] = s.p[i - len];
		}
		res.p[res.len] = '\0';
		return res;
	}
};

int main()
{
	String s("test");
	s.print();
	String s1;
	s1 = s;
	s1.print();
	String s2;
	s2 = s;
	s2 = s + s1;
	s2.print();
	return 0;
}


