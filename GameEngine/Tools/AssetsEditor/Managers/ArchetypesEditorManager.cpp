#include "ArchetypesEditorManager.hpp"
#include <Threads/MainThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Entities/EntitySerializationInfos.hpp>
#include <Components/EntityRepresentation.hpp>
#include <EntityHelpers/EntityImgui.hpp>
#include <Utils/Age_Imgui.hpp>
#include <Entities\ArchetypeScenes.hpp>
#include <Utils/Directory.hpp>
#include <Utils/FileSystemHelpers.hpp>
#include <Entities/ReadableEntityPack.hpp>
#include <Entities/EntityReadablePacker.hpp>
#include <Systems\AssetsAndComponentRelationsSystem.hpp>
#include <Entities\BinaryEntityPack.hpp>
#include <Components/ComponentRegistrationManager.hpp>

namespace AGE
{
	namespace WE
	{
		ArchetypeEditorManager::ArchetypeEditorManager()
		{
		}

		ArchetypeEditorManager::~ArchetypeEditorManager()
		{

		}

		void ArchetypeEditorManager::load()
		{
			Directory dir;
			const bool success = dir.open(_libraryFolder.c_str());
			AGE_ASSERT(success);
			for (auto it = dir.recursive_begin(); it != dir.recursive_end(); ++it)
			{
				if (Directory::IsFile(*it))
				{
					auto extension = AGE::FileSystemHelpers::GetExtension(*it);

					if (extension != "raw_archetype")
					{
						continue;
					}
					loadOne(AGE::FileSystemHelpers::RemoveExtension(AGE::FileSystemHelpers::GetName(*it)));
				}
			}
			dir.close();
		}

		void ArchetypeEditorManager::save()
		{
			for (auto &arch : _archetypesCollection)
			{
				if (arch.second->loaded == false)
				{
					continue;
				}
				ReadableEntityPack pack;
				CreateReadableEntityPack(pack, arch.second->root);
				auto path = _libraryFolder + "/" + arch.second->name + ".raw_archetype";
				pack.saveToFile(path);
				
				BinaryEntityPack binPack = pack.toBinary();
				binPack.saveToFile(_libraryFolder + "/" + arch.second->name + ".archetype");
			}
		}

		void ArchetypeEditorManager::loadOne(const std::string &name)
		{
			if (_archetypesCollection.find(name) != std::end(_archetypesCollection))
			{
				return;
			}

			auto representation = std::make_shared<ArchetypeEditorRepresentation>();
			representation->name = name;
			representation->loaded = false;
			_archetypesCollection.insert(std::make_pair(name, representation));
			_regenerateImGuiNamesList();
		}

		void ArchetypeEditorManager::addOne(const std::string &name, Entity &entity)
		{
			if (_archetypesCollection.find(name) != std::end(_archetypesCollection))
			{
				return;
			}
			auto scene = entity->getScene();
			auto representation = std::make_shared<ArchetypeEditorRepresentation>();
			representation->name = name;
			representation->loaded = true;

			//we copy the original entity into the archetype entity
			//for that we set the ArchetypeScene as the current scene
			//so that messages will be not passed to the renderScene of the WE's scene
			//but to the ArchetypeScene.
			GetMainThread()->setSceneAsActive(getScene().get());
			bool success = getScene()->copyEntity(entity, representation->root, true, false);

			GetMainThread()->setSceneAsActive(scene);
			AGE_ASSERT(success);

			representation->root->getComponent<EntityRepresentation>()->editorOnly = true;

			auto componentsCopy = entity->getComponentList();

			auto entityRepresentationComponent = entity->getComponent<EntityRepresentation>();
			AGE_ASSERT(entityRepresentationComponent != nullptr);

			//we remove original entity's components
			//replacing them by archetypes ones
			for (auto &e : componentsCopy)
			{
				if (e != nullptr && e != entityRepresentationComponent)
				{
					entity->copyComponent(representation->root->getComponent(e->getType()));
				}
			}

			std::vector<Entity*> tmpEntitiesList;
			std::vector<const char*> tmpEntitiesNameList;
			listEntityTree(entity, tmpEntitiesNameList, tmpEntitiesList);

			for (auto &entity : tmpEntitiesList)
			{
				auto &e = *entity;
				auto er = e->getComponent<EntityRepresentation>();
				e->addComponent<ArchetypeComponent>(name);
				e->getComponent<ArchetypeComponent>()->parentIsAnArchetype = false;
				if (er != entityRepresentationComponent)
				{
					e->getComponent<ArchetypeComponent>()->parentIsAnArchetype = true;
				}
			}

			tmpEntitiesList.clear(); tmpEntitiesNameList.clear();
			listEntityTree(representation->root, tmpEntitiesNameList, tmpEntitiesList);

			for (auto &entity : tmpEntitiesList)
			{
				auto &e = *entity;

				auto er = e->getComponent<EntityRepresentation>();
				er->_isArchetype = true;
			}

			entityRepresentationComponent->_archetypeLinked = representation;

			representation->entities.insert(entity);

			_archetypesCollection.insert(std::make_pair(name, representation));

			_regenerateImGuiNamesList();
		}

