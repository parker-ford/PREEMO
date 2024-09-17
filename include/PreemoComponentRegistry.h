#pragma once

namespace preemo {

	class Entity;

	class ComponentRegistry {
	public:
		template<typename T>
		void RegisterComponent(const std::string& name) {
			m_ComponentFactories[name] = [](Entity& entity) -> void* {
				return &entity.AddComponent<T>();
				};
		}

		void* CreateComponent(const std::string& name, Entity& entity);
		}
		/*void* CreateComponent(const std::string& name, Entity& entity) {
			auto it = m_ComponentFactories.find(name);
			if (it != m_ComponentFactories.end()) {
				return it->second(entity);
			}
			return nullptr;
		}*/

	private:
		std::unordered_map<std::string, std::function<void* (Entity&)>> m_ComponentFactories;
	};
}
