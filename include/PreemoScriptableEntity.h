#pragma once
#include "PreemoEntity.h"

namespace preemo{
	class ScriptableEntity{
	public:
		virtual ~ScriptableEntity() {}
		//~ScriptableEntity() = default;

		template<typename T>
		T& GetComponent() {
			return m_Entity.GetComponent<T>();
		}
		template<typename T>
		bool HasComponent() {
			return m_Entity.HasComponent<T>();
		}
	//protected: //PREEMO_TODO: protected was removed for emscripten testing
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate() {}
	private:
		Entity m_Entity;
		friend class Scene;
	};
}