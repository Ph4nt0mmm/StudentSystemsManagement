#ifndef BACKEND_H
#define BACKEND_H


#include "auth.h"

class Backend {
private:
    Database db;
    Session currentSession;

public:
    Backend();
    Session login(std::wstring user, std::wstring pass);
    void logout();
    Database* getDB();
    void saveData();
};

#endif