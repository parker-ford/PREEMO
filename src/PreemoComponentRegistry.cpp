#include "PreemoComponentRegistry.h"

namespace preemo {
	void* ComponentRegistry::CreateComponent(const std::string & name, Entity & entity) {
		auto it = m_ComponentFactories.find(name);
		if (it != m_ComponentFactories.end()) {
			return it->second(entity);
		}
		return nullptr;
	}
}