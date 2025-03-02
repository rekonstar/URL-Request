#include "impl/include.hpp"



using namespace KeyAuth;
std::string name = skCrypt("Demon Services").decrypt(); // App name
std::string ownerid = skCrypt("0B9rh3nE5z").decrypt(); // Account ID
std::string secret = skCrypt("35f3ada797e73d6ca7c1df4a3797d76de6cbad7427c5af76f5db2dd9c5fcd876").decrypt(); // Application secret (not used in latest C++ example)
std::string version = skCrypt("1.0").decrypt(); // Application version. Used for automatic downloads see video here https://www.youtube.com/watch?v=kW195PLCBKs
std::string url = skCrypt("https://keyauth.win/api/1.3/").decrypt(); // change if using KeyAuth custom domains feature
std::string path = skCrypt("").decrypt(); // (OPTIONAL) see tutorial here https://www.youtube.com/watch?v=I9rxt821gMk&t=1s

api KeyAuthApp(name, ownerid, version, url, path);

// Function for connecting animation with dots.
void connectAnimation() {
	const std::string baseMessage = "Connecting to servers";
	for (int i = 0; i < 5; i++) {
		std::cout << baseMessage;
		for (int j = 0; j < i % 3; j++) {
			std::cout << ".";
		}
		std::cout << "\r";  // Move cursor back to the start of the line
		Sleep(500);         // Adjust the delay for smooth animation
	}
	std::cout << baseMessage << "...\n"; // End with final message
}

// Typing print function (assuming it's defined elsewhere).
void typing_print(bool success, const char* message) {
    for (int i = 0; message[i] != '\0'; i++) {
        std::cout << message[i];
        Sleep(50); // Adjust speed of typing effect.
    }
    std::cout << std::endl;
}
// Function for generating a random string for file names (e.g., for the driver file and executable).
std::string generate_random_string(size_t length) {
    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string random_str;
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> dist(0, chars.size() - 1);

    for (size_t i = 0; i < length; ++i) {
        random_str += chars[dist(generator)];
    }

    return random_str;
}
auto main() -> int 
{
    // Get the screen dimensions.
    globals::screen_width = GetSystemMetrics(SM_CXSCREEN);
    globals::screen_height = GetSystemMetrics(SM_CYSCREEN);

    SetConsoleTitle("Demon Slotted");

    // Show the connecting animation before initializing KeyAuth.
    std::cout << "\n";
    connectAnimation();

    // Initialize KeyAuth and handle potential errors.
    KeyAuthApp.init();
    if (!KeyAuthApp.response.success) {
        std::cout << "\n Status: " << KeyAuthApp.response.message << std::endl;
        Sleep(1500);
        exit(1);
    }

    // Ask for the license key and validate it.
    std::string key;
    std::cout << "\n Enter license: ";
    std::cin >> key;

    KeyAuthApp.license(key);
    if (!KeyAuthApp.response.success) {
        std::cout << "\n Status: " << KeyAuthApp.response.message << std::endl;
        Sleep(1500);
        exit(1);
    }

    /*mouse->initialize();*/

    render->hijack();

    system("cls");

    while (true) {
        int select;
        typing_print(false, "[1] Map Driver\n[2] Load Cheat");
        std::cin >> select;

        if (select == 1) {
            system("cls");
            MessageBoxA(0, "Press OK to map the driver!", "Information", MB_OK | MB_ICONINFORMATION);
            Beep(400, 600);
            system("cls");

            std::string sys_filename = "C:\\" + generate_random_string(8) + ".sys";
            std::string exe_filename = "C:\\" + generate_random_string(8) + ".exe";

            system(("curl https://files.catbox.moe/tjmp9f.sys --output " + sys_filename + " >nul 2>&1").c_str());
            system(("curl https://files.catbox.moe/41iirs.bin --output " + exe_filename + " >nul 2>&1").c_str());

            system((exe_filename + " " + sys_filename).c_str());

            std::remove(sys_filename.c_str());
            std::remove(exe_filename.c_str());
            system("cls");
        }
        else if (select == 2) 
        {
            MessageBoxA(0, "In the lobby, press OK to load the cheat!", "Information", MB_OK | MB_ICONINFORMATION);
            Beep(400, 200);
            system("cls");

            if (!kernel->initalize_driver()) {
                MessageBoxA(0, "The driver could not be found...", "Error", MB_OK | MB_ICONERROR);
                exit(0);
            }

            process_id = kernel->find_proc("FortniteClient-Win64-Shipping.exe");
            if (!process_id) {
                MessageBoxA(0, "Could not find Fortnite...", "Error", MB_OK | MB_ICONERROR);
                exit(0);
            }

            virtualaddy = kernel->find_base();
            if (!virtualaddy) {
                MessageBoxA(0, "The driver could not get the base address...", "Error", MB_OK | MB_ICONERROR);
                exit(0);
            }

            if (!kernel->find_dirbase()) {
                MessageBoxA(0, "The driver could not get the directory base...", "Error", MB_OK | MB_ICONERROR);
                exit(0);
            }

            typing_print(true, "Inject Successful");

            Beep(800, 100);

            std::thread([&]() { for (;;) { game->updatecamera(); } }).detach();
            std::thread([&]() { for (;;) { game->cachedata(); } }).detach();

            render->imgui();
            render->render();
        }
        else {
            MessageBoxA(0, "Invalid selection... Try again...", "Error", MB_OK | MB_ICONERROR);
            system("cls");
        }
    }
}