#include "hash_table.h"

HashTable::HashTable() {
    count = 0;
    mem_size = 8;
    array = new Pair[mem_size];
}
HashTable::~HashTable() {
    delete[] array;
}
void HashTable::add(FruitName fr_name, int fr_count) {
    int index = -1, i = 0;
    // берем пробы по всем i от 0 до размера массива
    for (; i < mem_size; i++) {
        index = hash_func(fr_name, i);
        if (array[index].status == enPairStatus::free) {
            // найдена пустая ячейка, занимаем ее
            break;
        }
    }
    if (i >= mem_size) return; // все перебрали, нет места

    // кладем в свободную ячейку пару
    array[index] = Pair(fr_name, fr_count);
    count++;
}
int HashTable::hash_func(FruitName fr_name, int offset) {
    // вычисляем индекс
    int sum = 0, i = 0;
    for (; i < NAMELENGTH; i++) {
        sum += fr_name[i];
    }
    // линейные пробы
    return (sum % mem_size + offset * offset) % mem_size;
}
void HashTable::del(FruitName fr_name) {
    int index = -1, i = 0;
    // берем пробы по всем i от 0 до размера массива
    for (; i < mem_size; i++) {
        index = hash_func(fr_name, i);
        if (array[index].status == enPairStatus::engaged &&
            !strcmp(array[index].fruit_name, fr_name)) {
            array[index].status = enPairStatus::deleted;
            count--;
            return;
        }
        else if (array[index].status == enPairStatus::free) {
            return;
        }
    }
}
int HashTable::find(FruitName fr_name) {

    for (int i = 0; i < mem_size; i++) {
        int index = hash_func(fr_name, i);
        if (array[index].status == enPairStatus::engaged &&
            !strcmp(array[index].fruit_name, fr_name)) {
            return array[index].fruit_count;
        }
        else if (array[index].status == enPairStatus::free) {
            return -1;
        }
    }
    return -1;
}