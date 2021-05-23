#include <iostream>

#include "../library/intervalenumerator.hpp"
#include "../library/summation.hpp"

using namespace std;

class Factorial : public Summation<int> {
    virtual int func(const int& e) const {
        return e;
    }

    virtual int neutral() const override {
        return 1;
    }

    virtual int add( const int& a, const int& b) const override {
        return a * b;
    }
};

int main(int argc, char const *argv[])
{
    int n;
    cout << "Faktorialis: ";
    cin >> n;

    IntervalEnumerator enor(1, n);

    Factorial f;
    f.addEnumerator(&enor);
    f.run();
    cout << n << "! = " << f.result() << endl;
    
    return 0;
}
