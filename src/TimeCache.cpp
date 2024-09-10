#include "TimeCache.h"

TimeCache::TimeCache()
{}

bool TimeCache::isExpired(const CacheData& cacheData)
{
    return std::chrono::system_clock::now() > cacheData.expirationTime;
}

bool TimeCache::set(int key, int value, int durationInMilliseconds)
{
    const auto cachedItem = theCache.find(key);
    if(cachedItem == theCache.end())
    {
        // not in the map, add it
        theCache.try_emplace
        (
            key, 
            value, 
            std::chrono::system_clock::now() + std::chrono::milliseconds(durationInMilliseconds)
        );
        return false;
    }
    else
    {
        const bool expired = isExpired(cachedItem->second);
        cachedItem->second.value = value;
        cachedItem->second.expirationTime = std::chrono::system_clock::now() + std::chrono::milliseconds(durationInMilliseconds);
        return !expired;
    }
}

int TimeCache::get(int key)
{
    const auto cachedItem = theCache.find(key);
    if(cachedItem == theCache.end())
    {
        return -1;
    }
    else
    {
        if(isExpired(cachedItem->second))
        {
            // delete it
            theCache.erase(cachedItem);
            return -1;
        }
        else
        {
            return cachedItem->second.value;
        }
    }
}

int TimeCache::count()
{
    int count = 0;
    for(auto cachedItem = theCache.begin(); cachedItem != theCache.end(); )
    {
        if(isExpired(cachedItem->second))
        {
            cachedItem = theCache.erase(cachedItem);
        }
        else
        {
            ++cachedItem;
            ++count;
        }
    }
    return count;
}