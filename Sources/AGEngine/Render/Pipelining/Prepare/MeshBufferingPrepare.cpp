#include "MeshBufferingPrepare.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <Graphic/DRBMeshData.hpp>
#include <Graphic/DRBMesh.hpp>
#include <Graphic/DRBSkinnedMesh.hpp>

namespace AGE
{	
	namespace BasicCommandGeneration
	{
		bool MeshRawType::Treat(const BFCItem &item, BFCArray<MeshRawType> &result)
		{
			DRBMeshData * mesh = ((DRBMesh*)(item.getDrawable()))->getDatas().get();
			MeshRawType h;
			h.vertice = ConcatenateKey(mesh->getPainterKey(), mesh->getVerticesKey());
			h.material = ((DRBMesh*)(item.getDrawable()))->material;
			h.matrix = mesh->getTransformation();
			return result.push(h);
		}

		bool MeshRawType::Compare(const MeshRawType &a, const MeshRawType &b)
		{
			if (a.material == b.material)
			{
				return a.vertice < b.vertice;
			}
			return a.material < b.material;
		}

		MeshRawType MeshRawType::Invalid()
		{
			MeshRawType invalid;
			invalid.material = nullptr;
			invalid.vertice = -1;
			return invalid;
		}

		bool MeshRawType::operator!=(const MeshRawType &o) const
		{
			return (material != o.material || vertice != o.vertice);
		}

		//////////////////////////////////////////////////////////////////////////////////

		bool ShadowRawType::Treat(const BFCItem &item, BFCArray<ShadowRawType> &result)
		{
			DRBMeshData * mesh = ((DRBMesh*)(item.getDrawable()))->getDatas().get();
			ShadowRawType h;
			h.vertice = ConcatenateKey(mesh->getPainterKey(), mesh->getVerticesKey());
			h.matrix = mesh->getTransformation();
			return result.push(h);
		}

		bool ShadowRawType::Compare(const ShadowRawType &a, const ShadowRawType &b)
		{
			return a.vertice < b.vertice;
		}

		ShadowRawType ShadowRawType::Invalid()
		{
			ShadowRawType invalid;
			invalid.vertice = -1;
			return invalid;
		}

		bool ShadowRawType::operator!=(const ShadowRawType &o) const
		{
			return (vertice != o.vertice);
		}

		//////////////////////////////////////////////////////////////////////////////////////

		bool SkinnedMeshRawType::Treat(const BFCItem &item, BFCArray<SkinnedMeshRawType> &result)
		{
			DRBMeshData * mesh = ((DRBSkinnedMesh*)(item.getDrawable()))->getDatas().get();
			SkinnedMeshRawType h;
			h.vertice = ConcatenateKey(mesh->getPainterKey(), mesh->getVerticesKey());
			h.material = ((DRBMesh*)(item.getDrawable()))->material;
			h.matrix = mesh->getTransformation();
			h.bonesIndex = ((DRBSkinnedMesh*)(item.getDrawable()))->getSkinningIndex();
			return result.push(h);
		}

		bool SkinnedMeshRawType::Compare(const SkinnedMeshRawType &a, const SkinnedMeshRawType &b)
		{
			if (a.material == b.material)
			{
				if (a.bonesIndex == b.bonesIndex)
				{
						return a.vertice < b.vertice;
				}
				return a.bonesIndex < b.bonesIndex;
			}
			return a.material < b.material;
		}

		SkinnedMeshRawType SkinnedMeshRawType::Invalid()
		{
			SkinnedMeshRawType invalid;
			invalid.material = nullptr;
			invalid.vertice = -1;
			invalid.bonesIndex = -1;
			return invalid;
		}

		bool SkinnedMeshRawType::operator!=(const SkinnedMeshRawType &o) const
		{
			return (material != o.material || vertice != o.vertice || bonesIndex != o.bonesIndex);
		}

		//////////////////////////////////////////////////////////////////////////////////////

		bool SkinnedShadowRawType::Treat(const BFCItem &item, BFCArray<SkinnedShadowRawType> &result)
		{
			DRBMeshData * mesh = ((DRBSkinnedMesh*)(item.getDrawable()))->getDatas().get();
			SkinnedShadowRawType h;
			h.vertice = ConcatenateKey(mesh->getPainterKey(), mesh->getVerticesKey());
			h.matrix = mesh->getTransformation();
			h.bonesIndex = ((DRBSkinnedMesh*)(item.getDrawable()))->getSkinningIndex();
			return result.push(h);
		}

		bool SkinnedShadowRawType::Compare(const SkinnedShadowRawType &a, const SkinnedShadowRawType &b)
		{
			if (a.vertice == b.vertice)
			{
				return a.bonesIndex < b.bonesIndex;
			}
			return a.vertice < b.vertice;
		}

		SkinnedShadowRawType SkinnedShadowRawType::Invalid()
		{
			SkinnedShadowRawType invalid;
			invalid.vertice = -1;
			invalid.bonesIndex = 0;
			return invalid;
		}

		bool SkinnedShadowRawType::operator!=(const SkinnedShadowRawType &o) const
		{
			return (vertice != o.vertice || bonesIndex != o.bonesIndex);
		}
	}
}
