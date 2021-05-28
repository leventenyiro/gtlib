#include <iostream>
#include <sstream>
#include <string>

#include "../library/stringstreamenumerator.hpp"
#include "../library/maxsearch.hpp"
#include "../library/enumerator.hpp"
#include "../library/seqinfileenumerator.hpp"
#include "../library/summation.hpp"
#include "../library/counting.hpp"

using namespace std;

struct Record {
    string date;
    int wet, temp;
};

istream &operator>>(istream &is, Record &e) {
    return is >> e.date >> e.wet >> e.temp;
}

struct Weather {
    string city, institution;
    int maxTemp;
};

class MaxTemp : public MaxSearch<Record, int, Greater<int>> {
protected:
    virtual int func(const Record& e) const override {
        return e.temp;
    }
};

istream &operator>>(istream &is, Weather &e) {
    string line;
    if (getline(is, line)) {
        stringstream ss(line);

        ss >> e.city >> e.institution;

        // rekordokból átlagszámítás
        StringStreamEnumerator<Record> enor(ss);

        MaxTemp pr;
        pr.addEnumerator(&enor);
        pr.run();

        if (pr.found())
            e.maxTemp = pr.optElem().temp;
        else
            e.maxTemp = 0;
    }
    return is;
}

struct Hottest {
    string city;
    bool goodTemp;
};

class ReachedTemp40 : public Summation<Weather, bool> {
private:
    string _city;

public:
    ReachedTemp40(string city) : _city(city) {}

protected:
    virtual bool func(const Weather& e) const override {
        return e.maxTemp < 40;
    }

    virtual bool neutral() const override {
        return true;
    }

    virtual bool add( const bool& a, const bool& b) const override {
        return a && b;
    }

    virtual bool whileCond(const Weather& current) const override {
        return current.city == _city;
    }
};

class WeatherEnor : public Enumerator<Hottest> {
private:
    bool _end;
    Hottest _current;
    SeqInFileEnumerator<Weather> _enor;

public:
    WeatherEnor(string &filename) : _enor(filename) {}

protected:
    virtual void first() override {
        _enor.first();
        next();
    }

    virtual void next() override {
        if (!(_end = _enor.end())) {
            _current.city = _enor.current().city;

            ReachedTemp40 pr(_current.city);
            pr.addEnumerator(&_enor);
            pr.run();

            _current.goodTemp = pr.result();
        }
    }

    virtual bool end() const override {
        return _end;
    }

    virtual Hottest current() const override {
        return _current;
    }
};

class GoodWeatherList : public Summation<Hottest, string> {
protected:
    virtual string func(const Hottest& e) const override {
        return e.city + " ";
    }

    virtual string neutral() const override {
        return "";
    }

    virtual string add( const string& a, const string& b) const override {
        return a + b;
    }

    virtual bool cond(const Hottest& e) const override {
        return e.goodTemp;
    }
};

int main(int argc, char const *argv[])
{
    string filename = "input2.txt";
    WeatherEnor enor(filename);

    GoodWeatherList pr;
    pr.addEnumerator(&enor);
    pr.run();

    if (pr.result().length() != 0)
        cout << "Ezekben a varosokban nem haladta meg a homerseklet a 40 C-ot: " << pr.result() << endl;
    else
        cout << "Nincs olyan város, ahol a hőmérséklet ne haladná meg a 40 C-ot" << endl;
    
    return 0;
}
