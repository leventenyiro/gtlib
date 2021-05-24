#include <iostream>
#include <vector>

#include "../library/seqinfileenumerator.hpp"
#include "../library/summation.hpp"
#include "../library/intervalenumerator.hpp"
#include "../library/linsearch.hpp"

using namespace std;

class Copy : public Summation<int, vector<int>> {
    virtual int func(const int& e) const override {
        return e;
    }
};

class InnerSearch : public LinSearch<int> {
private:
    vector<int> _x;
    int _index;
public:
    InnerSearch(vector<int> x, int index) : _x(x), _index(index) {}

    virtual bool cond(const int& e) const override {
        return _x[_index] == _x[e];
    }
};

class OuterSearch : public LinSearch<int> {
private:
    vector<int> _x;
public:
    OuterSearch(vector<int> x) : _x(x) {}

    virtual bool cond(const int& e) const override {
        IntervalEnumerator enor(0, e - 1);

        InnerSearch src(_x, e);
        src.addEnumerator(&enor);
        src.run();

        return src.found();
    }
};

int main(int argc, char const *argv[])
{
    string filename = "input.txt";

    SeqInFileEnumerator<int> enor1(filename);

    Copy cp;
    cp.addEnumerator(&enor1);
    cp.run();
    vector<int> x = cp.result();

    IntervalEnumerator enor2(0, x.size() - 1);

    OuterSearch src(x);
    src.addEnumerator(&enor2);
    src.run();

    if (src.found())
        cout << "A(z) " << x[src.elem()] << " ismetlodott eloszor" << endl;
    else
        cout << "Nem volt ismetlodo elem";
    
    return 0;
}
