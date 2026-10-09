//
//  Fichier de test pour la classe vecteur<T> (vector.h / VecteurIpl.h)
//
//  Compilation : g++ -std=c++11 -Wall -Wextra main.cpp -o test_vecteur
//

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include "Vecteur.h"

using namespace std;

// ---------------------------------------------------------------
// Mini cadre de test
// ---------------------------------------------------------------
static int g_tests = 0;
static int g_echecs = 0;

#define VERIFIER(cond)                                                     \
    do {                                                                   \
        ++g_tests;                                                         \
        if (!(cond)) {                                                     \
            ++g_echecs;                                                    \
            cout << "  [ECHEC] ligne " << __LINE__ << " : " #cond << endl; \
        }                                                                  \
    } while (0)

#define VERIFIER_EXCEPTION(expr, TypeException)                            \
    do {                                                                   \
        ++g_tests;                                                         \
        bool attrapee = false;                                             \
        try { expr; }                                                      \
        catch (const TypeException&) { attrapee = true; }                  \
        catch (...) {}                                                     \
        if (!attrapee) {                                                   \
            ++g_echecs;                                                    \
            cout << "  [ECHEC] ligne " << __LINE__                         \
                 << " : exception attendue " #TypeException << endl;       \
        }                                                                  \
    } while (0)

static void section(const string& titre)
{
    cout << "\n=== " << titre << " ===" << endl;
}

// Petite classe pour tester avec un type non trivial
struct Point
{
    int x, y;
    Point(int a = 0, int b = 0) : x(a), y(b) {}
    bool operator==(const Point& p) const { return x == p.x && y == p.y; }
};

ostream& operator<<(ostream& out, const Point& p)
{
    return out << "(" << p.x << "," << p.y << ")";
}

// Remplit un vecteur avec 1..n
static vecteur<int> creer(size_t n)
{
    vecteur<int> v;
    for (size_t i = 1; i <= n; ++i)
        v.push_back(static_cast<int>(i));
    return v;
}

// ---------------------------------------------------------------
// Tests
// ---------------------------------------------------------------
static void test_constructeurs()
{
    section("Constructeurs");

    vecteur<int> v1;
    VERIFIER(v1.size() == 0);
    VERIFIER(v1.empty());

    vecteur<int> v2(5);
    VERIFIER(v2.size() == 5);
    VERIFIER(!v2.empty());

    vecteur<int> v3(0);
    VERIFIER(v3.size() == 0);
    VERIFIER(v3.empty());

    vecteur<string> vs(3);
    VERIFIER(vs.size() == 3);
    VERIFIER(vs[0] == "" && vs[1] == "" && vs[2] == "");
}

static void test_copie_et_affectation()
{
    section("Constructeur de copie et affectation");

    vecteur<int> original = creer(5);

    // Copie
    vecteur<int> copie(original);
    VERIFIER(copie.size() == original.size());
    for (size_t i = 0; i < original.size(); ++i)
        VERIFIER(copie[i] == original[i]);

    // Copie profonde : modifier la copie ne change pas l'original
    copie[0] = 999;
    VERIFIER(original[0] == 1);
    VERIFIER(copie[0] == 999);

    // Affectation
    vecteur<int> autre;
    autre = original;
    VERIFIER(autre.size() == 5);
    VERIFIER(autre[4] == 5);
    autre[4] = -1;
    VERIFIER(original[4] == 5);

    // Affectation vers un vecteur non vide, plus grand
    vecteur<int> grand = creer(10);
    grand = original;
    VERIFIER(grand.size() == 5);
    VERIFIER(grand.back() == 5);

    // Auto-affectation
    vecteur<int>& ref = original;
    original = ref;
    VERIFIER(original.size() == 5);
    VERIFIER(original[0] == 1 && original[4] == 5);

    // Affectations chainees
    vecteur<int> a, b;
    a = b = original;
    VERIFIER(a.size() == 5 && b.size() == 5);

    // Copie d'un vecteur vide
    vecteur<int> vide;
    vecteur<int> copieVide(vide);
    VERIFIER(copieVide.empty());
}

static void test_acces()
{
    section("Acces : [], at, front, back");

    vecteur<int> v = creer(4); // 1 2 3 4

    VERIFIER(v[0] == 1);
    VERIFIER(v[3] == 4);
    VERIFIER(v.at(0) == 1);
    VERIFIER(v.at(3) == 4);
    VERIFIER(v.front() == 1);
    VERIFIER(v.back() == 4);

    // Modification via les references
    v[1] = 20;
    v.at(2) = 30;
    v.front() = 10;
    v.back() = 40;
    VERIFIER(v[0] == 10 && v[1] == 20 && v[2] == 30 && v[3] == 40);

    // Version const
    const vecteur<int>& cv = v;
    VERIFIER(cv[1] == 20);
    VERIFIER(cv.at(2) == 30);
    VERIFIER(cv.front() == 10);
    VERIFIER(cv.back() == 40);

    // at() hors bornes
    VERIFIER_EXCEPTION(v.at(4), out_of_range);
    VERIFIER_EXCEPTION(v.at(1000), out_of_range);
    VERIFIER_EXCEPTION(cv.at(4), out_of_range);

    vecteur<int> vide;
    VERIFIER_EXCEPTION(vide.at(0), out_of_range);
}

