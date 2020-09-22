#include <iostream>
#include <string>
#include <Windows.h>
#include <fstream>

bool keyPressed(int key) {
	return GetAsyncKeyState(key) == -32767;
}
bool keyHeld(int key) {
	return GetAsyncKeyState(key) & 0x8000;
}
void sendKeyPress(int key) {
	static INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
	ip.ki.dwFlags = 0;
	ip.ki.wVk = key;
	SendInput(1, &ip, sizeof(INPUT));
	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));
}
void sendKeyRelease(int key) {
	static INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
	ip.ki.dwFlags = 0;
	ip.ki.wVk = key;
	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));
}
void SendKeys(const std::string& str) {
	for (char c : str) {
		int key;
		if (c == ' ')
			key = VK_SPACE;
		else if (!std::isdigit(c) && !std::isalpha(c))
			continue;
		else
			key = std::toupper(c);
		sendKeyPress(key);
	}
}

void bruteForce(std::string const& chars, size_t max_len, std::string const& cur) {
    std::ofstream newFile;
    newFile.open("Logs\\" + chars + ".txt", std::ios_base::app);
    if (cur.length() == max_len) {
        return;
    }
    else {
        for (auto c : chars) {
            std::string next = cur + c;
            std::cout << next << '\n';

            bruteForce(chars, max_len, next);
            newFile << next << '\n';
        }
    }
}

void sendBruteForce(std::string const& chars, size_t max_len, std::string const& cur) {
    std::ofstream newFile;
    newFile.open("Logs\\" + chars + ".txt", std::ios_base::app);
    if (cur.length() == max_len) {
        return;
    }
    else {
        for (auto c : chars) {
            std::string next = cur + c;
            std::cout << next << '\n';
            SendKeys(next); 
            newFile << next << '\n';
            sendBruteForce(chars, max_len, next);
        }
    }
}

void clear() {
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}

std::string clarify(std::string repeat) {
    if (repeat != "yes" && repeat != "no") {
        while (true) {
            std::cout << "sorry I didn't get that, can you try entering your response again?" << std::endl;
            std::string repeatResponse;
            std::cin >> repeatResponse;

            if (repeatResponse == "no") {
                repeat == "no";
                break;
            }
            if (repeatResponse == "yes") {
                repeat == "yes";
                break;
            }
        }
    }
    if (repeat == "no") {
        return 0;
    }    
}

int main() {
    while (true) {
        std::cout << "Please enter the characters or numbers you would like to brute force." << std::endl;
        std::string input;
        std::cin >> input;
        int inputLength = input.length();
        std::cout << "Would you like us to automatically type those values that we bruteforce for you?" << std::endl;
        std::string type;
        std::cin >> type;

        while (type != "yes" && type != "no") {
            std::cout << "Sorry, I didn't get that. Please try entering either yes or no again." << std::endl;
            std::string redoInput;
            std::cin >> redoInput;
            if (redoInput == "yes") {
                type = "yes";
                break;
            }
            if (redoInput == "no") {
                type = "no";
                break;
            }
        }

        if (type == "no") {
            for (int i = 10; i > 0; i--) {
                std::cout << "Starting in " << i << " seconds...";
                Sleep(1000);
                clear();
            }
            bruteForce(input, inputLength, "");
        }

        if (type == "yes") {
            std::cout << "Would you like us to hit the enter key after each typing?" << std::endl;
            std::string enter;
            std::cin >> enter;
            //clarify(enter);
            for (int i = 10; i > 0; i--) {
             std::cout << "Starting in " << i << " seconds...";
                Sleep(1000);
                clear();
           }
            sendBruteForce(input, inputLength, "");
            if (enter == "yes") {
                keybd_event(VK_RETURN, 0, 0, 0);
                keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
            }
        }
        std::cout << "Would you like to brute force some more words or numbers?" << std::endl;
        std::string repeat;
        std::cin >> repeat;
        //clarify(repeat);
        if (repeat != "yes" && repeat != "no") {
                std::cout << "sorry I didn't get that, can you try entering your response again?" << std::endl;
                std::string repeatResponse;
                std::cin >> repeatResponse;

                if (repeatResponse == "no") {
                    repeat == "no";
                   
                }
                if (repeatResponse == "yes") {
                    repeat == "yes";
                    
                }
            }
        if (repeat == "no") {
            return 0;
        }
    }
    return 0;
}