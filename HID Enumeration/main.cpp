#include <iostream>
using std::cout;
using std::cin;
using std::endl;
using std::cerr;
using std::clog;
using std::left;

#include <iomanip>
using std::setw;
using std::setprecision;

#include <cstdlib>
using std::system;
using std::exit;

#include <windows.h>

const UINT w = 25;

const TCHAR *deviceTypeToString(DWORD type)
{
	switch(type)
	{
	case RIM_TYPEHID:
		return "HID";
	case RIM_TYPEKEYBOARD:
		return "keyboard";
	case RIM_TYPEMOUSE:
		return "mouse";
	default:
		return "?";
	} // end switch
} // end function deviceTypeToString

void printDeviceInformation(const RID_DEVICE_INFO deviceInfo, UINT indent);

int main()
{
	UINT n = 0;
	UINT infoSize;	// in bytes
	GetRegisteredRawInputDevices(NULL,&n,sizeof(RAWINPUTDEVICE));
	cout << left << "number of registered devices = " << n << endl;

	GetRawInputDeviceList(NULL,&n,sizeof(RAWINPUTDEVICELIST));
	RAWINPUTDEVICELIST *devices = new RAWINPUTDEVICELIST[n];
	n = GetRawInputDeviceList(devices,&n,sizeof(RAWINPUTDEVICELIST));	// what if a device is added/removed in the meantime?!
	cout << "number of HIDs attached to the system = " << n << '\n' << endl;

	for(UINT i = 0 ; i < n ; ++i)
	{
		cout << "device " << i << ":\n";
		cout << setw(2) << ' ' << setw(w) << "handle: " << devices[i].hDevice << '\n';
		cout << setw(2) << ' ' << setw(w) << "type: " << deviceTypeToString(devices[i].dwType) << '\n';

		GetRawInputDeviceInfo(devices[i].hDevice,RIDI_DEVICENAME,NULL,&infoSize);
		TCHAR *deviceName = new TCHAR[infoSize];
		GetRawInputDeviceInfo(devices[i].hDevice,RIDI_DEVICENAME,deviceName,&infoSize);
		cout << setw(2) << ' ' << setw(w) << "name: " << deviceName << '\n';
		delete[] deviceName;

		//GetRawInputDeviceInfo(devices[i].hDevice,RIDI_PREPARSEDDATA,NULL,&infoSize);
		//char *parsedData = new char[infoSize];
		//GetRawInputDeviceInfo(devices[i].hDevice,RIDI_PREPARSEDDATA,parsedData,&infoSize);
		//cout << setw(2) << ' ' << setw(10) << "parsed data: " << parsedData << '\n';
		//delete[] parsedData;

		RID_DEVICE_INFO deviceInfo;
		deviceInfo.cbSize = infoSize = sizeof(deviceInfo);
		GetRawInputDeviceInfo(devices[i].hDevice,RIDI_DEVICEINFO,&deviceInfo,&infoSize);	// can it fail? require more/less space?
		printDeviceInformation(deviceInfo,2);
	} // end for
	cout << endl;

	delete[] devices;

	system("pause");
	return 0;
} // end function main

void printDeviceInformation(const RID_DEVICE_INFO deviceInfo, UINT indent)
{
	switch(deviceInfo.dwType)
	{
	case RIM_TYPEHID:
		cout << setw(2) << ' ' << setw(w) << "vendor ID: " << deviceInfo.hid.dwVendorId << '\n';
		cout << setw(2) << ' ' << setw(w) << "product ID: " << deviceInfo.hid.dwProductId << '\n';
		cout << setw(2) << ' ' << setw(w) << "version number: " << deviceInfo.hid.dwVersionNumber << '\n';
		cout << setw(2) << ' ' << setw(w) << "usage page: " << deviceInfo.hid.usUsagePage << '\n';
		cout << setw(2) << ' ' << setw(w) << "usage: " << deviceInfo.hid.usUsage << '\n';
		break;
	case RIM_TYPEKEYBOARD:
		cout << setw(2) << ' ' << setw(w) << "type: " << deviceInfo.keyboard.dwType << '\n';
		cout << setw(2) << ' ' << setw(w) << "subtype: " << deviceInfo.keyboard.dwSubType << '\n';
		cout << setw(2) << ' ' << setw(w) << "scan code mode: " << deviceInfo.keyboard.dwKeyboardMode << '\n';
		cout << setw(2) << ' ' << setw(w) << "number of function keys: " << deviceInfo.keyboard.dwNumberOfFunctionKeys << '\n';
		cout << setw(2) << ' ' << setw(w) << "number of indicators: " << deviceInfo.keyboard.dwNumberOfIndicators << '\n';
		cout << setw(2) << ' ' << setw(w) << "total number of keys: " << deviceInfo.keyboard.dwNumberOfKeysTotal << '\n';
		break;
	case RIM_TYPEMOUSE:
		cout << setw(2) << ' ' << setw(w) << "ID: " << deviceInfo.mouse.dwId << '\n';
		cout << setw(2) << ' ' << setw(w) << "number of buttons: " << deviceInfo.mouse.dwNumberOfButtons << '\n';
		cout << setw(2) << ' ' << setw(w) << "sample rate: " << deviceInfo.mouse.dwSampleRate << '\n';
		cout << setw(2) << ' ' << setw(w) << "horizontal wheel: " << deviceInfo.mouse.fHasHorizontalWheel << '\n';
		break;
	default:
		cout << setw(2) << ' ' << "Unknown Device Type!\n";
		break;
	} // end switch
} // end function printDeviceInformation
