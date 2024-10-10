#pragma once

#ifdef WEBGPU_BACKEND_EMSCRIPTEN
#include <emscripten/val.h>
#endif // WEBGPU_BACKEND_EMSCRIPTEN

#include <iostream>
#include <functional>

namespace preemo {

	// Forward Declaration
	class ScriptableEntity;

	struct TransformComponent {
		TransformComponent() = default;
		virtual ~TransformComponent() {};
	};

	struct ScriptComponent {
		ScriptableEntity* Instance = nullptr;
		ScriptableEntity* Reference = nullptr;

#ifdef WEBGPU_BACKEND_EMSCRIPTEN
		// std::function required for emscripten due to lambda capture
		std::function<ScriptableEntity* ()> InstantiateScript;
#else
		ScriptableEntity*(*InstantiateScript)();
#endif // WEBGPU_BACKEND_EMSCRIPTEN

		void(*DestroyScript)(ScriptComponent*);

#ifdef WEBGPU_BACKEND_EMSCRIPTEN
		// Non-templated method required for emscripten
		void Bind(ScriptableEntity* script);
#else
		template<typename T>
		void Bind() {
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](ScriptComponent* nativeScriptComponent) { delete nativeScriptComponent->Instance; nativeScriptComponent->Instance = nullptr; };
		}
#endif // WEBGPU_BACKEND_EMSCRIPTEN
	};
}