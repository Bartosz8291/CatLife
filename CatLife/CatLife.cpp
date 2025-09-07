#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <vector>
#include <fstream>
#include <filesystem>
#include <windows.h>

std::string getAppDataPath() {
    char* appdata;
    size_t len;
    _dupenv_s(&appdata, &len, "APPDATA");
    if (appdata) {
        std::string path = std::string(appdata) + "\\CatLife";
        std::string command = "cmd /c mkdir \"" + path + "\"";
        system(command.c_str());
        free(appdata);
        return path + "\\save.json";
    }
    return "save.json";
}

class Cat {
private:
    std::string name;
    int age;
    int hunger;
    int happiness;
    int energy;
    int health;
    int day;
    bool isAlive;

public:
    Cat(std::string catName) : name(catName), age(0), hunger(80), happiness(70), energy(90), health(95), day(1), isAlive(true) {}
    Cat() : name(""), age(0), hunger(80), happiness(70), energy(90), health(95), day(1), isAlive(true) {}

    void saveToFile(const std::string& filepath) {
        std::ofstream file(filepath);
        if (file.is_open()) {
            file << "{\n";
            file << "  \"name\": \"" << name << "\",\n";
            file << "  \"age\": " << age << ",\n";
            file << "  \"hunger\": " << hunger << ",\n";
            file << "  \"happiness\": " << happiness << ",\n";
            file << "  \"energy\": " << energy << ",\n";
            file << "  \"health\": " << health << ",\n";
            file << "  \"day\": " << day << ",\n";
            file << "  \"isAlive\": " << (isAlive ? "true" : "false") << "\n";
            file << "}\n";
            file.close();
        }
    }

