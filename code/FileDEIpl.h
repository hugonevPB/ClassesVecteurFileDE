//
// Created by hugo on 2026-10-11.
//

#ifndef CODE_FILEDEIPL_H
#define CODE_FILEDEIPL_H
#include "FileDe.h"

/*
Question: Il n'y a pas d'itérateurs pour cette structure, savez-vous pourquoi ?
La raison de pourquoi nous n'avons pas d'iterateur dans cette structure est a cause que le dernier element de cette structure peut se trouver au millieu de la file.
par exemple, si nous avons les element 0 a 4 en position 7 a 11 du tableau, si nous faisions iterateur++ pour acceder au prochain element, l'element 5 qui se trouve
a la position 0 du tableau, nous n'allons pas le trouver, car allors l'iterateur allait pointer vers la position 12 du tableau, qui n'existe pas.
Un iterateur ne fonctionerait pas dans le cas de cette classe, vue que la fileDE est un tableau circulaire, donc les elements peuvent ne pas etre contigues dans la memoire
contrairement aux Vecteurs.
 */

//Get un element de la file sans verification des bornes
template<typename  T>
T& fileDE<T>::operator[](size_t index)
{
    // (m_zero + index) % cap: cela marche vue que dans la file nous pouvons rajouter des element au debut de la file
    //donc cette formule nous permets d'aller chercher le bon element quand m_zero n'est pas au debut de la file
    return *m_debut[(m_zero + index) % m_cap];
}

//Get un element de la file sans verification des bornes en const
template<typename  T>
const T& fileDE<T>::operator[](size_t index) const
{
    return *m_debut[(m_zero + index) % m_cap];
}

//Get un element de la file avec verification des bornes
template<typename  T>
T& fileDE<T>::at(size_t index)
{
    if (index >= m_taille)
        throw std::out_of_range("Objet hors bornes");

    return *m_debut[(m_zero + index) % m_cap];
}

//Get un element de la file avec verification des bornes en const
template<typename  T>
const T& fileDE<T>::at(size_t index) const
{
    if (index >= m_taille)
        throw std::out_of_range("Objet hors bornes");

    return *m_debut[(m_zero + index) % m_cap];
}

//Constructeur qui fait une copie profonde d'une autre File
template<typename  T>
fileDE<T>::fileDE(const fileDe& source) : fileDE(source.m_taille)
{
    for (size_t i = 0; i < source.m_taille; ++i)
        *m_debut[i] = source[i];
}

//Augmente la capacite maximale de la file
template <typename T>
void fileDE<T>::reserve(size_t maxCap)
{
    if (maxCap <= m_cap)
        throw std::out_of_range("Capacite invalide");

    T** debut = new T*[maxCap];
    for (size_t i = 0; i < m_taille; ++i)
        debut[i] = m_debut[(m_zero + i) % m_cap];

    delete[] m_debut;
    m_debut = debut;
    m_cap = maxCap;
    m_zero = 0; // ? comment faire pour que le zero soit a la meme place que dans le tableau d'avant
}

//Chanfe la taille de la file, reduit ou augmente et, si necessaire, reserve de l'espace (augmente la capacite)
template <typename T>
void fileDE<T>::resize(size_t dim)
{
    //reduire la taille
    while (m_taille > dim)
    {
        delete m_debut[(m_zero + m_taille - 1) % m_cap];
        --m_taille;
    }

    //reserver si necessaire
    if (dim > m_cap)
        reserve(dim);

    //augmenter la taille
    while (m_taille < dim)
    {
        m_debut[(m_zero + m_taille) % m_cap] = new T();
        ++m_taille;
    }
}
#endif //CODE_FILEDEIPL_H
