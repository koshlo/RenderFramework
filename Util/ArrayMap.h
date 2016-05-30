#pragma once

#include <vector>
#include <algorithm>
#include <cstdint>
#include "../Platform.h"

template <typename K, typename V>
class ArrayMap
{
public:
	typedef std::vector<K> KeyList;
	typedef std::vector<V> ValueList;
	typedef typename KeyList::const_iterator KeyIterator;

	const V& operator[](const K& key) const
	{
		ASSERT(_keys.size() == _values.size());
		return getValue(getIterator(key));
	}

	KeyIterator getIterator(const K& key) const
	{
		return std::find(_keys.cbegin(), _keys.cend(), key);
	}

	bool isInRange(KeyIterator& it) const
	{
		return it != _keys.cend();
	}

	const V& getValue(KeyIterator& it) const
	{
		return _values[indexOf(it)];
	}

	V& add(const K& key, const V& value)
	{
		ASSERT(_keys.size() == _values.size());
		_keys.push_back(key);
		_values.push_back(value);
		return _values.back();
	}

	const ValueList& values() const { return _values; }

private:	
	uint32_t indexOf(KeyIterator& it) const
	{
		ASSERT(isInRange(it));
		KeyList::difference_type index = std::distance(_keys.cbegin(), it);
		ASSERT(index >= 0 && static_cast<typename ValueList::size_type>(index) < _values.size());
		return index;
	}

	KeyList _keys;
	ValueList _values;
};