    bool loadFromFile(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) return false;
        std::string line;
        while (std::getline(file, line)) {
            if (line.find("\"name\"") != std::string::npos) {
                size_t start = line.find("\"", line.find(":") + 1) + 1;
                size_t end = line.find("\"", start);
                name = line.substr(start, end - start);
            }
            else if (line.find("\"age\"") != std::string::npos) {
                age = std::stoi(line.substr(line.find(":") + 1));
            }
            else if (line.find("\"hunger\"") != std::string::npos) {
                hunger = std::stoi(line.substr(line.find(":") + 1));
            }
            else if (line.find("\"happiness\"") != std::string::npos) {
                happiness = std::stoi(line.substr(line.find(":") + 1));
            }
            else if (line.find("\"energy\"") != std::string::npos) {
                energy = std::stoi(line.substr(line.find(":") + 1));
            }
            else if (line.find("\"health\"") != std::string::npos) {
                health = std::stoi(line.substr(line.find(":") + 1));
            }
            else if (line.find("\"day\"") != std::string::npos) {
                day = std::stoi(line.substr(line.find(":") + 1));
            }
            else if (line.find("\"isAlive\"") != std::string::npos) {
                isAlive = line.find("true") != std::string::npos;
            }
        }
        file.close();
        return true;
    }

    void displayStats() {
        std::cout << "\n════════════════════════════════\n";
        std::cout << "CAT " << name << "'s Life - Day " << day << " (Age: " << age << " years)\n";
        std::cout << "════════════════════════════════\n";
        std::cout << "Hunger:    "; displayBar(hunger);
        std::cout << "Happiness: "; displayBar(happiness);
        std::cout << "Energy:    "; displayBar(energy);
        std::cout << "Health:    "; displayBar(health);
        std::cout << std::endl;
        if (health < 20) std::cout << "WARNING:  " << name << " looks very sick and weak!\n";
        else if (hunger < 20) std::cout << "WARNING:  " << name << " is desperately hungry!\n";
        else if (happiness < 30) std::cout << "WARNING: " << name << " seems very sad and lonely...\n";
        else if (energy < 20) std::cout << "WARNING: " << name << " can barely keep eyes open!\n";
        else if (happiness > 80 && health > 80) std::cout << name << " is purring happily and healthy!\n";
        else if (age > 10) std::cout << name << " is getting old but still loving life!\n";
    }

    void displayBar(int value) {
        std::cout << "[";
        int bars = value / 10;
        for (int i = 0; i < 10; i++) {
            if (i < bars) std::cout << "{}";
            else std::cout << "[]";
        }
        std::cout << "] " << std::setw(3) << value << "/100\n";
    }

    void feed() {
        if (hunger >= 95) { std::cout << name << " sniffs the food but walks away - too full!\n"; happiness -= 5; return; }
        std::vector<std::string> foods = { "delicious salmon","warm milk","premium cat food","a caught mouse (ew!)","chicken treats" };
        int foodChoice = rand() % foods.size();
        std::cout << "You give " << name << " " << foods[foodChoice] << "!\n";
        int hungerGain = 20 + rand() % 15;
        int happinessGain = 8 + rand() % 7;
        hunger += hungerGain; happiness += happinessGain; health += 3;
        if (hunger > 100) hunger = 100;
        if (happiness > 100) happiness = 100;
        if (health > 100) health = 100;
        std::cout << name << " devours the meal happily!\n";
    }

    void play() {
        if (energy < 15) { std::cout << name << " just stares at you sleepily - too tired to play!\n"; return; }
        std::vector<std::string> activities = {
            "You dangle a yarn ball and " + name + " pounces like a wild tiger!",
            "Laser pointer time! " + name + " chases the mysterious red dot everywhere!",
            "Feather toy makes " + name + " jump and do acrobatic flips!",
            "You give " + name + " a cardboard box - instant entertainment!",
            name + " hunts a toy mouse with intense focus!",
            "Zoomies activated! " + name + " races around like crazy!"
        };
        int activity = rand() % activities.size();
        std::cout << activities[activity] << std::endl;
        int happinessGain = 15 + rand() % 20;
        int energyCost = 10 + rand() % 15;
        happiness += happinessGain; energy -= energyCost; health += 8;
        if (happiness > 100) happiness = 100;
        if (energy < 0) energy = 0;
        if (health > 100) health = 100;
    }

    void sleep() {
        std::cout << name << " finds the perfect cozy spot and curls up...\n";
        std::cout << "*soft purring* Zzz... Zzz... Zzz...\n";
        energy += 35 + rand() % 20;
        health += 8 + rand() % 7;
        happiness += 10;
        if (energy > 100) energy = 100;
        if (health > 100) health = 100;
        if (happiness > 100) happiness = 100;
        std::vector<std::string> wakeups = { name + " stretches luxuriously and yawns!",name + " wakes up refreshed and ready!",name + " had sweet dreams about tuna!" };
        std::cout << wakeups[rand() % wakeups.size()] << std::endl;
    }

    void pet() {
        std::cout << "You reach out to pet " << name << "...\n";
        std::vector<std::string> reactions = { name + " melts into your hand with pure bliss!",name + " rubs cheek against your palm and closes eyes contentedly!",name + " flops over for belly rubs - ultimate trust!",name + " head-butts your hand affectionately!",name + " allows exactly 3 pets then walks away with dignity!" };
        int reaction = rand() % reactions.size();
        std::cout << reactions[reaction] << std::endl;
        if (reaction == 4) { happiness += 5; }
        else { happiness += 12 + rand() % 8; }
        if (happiness > 100) happiness = 100;
    }

    void passTime() {
        hunger -= (8 + rand() % 12);
        happiness -= (5 + rand() % 8);
        energy -= (10 + rand() % 10);
        if (age > 8) health -= rand() % 3;
        int event = rand() % 20;
        if (event == 0) { std::cout << name << " chatters excitedly at the window!\n"; happiness += 15; }
        else if (event == 1) { std::cout << name + " finds a perfect sunbeam and basks gloriously!\n"; happiness += 12; energy += 5; }
        else if (event == 2) { std::cout << name + " caught a toy mouse and brought it as a 'gift'!\n"; happiness += 20; }
        else if (event == 3) { std::cout << name + " accidentally steps in water dish - dramatic reaction!\n"; happiness -= 10; }
        else if (event == 4) { std::cout << "Amazon delivery! " + name + " claims the box immediately!\n"; happiness += 18; }
        if (hunger < 0) hunger = 0;
        if (happiness < 0) happiness = 0;
        if (energy < 0) energy = 0;
        if (health < 0) health = 0;
        day++;
        if (day % 365 == 0) { age++; std::cout << "\nHappy Birthday! " << name << " is now " << age << " years old!\n"; }
        if (health <= 0 || hunger <= 0) { isAlive = false; }
    }

    bool getAlive() { return isAlive; }
    int getAge() { return age; }
    std::string getName() { return name; }
};

