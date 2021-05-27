#include <iostream>
#include <sstream>
#include <string>

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

istream &operator>>(istream &is, Episode &e) {
    return is >> e.name >> e.length;
}

struct Season {
    int year;
    string actor;
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

        ss >> s.year >> s.actor;

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

/*class MinSeason : public MaxSearch<Season, int, Less<int>> {
protected:
    virtual int func(const Season& e) const override {
        return e.minEpisode.length;
    }

    virtual bool cond(const Season& e) const override {
        return e.year >= 2005;
    }
};*/

struct Actor {
    string name;
    bool goodActor;
};

class GoodActor : public Summation<Season, bool> {
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

    virtual bool whileCond(const Season& current) const override {
        return current.actor == _name;
    }
};

class ActorEnor : public Enumerator<Actor> {
private:
    bool _end;
    Actor _current;
    SeqInFileEnumerator<Season> _enor;

public:
    ActorEnor(string &filename) : _enor(filename) {}
    virtual void first() override {
        _enor.first();
        next();
    }

    virtual void next() override {
        if (!(_end = _enor.end())) {
            _current.name = _enor.current().actor;

            // GoodActor
            GoodActor pr(_current.name);
            pr.addEnumerator(&_enor);
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
    virtual bool cond(const Actor& e) const override {
        return e.goodActor;
    }
};

int main(int argc, char const *argv[])
{
    string filename = "input.txt";

    ActorEnor enor(filename);

    GoodActorCount pr;
    pr.addEnumerator(&enor);
    pr.run();

    cout << "Színészek száma: " << pr.result() << endl;
    
    return 0;
}