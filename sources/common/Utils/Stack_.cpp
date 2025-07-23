// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Stack_.h"
#include <cstdlib>
#include <cstring>


template Stack<int8>::Stack(int);
template Stack<int8>::~Stack();
template int8 Stack<int8>::Pop();
template void Stack<int8>::Push(int8);
template int Stack<int8>::Size() const;


template <typename T>
Stack<T>::Stack(int _size) : buffer(nullptr), size(_size), numElements(0U)
{
    buffer = static_cast<T *>(std::malloc(sizeof(T) * (uint)_size));
}


template <typename T>
Stack<T>::~Stack()
{
    std::free(buffer);
}


template <typename T>
void Stack<T>::Push(T elem)
{
    if(numElements < size)
    {
        buffer[numElements] = elem;
        numElements++;
    }
    else
    {
        std::memcpy(buffer, buffer + 1, (uint)(numElements - 1) * sizeof(T));
        buffer[numElements - 1] = elem;
    }
}


template <typename T>
T Stack<T>::Pop()
{
    if(numElements != 0)
    {
        numElements--;
        return buffer[numElements];
    }

    return (T)0;
}


template <typename T>
int Stack<T>::Size() const
{
    return numElements;
}


template <typename T>
bool Stack<T>::Empty() const
{
    return Size() == 0;
}


template <typename T>
int Stack<T>::NumFirstZeros() const
{
    int result = 0;

    for(int i = 0; i < size; i++)
    {
        if(buffer[i] != 0)
        {
            break;
        }
        result++;
    }

    return result;
}


bool ConverterToInteger::IsInterger() const
{
    for (int i = 0; i < Size(); i++)
    {
        if ((*this)[i] > '9' || (*this)[i] < '0')
        {
            return false;
        }
    }

    return Size() != 0;
}


int ConverterToInteger::GetValue() const
{
    int result = 0;

    int pow = 1;

    for (int i = Size() - 1; i >= 0; i--)
    {
        result += ((*this)[i] - 0x30) * pow;
        pow *= 10;
    }

    return result;
}

