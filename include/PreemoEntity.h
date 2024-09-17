#pragma once
#include <entt/entt.hpp>
#include "PreemoScene.h"
#include "Components.h"

//TEMP
#include <iostream>

//Taken from: https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Scene/Entity.h



namespace preemo {
	class Entity {
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		//Entity(const Entity& other) = default;

#ifndef WGPU_BACKEND_EMSCRIPTEN

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			//PREEMO_TODO: Look into what this does
			//m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		/*void* AddComponent(const std::string& componentName) {
			return g_ComponentRegistry.CreateComponent(componentName, *this);
		}*/
		
		template<typename T>
		bool HasComponent() {
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T>
		T& GetComponent()
		{
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
	}

		void AddComponent(std::string id);

#else
		void* AddComponent(int id);
#endif
		/*
		
		AddComponent(
		
		
		*/

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}