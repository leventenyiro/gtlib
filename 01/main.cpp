#include <iostream>

#include "../library/seqinfileenumerator.hpp"
#include "../library/summation.hpp"
#include "../library/maxsearch.hpp"

using namespace std;

class MySum : public Summation<int> {
     int func(const int& e) const override {
        return e;
    }

     int neutral() const override {
        return 0;
    }

     int add( const int& a, const int& b) const override {
        return a + b;
    }
};

class MyMaxSearch : public MaxSearch<int, int, Greater<int>> {
     int func(const int& e) const override {
        return e;
    }
};

int main(int argc, char const *argv[])
{
    string filename = "input.txt";

    SeqInFileEnumerator<int> enor(filename);

    /*MyMaxSearch max;
    max.addEnumerator(&enor);
    max.run();
    cout << "Sum: " << max.optElem() << endl;*/

    MySum pr;
    pr.addEnumerator(&enor);
    pr.run();
    
    cout << "Sum: " << pr.result() << endl;
    
    return 0;
}
