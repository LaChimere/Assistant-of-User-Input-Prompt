#include "Cache.hpp"
#include <fstream>
#include <sstream>

void LRUCache::init(const string &filename) //从文件中初始化
{
	ifstream ifs(filename);
	if (!ifs.good())
	{
		cerr << ">>> " << filename << " not found!" << endl;
		return;
	}

	string line, key, value;
	while (getline(ifs, line))
	{
		istringstream iss(line);
		ifs >> key >> value;
		set(key, value);
	}
	ifs.close();

	//初始化完毕,将更新标识位打开
	_updateStatus = true;
}

string LRUCache::get(const string &key)
{
	auto iter = _cacheMap.find(key);
	if (iter == _cacheMap.end())
		return string();
	else
	{
		_dataList.splice(_dataList.begin(), _dataList, iter->second);
		_cacheMap[key] = _dataList.begin();
		return _cacheMap[key]->second;
	}
}

void LRUCache::set(const string &key, const string &value)
{
	auto iter = _cacheMap.find(key);
	if (iter == _cacheMap.end())
	{
		//新节点
		if (_dataList.size() == _capacity)
		{
			//删除链表最末尾的元素
			_cacheMap.erase(_dataList.back().first);
			_dataList.pop_back();
		}
		//插入新节点到链表头部
		_dataList.push_front(make_pair(key, value));
		_cacheMap[key] = _dataList.begin();

		//为多个cache的更新准备的
		if (_updateStatus)
		{
			++_updateCount;
			_pendingUpdateData.push_front(make_pair(key, value));
		}
	}
	else
	{
		//更新节点数据
		if (_cacheMap[key]->second != value)
		{
			_cacheMap[key]->second = value;
		}
		_dataList.splice(_dataList.begin(), _dataList, iter->second);
		_cacheMap[key] = _dataList.begin();
	}
}

void LRUCache::update(LRUCache &cache)
{
	if (cache._pendingUpdateData.size() > 0)
	{
		for (auto &node : cache._pendingUpdateData)
			set(node.first, node.second);
	}
}

void LRUCache::clearUpdateData()
{
	_updateCount = 0;
	_pendingUpdateData.clear();
}
