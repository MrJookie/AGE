#include <Core/AssetsManager.hpp>
#include <Skinning/Skeleton.hpp>
#include <Skinning/Animation.hpp>
#include <Geometry/Mesh.hpp>
#include <Geometry/Material.hpp>
#include <Texture/Texture.hpp>

namespace AGE
{
	std::shared_ptr<MaterialSetInstance> AssetsManager::loadMaterial(const File &filePath)
	{
		if (_materials.find(filePath.getFullName()) != std::end(_materials))
			return _materials[filePath.getFullName()];
		if (!filePath.exists())
		{
			std::cerr << "AssetsManager : File [" << filePath.getFullName() << "] does not exists." << std::endl;
			assert(false);
		}

		MaterialDataSet data;
		auto material = std::make_shared<MaterialSetInstance>();

		std::ifstream ifs(filePath.getFullName(), std::ios::binary);
		cereal::PortableBinaryInputArchive ar(ifs);
		ar(data);

		for (auto &e : data.collection)
		{
			// TODO fill material with material key
		}

		_materials.insert(std::make_pair(filePath.getFullName(), material));
		return material;
	}

	std::shared_ptr<TextureInstance> AssetsManager::loadTexture(const File &filePath)
	{
		if (_textures.find(filePath.getFullName()) != std::end(_textures))
			return _textures[filePath.getFullName()];
		if (!filePath.exists())
		{
			std::cerr << "AssetsManager : File [" << filePath.getFullName() << "] does not exists." << std::endl;
			assert(false);
		}

		TextureData data;
		auto texture = std::make_shared<TextureInstance>();

		std::ifstream ifs(filePath.getFullName(), std::ios::binary);
		cereal::PortableBinaryInputArchive ar(ifs);
		ar(data);

		// TODO fill texture with texture key

		_textures.insert(std::make_pair(filePath.getFullName(), texture));
		return texture;
	}

	std::shared_ptr<Animation> AssetsManager::loadAnimation(const File &filePath)
	{
		if (_animations.find(filePath.getFullName()) != std::end(_animations))
			return _animations[filePath.getFullName()];
		if (!filePath.exists())
		{
			std::cerr << "AssetsManager : File [" << filePath.getFullName() << "] does not exists." << std::endl;
			assert(false);
		}

		auto animation = std::make_shared<Animation>();

		std::ifstream ifs(filePath.getFullName(), std::ios::binary);
		cereal::PortableBinaryInputArchive ar(ifs);
		ar(*animation.get());
		_animations.insert(std::make_pair(filePath.getFullName(), animation));
		return animation;
	}

	std::shared_ptr<Skeleton> AssetsManager::loadSkeleton(const File &filePath)
	{
		if (_skeletons.find(filePath.getFullName()) != std::end(_skeletons))
			return _skeletons[filePath.getFullName()];
		if (!filePath.exists())
		{
			std::cerr << "AssetsManager : File [" << filePath.getFullName() << "] does not exists." << std::endl;
			assert(false);
		}

		auto skeleton = std::make_shared<Skeleton>();

		std::ifstream ifs(filePath.getFullName(), std::ios::binary);
		cereal::PortableBinaryInputArchive ar(ifs);
		ar(*skeleton.get());
		_skeletons.insert(std::make_pair(filePath.getFullName(), skeleton));
		return skeleton;
	}

	std::shared_ptr<MeshInstance> AssetsManager::loadMesh(const File &filePath)
	{
		if (_meshs.find(filePath.getFullName()) != std::end(_meshs))
			return _meshs[filePath.getFullName()];
		if (!filePath.exists())
		{
			std::cerr << "AssetsManager : File [" << filePath.getFullName() << "] does not exists." << std::endl;
			assert(false);
		}

		std::ifstream ifs(filePath.getFullName(), std::ios::binary);
		cereal::PortableBinaryInputArchive ar(ifs);
		MeshData data;
		ar(data);

		auto meshInstance = std::make_shared<MeshInstance>();

		meshInstance->subMeshs.resize(data.subMeshs.size());
		meshInstance->name = data.name;

		for (std::size_t i = 0; i < data.subMeshs.size(); ++i)
		{
			// If no vertex pool correspond to submesh
			if (_pools.find(data.subMeshs[i].infos) == std::end(_pools))
			{
				createPool(data.subMeshs[i].infos);
			}
			loadSubmesh(data.subMeshs[i], meshInstance->subMeshs[i]);
		}
		_meshs.insert(std::make_pair(filePath.getFullName(), meshInstance));
		return meshInstance;
	}

