// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


template <class T> class List;


template <class T>
class ListElement
{
friend class List<T>;

public:

    ListElement(T *v) : value(v), next(nullptr), prev(nullptr) {}

    ~ListElement();

    T *Get();
    // ���������� ��������� �� ��������� �������
    ListElement *Next();
private:

    T* value;
    // ��������� �� ������ - ��������
    //List<T> *owningList;
    // ��������� �� ��������� �������. 0, ���� ��� ��� ��������� ������� � ������
    ListElement *next;
    // ��������� �� ���������� �������. 0, ���� ��� ������ ������� � ������
    ListElement *prev;
};



template <class T>
class List
{
public:
    List();
    // ��������� ����� ������� � ����� ������
    void Append(T *element);
    // ������� ������� �� ������
    void Remove(T *element);
    // ���������� true, ���� ������ �������� element
    bool IsMember(T *element);
    // ���������� ���������
    ListElement<T> *First();
    // ���������� ������
    uint Size() const;

    void Clear();

private:
    // ���������� ��������� �� ��������� ������� ������
    ListElement<T> *Last();
    // ��������� �� ������ ������� ������. 0, ���� ������ ����
    ListElement<T> *head;
};
