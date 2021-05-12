#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

int stepsAmount;

int firstPlayerPoints = 0;
int firstPlayerMaxPoints = 0;
int firstCurrentPlayer = true;

const int maxRows = 5;
const int maxCols = 5;

string grid[maxRows][maxCols];

vector<string> alphabet;
vector<string> words;
vector<string> usedWords;

bool isWord(const string &word);

bool isUnusedWord(const string &word);

bool foundLetterAround(int row, int col);

void putLetters();

void findWord(const string &word, int row, int col, const int &letterRow, const int &letterCol, bool (&visitedGrid)[maxRows][maxCols]);

void findWords(const int &letterRow, const int &letterCol);

int main() {
    std::ifstream file("files/input1.txt");

    if (!file) {
        std::cout << "No file!\n";
    } else {
        for (int i = 0; i < maxRows; ++i) {
            for (int ii = 0; ii < maxCols; ++ii) {
                file >> grid[i][ii];
            }
        }

        std::string word;
        while (file >> word) {
            words.push_back(word);

            for (char c: word) {
                if (find(alphabet.begin(), alphabet.end(), string(1, c)) == alphabet.end()) {
                    alphabet.emplace_back(1, c);
                }
            }
        }

        cout << "Максимальное количество ходов: ";
        cin >> stepsAmount;
        stepsAmount *= 2;

        putLetters();
        cout << "Первый игрок может набрать максимум: " << firstPlayerMaxPoints << " очков";
    }

    return 0;
}

bool isWord(const string &word) {
    return find(words.begin(), words.end(), word) != words.end();
}

bool isUnusedWord(const string &word) {
    return find(usedWords.begin(), usedWords.end(), word) == usedWords.end();
}

bool foundLetterAround(int row, int col) {
    return (row < maxRows - 1 && grid[row + 1][col] != "0")
           || (col < maxCols - 1 && grid[row][col + 1] != "0")
           || (row > 0 && grid[row - 1][col] != "0")
           || (col > 0 && grid[row][col - 1] != "0");
}

void putLetters() {
    for (int row = 0; row < maxRows; ++row) {
        for (int col = 0; col < maxCols; ++col) {
            if (grid[row][col] == "0" && foundLetterAround(row, col)) {
                for (const string &letter : alphabet) {
                    grid[row][col] = letter;

                    findWords(row, col);
                }

                grid[row][col] = "0";
            }
        }
    }
}

void findWord(const string &word, int row, int col, const int &letterRow, const int &letterCol, bool (&visitedGrid)[maxRows][maxCols]) {
    const int amountSteps = 4;
    int movesCol[amountSteps] = {-1, 0, 1, 0};
    int movesRow[amountSteps] = {0, 1, 0, -1};
    int rowStep, colStep;

    if (isWord(word) && isUnusedWord(word) && visitedGrid[letterRow][letterCol]) {
        usedWords.push_back(word);

        if(firstCurrentPlayer) {
            firstPlayerPoints += word.length();
        }

        if(firstPlayerPoints > firstPlayerMaxPoints) {
            firstPlayerMaxPoints = firstPlayerPoints;

        }

        firstCurrentPlayer = !firstCurrentPlayer;
        stepsAmount--;

        if(stepsAmount > 0) {
            putLetters();
        }

        stepsAmount++;
        firstCurrentPlayer = !firstCurrentPlayer;

        if(firstCurrentPlayer) {
            firstPlayerPoints -= word.length();
        }

        usedWords.pop_back();
    }

    for (int step = 0; step < amountSteps; ++step) {
        rowStep = row + movesRow[step];
        colStep = col + movesCol[step];

        if (rowStep >= maxRows || rowStep <= -1
            || colStep >= maxCols || colStep <= -1
            || grid[rowStep][colStep] == "0"
            || visitedGrid[rowStep][colStep]) {
            continue;
        }

        visitedGrid[rowStep][colStep] = true;

        findWord(word + grid[rowStep][colStep], rowStep, colStep, letterRow, letterCol, visitedGrid);

        visitedGrid[rowStep][colStep] = false;
    }
}

void findWords(const int &letterRow, const int &letterCol) {
    for (int row = 0; row < maxRows; ++row) {
        for (int col = 0; col < maxCols; ++col) {
            if (grid[row][col] != "0") {
                bool visitedGrid[maxRows][maxCols];
                for (int i = 0; i < maxRows; ++i) {
                    for (int ii = 0; ii < maxCols; ++ii) {
                        visitedGrid[i][ii] = false;
                    }
                }

                visitedGrid[row][col] = true;
                findWord(grid[row][col], row, col, letterRow, letterCol, visitedGrid);
            }
        }
    }
}