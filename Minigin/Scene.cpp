#include <algorithm>
#include "Scene.h"
#include <cassert>
#include <utility>
#include "Components/Component.h"
#include "Components/RenderComponent.h"
#include <stdexcept>
#include <SDL3/SDL_log.h>

namespace ge {
	int Scene::s_NextId = 0;

	void Scene::Add(std::unique_ptr<GameObject> object) {
		assert(object != nullptr && "Cannot add a null GameObject to the scene.");
		m_objects.emplace_back(std::move(object));
	}

	void Scene::Remove(const GameObject& object) {
		m_objects.erase(
			std::remove_if(
				m_objects.begin(),
				m_objects.end(),
				[&object](const auto& ptr) { return ptr.get() == &object; }
			),
			m_objects.end()
		);
	}

	void Scene::RemoveAll() {
		m_objects.clear();
	}

	GameObject& Scene::GetGameObjectByName(const std::string& name) const {
		for (const auto& object : m_objects) {
			if (object->GetName() == name) {
				return *object;
			}
		}
		throw std::runtime_error("GameObject '" + name + "' not found");
	}

	GameObject& Scene::GetGameObjectByIndex(const int idx) const {
		if (m_objects[idx] != nullptr) {
			return *m_objects[idx];
		}
		throw std::runtime_error("GameObject at idx: '" + std::to_string(idx) + "' not found");
	}

	const std::vector<std::unique_ptr<GameObject> >& Scene::GetGameObjects() const {
		return m_objects;
	}

	int Scene::GetSceneSize() const {
		return static_cast<int>(m_objects.size());
	}

	void Scene::Update() {
		for (const auto& object : m_objects) {
			if (object->IsActive()) {
				object->Update();
			}
		}

		for (auto& go : m_objects) {
			if (go) {
				go->CleanupDeadChildren();
			}
		}

		m_objects.erase(
			std::remove_if(m_objects.begin(), m_objects.end(),
				[](const std::unique_ptr<GameObject>& object) {
					if (object->MarkedForDeletion()) {
						SDL_Log("deleted %s", object->GetName().c_str());
					}
					return object->MarkedForDeletion();
				}),
			m_objects.end()
		);

		if (m_ClearRequested) {
			m_objects.clear();
			m_ClearRequested = false;
		}
	}

	void Scene::Render() const {
		//layer sorting so that backgrounds dont render in front of other gameObject elements
		std::vector<std::pair<Component*, int>> sortedComponents;

		for (const auto& object : m_objects) {
			if (!object->IsActive()) continue;

			int layer = 0;
			if (const auto rc = object->GetComponent<RenderComponent>()) {
				layer = rc->GetLayer();
			}

			for (const auto& component : object->GetComponents()) {
				sortedComponents.emplace_back(component.get(), layer);
			}
		}

		std::stable_sort(sortedComponents.begin(), sortedComponents.end(),
			[](const std::pair<Component*, int>& a, const std::pair<Component*, int>& b) {
				return a.second < b.second;
			});

		for (const auto& pair : sortedComponents) {
			pair.first->Render();
		}
	}

	void Scene::RenderGUI() const {
		for (const auto& object : m_objects) {
			if (object->IsActive()) {
				for (const auto& component : object->GetComponents()) {
					component->RenderGUI();
				}
			}
		}
	}

	void Scene::ClearGameObjects() {
		m_objects.clear();
	}

	void Scene::RequestClear() {
		m_ClearRequested = true;
	}

	void Scene::CleanupGeneration() {
		// Remove dead root-level GameObjects
		m_objects.erase(
			std::remove_if(m_objects.begin(), m_objects.end(),
				[](const std::unique_ptr<GameObject>& go) {
					return go->MarkedForDeletion();
				}),
			m_objects.end()
		);

		// Also tell remaining game objects to purge their dead children
		for (auto& go : m_objects) {
			go->CleanupDeadChildren();
		}
	}
}
