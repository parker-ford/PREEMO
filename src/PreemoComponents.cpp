#include "PreemoComponents.h"
#include "PreemoScriptableEntity.h"

namespace preemo {
#ifdef WEBGPU_BACKEND_EMSCRIPTEN
	void ScriptComponent::Bind(ScriptableEntity* script) {
		Reference = script;
		//InstantiateScript = [this]() { return new ScriptableEntity(*Reference); }; //PREEMO_TODO: This needs to create new instances rather than return a ref
		InstantiateScript = [this]() { return Reference; };
	}
#endif // WEBGPU_BACKEND_EMSCRIPTEN
}

