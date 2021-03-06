#pragma once
#include "instruction.h"

namespace sqf
{
	namespace inst
	{
		class endstatement : public instruction
		{
		public:
			virtual void execute(virtualmachine* vm) const;
			virtual insttype thistype(void) const { return insttype::endstatement; }
		};
	}
}