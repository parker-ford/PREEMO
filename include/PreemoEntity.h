#pragma once
#include <entt/entt.hpp>
#include "PreemoScene.h"
#include "PreemoComponents.h"

//TEMP
#include <iostream>

//Inspired by: https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Scene/Entity.h

namespace preemo {
	class Entity {
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		//Entity(const Entity& other) = default; // Causes emscripten to not compile

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			std::cout << "adding component " << std::endl;
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			//PREEMO_TODO: Look into what this does
			//m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		bool HasComponent() {
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T>
		T& GetComponent()
		{
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		// This is here to get around not being able to call templated function in javascipt
#ifdef WEBGPU_BACKEND_EMSCRIPTEN
		//Transform Component
		TransformComponent& AddComponentTransform();
		bool HasComponentTransform();

		//NativeScriptComponent
		ScriptComponent& AddComponentScript();
#endif // WEBGPU_BACKEND_EMSCRIPTEN

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}