//
// Created by hugo on 2026-10-11.
//

#ifndef CODE_FILEDEIPL_H
#define CODE_FILEDEIPL_H
#include "FileDe.h"

template<typename  T>
T& fileDE<T>::operator[](size_t index)
{
    return *m_debut[(m_zero + index) % m_cap];
}

template<typename  T>
const T& fileDE<T>::operator[](size_t index) const
{
    return *m_debut[(m_zero + index) % m_cap];
}

template<typename  T>
T& fileDE<T>::at(size_t index)
{
    if (index >= m_taille)
        throw std::out_of_range("Objet hors bornes");

    return *m_debut[(m_zero + index) % m_cap];
}

template<typename  T>
const T& fileDE<T>::at(size_t index) const
{
    if (index >= m_taille)
        throw std::out_of_range("Objet hors bornes");

    return *m_debut[(m_zero + index) % m_cap];
}

#endif //CODE_FILEDEIPL_H
