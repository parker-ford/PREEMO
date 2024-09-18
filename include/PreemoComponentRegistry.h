#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <iostream>
#include "PreemoEntity.h"

namespace preemo {

	class Entity;

	struct ComponentRegistryItem {
		std::function<void* (Entity&)> addComponent;
		std::function<bool(Entity&)> hasComponent;
		std::function<void* (Entity&)> getComponent;
	};

	class ComponentRegistry {
	public:

		template<typename T>
		void RegisterComponent(const std::string& name) {
			std::cout << "registering component: " << name << std::endl;
			ComponentRegistryItem item;

			item.addComponent = [](Entity& entity) -> void* {
				return &entity.AddComponent<T>();
				};

			item.hasComponent = [](Entity& entity) -> bool {
				return entity.HasComponent<T>();
				};

			item.getComponent = [](Entity& entity) -> void* {
				return &entity.GetComponent<T>();
				};

			m_ComponentFactories[name] = item;
		}

		void* AddComponent(const std::string& name, Entity& entity) {
			return m_ComponentFactories[name].addComponent(entity);
		}

		bool HasComponent(const std::string& name, Entity& entity) {
			return m_ComponentFactories[name].hasComponent(entity);
		}

		void* GetComponent(const std::string& name, Entity& entity) {
			return m_ComponentFactories[name].getComponent(entity);
		}

	private:
		std::unordered_map<std::string, ComponentRegistryItem> m_ComponentFactories;
	};
}
