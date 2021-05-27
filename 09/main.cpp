#include <iostream>
#include <sstream>

#include "../library/stringstreamenumerator.hpp"
#include "../library/summation.hpp"
#include "../library/linsearch.hpp"
#include "../library/seqinfileenumerator.hpp"

using namespace std;

struct Record {
    string date;
    int wet, temp;
};

istream &operator>>(istream &is, Record &e) {
    return is >> e.date >> e.wet >> e.temp;
}

struct Weather {
    string city;
    int avgWet;
};

struct AvgWeather {
    int sumWet, count;
};

class Avg : public Summation<Record, AvgWeather> {
protected:
    virtual AvgWeather func(const Record& e) const override {
        return {e.wet, 1};
    }
    
    virtual AvgWeather neutral() const override {
        return {0, 0};
    }
    
    virtual AvgWeather add( const AvgWeather& a, const AvgWeather& b) const override {
        return {a.sumWet + b.sumWet, a.count + b.count};
    }
    
};

istream &operator>>(istream &is, Weather &e) {
    string line;
    if (getline(is, line)) {
        stringstream ss(line);

        ss >> e.city;

        // rekordokból átlagszámítás
        StringStreamEnumerator<Record> enor(ss);

        Avg pr;
        pr.addEnumerator(&enor);
        pr.run();

        e.avgWet = pr.result().sumWet / pr.result().count;
    }
    return is;
}

class SearchWetGreaterOrEqual15 : public LinSearch<Weather> {
protected:
    virtual bool cond(const Weather& e) const override {
        return e.avgWet >= 15;
    }
};

int main(int argc, char const *argv[])
{
    // egy város, amelynek csapadékszintje meghaladja a 15%-ot
    SeqInFileEnumerator<Weather> enor("input1.txt");

    SearchWetGreaterOrEqual15 pr;
    pr.addEnumerator(&enor);
    pr.run();

    if (pr.found())
        cout << "Ebben a varosban haladta meg az atlag csapadekmennyiseg a 15%-ot: " << pr.elem().city << endl;
    else
        cout << "Nem volt ilyen varos" << endl;
    
    return 0;
}
