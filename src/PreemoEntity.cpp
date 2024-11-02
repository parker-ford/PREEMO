#include "PreemoEntity.h"
#include "PreemoSceneManager.h"

namespace preemo {

	extern SceneManager* g_SceneManager;

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}

	//PREEMO_TODO: This needs to have functions for all components
#if WEBGPU_BACKEND_EMSCRIPTEN
	TransformComponent& Entity::AddComponentTransform()
	{
		return this->AddComponent<TransformComponent>();
}
	bool Entity::HasComponentTransform()
	{
		return this->HasComponent<TransformComponent>();
	}
	ScriptComponent& Entity::AddComponentScript()
	{
		return this->AddComponent<ScriptComponent>();
	}
#endif // WGPU_BACKEND_EMSCRIPTEN

}
