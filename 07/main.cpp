#include <iostream>
#include <string>
#include <sstream>

#include "../library/stringstreamenumerator.hpp"
#include "../library/maxsearch.hpp"
#include "../library/seqinfileenumerator.hpp"

using namespace std;

struct Episode {
    string name;
    int length;
};

istream &operator>>(istream &is, Episode &s) {
    return is >> s.name >> s.length;
}

struct Season {
    int year;
    string actor;
    Episode minEpisode;
};

// mik közül, mi alapján, és hogyan
class MinEpisode : public MaxSearch<Episode, int, Less<int>> {
protected: //érdemes ezt kitenni
    virtual int func(const Episode &e) const override {
        return e.length;
    }
};

istream &operator>>(istream &is, Season &s) {
    string line;
    if (getline(is, line)) {
        stringstream ss(line);

        ss >> s.year >> s.actor;

        // epizódok beolvasása - legrövidebb megkeresése
        StringStreamEnumerator<Episode> enor(ss);
        MinEpisode pr;
        pr.addEnumerator(&enor);
        pr.run();

        if (pr.found())
            s.minEpisode = pr.optElem();
        else
            s.minEpisode.length = 0;
    }
    return is;
}

// legrövidebb szezon
class MinSeason : public MaxSearch<Season, int, Less<int>> {
protected:
    virtual int func(const Season &e) const override {
        return e.minEpisode.length;
    }

    virtual bool cond(const Season &e) const override {
        return e.year >= 2005;
    }
};

int main(int argc, char const *argv[])
{
    string filename = "input.txt";
    // sorokat olvas
    SeqInFileEnumerator<Season> enor(filename);

    /*for (enor.first(); !enor.end(); enor.next()) {
        cout << enor.current().actor << " " << enor.current().year << " " << enor.current().minEpisode.name << " " << enor.current().minEpisode.length << endl;
    }*/

    MinSeason pr;
    pr.addEnumerator(&enor);
    pr.run();

    if (pr.found())
        cout << pr.optElem().actor << " " << pr.optElem().year << " " << pr.optElem().minEpisode.name << " " << pr.optElem().minEpisode.length << endl;
    else
        cout << "Nem volt megfelelő évad" << endl;

    
    return 0;
}