static void test_push_pop()
{
    section("push_back et pop_back");

    vecteur<int> v;
    for (int i = 0; i < 100; ++i)
    {
        v.push_back(i);
        VERIFIER(v.size() == static_cast<size_t>(i + 1));
        VERIFIER(v.back() == i);
    }
    VERIFIER(v.front() == 0);
    bool contenuOk = true;
    for (int i = 0; i < 100; ++i)
        if (v[i] != i) contenuOk = false;
    VERIFIER(contenuOk);

    v.pop_back();
    VERIFIER(v.size() == 99);
    VERIFIER(v.back() == 98);

    while (!v.empty())
        v.pop_back();
    VERIFIER(v.size() == 0);
    VERIFIER(v.empty());

    // Reutilisation apres avoir tout retire
    v.push_back(7);
    VERIFIER(v.size() == 1);
    VERIFIER(v.front() == 7 && v.back() == 7);

    // push_back d'un element du vecteur lui-meme (alias lors d'une reallocation)
    vecteur<int> w = creer(1);
    for (int i = 0; i < 20; ++i)
        w.push_back(w[0]);
    VERIFIER(w.size() == 21);
    VERIFIER(w[20] == 1);

    // Avec des chaines
    vecteur<string> vs;
    vs.push_back("allo");
    vs.push_back("le");
    vs.push_back("monde");
    VERIFIER(vs.size() == 3);
    VERIFIER(vs[0] == "allo" && vs[2] == "monde");
}

static void test_resize()
{
    section("resize");

    vecteur<int> v = creer(5); // 1..5

    v.resize(10);
    VERIFIER(v.size() == 10);
    VERIFIER(v[0] == 1 && v[4] == 5); // contenu conserve

    v.resize(3);
    VERIFIER(v.size() == 3);
    VERIFIER(v[0] == 1 && v[2] == 3);
    VERIFIER(v.back() == 3);

    v.resize(3); // meme taille
    VERIFIER(v.size() == 3);
    VERIFIER(v[1] == 2);

    v.resize(0);
    VERIFIER(v.size() == 0);
    VERIFIER(v.empty());

    v.resize(4);
    VERIFIER(v.size() == 4);

    vecteur<string> vs;
    vs.resize(3);
    VERIFIER(vs.size() == 3);
    vs[1] = "x";
    vs.resize(5);
    VERIFIER(vs[1] == "x");
}

static void test_reserve()
{
    section("reserve");

    vecteur<int> v = creer(3);

    v.reserve(100);
    VERIFIER(v.size() == 3);           // reserve ne change pas la taille
    VERIFIER(v[0] == 1 && v[2] == 3);  // contenu conserve

    for (int i = 4; i <= 100; ++i)
        v.push_back(i);
    VERIFIER(v.size() == 100);
    VERIFIER(v.back() == 100);

    v.reserve(5);                      // reserve plus petite : aucun effet
    VERIFIER(v.size() == 100);
    VERIFIER(v[50] == 51);

    vecteur<int> vide;
    vide.reserve(10);
    VERIFIER(vide.size() == 0);
    VERIFIER(vide.empty());
    vide.push_back(1);
    VERIFIER(vide.front() == 1);
}

static void test_clear()
{
    section("clear");

    vecteur<int> v = creer(10);
    v.clear();
    VERIFIER(v.size() == 0);
    VERIFIER(v.empty());

    v.clear(); // clear sur un vecteur deja vide
    VERIFIER(v.empty());

    v.push_back(42);
    VERIFIER(v.size() == 1);
    VERIFIER(v[0] == 42);
}

static void test_swap()
{
    section("swap");

    vecteur<int> a = creer(3);
    vecteur<int> b = creer(6);

    a.swap(b);
    VERIFIER(a.size() == 6);
    VERIFIER(b.size() == 3);
    VERIFIER(a.back() == 6);
    VERIFIER(b.back() == 3);

    vecteur<int> vide;
    a.swap(vide);
    VERIFIER(a.empty());
    VERIFIER(vide.size() == 6);
}

