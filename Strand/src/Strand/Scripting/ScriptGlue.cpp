#include "sdpch.h"

#include "Strand/Scripting/ScriptGlue.h"

#include "mono/metadata/object.h"

namespace Strand {

#define SD_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Strand.InternalCalls::" #Name, Name)


	static void NativeLog(MonoString* string, int parameter)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);
		mono_free(cStr);
		std::cout << str << ", " << parameter << std::endl;
	}

	static void NativeLog_Vector(glm::vec3* parameter, glm::vec3* outResult)
	{
		//SD_CORE_WARN("Value: ({0}, {1}, {2})", parameter->x, parameter->y, parameter->z);
		SD_CORE_WARN("Value: {0}", *parameter);
		*outResult = glm::normalize(*parameter);
	}

	static float NativeLog_VectorDot(glm::vec3* parameter)
	{
		//SD_CORE_WARN("Value: ({0}, {1}, {2})", parameter->x, parameter->y, parameter->z);
		SD_CORE_WARN("Value: {0}", *parameter);
		return glm::dot(*parameter, *parameter);
	}

	void ScriptGlue::RegisterFunctions()
	{
		SD_ADD_INTERNAL_CALL(NativeLog);
		SD_ADD_INTERNAL_CALL(NativeLog_Vector);
		SD_ADD_INTERNAL_CALL(NativeLog_VectorDot);
	}
}