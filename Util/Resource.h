#ifndef _RESOURCE_ID_H_
#define _RESOURCE_ID_H_

#include <string>

template <typename Trait>
class ResourceId
{
public:
	typedef std::string IdType;

	ResourceId() : _id() {}
	ResourceId(IdType id) : _id(id) {}
	
	const IdType& getId() const { return _id; }
	bool isValid() const { return !_id.empty(); }
	bool operator== (const ResourceId& rhs) const { return _id == rhs._id; }

private:
	IdType _id;
};

#endif // _RESOURCE_ID_H_