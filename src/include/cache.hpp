#pragma once

#include <unordered_map>
#include <string>
#include <optional>

struct CacheValue{
    int result;
    int status;
};

class Cache{
public:
    bool Exists(const std::string& key) const{
        return map_.find(key) != map_.end();
    }
    std::optional<CacheValue> Get(const std::string& key) const {
        auto it = map_.find(key);
        if(it != map_.end()){
            return it->second;
        }
        return std::nullopt;
    }
    void Put(const std::string& key, const CacheValue& value){
        map_[key]=value;
    }

private:
    std::unordered_map<std::string, CacheValue> map_;
};