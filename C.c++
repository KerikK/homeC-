#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <algorithm>
#include <cctype>

using namespace std;

struct Question {
    string text;
    string answer; 
};

void setColor(int code) {
    cout << "\033[" << code << "m";
}

void resetColor() {
    cout << "\033[0m";
}

void delay(int milliseconds) {
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

void spinWheel() {
    setColor(33); 
    cout << "\n\t[ Spinning the Wheel: ";
    char animation[] = {'|', '/', '-', '\\'};
    for (int i = 0; i < 12; ++i) {
        cout << animation[i % 4] << "\b" << flush;
        delay(100);
    }
    cout << "Sector 'Play' on the wheel! ]\n\n";
    resetColor();
}

void printBorder() {
    setColor(36); 
    cout << "\t==================================================\n";
    resetColor();
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    vector<Question> bank = {
        {"The capital city of the United Kingdom?", "LONDON"},
        {"The largest ocean on Planet Earth?", "PACIFIC"},
        {"An input device used for typing on a computer?", "KEYBOARD"},
        {"Natural satellite of the Earth?", "MOON"},
        {"The hot central element of our solar system?", "SUN"}
    };

    Question current = bank[rand() % bank.size()];
    string secretWord = current.answer;
    string guessedWord(secretWord.length(), '*');

    int hintsUsed = 0;
    
 
    auto startTime = chrono::high_resolution_clock::now();

    cout << "\n";
    printBorder();
    setColor(35); 
    cout << "\t\t   WHEEL OF FORTUNE GAME\n";
    printBorder();
    
    spinWheel();

    while (guessedWord != secretWord) {
        printBorder();
        setColor(32); 
        cout << "\tQuestion: " << current.text << "\n";
        resetColor();
        
        cout << "\tEncrypted Word: ";
        setColor(33);
        for (char c : guessedWord) cout << c << ' ';
        cout << "\n";
        resetColor();

        printBorder();
        cout << "\tSelect an action:\n";
        cout << "\t1. Guess a single letter\n";
        cout << "\t2. Guess the whole word\n";
        cout << "\t3. Use a hint (+2 seconds penalty)\n";
        cout << "\tYour choice (1-3): ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\t[!] Invalid input. Please enter a number.\n";
            continue;
        }

        if (choice == 1) {
            cout << "\tEnter a letter: ";
            string letterInput;
            cin >> letterInput;
            
            string letter = letterInput;
            transform(letter.begin(), letter.end(), letter.begin(), ::toupper);

            bool found = false;
            for (size_t i = 0; i < secretWord.length(); ++i) {
                if (secretWord.substr(i, letter.length()) == letter) {
                    guessedWord.replace(i, letter.length(), letter);
                    found = true;
                }
            }

            if (found) {
                setColor(32);
                cout << "\n\t[✓] Great choice! The letter is in the word!\n\n";
            } else {
                setColor(31); 
                cout << "\n\t[X] Sorry, that letter is not in the word.\n\n";
            }
            resetColor();
            delay(500);

        } else if (choice == 2) {
            cout << "\tEnter the whole word: ";
            string fullWord;
            cin >> fullWord;
            transform(fullWord.begin(), fullWord.end(), fullWord.begin(), ::toupper);

            if (fullWord == secretWord) {
                guessedWord = secretWord;
                setColor(32);
                cout << "\n\t[✓] Correct! You guessed the entire word!\n\n";
                resetColor();
            } else {
                setColor(31);
                cout << "\n\t[X] Incorrect! That's not the word. Keep trying!\n\n";
                resetColor();
            }
            delay(500);

        } else if (choice == 3) {
            int unrevealedIndex = -1;
            for (size_t i = 0; i < guessedWord.length(); ++i) {
                if (guessedWord[i] == '*') {
                    unrevealedIndex = static_cast<int>(i);
                    break;
                }
            }

            if (unrevealedIndex != -1) {
                guessedWord[unrevealedIndex] = secretWord[unrevealedIndex];
                hintsUsed++;
                setColor(36);
                cout << "\n\t[💡] Hint used! One letter revealed (+2 sec penalty added).\n\n";
                resetColor();
            } else {
                cout << "\n\t[!] All letters are already revealed!\n\n";
            }
            delay(500);

        } else {
            cout << "\t[!] Please enter a valid option between 1 and 3.\n";
        }
    }

    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = endTime - startTime;

    double baseTime = elapsed.count();
    int penaltyTime = hintsUsed * 2;
    double totalTime = baseTime + penaltyTime;


    printBorder();
    setColor(32);
    cout << "\t\tCONGRATULATIONS! YOU WON!\n";
    resetColor();
    cout << "\tThe secret word was: ";
    setColor(33);
    cout << secretWord << "\n";
    resetColor();
    
    printBorder();
    cout << "\tGAME STATS:\n";
    cout << "\t- Elapsed time: " << static_cast<int>(baseTime) << " sec.\n";
    cout << "\t- Hints used: " << hintsUsed << " (+" << penaltyTime << " sec penalty)\n";
    setColor(35);
    cout << "\t- TOTAL FINAL TIME: " << static_cast<int>(totalTime) << " sec.\n";
    resetColor();
    printBorder();

    return 0;
}