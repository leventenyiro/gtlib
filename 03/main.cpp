#include <iostream>

#include "../library/seqinfileenumerator.hpp"
#include "../library/summation.hpp"

using namespace std;

class Assortment : public Summation<int, ostream> {
public:
    Assortment(ostream *f) : Summation<int, ostream>(f) {}

    virtual string func(const int& e) const override {
        ostringstream ss;
        ss << e << " ";
        return ss.str();
    }

    virtual bool cond(const int& e) const override {
        return e % 2 == 0;
    }
};

int main(int argc, char const *argv[])
{
    string filename = "input.txt";
    SeqInFileEnumerator<int> enor(filename);

    string outFilename = "out.txt";
    ofstream f(outFilename);
    Assortment ass(&f);
    ass.addEnumerator(&enor);
    ass.run();
    
    return 0;
}