	void AssetsManager::loadSubmesh(SubMeshData &data, SubMeshInstance &mesh)
	{
		auto &pools = _pools.find(data.infos)->second;
		auto geometryManager = _dependencyManager.lock()->getInstance<gl::GeometryManager>();
		assert(geometryManager != nullptr);

		std::size_t size = data.infos.count();

		std::vector<void*> buffer;
		buffer.resize(size);
		std::vector<std::size_t> nbrBuffer;
		nbrBuffer.resize(size);

		std::size_t ctr = 0;
		auto sizeofFloat = sizeof(float);
		auto maxSize = data.positions.size();
		for (std::size_t i = 0; i < data.infos.size(); ++i)
		{
			if (!data.infos.test(i))
				continue;
			switch (MeshInfos(i))
			{
			case Positions:
				buffer[ctr] = &data.positions[0].x;
				nbrBuffer[ctr] = data.positions.size() * 4 * sizeofFloat;
				if (data.positions.size() > maxSize)
					maxSize = data.positions.size();
				break;
			case Normals:
				buffer[ctr] = &data.normals[0].x;
				nbrBuffer[ctr] = data.normals.size() * 4 * sizeofFloat;
				if (data.normals.size() > maxSize)
					maxSize = data.normals.size();
				break;
			case Tangents:
				buffer[ctr] = &data.tangents[0].x;
				nbrBuffer[ctr] = data.tangents.size() * 4 * sizeofFloat;
				if (data.tangents.size() > maxSize)
					maxSize = data.tangents.size();
				break;
			case BiTangents:
				buffer[ctr] = &data.biTangents[0].x;
				nbrBuffer[ctr] = data.biTangents.size() * 4 * sizeofFloat;
				if (data.biTangents.size() > maxSize)
					maxSize = data.biTangents.size();
				break;
			case Uvs:
				buffer[ctr] = &data.uvs[0][0].x;
				nbrBuffer[ctr] = data.uvs[0].size() * 2 * sizeofFloat;
				break;
			case Weights:
				buffer[ctr] = &data.weights[0].x;
				nbrBuffer[ctr] = data.weights.size() * 4 * sizeofFloat;
				if (data.weights.size() > maxSize)
					maxSize = data.weights.size();
				break;
			case BoneIndices:
				buffer[ctr] = &data.boneIndices[0].x;
				nbrBuffer[ctr] = data.boneIndices.size() * 4 * sizeofFloat;
				if (data.boneIndices.size() > maxSize)
					maxSize = data.boneIndices.size();
				break;
			case Colors:
				buffer[ctr] = &data.colors[0].x;
				nbrBuffer[ctr] = data.colors.size() * 4 * sizeofFloat;
				if (data.colors.size() > maxSize)
					maxSize = data.colors.size();
				break;
			default:
				break;
			}
			++ctr;
		}
		mesh.vertices = geometryManager->addVertices(maxSize, size, nbrBuffer.data(), buffer.data());
		mesh.indices = geometryManager->addIndices(data.indices.size(), &data.indices[0]);
		mesh.bounding = data.boundingInfos;
		mesh.vertexPool = _pools.find(data.infos)->second.first;
		mesh.indexPool = _pools.find(data.infos)->second.second;
		geometryManager->attachVerticesToVertexPool(mesh.vertices, pools.first);
		geometryManager->attachIndicesToIndexPool(mesh.indices, pools.second);
	}

	// Create pool for meshs
	void AssetsManager::createPool(const std::bitset<MeshInfos::END> &infos)
	{
		auto geometryManager = _dependencyManager.lock()->getInstance<gl::GeometryManager>();
		assert(geometryManager != nullptr);

		std::size_t size = infos.count();

		GLenum *typeComponent = new GLenum[size]; // {GL_FLOAT, GL_FLOAT, GL_FLOAT};			
		uint8_t *sizeTypeComponent = new uint8_t[size]; // { sizeof(float), sizeof(float), sizeof(float) };
		uint8_t *nbrComponent = new uint8_t[size];// {4, 4, 4};

		std::size_t ctr = 0;
		for (std::size_t i = 0; i < infos.size(); ++i)
		{
			if (!infos.test(i))
				continue;
			switch (MeshInfos(i))
			{
			case Positions:
				typeComponent[ctr] = GL_FLOAT;
				sizeTypeComponent[ctr] = sizeof(float);
				nbrComponent[ctr] = 4;
				break;
			case Normals:
				typeComponent[ctr] = GL_FLOAT;
				sizeTypeComponent[ctr] = sizeof(float);
				nbrComponent[ctr] = 4;
				break;
			case Tangents:
				typeComponent[ctr] = GL_FLOAT;
				sizeTypeComponent[ctr] = sizeof(float);
				nbrComponent[ctr] = 4;
				break;
			case BiTangents:
				typeComponent[ctr] = GL_FLOAT;
				sizeTypeComponent[ctr] = sizeof(float);
				nbrComponent[ctr] = 4;
				break;
			case Uvs:
				typeComponent[ctr] = GL_FLOAT;
				sizeTypeComponent[ctr] = sizeof(float);
				nbrComponent[ctr] = 2;
				break;
			case Weights:
				typeComponent[ctr] = GL_FLOAT;
				sizeTypeComponent[ctr] = sizeof(float);
				nbrComponent[ctr] = 4;
				break;
			case BoneIndices:
				typeComponent[ctr] = GL_FLOAT;
				sizeTypeComponent[ctr] = sizeof(float);
				nbrComponent[ctr] = 4;
				break;
			case Colors:
				typeComponent[ctr] = GL_FLOAT;
				sizeTypeComponent[ctr] = sizeof(float);
				nbrComponent[ctr] = 4;
				break;

			default:
				break;
			}
			++ctr;
		}

		auto vpKey = geometryManager->addVertexPool(size, typeComponent, sizeTypeComponent, nbrComponent);
		auto indKey = geometryManager->addIndexPool();

		geometryManager->attachIndexPoolToVertexPool(vpKey, indKey);

		_pools.insert(std::make_pair(infos, std::make_pair(vpKey, indKey)));

		delete[] typeComponent;
		delete[] sizeTypeComponent;
		delete[] nbrComponent;
	}
}