#pragma once

#include "sha1.h"
#include <string>

#define SIZE 10
#define LOGINLENGTH 10

class Chat {
public:
    Chat();
    ~Chat();
    void reg(char _login[LOGINLENGTH], char _pass[], int pass_length);
    bool login(char _login[LOGINLENGTH], char _pass[], int pass_length);
    void add(char _login[LOGINLENGTH], uint* pass_sha1_hash);
    void del(char _login[LOGINLENGTH]);
    uint* find(char _login[LOGINLENGTH]);
private:
    enum enPairStatus {
        free,
        engaged,
        deleted
    };

    struct AuthData {
        AuthData(char _login[LOGINLENGTH], uint* sh1) :
            pass_sha1_hash(sh1),
            status(enPairStatus::engaged)
        {
            memcpy(login, _login, LOGINLENGTH);
        }
        AuthData() :
            login(""),
            pass_sha1_hash(0),
            status(enPairStatus::free)
        {}
        ~AuthData() {
            if (pass_sha1_hash != 0)
                delete[] pass_sha1_hash;
        }
        AuthData& operator = (const AuthData& other) {
            memcpy(login, other.login, LOGINLENGTH);

            if (pass_sha1_hash != 0)
                delete[] pass_sha1_hash;
            pass_sha1_hash = new uint[SHA1HASHLENGTHUINTS];

            memcpy(pass_sha1_hash, other.pass_sha1_hash, SHA1HASHLENGTHBYTES);
            
            status = other.status;

            return *this;
        }
        bool operator == (const AuthData& other) {
            return  status == other.status &&
                (status != enPairStatus::engaged || (pass_sha1_hash == other.pass_sha1_hash && !strcmp(login, other.login)));
        }
        char login[LOGINLENGTH];
        uint* pass_sha1_hash;
        enPairStatus status;
    };

    int hash_func(char _login[LOGINLENGTH], int offset);

    AuthData* array;
    int data_count;
    int mem_size;
};
