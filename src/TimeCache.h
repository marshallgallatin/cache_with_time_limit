// See https://leetcode.com/problems/cache-with-time-limit/description/ for implementation requirements

#pragma once

#include <map>
#include <chrono>

class TimeCache
{
    public:
        TimeCache();

        bool set(int key, int value, int durationInMilliseconds);

        int get(int key);

        int count();

    private:
        struct CacheData
        {
            CacheData(int value, std::chrono::time_point<std::chrono::system_clock> expirationTime) :
                value(value),
                expirationTime(expirationTime)
            {}

            int value;
            std::chrono::time_point<std::chrono::system_clock> expirationTime;
        };

        bool isExpired(const CacheData& cacheData);

        std::map<int, CacheData> theCache;
};