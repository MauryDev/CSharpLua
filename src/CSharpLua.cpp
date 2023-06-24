#include <metahost.h>
#pragma comment(lib, "mscoree.lib")

#include <lua.hpp>
#include <iostream>
#include <unordered_map>
#include <stack>
#include <mutex>
HRESULT hr;
ICLRMetaHost* pMetaHost = NULL;
ICLRRuntimeInfo* pRuntimeInfo = NULL;
ICLRRuntimeHost* pClrRuntimeHost = NULL;
std::unordered_map<std::string, std::stack<std::string>> messages;
std::mutex mtx;
void SendMessageRPC(std::string&& name,std::string&& message)
{
	std::lock_guard<std::mutex> mtxg(mtx);
	auto val = messages.find(name);
	if (val != messages.end())
	{
		val->second.push(message);
	}
	else
	{
		messages[name] = std::stack<std::string>();
		messages[name].push(message);
	}
}
std::string GetMessageRPC(std::string&& name,bool& success)
{
	std::lock_guard<std::mutex> mtxg(mtx);
	auto msg = messages.find(name);
	if (msg != messages.end())
	{
		auto& v = msg->second;
		if (!v.empty())
		{
			auto str = v.top();

			v.pop();
			success = true;
			return str;
		}
	}
	success = false;
	return {};
}
wchar_t* ToWcharArr(const char* str)
{
	if (str == NULL)
	{
		return NULL;
	}
	size_t writed = 0;
	auto dllpath_len = strlen(str) + 1;
	wchar_t* WBuf = new wchar_t[dllpath_len + 1];
	WBuf[dllpath_len] = '\0';
	mbstowcs_s(&writed, WBuf, dllpath_len + 1, str, dllpath_len);
	return WBuf;
}

int CSharpLua_Init(lua_State* state)
{
	if (pMetaHost == NULL)
	{
		hr = CLRCreateInstance(CLSID_CLRMetaHost, IID_PPV_ARGS(&pMetaHost));
		hr = pMetaHost->GetRuntime(L"v4.0.30319", IID_PPV_ARGS(&pRuntimeInfo));
		hr = pRuntimeInfo->GetInterface(CLSID_CLRRuntimeHost,
			IID_PPV_ARGS(&pClrRuntimeHost));

		hr = pClrRuntimeHost->Start();



	}
	return 0;
}

int CSharpLua_LoadLibrary(lua_State* L)
{
	auto argc = lua_gettop(L);
	if (pMetaHost != NULL && argc >= 4)
	{
		DWORD dwRet = 0;
		auto dllpath = lua_tostring(L, 1);
		auto _dllpath = ToWcharArr(dllpath);

		auto Typename = lua_tostring(L, 2);
		auto _Typename = ToWcharArr(Typename);

		auto method = lua_tostring(L, 3);
		auto _method = ToWcharArr(method);

		auto parameter = lua_tostring(L, 4);
		auto _parameter = ToWcharArr(parameter);
		hr = pClrRuntimeHost->ExecuteInDefaultAppDomain(
			_dllpath, _Typename, _method, _parameter, &dwRet);

		

		delete _dllpath;
		delete _Typename;
		delete _method;
		delete _parameter;
		lua_pushinteger(L, dwRet);
		lua_pushinteger(L, hr);
		
		return 2;
	}
	else
	{
		lua_pushnil(L);
	}
	return 1;
}

int CSharpLua_GetMessage(lua_State* L)
{
	auto argc = lua_gettop(L);
	if (argc >= 1) {
		auto name = lua_tostring(L, 1);
		bool success = false;
		auto message = GetMessageRPC(name, success);
		if (success)
		{
			lua_pushstring(L, message.c_str());
			return 1;
		}
	}
	lua_pushnil(L);
	return 1;
}
int CSharpLua_SendMessage(lua_State* L)
{
	auto argc = lua_gettop(L);
	if (argc >= 2) {
		auto name = lua_tostring(L, 1);
		auto message = lua_tostring(L, 2);
		SendMessageRPC(name, message);
		
	}
	return 0;
}

int CSharpLua_Destroy(lua_State* state)
{
	if (pClrRuntimeHost != NULL)
	{
		hr = pClrRuntimeHost->Stop();

		pClrRuntimeHost->Release();
	}
	return 0;
}


BOOL APIENTRY DllMain(
	HANDLE hModule, // Handle to DLL module
	DWORD ul_reason_for_call,
	LPVOID lpReserved) // Reserved
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		return true;
	case DLL_PROCESS_DETACH:
		break;
	};
	return true;
}

extern "C"
{
	__declspec (dllexport) void CSharpLua_SendMessage(const char* name, const char* message)
	{
		SendMessageRPC(name, message);
	}
	__declspec (dllexport) bool CSharpLua_GetMessage(const char* name, const void** str)
	{
		bool success = false;
		auto message = GetMessageRPC(name, success);
		if (success)
		{
			auto len = message.size() + 1;
			auto strret = CoTaskMemAlloc(len);
			memcpy(strret, message.c_str(), len);
			*str = strret;



		}
		else
		{
			*str = NULL;
		}
		return success;
	}

	
	__declspec (dllexport) int luaopen_CSharpLua(lua_State* L)
	{

		static const luaL_Reg mylib[] = {
			{"Init",CSharpLua_Init},
			{"LoadLibrary",CSharpLua_LoadLibrary},
			{"Destroy",CSharpLua_Destroy},
			{"GetMessage",CSharpLua_GetMessage},
			{"SendMessage",CSharpLua_SendMessage},
			{NULL, NULL}
		};
		luaL_newlib(L, mylib);
		return 1;
	}
}
