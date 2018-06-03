/*
Copyright 2018 Igor Santarek

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <iostream>
#include <windows.h>
#include <bitset>

using namespace std;

const bool PRESSED_DOWN = true;

template <class T>
class TrackedValue {
	T value;
	bool _isChanged;

public:
	TrackedValue(T value) {
		this->value = value;
		_isChanged = false;
	}

	void set(T newValue) {
		if(value != newValue) {
			value = newValue;
			_isChanged = true;
		}
	}

	T get() {
		return value;
	}

	bool isChanged() {
		auto result = _isChanged;
		_isChanged = false;
		return result;
	}
};

bool isKeyPressedDown(bitset<16> keyState) {
	// I'm checking bits because of return value of GetAsyncKeyState (https://msdn.microsoft.com/pl-pl/library/windows/desktop/ms646293(v=vs.85).aspx)
	return (keyState[15] == true) || (keyState[15] == true && keyState[0] == true);
}

int getNextVirtualKeyCode(int actualVirtualKeyCode) {
	if(actualVirtualKeyCode == 0x31) return 0x32; // return keyboard 2 button
	if(actualVirtualKeyCode == 0x32) return 0x31; // return keyboard 1 button
}

int main() {
	int actualVirtualKeyCode = 0x31; // keyboard 1 button
	// 0xC0 - tilde key code on keyboard
	// GetAsyncKeyState - https://msdn.microsoft.com/pl-pl/library/windows/desktop/ms646293(v=vs.85).aspx
	TrackedValue<bool> keyState( isKeyPressedDown(GetAsyncKeyState(0xC0)) );

	while(1) {
		keyState.set( isKeyPressedDown(GetAsyncKeyState(0xC0)) );
		if((keyState.get() == PRESSED_DOWN) && keyState.isChanged()) {
			keybd_event(actualVirtualKeyCode, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
			keybd_event(actualVirtualKeyCode, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
			actualVirtualKeyCode = getNextVirtualKeyCode(actualVirtualKeyCode);
			cout<<"PRESSED KEY WITH VIRTUAL CODE: "<<actualVirtualKeyCode<<endl;
		}
	}
	return 0;
}