#ifndef		__BARCODE_H__
# define	__BARCODE_H__

#include   <bitset>
#include   <assert.h>
#include   <boost/dynamic_bitset/dynamic_bitset.hpp>

namespace   Component
{
	struct  Base;
};

class	EntityData;

class Barcode
{
public:

	Barcode();
	Barcode(::EntityData &entity);
	Barcode(Barcode &&o);
	Barcode(Barcode &other);
	Barcode &operator=(const Barcode &other);
	~Barcode();
	bool match(boost::dynamic_bitset<> &set);
	bool match(::EntityData &entity);
	bool match(Barcode &entity);
	void reset();
	void add(unsigned int componentId);
	void remove(unsigned int componentId);
	bool isEmpty() const;
	bool isSet(unsigned int componentId) const;

private:
	void applyChange(unsigned int componentId, bool tof)
	{
		if (code_.size() <= componentId)
			code_.resize(componentId + 1);
		code_[componentId] = tof;
	}

	boost::dynamic_bitset<> code_;
};

#endif		//__BARCODE_H__