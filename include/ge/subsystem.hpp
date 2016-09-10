#ifndef GE_SUBSYSTEM_HPP
#define GE_SUBSYSTEM_HPP

#pragma once

#include <chrono>

namespace ge
{
struct runtime;

struct subsystem {
	
	subsystem() = default;
	subsystem(const subsystem&) = delete;
	subsystem(subsystem&&) = delete;
	
	subsystem& operator=(const subsystem&) = delete;
	subsystem& operator=(subsystem&&) = delete;
	
	virtual ~subsystem(){};

	virtual bool update(std::chrono::duration<float> delta) { return true; };
	virtual bool shutdown() { return true; };
	runtime* m_runtime;
};

}  // ge

#endif  // GE_ASSET_HPP