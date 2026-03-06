#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Scene.h"
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(std::string name);

		[[nodiscard]] Scene& GetSceneByName(const std::string& name) const;
		[[nodiscard]] Scene& GetSceneByIdx(int idx) const;
		void Update();
		void Render() const;
		void RenderGUI() const;

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::unique_ptr<Scene>> m_scenes{};
	};
}
