#include <Render/Properties/Properties.hh>

namespace AGE
{

	Properties::Properties(std::vector<std::shared_ptr<IProperty>> const &properties)
	{
		RWLockGuard lock(_lock, true);
		_properties = properties;
	}

	Properties::Properties(Properties &&other)
	{
		RWLockGuard lock(_lock, true);
		RWLockGuard lockO(other._lock, false);
		_properties = std::move(other._properties);
	}

	Properties::Properties(Properties const &other)
	{
		RWLockGuard lock(_lock, true);
		RWLockGuard lockO(other._lock, false);
		_properties = other._properties;
	}


	Key<Property> Properties::add_property(std::shared_ptr<IProperty> const &prop)
	{
		RWLockGuard lock(_lock, true);
		_properties.emplace_back(prop);
		return (Key<Property>::createKey(int(_properties.size()) - 1));
	}

	void Properties::remove_property(Key<IProperty> const &prop)
	{
		RWLockGuard lock(_lock, true);
		if (prop.getId() != _properties.size() - 1)
		{
			std::swap(_properties[prop.getId()], _properties[_properties.size() - 1]);
		}
		_properties.resize(_properties.size() - 1);
	}

	void Properties::update_properties(std::shared_ptr<Program> const &p) const
	{
		RWLockGuard lock(_lock, false);
		for (auto property : _properties) {
			property->update(p);
		}
	}
}