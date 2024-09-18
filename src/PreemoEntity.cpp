#include "PreemoEntity.h"
#include "PreemoSceneManager.h"

namespace preemo {

	extern SceneManager* g_SceneManager;

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
		std::cout << "entity created" << std::endl;
	}

	void* Entity::AddComponent(std::string id) {
		return g_SceneManager->m_ComponentRegistry.AddComponent(id, *this);
	}

	bool Entity::HasComponent(std::string id)
	{
		return g_SceneManager->m_ComponentRegistry.HasComponent(id, *this);
	}

	/*void Entity::AddComponent(std::string id)
	{

		if (!m_Scene) {
			std::cerr << "Error: Scene is null" << std::endl;
			return;
		}

		if (!m_Scene->m_Registry.valid(m_EntityHandle)) {
			std::cerr << "Error: Invalid entity" << std::endl;
			return;
		}

		using namespace entt::literals;

		auto type = entt::resolve(entt::hashed_string{ id.c_str() });

		if (type) {
			std::cout << "Found Type: " << id << std::endl;
			auto instance = type.construct();
			if (instance) {
				std::cout << "Created Instance " << std::endl;
				try {
					auto emplace_func = type.func("emplace"_hs);
					if (emplace_func) {
						auto result = emplace_func.invoke({}, std::ref(m_Scene->m_Registry), m_EntityHandle);
						if (result) {
							std::cout << "Component added successfully" << std::endl;
						}
						else {
							std::cout << "Failed to add component" << std::endl;
						}
					}
					else {
						std::cerr << "Error: 'emplace' function not found for type " << id << std::endl;
					}
				}
				catch (const std::exception& e) {
					std::cerr << "Exception caught while adding component: " << e.what() << std::endl;
				}
			}
			else {
				std::cout << "Could Not Create Instance " << std::endl;
			}
		}
		else {
			std::cout << "Could not find Type: " << id << std::endl;
		}
	}*/



#if WGPU_BACKEND_EMSCRIPTEN
	void* Entity::AddComponent(int component_id) {
		if (component_id == 1) {
			m_Scene->m_Registry.emplace<TransformComponent>(m_EntityHandle);
		}
	}
#endif // WGPU_BACKEND_EMSCRIPTEN

}
