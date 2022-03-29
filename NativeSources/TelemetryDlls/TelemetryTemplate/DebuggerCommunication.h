#pragma once

#include <Windows.h>

extern "C" {

	/// <summary>
	/// Routine to throw messages towards the Insight API debugger if attached. 
	/// </summary>
	/// <param name="BaseMessage">Use one of the MSG_MAIN_XXX values in MessageDescriptionTypes.h or your own.</param>
	/// <param name="UserMessageClass">Use one of the MSG_RESOURCE_XXX values in MessageDescriptionTypes.h or your own.</param>
	/// <param name="UserMessageType"></param>
	/// <param name="ArgCount"></param>
	/// <param name="Args"></param>
	/// <returns></returns>
	int MessageDebugger(DWORD BaseMessage, DWORD UserMessageClass, DWORD UserMessageType, DWORD ArgCount, ULONG_PTR* Args);
}
