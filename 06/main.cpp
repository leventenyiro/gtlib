#include <iostream>
#include "math.h"

#include "../library/stringstreamenumerator.hpp"
#include "../library/summation.hpp"
#include "../library/seqinfileenumerator.hpp"
#include "../library/maxsearch.hpp"

using namespace std;

struct Line {
    string name, subject;
    int mark;
};

struct AvgResult {
    int sum, count;
};

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

istream &operator>>(istream &is, Line &st) {
    string line;
    if (getline(is, line)) {
        stringstream ss(line);
        ss >> st.name >> st.subject;

        StringStreamEnumerator<int> enor(ss);

        Avg pr;
        pr.addEnumerator(&enor);
        pr.run();

        if (pr.result().count == 0)
            st.mark = 0;
        else
            st.mark = round(pr.result().sum / (double)pr.result().count);
    }

    return is;
}

// 2. rész - ugyanolyan nevűek
class Avg2 : public Summation<Line, AvgResult> {
private:
    string _name;

public:
    Avg2(string name) : _name(name) {}
    
protected:
    virtual AvgResult func(const Line& e) const {
        return {e.mark, 1};
    }

    virtual AvgResult neutral() const override {
        return {0, 0};
    }

    virtual AvgResult add( const AvgResult& a, const AvgResult& b) const override {
        return {a.sum + b.sum, a.count + b.count};
    }

    virtual bool whileCond(const Line& current) const override {
        return current.name == _name;
    }

    virtual void first() override {}
};

struct Student {
    string name;
    double avg;
};

class StudentEnor : public Enumerator<Student> {
private:
    bool _end;
    Student _current;
    SeqInFileEnumerator<Line> _enor;

public:
    StudentEnor(string &filename) : _enor(filename) {}

    virtual void first() override {
        _enor.first();
        next();
    }

    virtual void next() override {
        if (!(_end = _enor.end())) {
            _current.name = _enor.current().name;

            Avg2 pr(_current.name);
            pr.addEnumerator(&_enor);
            pr.run();

            if (pr.result().count == 0)
                _current.avg = 0;
            else
                _current.avg = pr.result().sum / (double)pr.result().count;
        }
    }

    virtual bool end() const override {
        return _end;
    }

    virtual Student current() const override {
        return _current;
    }
};

class BestStudent : public MaxSearch<Student, double, Greater<double>> {
protected:
    virtual double func(const Student& e) const override {
        return e.avg;
    }
};

int main(int argc, char const *argv[])
{
    string filename = "input.txt";

    //SeqInFileEnumerator<Line> enor(filename);

    /*for (enor.first(); !enor.end(); enor.next()) {
        cout << enor.current().name << " (" << enor.current().subject << "): " << enor.current().avg << endl;
    }*/


    StudentEnor enor(filename);

    /*for (enor.first(); !enor.end(); enor.next()) {
        cout << enor.current().name << ": " << enor.current().avg << endl;
    }*/

    BestStudent pr;
    pr.addEnumerator(&enor);
    pr.run();

    if (pr.found())
        cout << pr.optElem().name << ": " << pr.optElem().avg << endl;
    else
        cout << "Nincs ilyen hallgato" << endl;
    
    return 0;
}
