#pragma once
#include <postgresql/libpq-fe.h>

class PgResult{
public:
    explicit PgResult(PGresult* res) : res_(res){}

    ~PgResult(){
        if(res_) PQclear(res_);
    }

    PGresult* Get() const {return res_;}
private:
    PGresult* res_;
};