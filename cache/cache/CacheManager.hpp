#ifndef __WD_CACHEMANAGER_H__
#define __WD_CACHEMANAGER_H__

#include "Cache.hpp"
#include <vector>

class CacheManager
{
public:
	static CacheManager *getInstance()
	{
		if (nullptr == _pInstance)
		{
			_pInstance = new CacheManager();
		}
		return _pInstance;
	}

	static void destroy()
	{
		if (_pInstance)
			delete _pInstance;
	}

	void init(size_t cacheSize, const string &filename);

	LRUCache &getCache(size_t idx) { return _caches[idx]; }

	void periodicUpdateCaches();

private:
	static CacheManager *_pInstance;
	vector<LRUCache> _caches;
};

#endif
