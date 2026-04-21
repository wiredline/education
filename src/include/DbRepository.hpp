#pragma once

#include <PgClient.hpp>
#include <PgResult.hpp>
#include <string>
#include <memory>
#include <vector>
#include <tuple>

class DbRepository {
public:
    explicit DbRepository(PgClient& db): db_(db){}
    void InsertOperation(const std::string& json,
                         const std::string& key,
                         int result,
                         int status){
    std::string res = std::to_string(result);
    std::string st = std::to_string(status);
    const char* values[4] = {
        json.c_str(),
        key.c_str(),
        res.c_str(),
        st.c_str()
    };
    
    PGresult* raw = PQexecParams(
        db_.GetRaw(),
            "INSERT INTO operations (request, normalized_key, result, status) "
            "VALUES ($1, $2, $3, $4) "
            "ON CONFLICT (normalized_key) DO NOTHING;",
            4,
            nullptr,
            values,
            nullptr,
            nullptr,
            0
        );
    if(!raw) throw std::runtime_error("insertion failed");
    PgResult res_guard(raw); //RAII cleaner
    if(PQresultStatus(raw) != PGRES_COMMAND_OK) throw std::runtime_error(PQerrorMessage(db_.GetRaw()));
}
    std::vector<std::tuple<std::string, int , int>> LoadAll(){
        PGresult* raw = PQexec(
            db_.GetRaw(), 
            "SELECT normalized_key, result, status FROM operations;"
        );

        if(!raw){
            throw std::runtime_error("SELECT failed");
        }
        PgResult res_guard(raw);

        if(PQresultStatus(raw)!= PGRES_TUPLES_OK){
            throw std::runtime_error(PQerrorMessage(db_.GetRaw()));
        }
        int rows = PQntuples(raw);

        std::vector<std::tuple<std::string, int , int>> data;
        data.reserve(rows);

        for(int i =0; i<rows;++i){
            std::string key = PQgetvalue(raw, i, 0);
            int result = std::stoi(PQgetvalue(raw, i, 1));
            int status = std::stoi(PQgetvalue(raw, i, 2));

            data.emplace_back(key, result, status);
        }
        return data;
    }
private:
    PgClient& db_;
};