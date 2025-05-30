#include "windowsFileManipulation.h"

#include <Windows.h>
#include <ShObjIdl.h>
#include <sstream>
#include <codecvt>

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

std::string open_file_dialog()
{
	std::string resultStr = "";

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileDialog* pFileOpen = nullptr;

		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		if (SUCCEEDED(hr))
		{
			hr = pFileOpen->Show(NULL);

			if (SUCCEEDED(hr))
			{
				IShellItem* pitem;
				hr = pFileOpen->GetResult(&pitem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pitem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					if (SUCCEEDED(hr))
					{
						//do stuff with file here
						std::wstringstream wss;
						wss << pszFilePath;
						std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
						resultStr = converter.to_bytes(wss.str());

						CoTaskMemFree(pszFilePath);
					}
					pitem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}

	return resultStr;
}