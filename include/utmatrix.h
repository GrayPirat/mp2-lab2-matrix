﻿// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// utmatrix.h - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (21.04.2015)
//
// Верхнетреугольная матрица - реализация на основе шаблона вектора
#pragma once

#include <iostream>
#include <limits>
#include <algorithm>

struct OutOfIndException {
    int a;
};

struct OverLimitException {
    int b;
};

// Шаблон вектора
template <class T>
class TVector
{
protected:
    T* pVector;
    size_t size;       // размер вектора
    size_t startIndex; // индекс первого элемента вектора
public:
    static const size_t max_size = std::numeric_limits<unsigned int>::max();

    TVector() : size(0), startIndex(0), pVector(nullptr) {}
    TVector(int size, int startIndex = 0);      //конструктор инициализации
    TVector(size_t size, size_t startIndex = 0); //конструктор инициализации

    template <class TSize, class TStartIndex>
    TVector(TSize size, TStartIndex startIndex) = delete;

    TVector(const TVector& v);                // конструктор копирования
    ~TVector();
    size_t getSize() { return size; } // размер вектора
    size_t getStartIndex() { return startIndex; } // индекс первого элемента
    T& getElement(int i);
    T& getElement(size_t i);
    void setElement(int index, T element);
    void setElement(size_t index, T element);

    T& operator[](int pos);                  // доступ
    T& operator[](size_t pos);               // доступ
    bool operator==(const TVector& v) const; // сравнение
    bool operator!=(const TVector& v) const; // сравнение
    TVector& operator=(const TVector& v);    // присваивание


    // скалярные операции
    TVector operator+(const T& val); // прибавить скаляр
    TVector operator-(const T& val); // вычесть скаляр
    TVector operator*(const T& val); // умножить на скаляр

    // векторные операции
    TVector operator+(const TVector& v); // сложение
    TVector operator-(const TVector& v); // вычитание
    T operator*(const TVector& v);       // скалярное произведение

    // ввод-вывод
    friend std::istream& operator>>(std::istream& in, TVector& v)
    {
        return in;
    }
    friend std::ostream& operator<<(std::ostream& out, const TVector& v)
    {
        return out;
    }
};

template <class T>//конструктор инициализации
TVector<T>::TVector(int _size, int _startIndex = 0) : size(_size), startIndex(startIndex)
{
    size = _size;
    startIndex = _startIndex;
    if (size <= 0 || startIndex < 0 || size>max_size || startIndex > size) {
        throw OutOfIndException();
    }
    else {
        pVector = new T[size - startIndex];
        for (int i = 0; i < size - startIndex; i++) {
            pVector[i] = (T)1;
        }
    }
} /*-------------------------------------------------------------------------*/

template <class T>//конструктор инициализации
TVector<T>::TVector(size_t _size, size_t startIndex) : size(_size), startIndex(startIndex)
{
    if (_size > max_size) {
        throw OverLimitException();
    }
    else {
        size = _size;
        startIndex = startIndex;
        pVector = new T[_size - startIndex];
        for (size_t i = 0; i < size - startIndex; i++) {
            pVector[i] = (T)1;
        }
    }
} /*-------------------------------------------------------------------------*/

template <class T> //конструктор копирования
TVector<T>::TVector(const TVector<T>& v)
{
    //delete[] pVector;
    startIndex = v.startIndex;
    size = v.size;
    pVector = new T[v.size];
    if (size > 0)
        for (size_t i = 0; i < size - startIndex; i++) {
            pVector[i] = v.pVector[i];
            
        }
} /*-------------------------------------------------------------------------*/

template <class T> //деструктор
TVector<T>::~TVector()
{
    delete[] pVector;
} /*-------------------------------------------------------------------------*/

template <class T> // доступ
T& TVector<T>::operator[](int pos)
{
    if (pos < 0 || pos >= size) {
        throw OutOfIndException();
    }
    if (pos < startIndex) {
        T temp = 0;
        return temp;
    }
    return pVector[pos - startIndex];
} /*-------------------------------------------------------------------------*/

template <class T> // доступ
T& TVector<T>::operator[](size_t pos)
{
    if (pos < 0 || pos >= size) {
        throw OutOfIndException;
    }
    else if (pos < startIndex) {
        T temp = 0;
        return temp;
    }
    return pVector[pos - startIndex];
} /*-------------------------------------------------------------------------*/

template <class T> // сравнение
bool TVector<T>::operator==(const TVector& v) const
{
    if (this == &v) return true;
    if ((v.size != size) || (v.startIndex != startIndex)) return false;
    for (size_t i = 0; i < v.size - v.startIndex; i++) {
        if (pVector[i] != v.pVector[i]) return false;
    }
    return true;
} /*-------------------------------------------------------------------------*/

