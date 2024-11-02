#pragma once

#include <entt/entt.hpp>

namespace preemo {
	class Entity;

	class Scene {
		friend class Entity;
	public:
		Scene() = default;
		Entity CreateEntity();
		void UpdateScriptableEntities();

	private:
		entt::registry m_Registry;
	};
}