#include "chat.h"
#include <iostream>
#include <string>

Chat::Chat() {
    data_count = 0;
    mem_size = 8;
    array = new AuthData[mem_size];
}
Chat::~Chat() {
    delete[] array;
}
void Chat::reg(char _login[LOGINLENGTH], char _pass[], int pass_length) {
    array[data_count++] = AuthData(_login, sha1(_pass, pass_length));
}
bool Chat::login(char _login[LOGINLENGTH], char _pass[], int pass_length) {
    int i = 0;
    for (; i < data_count; i++) {
        AuthData& ad = array[i];
        if (!strcmp(ad.login, _login)) {
            break;
        }
    }
    if (i >= data_count) return false;

    uint* digest = sha1(_pass, pass_length);

    bool cmpHashes = !memcmp(
        array[i].pass_sha1_hash,
        digest,
        SHA1HASHLENGTHBYTES);
    delete[] digest;

    return cmpHashes;
}
void Chat::add(char _login[LOGINLENGTH], uint* pass_sha1_hash) {
    int index = -1, i = 0;
    // берем пробы по всем i от 0 до размера массива
    for (; i < mem_size; i++) {
        index = hash_func(_login, i);
        if (array[index].status == enPairStatus::free) {
            // найдена пустая ячейка, занимаем ее
            break;
        }
    }
    if (i >= mem_size) return; // все перебрали, нет места

    // кладем в свободную ячейку пару
    array[index] = AuthData(_login, pass_sha1_hash);
    data_count++;
}
int Chat::hash_func(char _login[LOGINLENGTH], int offset) {
    // вычисляем индекс
    int sum = 0, i = 0;
    for (; i < LOGINLENGTH; i++) {
        sum += _login[i];
    }
    // линейные пробы
    return (sum % mem_size + offset * offset) % mem_size;
}
void Chat::del(char _login[LOGINLENGTH]) {
    int index = -1, i = 0;
    // берем пробы по всем i от 0 до размера массива
    for (; i < mem_size; i++) {
        index = hash_func(_login, i);
        if (array[index].status == enPairStatus::engaged &&
            !strcmp(array[index].login, _login)) {
            array[index].status = enPairStatus::deleted;
            data_count--;
            return;
        }
        else if (array[index].status == enPairStatus::free) {
            return;
        }
    }
}
uint* Chat::find(char _login[LOGINLENGTH]) {

    for (int i = 0; i < mem_size; i++) {
        int index = hash_func(_login, i);
        if (array[index].status == enPairStatus::engaged &&
            !strcmp(array[index].login, _login)) {
            return array[index].pass_sha1_hash;
        }
        else if (array[index].status == enPairStatus::free) {
            return nullptr;
        }
    }
    return nullptr;
}