		void ArchetypeEditorManager::loadFromFile(std::shared_ptr<ArchetypeEditorRepresentation> ptr)
		{
			AGE_ASSERT(ptr != nullptr);

			if (ptr->loaded == false)
			{
				auto currentScene = GetMainThread()->getActiveScene();
				GetMainThread()->setSceneAsActive(getScene().get());

				auto path = _libraryFolder + "/" + ptr->name + ".raw_archetype";
				ReadableEntityPack pack;
				pack.scene = getScene().get();
				pack.loadFromFile(path);

				ptr->root = pack.entities.front().entity;
				ptr->loaded = true;

				if (currentScene)
				{
					GetMainThread()->setSceneAsActive(currentScene);
				}
			}
		}

		void ArchetypeEditorManager::spawn(Entity &entity, const std::string &name)
		{
			auto it = _archetypesCollection.find(name);
			AGE_ASSERT(it != std::end(_archetypesCollection));
			loadFromFile(it->second);
			entity->getScene()->copyEntity(it->second->root, entity, true, false);

			auto representation = entity->getComponent<EntityRepresentation>();

			representation->editorOnly = false;
			representation->_archetypeLinked = it->second;

			std::vector<Entity*> tmpEntitiesList;
			std::vector<const char*> tmpEntitiesNameList;
			listEntityTree(entity, tmpEntitiesNameList, tmpEntitiesList);

			for (auto &en: tmpEntitiesList)
			{
				auto &e = *en;
				e->addComponent<ArchetypeComponent>(name);
				if (e != entity)
				{
					e->getComponent<ArchetypeComponent>()->parentIsAnArchetype = true;
				}
			}

			it->second->entities.insert(entity);
		}

		void ArchetypeEditorManager::updateMenu()
		{
			if (ImGui::MenuItem("Show", nullptr, &_displayWindow)) {}
			if (ImGui::MenuItem("Graphnode display", nullptr, &_graphNodeDisplay, _displayWindow)) {}
			if (ImGui::MenuItem("Save library"))
			{
				_save = true;
			}
		}

