#include <node.h>
#include <Windows.h>
#include <LM.h>
#include <locale>

#pragma comment(lib,"Netapi32.lib")

namespace nodeAddon {

	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Local;
	using v8::Object;
	using v8::Value;

	bool isEqualsCaseInsensitive(const std::wstring& a, const std::wstring& b)
	{
		return std::equal(a.begin(), a.end(), b.begin(), b.end(), [](wchar_t a, wchar_t b) {
			//assert(towlower(L'Ц') == towlower(L'ц'));
			return towlower(a) == towlower(b);
		});
	}

	void checkGroup(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		std::wstring groupName = (LPWSTR)*v8::String::Value(args[0]->ToString());

		NET_API_STATUS nStatus;
		PGROUP_INFO_0 pGroups = NULL;
		DWORD dwEntriesRead = 0;
		DWORD dwTotalEntries = 0;
		nStatus = NetLocalGroupEnum(NULL, 0, (LPBYTE*)&pGroups, MAX_PREFERRED_LENGTH, &dwEntriesRead, &dwTotalEntries, NULL);
		if (nStatus == NERR_Success) {
			args.GetReturnValue().Set(true);
			for (DWORD i = 0; i < dwEntriesRead; ++i) {
				//Регистронезависимое сравнение имени группы
				if (isEqualsCaseInsensitive(groupName, pGroups[i].grpi0_name))
					return;
			}
			args.GetReturnValue().Set(false);
		}
	}

	void Initialize(Local<Object> exports) {
		setlocale(LC_ALL, "");
		NODE_SET_METHOD(exports, "checkGroup", checkGroup);
	}

	NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)

}  // namespace nodeAddon