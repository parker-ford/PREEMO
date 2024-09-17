#include "PreemoScene.h"
#include "PreemoEntity.h"

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
				(void)entity;
				if (!nativeScriptComponent.Instance) {
					nativeScriptComponent.InstantiateFunction();
					nativeScriptComponent.OnCreateFunction(nativeScriptComponent.Instance);
				}

				nativeScriptComponent.OnUpdateFunction(nativeScriptComponent.Instance);
			}
		);
	}
}


