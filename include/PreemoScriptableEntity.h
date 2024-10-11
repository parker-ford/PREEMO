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

		int test = 1;

		Entity* GetEntity() {
			return &m_Entity;
		}

	//protected: //PREEMO_TODO: protected was removed for emscripten testing
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate() {}
	//private: //PREEMO_TODO: removed private for emscripten testing
		Entity m_Entity;
		friend class Scene;
	};
}