#ifndef _OPTIONAL_H_
#define _OPTIONAL_H_

#include <functional>
#include <cstdint>
#include "../Platform.h"

namespace impl
{
	template <typename T>
	struct OptionalHolder
	{
		typedef T ValueType;
		ValueType _data;

		OptionalHolder(const T& value) : _data(value) {}
		OptionalHolder(T&& value) : _data(std::move(value)) {}

		const T& Get() const { return _data; }
		T& Get() { return _data; }
	};

	template <typename T>
	struct OptionalHolder<T&>
	{
		typedef std::reference_wrapper<T> ValueType;
		ValueType _data;

		OptionalHolder(T& value) : _data(value) {}

		const T& Get() const { return _data.get(); }
		T& Get() { return _data.get(); }
	};

	template <typename T>
	struct OptionalHolder<T*>
	{
		T& Get() { static_assert(false, "Usage of pointer inside optional is redundant"); }
	};
}

template <typename T>
class Optional
{
public:
	typedef typename std::remove_reference<T>::type& RefType;
	typedef typename std::remove_reference<T>::type const& ConstRefType;

	Optional() : _inited(false) {}
	
	explicit Optional(const T& value) : _inited(true)
	{
		Construct(value);
	}

	Optional(const Optional& rhs) : _inited(rhs._inited)
	{
		if (_inited)
		{
			Construct(rhs.GetHolder()->Get());
		}
	}

	Optional& operator=(const Optional& rhs)
	{
		Destruct();
		_inited = rhs._inited;
		if (_inited)
		{
			Construct(*rhs);
		}
		return *this;
	}

	Optional& operator=(T&& value)
	{
		Destruct();
		_inited = true;
		Emplace(std::move(value));
		return *this;
	}

	~Optional()
	{
		Destruct();
	}

	operator bool() const
	{
		return _inited;
	}

	RefType getVal()
	{
		ASSERT(_inited);
		return GetHolder()->Get();
	}

	ConstRefType getVal() const
	{
		ASSERT(_inited);
		return GetHolder()->Get();
	}

	RefType operator*()
	{
		return getVal();
	}

	ConstRefType operator*() const
	{
		return getVal();
	}

private:
	typedef impl::OptionalHolder<T> ValueType;

	ValueType* GetHolder() { return reinterpret_cast<ValueType*>(_storage); }
	const ValueType* GetHolder() const { return reinterpret_cast<const ValueType*>(_storage); }

	void Construct(const T& data)
	{
		new (_storage) ValueType(data);
	}

	void Emplace(T&& data)
	{
		new (_storage) ValueType(std::move(data));
	}

	void Destruct()
	{
		if (_inited)
		{
			GetHolder()->~ValueType();
		}
	}

	uint8_t _storage[sizeof(ValueType)];
	bool _inited;
};

#endif // _OPTIONAL_H_