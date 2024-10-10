#include "PreemoScene.h"
#include "PreemoComponents.h"
#include "PreemoScriptableEntity.h"

namespace preemo {

	Entity Scene::CreateEntity()
	{
		return Entity(m_Registry.create(), this);
	}

	void Scene::UpdateScriptableEntities()
	{
		// PREEMO_TODO: Handle deletion
		m_Registry.view<ScriptComponent>().each([=](auto entity, auto& scriptComponent)
			{
				if (!scriptComponent.Instance) {
					scriptComponent.Instance = scriptComponent.InstantiateScript();
					scriptComponent.Instance->m_Entity = Entity{ entity, this };
					scriptComponent.Instance->OnCreate();
				}
				scriptComponent.Instance->OnUpdate();
			}
		);
	}
}