template <class T> // сравнение
bool TVector<T>::operator!=(const TVector& v) const
{
    if (*this == v) return false;
    return true;
} /*-------------------------------------------------------------------------*/

template <class T> // присваивание
TVector<T>& TVector<T>::operator=(const TVector& v)
{
    if (this != &v) {
        pVector = new T[v.size - v.startIndex];
        startIndex = v.startIndex;
        size = v.size;
        for (size_t i = 0; i < size - startIndex; i++) {
            pVector[i] = v.pVector[i];
        }
    }
    return *this;
} /*-------------------------------------------------------------------------*/

template <class T> // прибавить скаляр
TVector<T> TVector<T>::operator+(const T& val)
{
    if (val) {
        startIndex = 0;
        for (int i = 0; i < size; i++) {
            pVector[i] += val;
        }
    }
    return *this;
} /*-------------------------------------------------------------------------*/

template <class T> // вычесть скаляр
TVector<T> TVector<T>::operator-(const T& val)
{
    int negval = -val;
    return (*this + negval);
} /*-------------------------------------------------------------------------*/

template <class T> // умножить на скаляр
TVector<T> TVector<T>::operator*(const T& val)
{
    if (val == 0) {
        startIndex = size;
        pVector[size] = 0;
    }
    for (int i = 0; i < size; i++) {
        pVector[i] *= val;
    }
    return *this;
} /*-------------------------------------------------------------------------*/

template <class T> // сложение
TVector<T> TVector<T>::operator+(const TVector<T>& v)
{
    if (size != v.size) {
        throw OutOfIndException();
    }
    else {
        TVector<T> temp(size);
        temp.size = size;
        if (startIndex == v.startIndex) {
            temp.startIndex = startIndex;
            for (int i = 0; i < size-startIndex; i++) {
                temp.pVector[i] = pVector[i] + v.pVector[i];
            }
        }
        else {
            if (startIndex < v.startIndex) {
                temp.startIndex = startIndex;
                for (int i = temp.startIndex; i < v.startIndex; i++) {
                    temp.pVector[i] = pVector[i];
                }
                for (int i = v.startIndex; i < v.size; i++) {
                    temp.pVector[i] = pVector[i] + v.pVector[i];
                }
            }
            else {
                temp.startIndex = v.startIndex;
                for (int i = temp.startIndex; i < startIndex; i++) {
                    temp.pVector[i] = v.pVector[i];
                }
                for (int i = startIndex; i < size; i++) {
                    temp.pVector[i] = pVector[i] + v.pVector[i];
                }
            }
        }
        return temp;
    }
} /*-------------------------------------------------------------------------*/

template <class T> // вычитание
TVector<T> TVector<T>::operator-(const TVector<T>& v)
{
    if (size != v.size) {
        throw OutOfIndException();
    }
    else {
        TVector<T> temp(v.size);
        temp = v;
        temp = temp * (-1);
        return temp + *this;
    }
} /*-------------------------------------------------------------------------*/

template <class T> // скалярное произведение
T TVector<T>::operator*(const TVector<T>& v)
{
    T sum = 0;

    if (size != v.size) {
        throw OutOfIndException();
    }
    else {
        TVector<T> temp(v.size);
        if (startIndex > v.startIndex) temp.startIndex = startIndex;
        else temp.startIndex = v.startIndex;
        for (int i = temp.startIndex; i < size; i++) {
            sum += v.pVector[i] * pVector[i];
        }
        return sum;
    }

} /*-------------------------------------------------------------------------*/

template <class T>
T& TVector<T>::getElement(int index)
{
    if (index < 0 || index >= size) {
        throw OutOfIndException();
    }
    if (index < startIndex) {
        T temp = 0;
        return temp;
    }
    return pVector[index - startIndex];
}

template <class T>
T& TVector<T>::getElement(size_t index)
{
    if (index < 0 || index >= size) {
        throw OutOfIndException();
    }
    if (index < startIndex) {
        T temp = 0;
        return temp;
    }
    return pVector[index - startIndex];
}

template <class T>
void TVector<T>::setElement(int index, T element)
{
    if (index < 0 || index >= size) {
        throw OutOfIndException();
    }
    if (index < startIndex) {
        TVector<T> res((int)size, index);
        res.pVector[0] = element;
        for (int i = 1; i < startIndex - index; i++)
            res.pVector[i] = 0;
        for (int i = startIndex - index; i < size - index; i++)
            res.pVector[i] = pVector[i + index - startIndex];
        *this = res;
    }
    else pVector[index - startIndex] = element;
}

