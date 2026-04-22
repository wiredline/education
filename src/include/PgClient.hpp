#ifndef PGCLIENT_H
#define PGCLIENT_H

#include <postgresql/libpq-fe.h>
#include <stdexcept>
#include <string>
#include <memory>

using PGconnPtr = std::unique_ptr<PGconn, decltype(&PQfinish)>;
using PGResultPtr = std::unique_ptr<PGresult, decltype(&PQclear)>;

class PgClient{
public:
    explicit PgClient(const std::string& conninfo)
    :conn_(PQconnectdb(conninfo.c_str()), &PQfinish)
    {
        if (!conn_ || PQstatus(conn_.get()) != CONNECTION_OK) {
            throw std::runtime_error("Connection to db failed");
        }
    }

    ~PgClient() = default; //destructor
    PgClient(const PgClient&) = delete;//copy constr
    PgClient& operator=(const PgClient&)  =  delete;//copy assig
    PgClient(PgClient&&) noexcept = delete;//move constr
    PgClient& operator=(PgClient&&) noexcept = delete;//move assig

    PGResultPtr Exec(const std::string& query){
        PGResultPtr res(PQexec(conn_.get(), query.c_str()), &PQclear);

        if(!res) throw std::runtime_error("Query execution fail");

        auto status = PQresultStatus(res.get());
        if(status != PGRES_COMMAND_OK && status != PGRES_TUPLES_OK) throw std::runtime_error(std::string(PQerrorMessage(conn_.get())));

        return res;
    }
    PGconn* GetRaw(){return conn_.get();}
private:
    PGconnPtr conn_;
};
#endif