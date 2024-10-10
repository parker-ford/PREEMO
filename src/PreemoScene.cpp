#include "PreemoScene.h"
#include "Components.h"
#include "PreemoScriptableEntity.h"

namespace preemo {
	Scene::Scene()
	{
		std::cout << "Creating Scene " << std::endl;
		//Setup components
		/*{
			using namespace entt::literals;
			entt::meta<TransformComponent>()
				.type("TransformComponent"_hs)
				.ctor<>()
				.func<&entt::registry::emplace<TransformComponent>>("emplace"_hs);;

		}*/
	}
	Entity Scene::CreateEntity()
	{
		auto ent = Entity(m_Registry.create(), this);
		std::cout << "Size of entities view " << m_Registry.view<entt::entity>().size_hint() << std::endl;
		return ent;
	}

	void Scene::UpdateScriptableEntities()
	{
		//std::cout << "Size of entities view " << m_Registry.view<entt::entity>().size_hint() << std::endl;
		// Check if the view is empty
		// May have something to do with how native script is registered??
		// Has something to do with viewing components. Need to investigate
		// First check to see how to view all Transform components
		/*auto view = m_Registry.view<entt::entity>();
		if (view.) {
			std::cout << "View is empty" << std::endl;
			return;
		}*/
		/*std::cout << "Beginning test print " << std::endl;
		std::cout << "Size of entities view " << m_Registry.view<entt::entity>().size_hint() << std::endl;
		for (auto entity : m_Registry.view<entt::entity>())
		{
			if (m_Registry.all_of<TransformComponent>(entity)) {
				std::cout << "has transform in it" << std::endl;
			}
			else {
				std::cout << "no has transform in it" << std::endl;
			}
		}*/


		// PREEMO_TODO: Handle deletion

		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nativeScriptComponent)
			{
				//(void)entity;
				if (!nativeScriptComponent.Instance) {
					nativeScriptComponent.Instance = nativeScriptComponent.InstantiateScript();
					nativeScriptComponent.Instance->m_Entity = Entity{ entity, this };
					nativeScriptComponent.Instance->OnCreate();
				}
				nativeScriptComponent.Instance->OnUpdate();
			}
		);
	}
}


