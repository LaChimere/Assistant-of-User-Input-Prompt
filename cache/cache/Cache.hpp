#ifndef __WD_LRUCACHE_H__
#define __WD_LRUCACHE_H__
#include <iostream>
#include <list>
#include <unordered_map>
using namespace std;

class LRUCache
{
public:
	using CacheNode = pair<string, string>;
	using iterator = list<CacheNode>::iterator;

	LRUCache(size_t capacity = 1000)
		: _capacity(capacity), _updateCount(0), _updateStatus(false)
	{
	}

	void init(const string &filename); //从文件中初始化

	string get(const string &key);

	void set(const string &key, const string &value);

	size_t updateCount() const { return _updateCount; }

	void update(LRUCache &cache);

	void clearUpdateData();

private:
	size_t _capacity;	 //能容纳的总记录数
	size_t _updateCount; //待更新的记录数
	bool _updateStatus;
	unordered_map<string, iterator> _cacheMap;
	list<CacheNode> _dataList;
	list<CacheNode> _pendingUpdateData; //待更新的数据
};

#endif
