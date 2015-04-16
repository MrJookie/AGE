#include "BulletCollisionManager.hpp"
#include <Utils/OldFile.hpp>
#include <bullet/src/Serialize/BulletWorldImporter/btBulletWorldImporter.h>

namespace AGE
{
	std::shared_ptr<btCollisionShape> BulletCollisionManager::loadShape(const std::string &_path)
	{
		OldFile filePath(_assetsDirectory + _path);
		if (!filePath.exists())
		{
			std::cerr << "Bullet file not found" << std::endl;
			return nullptr;
		}
		if (_collisionShapes.find(filePath.getFullName()) != std::end(_collisionShapes))
			return _collisionShapes[filePath.getFullName()];
		btBulletWorldImporter import(0);
		auto loadStatus = import.loadFile(filePath.getFullName().c_str());
		if (loadStatus != true)
		{
			std::cerr << "Bullet importer cannot open file." << std::endl;
			return nullptr;
		}
		int n = import.getNumCollisionShapes();
		if (n <= 0)
		{
			std::cerr << "Bullet file is not correct. No collision box inside." << std::endl;
			return nullptr;
		}
		auto o = import.getCollisionShapeByIndex(0);
		auto shape = std::shared_ptr<btCollisionShape>(static_cast<btCollisionShape*>(o));
		_collisionShapes.insert(std::make_pair(filePath.getFullName(), shape));
		return shape;
	}
}