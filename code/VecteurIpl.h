//
// Created by hugo on 2026-10-09.
//
#pragma once

#ifndef CODE_VECTEURIPL_H
#define CODE_VECTEURIPL_H

template <typename T>
void vecteur<T>::reserve(size_t cap)
{
    if (cap <= m_finCap - m_debut)
        return;

    size_t dim = size();
    T* debut = new T[cap];
    for (size_t i = 0; i < dim; ++i)
        debut[i] = m_debut[i];

    delete [] m_debut;
    m_debut = debut;
    m_finDim = m_debut + dim;
    m_finCap = m_debut + cap;
}

template <typename T>
void vecteur<T>::resize(size_t dim)
{
    if (dim > m_finCap - m_debut)
        reserve(dim);

    T placeHolder = T();

    for (size_t i = size(); i < dim; ++i)
        m_debut[i] = placeHolder;

    m_finDim = m_debut + dim;
}

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
        throw std::out_of_range("Objet hors bornes");
    return m_debut[i];
}
template <typename T>
const T& vecteur<T>::at(size_t i) const
{
    if (i >= size())
        throw std::out_of_range("Objet hors bornes");
    return m_debut[i];
}

template <typename T>
void vecteur<T>::push_back(const T& t)
{
    if (m_finCap > m_finDim)
    {
        *m_finDim = t;
    }
    else
    {
        T copie = t;
        size_t cap = m_finCap - m_debut;
        if (cap == 0)
            reserve(1);
        else
            reserve(cap + 2);
        *m_finDim = copie;
    }
    ++m_finDim;
}

template <typename T>
void vecteur<T>::pop_back()
{
    if (m_finDim > m_debut)
        --m_finDim;
}


template <typename T>
typename vecteur<T>::iterateur vecteur<T>::begin()
{
    return iterateur(m_debut);
}

template <typename T>
typename vecteur<T>::iterateur vecteur<T>::end()
{
    return iterateur(m_finDim);
}

template <typename T>
typename vecteur<T>::const_iterateur vecteur<T>::begin() const
{
    return const_iterateur(m_debut);
}

template <typename T>
typename vecteur<T>::const_iterateur vecteur<T>::end() const
{
    return const_iterateur(m_finDim);
}

template <typename T>
T& vecteur<T>::iterateur::operator*()
{
    return *m_pointeur;
}

template <typename T>
const T& vecteur<T>::const_iterateur::operator*() const
{
    return *m_pointeur;
}


#endif //CODE_VECTEURIPL_H