template <class T>
void TVector<T>::setElement(size_t index, T element)
{
    if (index < 0 || index >= size) {
        throw OutOfIndException();
    }
    if (index < startIndex) {
        TVector<T> res((size_t)size, index);
        res.pVector[0] = element;
        for (int i = 1; i < startIndex - index; i++)
            res.pVector[i] = 0;
        for (int i = startIndex - index; i < size - index; i++)
            res.pVector[i] = pVector[i + index - startIndex];
        *this = res;
    }
    else pVector[index - startIndex] = element;
}

// Верхнетреугольная матрица
// val1 val2 val3 ... valn-1 valn
// 0    val2 val3 ... valn-1 valn
// 0    0    val3 ... valn-1 valn
// ..............................
// 0    0    0    ... 0      valn 
template <class T>
class TMatrix : public TVector<TVector<T> >
{
public:
    TMatrix(int s);
    TMatrix(size_t s);

    template <class TSize>
    TMatrix(TSize size) = delete;

    TMatrix(const TMatrix& mt);               // копирование
    TMatrix(const TVector<TVector<T> >& mt);  // преобразование типа
    bool operator==(const TMatrix& mt) const; // сравнение
    bool operator!=(const TMatrix& mt) const; // сравнение
    TMatrix& operator= (const TMatrix& mt);   // присваивание
    TMatrix operator+ (const TMatrix& mt);    // сложение
    TMatrix operator- (const TMatrix& mt);    // вычитание

    // ввод / вывод
    friend std::istream& operator>>(std::istream& in, TMatrix& mt)
    {
        for (int i = 0; i < mt.size; i++)
            in >> mt.pVector[i];
        return in;
    }
    friend std::ostream& operator<<(std::ostream& out, const TMatrix& mt)
    {
        for (int i = 0; i < mt.size; i++)
            out << mt.pVector[i] << std::endl;
        return out;
    }
};

template <class T>
TMatrix<T>::TMatrix(int s) : TVector<TVector<T>>(s)
{
    if (static_cast<unsigned long long>(s * s) > TVector<T>::max_size)
        throw OverLimitException();
    for (int i = 0; i < s; i++)
        TVector<TVector<T>>::pVector[i] = TVector<T>(s, i);
} /*-------------------------------------------------------------------------*/

template <class T> // конструктор копирования
TMatrix<T>::TMatrix(const TMatrix<T>& mt) :TVector<TVector<T>>(mt){}

template <class T> // конструктор преобразования типа
TMatrix<T>::TMatrix(const TVector<TVector<T> >& mt) : TVector<TVector<T> >(mt) {}

template <class T> // сравнение
bool TMatrix<T>::operator==(const TMatrix<T>& mt) const
{
    if (TVector<TVector<T>>::size != mt.size || TVector<TVector<T>>::startIndex != mt.startIndex)
        return false;
    for (int i = 0; i < TVector<TVector<T>>::size; i++) {
        if (TVector<TVector<T>>::pVector[i] != mt.pVector[i])
            return false;
    }
    return true;
} /*-------------------------------------------------------------------------*/

template <class T> // сравнение
bool TMatrix<T>::operator!=(const TMatrix<T>& mt) const
{
    if (*this == mt) return false;
    return true;
} /*-------------------------------------------------------------------------*/

template <class T> // присваивание
TMatrix<T>& TMatrix<T>::operator=(const TMatrix<T>& mt)
{
    if (this == &mt)
        return *this;
    this->size = mt.size;
    this->startIndex = mt.startIndex;
    delete[] this->pVector;
    this->pVector = new TVector<T>[this->size];
    for (int i = 0; i < this->size; i++)
        TVector<TVector<T>>::pVector[i] = mt.pVector[i];
    return *this;
} /*-------------------------------------------------------------------------*/

template <class T> // сложение
TMatrix<T> TMatrix<T>::operator+(const TMatrix<T>& mt)
{
    if (this->size != mt.size || this->startIndex != mt.startIndex) {
        throw OutOfIndException();
    }
    for (int i = 0; i < this->size; i++) {
        TVector<TVector<T>>::pVector[i] = TVector<TVector<T>>::pVector[i] + mt.pVector[i];
        std::cout << TVector<TVector<T>>::pVector[i] << std::endl;
    }
    return *this;
} /*-------------------------------------------------------------------------*/

template <class T> // вычитание
TMatrix<T> TMatrix<T>::operator-(const TMatrix<T>& mt)
{
    if (this->size != mt.size || this->startIndex != mt.startIndex) {
        throw OutOfIndException();
    }
    for (int i = 0; i < this->size; i++)
        TVector<TVector<T>>::pVector[i] = TVector<TVector<T>>::pVector[i] - mt.pVector[i];
    return *this;
} /*-------------------------------------------------------------------------*/