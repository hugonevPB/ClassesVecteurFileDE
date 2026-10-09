//
// Created by hugo on 2026-10-09.
//
#pragma once

#ifndef CODE_VECTEURIPL_H
#define CODE_VECTEURIPL_H


template <typename T>
void vecteur<T>::clear()
{
    delete[] m_debut;
    m_debut = m_finCap = m_finDim = nullptr;
}

template <typename T>
T& vecteur<T>::operator[](size_t i)
{
    return m_debut[i];
}
template <typename T>
const T& vecteur<T>::operator[](size_t i) const
{
    return m_debut[i];
}

template <typename T>
T& vecteur<T>::at(size_t i)
{
    if (i >= size())
        throw std::out_of_range("vecteur::at : indice hors bornes");
    return m_debut[i];
}
template <typename T>
const T& vecteur<T>::at(size_t i) const
{
    if (i >= size())
        throw std::out_of_range("vecteur::at : indice hors bornes");
    return m_debut[i];
}

template <typename T>
void vecteur<T>::push_back(const T& t)
{
    if (m_finCap > m_finDim)
    {
        *m_finDim = t;
        ++m_finDim;
    }
    else
    {
        //besoin de reserve pour cela
    }
}

template <typename T>
void vecteur<T>::pop_back()
{
    if (m_finDim > m_debut)
        --m_finDim;
}



#endif //CODE_VECTEURIPL_H