static void test_iterateurs()
{
    section("Iterateurs");

    vecteur<int> v = creer(5); // 1..5

    // begin / end
    VERIFIER(v.begin() != v.end());
    VERIFIER(*v.begin() == 1);

    // Parcours en lecture
    int somme = 0;
    for (auto it = v.begin(); it != v.end(); ++it)
        somme += *it;
    VERIFIER(somme == 15);

    // Parcours avec ecriture
    for (auto it = v.begin(); it != v.end(); ++it)
        *it *= 2;
    VERIFIER(v[0] == 2 && v[4] == 10);

    // for-range
    somme = 0;
    for (int x : v)
        somme += x;
    VERIFIER(somme == 30);

    // Post/pre incrementation et decrementation
    auto it = v.begin();
    VERIFIER(*it++ == 2);
    VERIFIER(*it == 4);
    VERIFIER(*++it == 6);
    VERIFIER(*it-- == 6);
    VERIFIER(*it == 4);
    VERIFIER(*--it == 2);

    // Egalite
    auto it1 = v.begin();
    auto it2 = v.begin();
    VERIFIER(it1 == it2);
    ++it2;
    VERIFIER(it1 != it2);

    // Vecteur vide
    vecteur<int> vide;
    VERIFIER(vide.begin() == vide.end());

    // Iterateurs constants
    const vecteur<int>& cv = v;
    somme = 0;
    for (auto cit = cv.begin(); cit != cv.end(); ++cit)
        somme += *cit;
    VERIFIER(somme == 30);

    auto cit = cv.begin();
    VERIFIER(*cit == 2);
    ++cit;
    VERIFIER(*cit == 4);
    --cit;
    VERIFIER(*cit == 2);
    VERIFIER(cv.begin() != cv.end());

    const vecteur<int> cvide;
    VERIFIER(cvide.begin() == cvide.end());

    // Avec un type non trivial
    vecteur<Point> vp;
    vp.push_back(Point(1, 2));
    vp.push_back(Point(3, 4));
    int sx = 0;
    for (auto p = vp.begin(); p != vp.end(); ++p)
        sx += (*p).x;
    VERIFIER(sx == 4);
}

static void test_affichage()
{
    section("operator<< et afficher");

    vecteur<int> v = creer(3);
    ostringstream oss;
    oss << v;
    VERIFIER(oss.str() == "[1,2,3]");

    vecteur<int> vide;
    ostringstream oss2;
    oss2 << vide;
    VERIFIER(oss2.str() == "[]");

    vecteur<string> vs;
    vs.push_back("a");
    vs.push_back("b");
    ostringstream oss3;
    oss3 << vs;
    VERIFIER(oss3.str() == "[a,b]");

    // Affichage visuel de l'etat interne (aucune verification automatique)
    cout << "  Etat interne d'un vecteur de 3 elements avec reserve :" << endl;
    v.reserve(8);
    v.afficher(cout);
}

static void test_type_personnalise()
{
    section("Type personnalise (Point)");

    vecteur<Point> v;
    v.push_back(Point(1, 1));
    v.push_back(Point(2, 2));
    v.push_back(Point(3, 3));

    VERIFIER(v.size() == 3);
    VERIFIER(v.front() == Point(1, 1));
    VERIFIER(v.back() == Point(3, 3));
    VERIFIER(v[1] == Point(2, 2));

    vecteur<Point> copie(v);
    copie[1] = Point(9, 9);
    VERIFIER(v[1] == Point(2, 2));
    VERIFIER(copie[1] == Point(9, 9));

    v.resize(5);
    VERIFIER(v.size() == 5);
    VERIFIER(v[3] == Point(0, 0)); // construction par defaut
}

static void test_gros_volume()
{
    section("Gros volume");

    vecteur<int> v;
    const int N = 100000;
    for (int i = 0; i < N; ++i)
        v.push_back(i);
    VERIFIER(v.size() == static_cast<size_t>(N));
    VERIFIER(v.front() == 0);
    VERIFIER(v.back() == N - 1);
    VERIFIER(v[N / 2] == N / 2);

    vecteur<int> copie(v);
    VERIFIER(copie.size() == v.size());
    VERIFIER(copie[N - 1] == N - 1);

    v.clear();
    VERIFIER(v.empty());
    VERIFIER(copie.size() == static_cast<size_t>(N));
}

// ---------------------------------------------------------------
// Programme principal
// ---------------------------------------------------------------
int main()
{
    cout << "Tests de la classe vecteur<T>" << endl;

    test_constructeurs();
    test_copie_et_affectation();
    test_acces();
    test_push_pop();
    test_resize();
    test_reserve();
    test_clear();
    test_swap();
    test_iterateurs();
    test_affichage();
    test_type_personnalise();
    test_gros_volume();

    cout << "\n------------------------------------" << endl;
    cout << g_tests - g_echecs << " / " << g_tests << " tests reussis" << endl;

    if (g_echecs == 0)
        cout << "Tous les tests sont passes." << endl;
    else
        cout << g_echecs << " test(s) en echec." << endl;

    return g_echecs == 0 ? 0 : 1;
}