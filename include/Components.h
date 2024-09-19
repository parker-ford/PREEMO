#pragma once
//#include "Preemo.h"
//#include "PreemoScriptableEntity.h"


namespace preemo {
	struct TransformComponent {
		int testValue = 1;
		TransformComponent() = default;
		int getTestValue() { return testValue; }
	};

	class ScriptableEntity;

	struct NativeScriptComponent {
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*InstantiateScript)();
		void(*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind() {
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nativeScriptComponent) { delete nativeScriptComponent->Instance; nativeScriptComponent->Instance = nullptr; };
		}
	};
}