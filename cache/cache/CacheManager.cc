#include "CacheManager.hpp"

CacheManager *CacheManager::_pInstance = nullptr;

void CacheManager::init(size_t cacheSize, const string &filename)
{
	_caches.reserve(cacheSize);

	//初始化一个LRUCache
	LRUCache cache;
	cache.init(filename);

	//复制N个Cache
	for (size_t idx = 0; idx < cacheSize; ++idx)
	{
		_caches.push_back(cache);
	}
}

void CacheManager::periodicUpdateCaches()
{
	size_t cnt = 0;
	for (auto &cache : _caches)
		cnt += cache.updateCount();
	if (cnt == 0)
	{
		cout << ">> 不需要进行更新" << endl;
		return;
	}
	else
	{
		cout << ">> " << cnt << "条记录需要更新" << endl;
	}

	//先将其他Cache中的新记录添加到0号Cache
	for (size_t idx = 1; idx != _caches.size(); ++idx)
	{
		_caches[0].update(_caches[idx]);
	}

	//再将0号Cache中的新记录添加到其他Cache之中
	for (size_t idx = 1; idx != _caches.size(); ++idx)
	{
		_caches[idx].update(_caches[0]);
	}

	//最后清空所有Cache中的待更新数据
	for (size_t idx = 0; idx != _caches.size(); ++idx)
	{
		_caches[idx].clearUpdateData();
	}
}
