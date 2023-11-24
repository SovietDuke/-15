#pragma once

#include <string.h>

#define NAMELENGTH 10  // �����c����� ����� �����

typedef char FruitName[NAMELENGTH]; // ��� ��� ������

class HashTable { // ��� �������
public:

    HashTable();
    ~HashTable();
    void add(FruitName fr_name, int fr_count);
    void del(FruitName fr_name);
    int find(FruitName fr_name);
private:

    enum enPairStatus {
        free,
        engaged,
        deleted
    };

    struct Pair { // ���� ����-��������

        Pair() :
            fruit_name(""),
            fruit_count(-1),
            status(enPairStatus::free)
        {}
        Pair(FruitName fr_name, int fr_count) :
            fruit_count(fr_count),
            status(enPairStatus::engaged) {
            memcpy(fruit_name, fr_name, NAMELENGTH);
        }
        Pair& operator = (const Pair& other) {
            fruit_count = other.fruit_count;
            memcpy(fruit_name, other.fruit_name, NAMELENGTH);
            status = other.status;

            return *this;
        }

        bool operator == (const Pair& other) {
            return  status == other.status &&
                (status != enPairStatus::engaged || (fruit_count == other.fruit_count && !strcmp(fruit_name, other.fruit_name)));
        }

        FruitName fruit_name;
        int fruit_count;

        enPairStatus status;
    };

    int hash_func(FruitName fr_name, int offset);

    Pair* array;
    int mem_size;
    int count;
};