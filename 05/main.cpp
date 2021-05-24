#include <iostream>

#include "../library/stringstreamenumerator.hpp"
#include "../library/summation.hpp"
#include "../library/seqinfileenumerator.hpp"
#include "../library/maxsearch.hpp"

using namespace std;

struct Student {
    string name;
    double avg;
};

struct AvgResult {
    int sum, count;
};

// összegezzük a jegyeket és megszámoljuk hány jegy van
class Avg : public Summation<int, AvgResult> {
    virtual AvgResult func(const int& e) const {
        return {e, 1};
    }

    virtual AvgResult neutral() const override {
        return {0, 0};
    }

    virtual AvgResult add( const AvgResult& a, const AvgResult& b) const override {
        return {a.sum + b.sum, a.count + b.count};
    }
};

// beolvasás - itt csinálunk átlagot
istream &operator>>(istream &is, Student &st) {
    string line;
    if (getline(is, line)) {
        stringstream ss(line);
        ss >> st.name;

        StringStreamEnumerator<int> enor(ss);

        Avg pr;
        pr.addEnumerator(&enor);
        pr.run();

        if (pr.result().count == 0)
            st.avg = 0;
        else
            st.avg = pr.result().sum / (double)pr.result().count;
    }

    return is;
}

// maximumkeresés átlag alapján
class BestStudent : public MaxSearch<Student, double, Greater<double>> {
protected:
    virtual double func(const Student& e) const override {
        return e.avg;
    };
};

int main(int argc, char const *argv[])
{
    string filename = "input.txt";

    SeqInFileEnumerator<Student> enor(filename);
    
    /*for (enor.first(); !enor.end(); enor.next()) {
        cout << enor.current().name << ": " << enor.current().avg << endl;
    }*/
    
    BestStudent pr;
    pr.addEnumerator(&enor);
    pr.run();

    if (pr.found())
        cout << pr.optElem().name << ": " << pr.optElem().avg << endl;
    else
        cout << "Nincs ilyen diak" << endl;
    
    return 0;
}
