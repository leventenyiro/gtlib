#include <iostream>
#include <sstream>
#include <string>

#include "../library/stringstreamenumerator.hpp"
#include "../library/maxsearch.hpp"
#include "../library/seqinfileenumerator.hpp"

using namespace std;

struct Episode {
    string name;
    int length;
};

istream &operator>>(istream &is, Episode &e) {
    return is >> e.name >> e.length;
}

struct Season {
    int year;
    string name;
    Episode minEpisode;
};

class MinEpisode : public MaxSearch<Episode, int, Less<int>> {
protected:
    virtual int func(const Episode& e) const override {
        return e.length;
    }
};

istream &operator>>(istream &is, Season &s) {
    string line;
    if (getline(is, line)) {
        stringstream ss(line);

        ss >> s.year >> s.name;

        StringStreamEnumerator<Episode> enor(ss);

        MinEpisode pr;
        pr.addEnumerator(&enor);
        pr.run();

        if (pr.found())
            s.minEpisode = pr.optElem();
        else {
            s.minEpisode.length = 0;
            s.minEpisode.name = nullptr;
        }
    }
    return is;
}

class MinSeason : public MaxSearch<Season, int, Less<int>> {
protected:
    virtual int func(const Season& e) const override {
        return e.minEpisode.length;
    }

    virtual bool cond(const Season& e) const override {
        return e.year >= 2005;
    }
};

int main(int argc, char const *argv[])
{
    string filename = "input.txt";

    SeqInFileEnumerator<Season> enor(filename);

    MinSeason pr;
    pr.addEnumerator(&enor);
    pr.run();

    if (pr.found())
        cout << pr.optElem().name << " " << pr.optElem().year << " " << pr.optElem().minEpisode.name << " " << pr.optElem().minEpisode.length << endl;
    else
        cout << "Nem volt megfelelő évad" << endl;
    
    return 0;
}