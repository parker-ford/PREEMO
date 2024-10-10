#pragma once
//#include "Preemo.h"
//#include "PreemoScriptableEntity.h"
#ifdef WEBGPU_BACKEND_EMSCRIPTEN
#include <emscripten/val.h>

#endif // WEBGPU_BACKEND_EMSCRIPTEN
#include <iostream>
#include <functional>


namespace preemo {
	struct TransformComponent {
		TransformComponent() { std::cout << "Creating transform component " << std::endl; }
		virtual ~TransformComponent() {};
	};

	class ScriptableEntity;

	struct NativeScriptComponent {
		ScriptableEntity* Instance = nullptr;
		ScriptableEntity* Reference = nullptr;

		//ScriptableEntity*(*InstantiateScript)();
		std::function<ScriptableEntity* ()> InstantiateScript;
		void(*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind() {
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nativeScriptComponent) { delete nativeScriptComponent->Instance; nativeScriptComponent->Instance = nullptr; };
		}
		void Bind(ScriptableEntity* script);
		//void Bind(ScriptableEntity* script) {
		//	Reference = script;
		//	InstantiateScript = [this]() { return new ScriptableEntity(*Reference); };
		//	std::cout << "Binding " << script << std::endl;
		//}
	};
}