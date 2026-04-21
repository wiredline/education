#ifndef PGCLIENT_H
#define PGCLIENT_H

#include <postgresql/libpq-fe.h>
#include <stdexcept>
#include <string>

class PgClient{
public:
    explicit PgClient(const std::string& conninfo){
        conn_ = PQconnectdb(conninfo.c_str());

        if(PQstatus(conn_) != CONNECTION_OK){
            std::string error = PQerrorMessage(conn_);
            PQfinish(conn_);
            throw std::runtime_error("DB connection failed: " + error);
        }
    }

    ~PgClient(){ //destructor
        if(conn_) PQfinish(conn_);
    }
    PgClient(const PgClient&) = delete;//copy constr
    PgClient& operator=(const PgClient&) =  delete;//copy assig
    PgClient(const PgClient&&) = delete;//move constr
    PgClient& operator=(const PgClient&&)=delete;//move assig

    PGresult* Exec(const std::string& query){
        PGresult* res = PQexec(conn_, query.c_str());
        if(!res){
            throw std::runtime_error("Query execution fail");
        }
        if(PQresultStatus(res) != PGRES_COMMAND_OK&&
        PQresultStatus(res) != PGRES_TUPLES_OK){
            std::string err = PQerrorMessage(conn_);
            PQclear(res);
            throw std::runtime_error(err);
        }
        return res;
    }
    PGconn* GetRaw(){return conn_;}
private:
    PGconn* conn_;
};
#endif