#pragma once
#include "PreemoEntity.h"

namespace preemo{
	class ScriptableEntity{
	public:
		template<typename T>
		T& GetComponent() {
			return m_Entity.GetComponent<T>();
		}
		template<typename T>
		bool HasComponent() {
			return m_Entity.HasComponent<T>();
		}
	private:
		Entity m_Entity;
		friend class Scene;
	};
}