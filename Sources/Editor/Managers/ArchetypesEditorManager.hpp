#pragma once

#include <set>
#include <map>
#include <Entity/Archetype.hpp>
#include <Entities/ReadableEntityPack.hpp>
#include <Entity/IArchetypeManager.hpp>

namespace AGE
{
	namespace WE
	{
		struct ArchetypeEditorRepresentation
		{
			std::set<Entity> entities;
			Entity root;
			std::string name;
			bool loaded = false;
		};

		class ArchetypeEditorManager : public IArchetypeManager
		{
		public:
			ArchetypeEditorManager();
			virtual ~ArchetypeEditorManager();
			void update(AScene *scene);
			virtual void save();
			virtual void load();
			virtual void loadOne(const std::string &name);
			virtual void addOne(const std::string &name, Entity &entity);
			virtual void spawn(Entity &entity, const std::string &name);
			// will update all the instanced
			virtual void updateArchetype(const std::string &name);
			void loadFromFile(std::shared_ptr<ArchetypeEditorRepresentation> ptr);

			AScene *getScene();
			void enableScene();
			void disableScene();
			void updateMenu();
		private:
			std::map<std::string, std::shared_ptr<ArchetypeEditorRepresentation>> _archetypesCollection;
			std::vector<const char*> _archetypesImGuiNamesList;
			int _selectedArchetypeIndex = 0;
			std::shared_ptr<ArchetypeEditorRepresentation> _selectedArchetype = nullptr;
			int _selectedEntityIndex = 0;
			std::vector<const char*> _entitiesNames;
			Entity *_selectedEntity = nullptr;
			bool _graphNodeDisplay = false;
			bool _selectParent = false;
			bool _displayWindow = true;
			AScene *_archetypesScene;
			bool _save = false;

			void _copyArchetypeToInstanciedEntity(Entity &archetype, Entity &entity);
			void _regenerateImGuiNamesList();
		};
	}
}