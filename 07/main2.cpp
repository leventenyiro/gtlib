#include <iostream>
#include <string>
#include <sstream>

#include "../library/stringstreamenumerator.hpp"
#include "../library/maxsearch.hpp"
#include "../library/seqinfileenumerator.hpp"
#include "../library/enumerator.hpp"
#include "../library/summation.hpp"
#include "../library/counting.hpp"

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

// ----------------------------------------------------------

struct Actor {
    string name;
    bool goodActor;
};

class GoodActor : public Summation<Season, bool> { // évadok alapján megfelelt-e a színész
private:
    string _name;

public:
    GoodActor(string name) : _name(name) {}

protected:
    virtual bool func(const Season& e) const override {
        return e.minEpisode.length >= 40;
    }
    virtual bool neutral() const override {
        return true;
    }
    virtual bool add( const bool& a, const bool& b) const override {
        return a && b;
    }

    virtual bool whileCond(const Season &current) const override { // summation ősosztályában található fejléc
        return current.actor == _name;
    }
};

class ActorEnor : public Enumerator<Actor> {
private:
    bool _end;
    Actor _current;
    SeqInFileEnumerator<Season> _t;

public:
    ActorEnor(string &filename) : _t(filename) {}
    virtual void first() override {
        _t.first();
        next();
    }

    virtual void next() override {
        if (!(_end = _t.end())) {
            _current.name = _t.current().actor;

            GoodActor pr(_current.name);
            pr.addEnumerator(&_t);
            pr.run();

            _current.goodActor = pr.result();
        }
    }

    virtual bool end() const override {
        return _end;
    }

    virtual Actor current() const override {
        return _current;
    }
};

class GoodActorCount : public Counting<Actor> {
protected:
    virtual bool  cond(const Actor& e) const {
        return e.goodActor;
    }
};

int main(int argc, char const *argv[])
{
    string filename = "input.txt";
    {
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
    }
    {
        ActorEnor enor(filename);
        /*for (enor.first(); !enor.end(); enor.next()) {
            cout << enor.current().name << " " << enor.current().goodActor << endl;
        }*/
        GoodActorCount pr;
        pr.addEnumerator(&enor);
        pr.run();

        cout << "Színészek száma: " << pr.result() << endl;
    }

    
    return 0;
}