		void ArchetypeEditorManager::update(AScene *scene)
		{
			if (_save == true)
			{
				_save = false;
				save();
			}

			if (_displayWindow == false)
			{
				return;
			}

			ImGui::Begin("Archetypes", nullptr, ImGuiWindowFlags_MenuBar);
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Options"))
				{
					updateMenu();
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			if (ImGui::Combo("", &_selectedArchetypeIndex, &_archetypesImGuiNamesList[0], static_cast<int>(_archetypesImGuiNamesList.size())))
			{
				if (_selectedArchetypeIndex < _archetypesImGuiNamesList.size())
				{
					auto name = _archetypesImGuiNamesList[_selectedArchetypeIndex];
					auto find = _archetypesCollection.find(name);
					if (find == std::end(_archetypesCollection))
					{
						_selectedArchetype = nullptr;
					}
					else
					{
						_selectedArchetype = find->second;
						if (_selectedArchetype->loaded == false)
						{
							loadFromFile(_selectedArchetype);
						}
					}
				}
			}

			if (_selectedArchetype != nullptr)
			{
				if (_selectedEntity == nullptr)
				{
					_selectedEntity = &_selectedArchetype->root;
				}

				auto entity = _selectedArchetype->root;
				auto modified = false;

				GetMainThread()->setSceneAsActive(getScene().get());
				if (_graphNodeDisplay)
				{
					modified |= recursiveDisplayList(entity, _selectedEntity, _selectParent, false);
					if (_selectedEntity)
					{
						modified |= displayEntity(*_selectedEntity, scene);
					}
				}
				else
				{
					_entitiesNames.clear();
					std::vector<Entity*> tmpEntitiesList;
					listEntityTree(_selectedArchetype->root, _entitiesNames, tmpEntitiesList);

					if (tmpEntitiesList.size() > 0)
					{

						if (_selectedEntityIndex > tmpEntitiesList.size())
						{
							_selectedEntityIndex = static_cast<int>(tmpEntitiesList.size() - 1);
						}

						ImGui::PushItemWidth(-1);
						//ImGui::ListBoxHeader("##empty");
						if (ImGui::ListBox("##empty", &_selectedEntityIndex, &(_entitiesNames.front()), (int)(_entitiesNames.size())))
						{
							if (tmpEntitiesList.size() > 0 && _selectedEntityIndex < tmpEntitiesList.size())
							{
								_selectedEntity = tmpEntitiesList[_selectedEntityIndex];
							}
							else
							{
								_selectedEntity = nullptr;
							}
						}
						//ImGui::ListBoxFooter();
						ImGui::PopItemWidth();
						if (_selectedEntity)
						{
							modified |= displayEntity(*_selectedEntity, scene);
						}
					}
				}

				if (_selectedEntity)
				{
					auto &types = ComponentRegistrationManager::getInstance().getExposedType();
					auto &creationFn = ComponentRegistrationManager::getInstance().getCreationFunctions();

					for (auto &t : types)
					{
						if (!(*_selectedEntity)->haveComponent(t.second)
							&& t.second != Component<ArchetypeComponent>::getTypeId())
						{
							if (ImGui::SmallButton(std::string("Add : " + ComponentRegistrationManager::getInstance().getComponentName(t.second)).c_str()))
							{
								creationFn.at(t.first)(_selectedEntity);
								modified = true;
							}
						}
					}
				}
				ImGui::Separator();

				GetMainThread()->setSceneAsActive(scene);

				if (modified)
				{
					for (auto e : _selectedArchetype->entities)
					{
						_copyArchetypeToInstanciedEntity(_selectedArchetype->root, e);
					}
				}

				if (ImGui::SmallButton("Create an instance"))
				{
						auto &name = _archetypesImGuiNamesList[_selectedArchetypeIndex];
						Entity duplicate = scene->createEntity();
						spawn(duplicate, name);
				}
			}

			ImGui::End();
		}

		void ArchetypeEditorManager::_copyArchetypeToInstanciedEntity(Entity &archetype, Entity &entity)
		{
			auto &componentList = archetype->getComponentList();

			auto representation = entity->getComponent<EntityRepresentation>();
			auto archetypeCpt = entity->getComponent<ArchetypeComponent>();

			auto &copyComponentList = entity->getComponentList();

			if (archetypeCpt->synchronizePosition)
			{
				entity->getLink().setPosition(archetype->getLink().getPosition());
			}
			if (archetypeCpt->synchronizeRotation)
			{
				entity->getLink().setOrientation(archetype->getLink().getOrientation());
			}
			if (archetypeCpt->synchronizeScale)
			{
				entity->getLink().setScale(archetype->getLink().getScale());
			}

			// we delete all existing components
			for (auto c : copyComponentList)
			{
				if (c != nullptr && c != representation && c != archetypeCpt)
				{
					entity->removeComponent(c->getType());
				}
			}

			// and replace them by fresh copy
			representation = archetype->getComponent<EntityRepresentation>();
			for (auto c : componentList)
			{
				if (c != nullptr && c != representation && c != archetypeCpt)
				{
					entity->copyComponent(c);
				}
			}

			const auto &archetypesChild = archetype->getLink().getChildren();
			const auto &entityChild = entity->getLink().getChildren();

			AGE_ASSERT(archetypesChild.size() == entityChild.size());

			for (std::size_t i = 0; i < archetypesChild.size(); ++i)
			{
				_copyArchetypeToInstanciedEntity(archetypesChild[i]->getEntity()->getEntity(), entityChild[i]->getEntity()->getEntity());
			}
		}

		void ArchetypeEditorManager::_regenerateImGuiNamesList()
		{
			_archetypesImGuiNamesList.clear();
			for (auto &e : _archetypesCollection)
			{
				_archetypesImGuiNamesList.push_back(e.first.c_str());
			}
		}

		std::shared_ptr<AScene> ArchetypeEditorManager::getScene()
		{
			if (_archetypesScene == nullptr)
			{
				_archetypesScene = std::make_shared<ArchetypeScene>(GetEngine());
				GetEngine()->addScene(_archetypesScene, "EDITOR_ARCHETYPES_SCENE");
				_archetypesScene->addSystem<WE::AssetsAndComponentRelationsSystem>(0);
				GetEngine()->initScene("EDITOR_ARCHETYPES_SCENE");
				GetEngine()->enableScene("EDITOR_ARCHETYPES_SCENE", 0);
			}
			return _archetypesScene;
		}

		void ArchetypeEditorManager::enableScene()
		{
			GetEngine()->enableScene("EDITOR_ARCHETYPES_SCENE", 0);
		}

		void ArchetypeEditorManager::disableScene()
		{
			GetEngine()->enableScene("EDITOR_ARCHETYPES_SCENE", 0);
		}

	}
}