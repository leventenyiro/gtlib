#include <iostream>
#include <sstream>

#include "../library/stringstreamenumerator.hpp"
#include "../library/summation.hpp"
#include "../library/seqinfileenumerator.hpp"
#include "../library/linsearch.hpp"

using namespace std;

struct Meccs {
    string hazai, vendeg;
    int hazaiGol, vendegGol;
};

istream &operator>>(istream &is, Meccs &m) {
    return is >> m.hazai >> m.vendeg >> m.hazaiGol >> m.vendegGol;
}

struct Szeria {
    int id;
    int nullasMeccs;
};

class NullasMeccs : public Summation<Meccs, int> {
    virtual int func(const Meccs& e) const override {
        return 1;
    }

    virtual int neutral() const override {
        return 0;
    }

    virtual int add( const int& a, const int& b) const override {
        return a + b;
    }

    virtual bool cond(const Meccs& e) const {
        return e.hazaiGol == 0 && e.vendegGol == 0;
    }
};

istream &operator>>(istream &is, Szeria &s) {
    string line;
    if (getline(is, line)) {
        stringstream ss(line);

        ss >> s.id;

        StringStreamEnumerator<Meccs> enor(ss);

        // hány olyan meccs, ami 0-0
        NullasMeccs pr;
        pr.addEnumerator(&enor);
        pr.run();

        s.nullasMeccs = pr.result();
    }
    return is;
}

class SearchNullasMeccs : public LinSearch<Szeria> {
    virtual bool cond(const Szeria& e) const override {
        return e.nullasMeccs > 3;
    }
};

int main(int argc, char const *argv[])
{
    // Feladat: Első olyan forduló sorszáma, ahol 3-nál több olyan meccs, ahol minden 0 0

    SeqInFileEnumerator<Szeria> enor("input.txt");

    // keresés
    SearchNullasMeccs pr;
    pr.addEnumerator(&enor);
    pr.run();

    if (pr.found())
        cout << pr.elem().id << endl;
    else
        cout << "Nem volt ilyen meccs" << endl;
    
    return 0;
}