void showMenu() {
    std::cout << "\nWhat would you like to do?\n";
    std::cout << "1. Feed your cat\n";
    std::cout << "2. Play with your cat\n";
    std::cout << "3. Let your cat sleep\n";
    std::cout << "4. Pet your cat\n";
    std::cout << "5. Skip time\n";
    std::cout << "6. View detailed stats\n";
    std::cout << "7. Save game\n";
    std::cout << "8. Load game\n";
    std::cout << "9. Quit game\n";
    std::cout << "\nChoice: ";
}

int main() {
    SetConsoleTitle(L"CatLife");
    srand(time(0));
    std::cout << "Welcome to Cat Life Simulator!\n";
    std::cout << "-----------------------------------\n";
    Cat myCat;
    std::string savePath = getAppDataPath();
    std::cout << "1. Start new game\n";
    std::cout << "2. Load existing game\n";
    std::cout << "Choice: ";
    int startChoice;
    std::cin >> startChoice;
    std::cin.ignore();
    if (startChoice == 2) {
        if (myCat.loadFromFile(savePath)) { std::cout << "Game loaded successfully!\n"; }
        else {
            std::cout << "No save file found. Starting new game.\n";
            std::cout << "What would you like to name your kitten? ";
            std::string catName;
            std::getline(std::cin, catName);
            myCat = Cat(catName);
            std::cout << "\nCongratulations! You've adopted " << catName << "!\n";
        }
    }
    else {
        std::cout << "What would you like to name your kitten? ";
        std::string catName;
        std::getline(std::cin, catName);
        myCat = Cat(catName);
        std::cout << "\nCongratulations! You've adopted " << catName << "!\n";
    }
    while (myCat.getAlive()) {
        myCat.displayStats();
        showMenu();
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        std::cout << std::endl;
        switch (choice) {
        case 1: myCat.feed(); break;
        case 2: myCat.play(); break;
        case 3: myCat.sleep(); break;
        case 4: myCat.pet(); break;
        case 5: std::cout << "Time passes...\n"; myCat.passTime(); myCat.saveToFile(savePath); continue;
        case 6: myCat.saveToFile(savePath); continue;
        case 7: myCat.saveToFile(savePath); std::cout << "Game saved to " << savePath << "\n"; continue;
        case 8: if (myCat.loadFromFile(savePath)) { std::cout << "Game loaded successfully!\n"; }
              else { std::cout << "No save file found!\n"; } continue;
        case 9: myCat.saveToFile(savePath); std::cout << "Game saved! Thanks for playing! " << myCat.getName() << " will miss you!\n"; return 0;
        default: std::cout << "Invalid choice! Please try again.\n"; continue;
        }
        myCat.passTime();
        myCat.saveToFile(savePath);
        if (!myCat.getAlive()) {
            std::cout << "\nOh no... " << myCat.getName() << " has passed away...\n";
            std::cout << "They lived a good life of " << myCat.getAge() << " years.\n";
            std::cout << "Thank you for taking care of them.\n";
            std::cout << "\nPress Enter to exit...";
            std::cin.get();
        }
    }
    return 0;
}
