//
// Created by hugo on 2026-10-11.
//

#ifndef CODE_FILEDEIPL_H
#define CODE_FILEDEIPL_H
#include "FileDe.h"

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
#endif //CODE_FILEDEIPL_H
