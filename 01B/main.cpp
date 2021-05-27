#include <iostream>
#include "../library/seqinfileenumerator.hpp"
#include "../library/summation.hpp"

using namespace std;

struct Input{
    string name;
    int value;
    //friend istream& operator>>(istream &inp, Input &e);
};

istream& operator>>(istream &inp, Input &e)
{
    inp >> e.name >> e.value;
    return inp;
}

class MySum : public Summation<Input, int>{
protected:
    virtual int neutral() const override { return 0; }
    virtual int add(const int &a, const int &b) const override { return a+b; }
    virtual int func(const Input &e) const override { return e.value;}
};

int main()
{
    
    SeqInFileEnumerator<Input> enor("input.txt");
    /*sum.addEnumerator(&enor);
    sum.run();
    cout << sum.result() << endl;*/
    MySum sum;
    sum.addEnumerator(&enor);
    sum.run();
    cout << "Sum: " << sum.result() << endl;
    
    return 0;
}