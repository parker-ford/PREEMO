#include "Components.h"
#include "PreemoScriptableEntity.h"

namespace preemo {
	void NativeScriptComponent::Bind(ScriptableEntity* script) {
		std::cout << "testing update: " << std::endl;
		script->OnUpdate();
		Reference = script;
		//InstantiateScript = [this]() { return new ScriptableEntity(*Reference); };
		InstantiateScript = [this]() { return Reference; };
		//InstantiateScript = []() {std::cout << "test script " << std::endl;  return nullptr; };
		std::cout << "Binding " << script << std::endl;
	}
}

