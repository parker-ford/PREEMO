#include "PreemoScene.h"
#include "Components.h"
#include "PreemoScriptableEntity.h"

namespace preemo {
	Scene::Scene()
	{
		//Setup components
		{
			using namespace entt::literals;
			entt::meta<TransformComponent>()
				.type("TransformComponent"_hs)
				.ctor<>()
				.func<&entt::registry::emplace<TransformComponent>>("emplace"_hs);;

		}
	}
	Entity Scene::CreateEntity()
	{
		return Entity(m_Registry.create(), this);
	}

	void Scene::UpdateScriptableEntities()
	{
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


