//
//  Fichier de test pour les classes vecteur<T> (vector.h) et fileDE<T> (FileDe.h)
//
//  Compilation : g++ -std=c++11 -Wall -Wextra main.cpp -o tests
//  Avec detection d'erreurs memoire : ajouter -fsanitize=address -g
//
//  Pour ne tester qu'une seule des deux classes (par exemple tant que
//  FileDeIpl.h n'est pas ecrit), mettre l'autre constante a 0.
//

#define TESTER_VECTEUR 1
#define TESTER_FILEDE  0

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

#if TESTER_VECTEUR
#include "vecteur.h"
#endif

#if TESTER_FILEDE
#include "FileDe.h"
#endif

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

// ===============================================================
//                         vecteur<T>
// ===============================================================
#if TESTER_VECTEUR

// Remplit un vecteur avec 1..n
static vecteur<int> creerVecteur(size_t n)
{
    vecteur<int> v;
    for (size_t i = 1; i <= n; ++i)
        v.push_back(static_cast<int>(i));
    return v;
}

static void test_vecteur_constructeurs()
{
    section("vecteur : constructeurs");

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

static void test_vecteur_copie_et_affectation()
{
    section("vecteur : copie et affectation");

    vecteur<int> original = creerVecteur(5);

    vecteur<int> copie(original);
    VERIFIER(copie.size() == original.size());
    for (size_t i = 0; i < original.size(); ++i)
        VERIFIER(copie[i] == original[i]);

    // Copie profonde
    copie[0] = 999;
    VERIFIER(original[0] == 1);
    VERIFIER(copie[0] == 999);

    vecteur<int> autre;
    autre = original;
    VERIFIER(autre.size() == 5);
    VERIFIER(autre[4] == 5);
    autre[4] = -1;
    VERIFIER(original[4] == 5);

    vecteur<int> grand = creerVecteur(10);
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

    vecteur<int> vide;
    vecteur<int> copieVide(vide);
    VERIFIER(copieVide.empty());
}

static void test_vecteur_acces()
{
    section("vecteur : acces ([], at, front, back)");

    vecteur<int> v = creerVecteur(4); // 1 2 3 4

    VERIFIER(v[0] == 1);
    VERIFIER(v[3] == 4);
    VERIFIER(v.at(0) == 1);
    VERIFIER(v.at(3) == 4);
    VERIFIER(v.front() == 1);
    VERIFIER(v.back() == 4);

    v[1] = 20;
    v.at(2) = 30;
    v.front() = 10;
    v.back() = 40;
    VERIFIER(v[0] == 10 && v[1] == 20 && v[2] == 30 && v[3] == 40);

    const vecteur<int>& cv = v;
    VERIFIER(cv[1] == 20);
    VERIFIER(cv.at(2) == 30);
    VERIFIER(cv.front() == 10);
    VERIFIER(cv.back() == 40);

    VERIFIER_EXCEPTION(v.at(4), out_of_range);
    VERIFIER_EXCEPTION(v.at(1000), out_of_range);
    VERIFIER_EXCEPTION(cv.at(4), out_of_range);

    vecteur<int> vide;
    VERIFIER_EXCEPTION(vide.at(0), out_of_range);
}

static void test_vecteur_push_pop()
{
    section("vecteur : push_back et pop_back");

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

    v.push_back(7);
    VERIFIER(v.size() == 1);
    VERIFIER(v.front() == 7 && v.back() == 7);

    // push_back d'un element du vecteur lui-meme (alias lors d'une reallocation)
    vecteur<int> w = creerVecteur(1);
    for (int i = 0; i < 20; ++i)
        w.push_back(w[0]);
    VERIFIER(w.size() == 21);
    VERIFIER(w[20] == 1);

    vecteur<string> vs;
    vs.push_back("allo");
    vs.push_back("le");
    vs.push_back("monde");
    VERIFIER(vs.size() == 3);
    VERIFIER(vs[0] == "allo" && vs[2] == "monde");
}

static void test_vecteur_resize()
{
    section("vecteur : resize");

    vecteur<int> v = creerVecteur(5); // 1..5

    v.resize(10);
    VERIFIER(v.size() == 10);
    VERIFIER(v[0] == 1 && v[4] == 5);
    VERIFIER(v[5] == 0 && v[9] == 0); // nouveaux elements : valeur par defaut

    v.resize(3);
    VERIFIER(v.size() == 3);
    VERIFIER(v[0] == 1 && v[2] == 3);
    VERIFIER(v.back() == 3);

    // Agrandir apres avoir reduit : les anciennes valeurs ne doivent pas reapparaitre
    v.resize(5);
    VERIFIER(v.size() == 5);
    VERIFIER(v[3] == 0 && v[4] == 0);

    v.resize(5);
    VERIFIER(v.size() == 5);

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

static void test_vecteur_reserve()
{
    section("vecteur : reserve");

    vecteur<int> v = creerVecteur(3);

    v.reserve(100);
    VERIFIER(v.size() == 3);
    VERIFIER(v[0] == 1 && v[2] == 3);

    for (int i = 4; i <= 100; ++i)
        v.push_back(i);
    VERIFIER(v.size() == 100);
    VERIFIER(v.back() == 100);

    VERIFIER_EXCEPTION(v.reserve(5), out_of_range);
    VERIFIER(v.size() == 100);
    VERIFIER(v[50] == 51);

    vecteur<int> vide;
    vide.reserve(10);
    VERIFIER(vide.size() == 0);
    VERIFIER(vide.empty());
    vide.push_back(1);
    VERIFIER(vide.front() == 1);
}

static void test_vecteur_clear_swap()
{
    section("vecteur : clear et swap");

    vecteur<int> v = creerVecteur(10);
    v.clear();
    VERIFIER(v.size() == 0);
    VERIFIER(v.empty());

    v.clear();
    VERIFIER(v.empty());

    v.push_back(42);
    VERIFIER(v.size() == 1);
    VERIFIER(v[0] == 42);

    vecteur<int> a = creerVecteur(3);
    vecteur<int> b = creerVecteur(6);
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

static void test_vecteur_iterateurs()
{
    section("vecteur : iterateurs");

    vecteur<int> v = creerVecteur(5); // 1..5

    VERIFIER(v.begin() != v.end());
    VERIFIER(*v.begin() == 1);

    int somme = 0;
    for (auto it = v.begin(); it != v.end(); ++it)
        somme += *it;
    VERIFIER(somme == 15);

    for (auto it = v.begin(); it != v.end(); ++it)
        *it *= 2;
    VERIFIER(v[0] == 2 && v[4] == 10);

    somme = 0;
    for (int x : v)
        somme += x;
    VERIFIER(somme == 30);

    auto it = v.begin();
    VERIFIER(*it++ == 2);
    VERIFIER(*it == 4);
    VERIFIER(*++it == 6);
    VERIFIER(*it-- == 6);
    VERIFIER(*it == 4);
    VERIFIER(*--it == 2);

    auto it1 = v.begin();
    auto it2 = v.begin();
    VERIFIER(it1 == it2);
    ++it2;
    VERIFIER(it1 != it2);

    vecteur<int> vide;
    VERIFIER(vide.begin() == vide.end());

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

    vecteur<Point> vp;
    vp.push_back(Point(1, 2));
    vp.push_back(Point(3, 4));
    int sx = 0;
    for (auto p = vp.begin(); p != vp.end(); ++p)
        sx += (*p).x;
    VERIFIER(sx == 4);
}

static void test_vecteur_affichage()
{
    section("vecteur : operator<< et afficher");

    vecteur<int> v = creerVecteur(3);
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

    cout << "  Etat interne d'un vecteur de 3 elements avec reserve :" << endl;
    v.reserve(8);
    v.afficher(cout);
}

static void test_vecteur_type_personnalise()
{
    section("vecteur : type personnalise (Point)");

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
    VERIFIER(v[3] == Point(0, 0));
}

static void test_vecteur_gros_volume()
{
    section("vecteur : gros volume");

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

static void tester_vecteur()
{
    cout << "\n######## Tests de vecteur<T> ########";
    test_vecteur_constructeurs();
    test_vecteur_copie_et_affectation();
    test_vecteur_acces();
    test_vecteur_push_pop();
    test_vecteur_resize();
    test_vecteur_reserve();
    test_vecteur_clear_swap();
    test_vecteur_iterateurs();
    test_vecteur_affichage();
    test_vecteur_type_personnalise();
    test_vecteur_gros_volume();
}

#endif // TESTER_VECTEUR

// ===============================================================
//                          fileDE<T>
// ===============================================================
#if TESTER_FILEDE

// Cree une file contenant 1..n (via push_back)
static fileDE<int> creerFile(size_t n)
{
    fileDE<int> f;
    for (size_t i = 1; i <= n; ++i)
        f.push_back(static_cast<int>(i));
    return f;
}

// Verifie que la file contient exactement les valeurs attendues, dans l'ordre
static bool contient(const fileDE<int>& f, const int attendu[], size_t n)
{
    if (f.size() != n)
        return false;
    for (size_t i = 0; i < n; ++i)
        if (f[i] != attendu[i])
            return false;
    return true;
}

static void test_file_constructeurs()
{
    section("fileDE : constructeurs");

    fileDE<int> f1;
    VERIFIER(f1.size() == 0);
    VERIFIER(f1.empty());

    fileDE<int> f2(5);
    VERIFIER(f2.size() == 5);
    VERIFIER(!f2.empty());
    bool zeros = true;
    for (size_t i = 0; i < 5; ++i)
        if (f2[i] != 0) zeros = false;
    VERIFIER(zeros); // elements construits par defaut

    fileDE<int> f3(0);
    VERIFIER(f3.size() == 0);
    VERIFIER(f3.empty());

    fileDE<string> fs(3);
    VERIFIER(fs.size() == 3);
    VERIFIER(fs[0] == "" && fs[1] == "" && fs[2] == "");
}

static void test_file_copie_et_affectation()
{
    section("fileDE : copie et affectation");

    fileDE<int> original = creerFile(5);

    fileDE<int> copie(original);
    VERIFIER(copie.size() == original.size());
    for (size_t i = 0; i < original.size(); ++i)
        VERIFIER(copie[i] == original[i]);

    // Copie profonde (la file contient des pointeurs : les elements ne
    // doivent surtout pas etre partages)
    copie[0] = 999;
    VERIFIER(original[0] == 1);
    VERIFIER(copie[0] == 999);

    fileDE<int> autre;
    autre = original;
    VERIFIER(autre.size() == 5);
    VERIFIER(autre[4] == 5);
    autre[4] = -1;
    VERIFIER(original[4] == 5);

    fileDE<int> grande = creerFile(10);
    grande = original;
    VERIFIER(grande.size() == 5);
    VERIFIER(grande[4] == 5);

    // Auto-affectation
    fileDE<int>& ref = original;
    original = ref;
    VERIFIER(original.size() == 5);
    VERIFIER(original[0] == 1 && original[4] == 5);

    // Copie d'une file vide
    fileDE<int> vide;
    fileDE<int> copieVide(vide);
    VERIFIER(copieVide.empty());

    // Copie d'une file dont le debut a "tourne" dans le tampon circulaire
    fileDE<int> tournee = creerFile(5);
    tournee.pop_front();
    tournee.pop_front();
    tournee.push_back(6);
    tournee.push_back(7);       // 3 4 5 6 7
    fileDE<int> copieTournee(tournee);
    const int att[] = {3, 4, 5, 6, 7};
    VERIFIER(contient(copieTournee, att, 5));
}

static void test_file_acces()
{
    section("fileDE : acces ([] et at)");

    fileDE<int> f = creerFile(4); // 1 2 3 4

    VERIFIER(f[0] == 1);
    VERIFIER(f[3] == 4);
    VERIFIER(f.at(0) == 1);
    VERIFIER(f.at(3) == 4);

    f[1] = 20;
    f.at(2) = 30;
    VERIFIER(f[1] == 20 && f[2] == 30);

    const fileDE<int>& cf = f;
    VERIFIER(cf[1] == 20);
    VERIFIER(cf.at(2) == 30);

    VERIFIER_EXCEPTION(f.at(4), out_of_range);
    VERIFIER_EXCEPTION(f.at(1000), out_of_range);
    VERIFIER_EXCEPTION(cf.at(4), out_of_range);

    fileDE<int> vide;
    VERIFIER_EXCEPTION(vide.at(0), out_of_range);
}

static void test_file_push_pop_back()
{
    section("fileDE : push_back et pop_back");

    fileDE<int> f;
    for (int i = 0; i < 100; ++i)
    {
        f.push_back(i);
        VERIFIER(f.size() == static_cast<size_t>(i + 1));
        VERIFIER(f[f.size() - 1] == i);
    }
    VERIFIER(f[0] == 0);

    bool contenuOk = true;
    for (int i = 0; i < 100; ++i)
        if (f[i] != i) contenuOk = false;
    VERIFIER(contenuOk);

    f.pop_back();
    VERIFIER(f.size() == 99);
    VERIFIER(f[f.size() - 1] == 98);

    while (!f.empty())
        f.pop_back();
    VERIFIER(f.size() == 0);
    VERIFIER(f.empty());

    f.push_back(7);
    VERIFIER(f.size() == 1);
    VERIFIER(f[0] == 7);

    // push_back d'un element de la file elle-meme
    fileDE<int> g = creerFile(1);
    for (int i = 0; i < 20; ++i)
        g.push_back(g[0]);
    VERIFIER(g.size() == 21);
    VERIFIER(g[20] == 1);

    fileDE<string> fs;
    fs.push_back("allo");
    fs.push_back("le");
    fs.push_back("monde");
    VERIFIER(fs.size() == 3);
    VERIFIER(fs[0] == "allo" && fs[2] == "monde");
}

static void test_file_push_pop_front()
{
    section("fileDE : push_front et pop_front");

    fileDE<int> f;
    for (int i = 0; i < 100; ++i)
    {
        f.push_front(i);
        VERIFIER(f.size() == static_cast<size_t>(i + 1));
        VERIFIER(f[0] == i);
    }
    // Le dernier ajoute est au debut : 99 98 ... 0
    bool contenuOk = true;
    for (int i = 0; i < 100; ++i)
        if (f[i] != 99 - i) contenuOk = false;
    VERIFIER(contenuOk);

    f.pop_front();
    VERIFIER(f.size() == 99);
    VERIFIER(f[0] == 98);

    while (!f.empty())
        f.pop_front();
    VERIFIER(f.size() == 0);
    VERIFIER(f.empty());

    f.push_front(5);
    VERIFIER(f.size() == 1);
    VERIFIER(f[0] == 5);

    fileDE<int> g = creerFile(1);
    for (int i = 0; i < 20; ++i)
        g.push_front(g[0]);
    VERIFIER(g.size() == 21);
    VERIFIER(g[0] == 1 && g[20] == 1);

    fileDE<string> fs;
    fs.push_front("monde");
    fs.push_front("le");
    fs.push_front("allo");
    VERIFIER(fs[0] == "allo" && fs[1] == "le" && fs[2] == "monde");
}

static void test_file_double_extremite()
{
    section("fileDE : melange des deux extremites");

    fileDE<int> f;
    f.push_back(1);
    f.push_back(2);
    f.push_back(3);
    f.push_front(0);
    f.push_front(-1);
    const int att1[] = {-1, 0, 1, 2, 3};
    VERIFIER(contient(f, att1, 5));

    f.pop_front();
    f.pop_back();
    const int att2[] = {0, 1, 2};
    VERIFIER(contient(f, att2, 3));

    // Faire grandir la file alors que le debut est decale dans le tampon
    // (force reserve() a recopier correctement un tampon circulaire)
    for (int i = 10; i < 30; ++i)
        f.push_back(i);
    for (int i = -1; i > -20; --i)
        f.push_front(i);

    bool ok = (f.size() == 3 + 20 + 19);
    int attendu = -19;
    for (size_t i = 0; ok && i < 19; ++i, ++attendu)
        if (f[i] != attendu) ok = false;
    // 0 1 2
    for (int v = 0; ok && v <= 2; ++v, ++attendu)
        if (f[19 + v] != v) ok = false;
    VERIFIER(ok);
    VERIFIER(f[22] == 10);
    VERIFIER(f[f.size() - 1] == 29);

    // Utilisation en file (FIFO) : l'indice de debut doit "tourner" sans fin
    fileDE<int> q;
    q.push_back(0);
    q.push_back(1);
    q.push_back(2);
    bool fifoOk = true;
    for (int i = 3; i < 1000; ++i)
    {
        if (q[0] != i - 3) fifoOk = false;
        q.pop_front();
        q.push_back(i);
        if (q.size() != 3) fifoOk = false;
    }
    VERIFIER(fifoOk);
    const int attQ[] = {997, 998, 999};
    VERIFIER(contient(q, attQ, 3));

    // Utilisation en pile par l'avant (LIFO)
    fileDE<int> pile;
    for (int i = 0; i < 50; ++i)
        pile.push_front(i);
    bool lifoOk = true;
    for (int i = 49; i >= 0; --i)
    {
        if (pile[0] != i) lifoOk = false;
        pile.pop_front();
    }
    VERIFIER(lifoOk);
    VERIFIER(pile.empty());
}

static void test_file_resize()
{
    section("fileDE : resize");

    fileDE<int> f = creerFile(5); // 1..5

    f.resize(10);
    VERIFIER(f.size() == 10);
    VERIFIER(f[0] == 1 && f[4] == 5);
    VERIFIER(f[5] == 0 && f[9] == 0); // nouveaux elements : valeur par defaut

    f.resize(3);
    VERIFIER(f.size() == 3);
    const int att[] = {1, 2, 3};
    VERIFIER(contient(f, att, 3));

    f.resize(3);
    VERIFIER(f.size() == 3);
    VERIFIER(f[1] == 2);

    f.resize(0);
    VERIFIER(f.size() == 0);
    VERIFIER(f.empty());

    f.resize(4);
    VERIFIER(f.size() == 4);

    // resize alors que le debut est decale dans le tampon
    fileDE<int> g = creerFile(5);
    g.pop_front();
    g.pop_front();
    g.push_back(6);                 // 3 4 5 6
    g.resize(8);
    VERIFIER(g.size() == 8);
    VERIFIER(g[0] == 3 && g[3] == 6);
    VERIFIER(g[4] == 0 && g[7] == 0);
    g.resize(2);
    const int att2[] = {3, 4};
    VERIFIER(contient(g, att2, 2));

    fileDE<string> fs;
    fs.resize(3);
    VERIFIER(fs.size() == 3);
    fs[1] = "x";
    fs.resize(5);
    VERIFIER(fs[1] == "x");
}

static void test_file_clear_swap()
{
    section("fileDE : clear et swap");

    fileDE<int> f = creerFile(10);
    f.clear();
    VERIFIER(f.size() == 0);
    VERIFIER(f.empty());

    f.clear(); // clear sur une file deja vide
    VERIFIER(f.empty());

    // La file reste utilisable apres clear
    f.push_back(42);
    VERIFIER(f.size() == 1);
    VERIFIER(f[0] == 42);
    f.push_front(41);
    VERIFIER(f[0] == 41 && f[1] == 42);

    fileDE<int> a = creerFile(3);
    fileDE<int> b = creerFile(6);
    a.swap(b);
    VERIFIER(a.size() == 6);
    VERIFIER(b.size() == 3);
    VERIFIER(a[5] == 6);
    VERIFIER(b[2] == 3);

    fileDE<int> vide;
    a.swap(vide);
    VERIFIER(a.empty());
    VERIFIER(vide.size() == 6);
}

static void test_file_affichage()
{
    section("fileDE : operator<< et afficher");

    fileDE<int> f = creerFile(3);
    ostringstream oss;
    oss << f;
    VERIFIER(oss.str() == "[1,2,3]");

    // Meme resultat si le debut est decale dans le tampon
    f.push_front(0);
    f.pop_back();
    ostringstream oss2;
    oss2 << f;
    VERIFIER(oss2.str() == "[0,1,2]");

    fileDE<int> vide;
    ostringstream oss3;
    oss3 << vide;
    VERIFIER(oss3.str() == "[]");

    fileDE<string> fs;
    fs.push_back("a");
    fs.push_back("b");
    ostringstream oss4;
    oss4 << fs;
    VERIFIER(oss4.str() == "[a,b]");

    cout << "  Etat interne d'une fileDE (avec decalage du debut) :" << endl;
    f.afficher(cout);
}

static void test_file_type_personnalise()
{
    section("fileDE : type personnalise (Point)");

    fileDE<Point> f;
    f.push_back(Point(2, 2));
    f.push_back(Point(3, 3));
    f.push_front(Point(1, 1));

    VERIFIER(f.size() == 3);
    VERIFIER(f[0] == Point(1, 1));
    VERIFIER(f[1] == Point(2, 2));
    VERIFIER(f[2] == Point(3, 3));

    fileDE<Point> copie(f);
    copie[1] = Point(9, 9);
    VERIFIER(f[1] == Point(2, 2));
    VERIFIER(copie[1] == Point(9, 9));

    f.resize(5);
    VERIFIER(f.size() == 5);
    VERIFIER(f[3] == Point(0, 0));
}

static void test_file_gros_volume()
{
    section("fileDE : gros volume");

    fileDE<int> f;
    const int N = 50000;
    for (int i = 0; i < N; ++i)
    {
        f.push_back(i);
        f.push_front(-i - 1);
    }
    VERIFIER(f.size() == static_cast<size_t>(2 * N));
    VERIFIER(f[0] == -N);
    VERIFIER(f[f.size() - 1] == N - 1);
    VERIFIER(f[N] == 0);

    fileDE<int> copie(f);
    VERIFIER(copie.size() == f.size());
    VERIFIER(copie[0] == -N);
    VERIFIER(copie[copie.size() - 1] == N - 1);

    f.clear();
    VERIFIER(f.empty());
    VERIFIER(copie.size() == static_cast<size_t>(2 * N));
}

static void tester_file()
{
    cout << "\n######## Tests de fileDE<T> ########";
    test_file_constructeurs();
    test_file_copie_et_affectation();
    test_file_acces();
    test_file_push_pop_back();
    test_file_push_pop_front();
    test_file_double_extremite();
    test_file_resize();
    test_file_clear_swap();
    test_file_affichage();
    test_file_type_personnalise();
    test_file_gros_volume();
}

#endif // TESTER_FILEDE

// ---------------------------------------------------------------
// Programme principal
// ---------------------------------------------------------------
int main()
{
#if TESTER_VECTEUR
    tester_vecteur();
#endif
#if TESTER_FILEDE
    tester_file();
#endif

    cout << "\n------------------------------------" << endl;
    cout << g_tests - g_echecs << " / " << g_tests << " tests reussis" << endl;

    if (g_echecs == 0)
        cout << "Tous les tests sont passes." << endl;
    else
        cout << g_echecs << " test(s) en echec." << endl;

    return g_echecs == 0 ? 0 : 1;
}