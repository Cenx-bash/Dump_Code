#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <cctype>
#include <random>
#include <sstream>
#include <map>
#include <limits>
#include <thread>
#include <chrono>

using namespace std;

// ==================== UTILITY FUNCTIONS ====================

void slowPrint(const string& text, int delayMs = 30) {
    for (char c : text) {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
    cout << endl;
}

void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

void pressAnyKey() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ==================== CORE DATA STRUCTURES ====================

struct GridPosition {
    int row;
    int col;
    
    GridPosition(int r = 0, int c = 0) : row(r), col(c) {}
    
    bool operator==(const GridPosition& other) const {
        return row == other.row && col == other.col;
    }
};

class PatternGrid {
private:
    vector<vector<char>> grid;
    int size;

public:
    PatternGrid(int gridSize = 4) : size(gridSize) {
        grid.resize(size, vector<char>(size, '_'));
    }
    
    PatternGrid(const vector<vector<char>>& initialGrid) {
        grid = initialGrid;
        size = grid.size();
    }
    
    int getSize() const { return size; }
    
    char getCell(int row, int col) const {
        if (row >= 0 && row < size && col >= 0 && col < size) {
            return grid[row][col];
        }
        return '_';
    }
    
    void setCell(int row, int col, char value) {
        if (row >= 0 && row < size && col >= 0 && col < size) {
            grid[row][col] = value;
        }
    }
    
    void fillRow(int row, char value) {
        if (row >= 0 && row < size) {
            for (int col = 0; col < size; col++) {
                grid[row][col] = value;
            }
        }
    }
    
    void fillColumn(int col, char value) {
        if (col >= 0 && col < size) {
            for (int row = 0; row < size; row++) {
                grid[row][col] = value;
            }
        }
    }
    
    void replaceAll(char oldVal, char newVal) {
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                if (grid[row][col] == oldVal) {
                    grid[row][col] = newVal;
                }
            }
        }
    }
    
    void clear() {
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                grid[row][col] = '_';
            }
        }
    }
    
    string toString() const {
        string result;
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                result += grid[row][col];
                if (col < size - 1) result += ' ';
            }
            if (row < size - 1) result += '\n';
        }
        return result;
    }
    
    bool operator==(const PatternGrid& other) const {
        if (size != other.size) return false;
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                if (grid[row][col] != other.grid[row][col]) {
                    return false;
                }
            }
        }
        return true;
    }
    
    PatternGrid getDifference(const PatternGrid& other) const {
        PatternGrid diff(size);
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                if (grid[row][col] == other.grid[row][col]) {
                    diff.setCell(row, col, grid[row][col]);
                } else {
                    diff.setCell(row, col, 'X');
                }
            }
        }
        return diff;
    }
};

// ==================== MESSAGE PROCESSING & NOISE SIMULATION ====================

class MessageNoiseSimulator {
private:
    mt19937 rng;
    double forgetProbability;
    double misinterpretProbability;
    
public:
    MessageNoiseSimulator(double forgetProb = 0.1, double misinterpretProb = 0.15) 
        : forgetProbability(forgetProb), misinterpretProbability(misinterpretProb) {
        random_device rd;
        rng.seed(rd());
    }
    
    string applyNoise(const string& message) {
        vector<string> words = splitMessage(message);
        vector<string> noisyWords;
        
        for (const auto& word : words) {
            uniform_real_distribution<double> dist(0.0, 1.0);
            double roll = dist(rng);
            
            if (roll < forgetProbability) {
                continue;
            } else if (roll < forgetProbability + misinterpretProbability) {
                noisyWords.push_back(misinterpretWord(word));
            } else {
                noisyWords.push_back(word);
            }
        }
        
        return joinWords(noisyWords);
    }
    
private:
    vector<string> splitMessage(const string& message) {
        vector<string> words;
        istringstream stream(message);
        string word;
        
        while (stream >> word) {
            words.push_back(word);
        }
        return words;
    }
    
    string joinWords(const vector<string>& words) {
        string result;
        for (size_t i = 0; i < words.size(); i++) {
            result += words[i];
            if (i < words.size() - 1) result += ' ';
        }
        return result;
    }
    
    string misinterpretWord(const string& word) {
        map<string, vector<string>> misinterpretations = {
            {"A", {"B", "eight", "hey"}},
            {"B", {"D", "P", "three"}},
            {"C", {"see", "sea", "G"}},
            {"D", {"B", "the", "P"}},
            {"ROW", {"COLUMN", "LINE", "ARRAY"}},
            {"COLUMN", {"ROW", "COLLUM", "COMLUMN"}},
            {"FILL", {"FULL", "FEEL", "PUT"}},
            {"SET", {"PUT", "SAT", "LET"}},
            {"REPLACE", {"SUBSTITUTE", "REPLAY", "DISPLACE"}},
            {"ALL", {"WHOLE", "EACH", "EVERY"}},
            {"FIRST", {"ONE", "1ST", "BEGINNING"}},
            {"SECOND", {"TWO", "2ND", "NEXT"}},
            {"THIRD", {"THREE", "3RD"}},
            {"FOURTH", {"FOUR", "4TH", "LAST"}}
        };
        
        string upperWord = toUpper(word);
        auto it = misinterpretations.find(upperWord);
        if (it != misinterpretations.end()) {
            uniform_int_distribution<int> dist(0, it->second.size() - 1);
            return it->second[dist(rng)];
        }
        
        if (word.length() > 2) {
            uniform_int_distribution<int> dist(0, word.length() - 1);
            int pos = dist(rng);
            string misinterpreted = word;
            misinterpreted.erase(pos, 1);
            return misinterpreted;
        }
        
        return word;
    }
    
    string toUpper(const string& str) {
        string result = str;
        transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }
};

// ==================== GAME ROLES ====================

class Dispatcher {
private:
    PatternGrid targetPattern;
    vector<string> sentMessages;
    
public:
    Dispatcher(const PatternGrid& target) : targetPattern(target) {}
    
    string getTargetDescription() const {
        stringstream ss;
        ss << "TARGET GRID (4×4)\n";
        ss << "Row 1: A B C D\n";
        ss << "Row 2: A A D D\n";
        ss << "Row 3: C B B A\n";
        ss << "Row 4: D C C C";
        return ss.str();
    }
    
    string createInitialDescription() {
        stringstream description;
        description << "The grid is four rows. ";
        description << "Row 1 is A, B, C, D. ";
        description << "Row 2 starts with two A's then two D's. ";
        description << "Row 3 begins with C, then two B's, then A. ";
        description << "Row 4 starts with D followed by three C's.";
        
        string message = description.str();
        sentMessages.push_back(message);
        return message;
    }
    
    const vector<string>& getSentMessages() const {
        return sentMessages;
    }
    
    const PatternGrid& getTargetPattern() const {
        return targetPattern;
    }
};

class Messenger {
private:
    shared_ptr<MessageNoiseSimulator> noiseSimulator;
    vector<string> receivedMessages;
    vector<string> sentMessages;
    int maxLinesPerTurn;
    bool canAskForRepeat;
    
public:
    Messenger(shared_ptr<MessageNoiseSimulator> simulator, int maxLines = 2, bool canAsk = true)
        : noiseSimulator(simulator), maxLinesPerTurn(maxLines), canAskForRepeat(canAsk) {}
    
    string processMessage(const string& message) {
        receivedMessages.push_back(message);
        
        string noisyMessage = noiseSimulator->applyNoise(message);
        string paraphrased = paraphraseMessage(noisyMessage);
        string limited = enforceLineLimit(paraphrased);
        
        sentMessages.push_back(limited);
        return limited;
    }
    
    bool canStillAsk() const {
        return canAskForRepeat;
    }
    
    void useRepeatAsk() {
        canAskForRepeat = false;
    }
    
    const vector<string>& getSentMessages() const {
        return sentMessages;
    }
    
private:
    string paraphraseMessage(const string& message) {
        vector<string> sentences = splitSentences(message);
        
        if (sentences.empty()) return message;
        
        mt19937 rng(random_device{}());
        if (sentences.size() > 1 && uniform_real_distribution<double>(0.0, 1.0)(rng) > 0.5) {
            shuffle(sentences.begin(), sentences.end(), rng);
        }
        
        return joinSentences(sentences);
    }
    
    vector<string> splitSentences(const string& text) {
        vector<string> sentences;
        istringstream stream(text);
        string sentence;
        
        while (getline(stream, sentence, '.')) {
            if (!sentence.empty()) {
                sentence.erase(0, sentence.find_first_not_of(" \t"));
                sentence.erase(sentence.find_last_not_of(" \t") + 1);
                if (!sentence.empty()) {
                    sentences.push_back(sentence);
                }
            }
        }
        return sentences;
    }
    
    string joinSentences(const vector<string>& sentences) {
        string result;
        for (size_t i = 0; i < sentences.size(); i++) {
            result += sentences[i];
            if (i < sentences.size() - 1) result += ". ";
        }
        return result;
    }
    
    string enforceLineLimit(const string& message) {
        vector<string> lines;
        istringstream stream(message);
        string line;
        
        while (getline(stream, line, '\n')) {
            lines.push_back(line);
        }
        
        if (lines.size() <= maxLinesPerTurn) {
            return message;
        }
        
        string result;
        for (int i = 0; i < maxLinesPerTurn && i < static_cast<int>(lines.size()); i++) {
            result += lines[i];
            if (i < maxLinesPerTurn - 1) result += '\n';
        }
        return result;
    }
};

class Builder {
private:
    PatternGrid currentGrid;
    vector<string> receivedInstructions;
    vector<string> actionLog;
    
public:
    Builder(int gridSize = 4) : currentGrid(gridSize) {}
    
    bool executeInstruction(const string& instruction) {
        receivedInstructions.push_back(instruction);
        string upperInstruction = toUpper(instruction);
        
        if (parseSetCommand(upperInstruction)) {
            return true;
        } else if (parseFillRowCommand(upperInstruction)) {
            return true;
        } else if (parseFillColumnCommand(upperInstruction)) {
            return true;
        } else if (parseReplaceCommand(upperInstruction)) {
            return true;
        } else {
            actionLog.push_back("Failed to parse: " + instruction);
            return false;
        }
    }
    
    const PatternGrid& getCurrentGrid() const {
        return currentGrid;
    }
    
    string getGridDisplay() const {
        string result = "[ _ _ _ _ ]\n[ _ _ _ _ ]\n[ _ _ _ _ ]\n[ _ _ _ _ ]\n";
        
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                size_t pos = row * 10 + col * 2 + 2;
                if (pos < result.length()) {
                    result[pos] = currentGrid.getCell(row, col);
                }
            }
        }
        return result;
    }
    
    const vector<string>& getActionLog() const {
        return actionLog;
    }
    
    void reset() {
        currentGrid.clear();
        receivedInstructions.clear();
        actionLog.clear();
    }
    
private:
    string toUpper(const string& str) {
        string result = str;
        transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }
    
    bool parseSetCommand(const string& instruction) {
        size_t setPos = instruction.find("SET");
        size_t putPos = instruction.find("PUT");
        
        if (setPos == string::npos && putPos == string::npos) {
            return false;
        }
        
        size_t startPos = (setPos != string::npos) ? setPos + 3 : putPos + 3;
        size_t eqPos = instruction.find('=', startPos);
        
        if (eqPos == string::npos) return false;
        
        string coordStr = instruction.substr(startPos, eqPos - startPos);
        string valueStr = instruction.substr(eqPos + 1);
        
        coordStr.erase(remove(coordStr.begin(), coordStr.end(), ' '), coordStr.end());
        coordStr.erase(remove(coordStr.begin(), coordStr.end(), '('), coordStr.end());
        coordStr.erase(remove(coordStr.begin(), coordStr.end(), ')'), coordStr.end());
        
        size_t commaPos = coordStr.find(',');
        if (commaPos == string::npos) return false;
        
        try {
            int row = stoi(coordStr.substr(0, commaPos)) - 1;
            int col = stoi(coordStr.substr(commaPos + 1)) - 1;
            char value = valueStr.empty() ? '_' : valueStr[0];
            
            if (row >= 0 && row < currentGrid.getSize() && col >= 0 && col < currentGrid.getSize()) {
                currentGrid.setCell(row, col, value);
                actionLog.push_back("SET(" + to_string(row + 1) + "," + to_string(col + 1) + ") = " + value);
                return true;
            }
        } catch (const exception&) {
            return false;
        }
        
        return false;
    }
    
    bool parseFillRowCommand(const string& instruction) {
        size_t fillPos = instruction.find("FILL");
        size_t rowPos = instruction.find("ROW");
        
        if (fillPos == string::npos || rowPos == string::npos) return false;
        
        size_t withPos = instruction.find("WITH", rowPos);
        if (withPos == string::npos) return false;
        
        string rowStr = instruction.substr(rowPos + 3, withPos - (rowPos + 3));
        string valueStr = instruction.substr(withPos + 4);
        
        try {
            int row = stoi(rowStr) - 1;
            char value = valueStr.empty() ? '_' : valueStr[0];
            
            if (row >= 0 && row < currentGrid.getSize()) {
                currentGrid.fillRow(row, value);
                actionLog.push_back("FILL ROW " + to_string(row + 1) + " WITH " + value);
                return true;
            }
        } catch (const exception&) {
            return false;
        }
        
        return false;
    }
    
    bool parseFillColumnCommand(const string& instruction) {
        size_t fillPos = instruction.find("FILL");
        size_t colPos = instruction.find("COLUMN");
        
        if (fillPos == string::npos || colPos == string::npos) return false;
        
        size_t withPos = instruction.find("WITH", colPos);
        if (withPos == string::npos) return false;
        
        string colStr = instruction.substr(colPos + 6, withPos - (colPos + 6));
        string valueStr = instruction.substr(withPos + 4);
        
        try {
            int col = stoi(colStr) - 1;
            char value = valueStr.empty() ? '_' : valueStr[0];
            
            if (col >= 0 && col < currentGrid.getSize()) {
                currentGrid.fillColumn(col, value);
                actionLog.push_back("FILL COLUMN " + to_string(col + 1) + " WITH " + value);
                return true;
            }
        } catch (const exception&) {
            return false;
        }
        
        return false;
    }
    
    bool parseReplaceCommand(const string& instruction) {
        size_t replacePos = instruction.find("REPLACE");
        size_t allPos = instruction.find("ALL", replacePos);
        
        if (replacePos == string::npos || allPos == string::npos) return false;
        
        size_t withPos = instruction.find("WITH", allPos);
        if (withPos == string::npos) return false;
        
        string oldValStr = instruction.substr(allPos + 3, withPos - (allPos + 3));
        string newValStr = instruction.substr(withPos + 4);
        
        char oldVal = oldValStr.empty() ? '_' : oldValStr[0];
        char newVal = newValStr.empty() ? '_' : newValStr[0];
        
        currentGrid.replaceAll(oldVal, newVal);
        actionLog.push_back("REPLACE ALL " + string(1, oldVal) + " WITH " + newVal);
        return true;
    }
};

// ==================== GAME MANAGER ====================

class DispatchGame {
private:
    unique_ptr<Dispatcher> dispatcher;
    unique_ptr<Messenger> messenger;
    unique_ptr<Builder> builder;
    shared_ptr<MessageNoiseSimulator> noiseSimulator;
    
    int totalTurns;
    int currentTurn;
    int maxTurns;
    int messageLimit;
    int messagesUsed;
    
public:
    DispatchGame(const PatternGrid& targetPattern, double noiseLevel = 0.2) 
        : totalTurns(0), currentTurn(0), maxTurns(20), messageLimit(20), messagesUsed(0) {
        
        noiseSimulator = make_shared<MessageNoiseSimulator>(noiseLevel * 0.5, noiseLevel * 0.75);
        dispatcher = make_unique<Dispatcher>(targetPattern);
        messenger = make_unique<Messenger>(noiseSimulator, 2, true);
        builder = make_unique<Builder>(targetPattern.getSize());
    }
    
    void playEpisode() {
        showIntroCutscene();
        
        clearScreen();
        slowPrint("🎮 GAME START — ROLE: DISPATCHER");
        slowPrint("\nYou receive the target pattern:\n");
        cout << dispatcher->getTargetDescription() << "\n\n";
        
        slowPrint("Your Only Ability:");
        slowPrint("Send text instructions to the Messenger.");
        slowPrint("No copy-pasting the grid. No images. You must rewrite it.\n");
        
        while (currentTurn < maxTurns && messagesUsed < messageLimit) {
            currentTurn++;
            cout << "--- Turn " << currentTurn << " ---\n";
            cout << "Messages used: " << messagesUsed << "/" << messageLimit << "\n\n";
            
            string dispatcherMessage;
            if (currentTurn == 1) {
                dispatcherMessage = dispatcher->createInitialDescription();
                cout << "Dispatcher: " << dispatcherMessage << "\n";
            } else {
                cout << "Enter your instructions to Messenger: ";
                string input;
                getline(cin, input);
                dispatcherMessage = input;
            }
            
            messagesUsed++;
            
            clearScreen();
            slowPrint("🏃‍♂️ ROLE: MESSENGER");
            slowPrint("\nThe messenger gets your text — but they must paraphrase it.\n");
            
            string messengerMessage = messenger->processMessage(dispatcherMessage);
            cout << "Messenger → Builder: " << messengerMessage << "\n\n";
            
            pressAnyKey();
            clearScreen();
            
            slowPrint("🏗️ ROLE: BUILDER");
            slowPrint("\nThe Builder receives the messenger's paraphrased instructions.\n");
            cout << "Current Grid:\n" << builder->getGridDisplay() << "\n";
            
            bool success = builder->executeInstruction(messengerMessage);
            if (!success) {
                cout << "Builder: I didn't understand that instruction.\n";
            } else {
                cout << "Builder executed instruction.\n";
            }
            
            cout << "\nUpdated Grid:\n" << builder->getGridDisplay() << "\n";
            
            if (isComplete()) {
                break;
            }
            
            if (currentTurn < maxTurns && messagesUsed < messageLimit) {
                cout << "\nOptions:\n";
                cout << "1. Send another message\n";
                if (messenger->canStillAsk()) {
                    cout << "2. Ask Messenger to repeat last message (Clarification)\n";
                }
                cout << "3. Give up and show results\n";
                
                string choice;
                cout << "Choose: ";
                getline(cin, choice);
                
                if (choice == "2" && messenger->canStillAsk()) {
                    messenger->useRepeatAsk();
                    string repeated = messenger->processMessage(dispatcherMessage);
                    cout << "Messenger (repeating): " << repeated << "\n";
                    builder->executeInstruction(repeated);
                    messagesUsed++;
                    cout << "Current Grid:\n" << builder->getGridDisplay() << "\n\n";
                    pressAnyKey();
                } else if (choice == "3") {
                    break;
                }
            }
            
            clearScreen();
            slowPrint("🎮 ROLE: DISPATCHER");
            cout << "\nTarget Pattern:\n" << dispatcher->getTargetDescription() << "\n\n";
        }
        
        showResults();
        showEpisodeSummary();
    }
    
    bool isComplete() const {
        return builder->getCurrentGrid() == dispatcher->getTargetPattern();
    }
    
    void showIntroCutscene() {
        clearScreen();
        slowPrint("🎬 EPISODE 1 — \"THE FIRST JOB\"");
        slowPrint("\n🌆 INTRO CUTSCENE (TEXT)\n");
        
        slowPrint("You arrive at Clearline Dispatch, a small logistics office with");
        slowPrint("flickering lights, humming servers, and stacks of half-finished paperwork.");
        slowPrint("\nA tired supervisor looks up from her desk.\n");
        
        slowPrint("Supervisor Mara: \"You must be the new Dispatcher.");
        slowPrint("We're short-staffed, so you're starting right now.");
        slowPrint("Your first task is simple: guide our warehouse team through a basic layout job.\"");
        
        slowPrint("\nShe hands you a clipboard with a 4×4 layout pattern printed on faded paper.");
        
        slowPrint("\nMara: \"You'll give instructions to the Messenger.");
        slowPrint("The Messenger will relay it to the Builder.");
        slowPrint("The Builder will recreate the pattern.");
        slowPrint("None of you can see each other's info.");
        slowPrint("If the reconstruction is correct, you pass your first day.\"");
        
        slowPrint("\nShe leans back:");
        slowPrint("\nMara: \"Welcome to dispatch.\"");
        
        slowPrint("\nFade to black.");
        pressAnyKey();
    }
    
    void showResults() {
        clearScreen();
        slowPrint("🔍 RESULT PHASE");
        slowPrint("\nSupervisor Mara reviews the final layout.\n");
        
        cout << "Target Pattern:\n" << dispatcher->getTargetDescription() << "\n\n";
        cout << "Final Builder Grid:\n" << builder->getGridDisplay() << "\n";
        
        if (isComplete()) {
            slowPrint("\nMara: \"Not bad, Dispatcher.");
            slowPrint("You handled your first transmission cleanly.");
            slowPrint("The team built the layout perfectly.");
            slowPrint("Let's see if you survive the next shift.\"");
            slowPrint("\n✅ Episode Complete");
        } else {
            slowPrint("\nMara: \"Close, but not enough.");
            slowPrint("Breakdown was either in your instructions or the Messenger's relay.");
            slowPrint("Review your communication.");
            slowPrint("Precision saves time — and money.\"");
            slowPrint("\n❌ Pattern Mismatch - Retry or Continue");
            
            PatternGrid diff = dispatcher->getTargetPattern().getDifference(builder->getCurrentGrid());
            cout << "\nDifference (X marks errors):\n" << diff.toString() << "\n";
        }
        
        pressAnyKey();
    }
    
    void showEpisodeSummary() {
        clearScreen();
        slowPrint("🏅 EPISODE SUMMARY");
        
        int totalCells = dispatcher->getTargetPattern().getSize() * dispatcher->getTargetPattern().getSize();
        int correctCells = 0;
        PatternGrid diff = dispatcher->getTargetPattern().getDifference(builder->getCurrentGrid());
        
        for (int row = 0; row < diff.getSize(); row++) {
            for (int col = 0; col < diff.getSize(); col++) {
                if (diff.getCell(row, col) != 'X') {
                    correctCells++;
                }
            }
        }
        
        double accuracy = (static_cast<double>(correctCells) / totalCells) * 100.0;
        
        cout << "\nMetrics:\n";
        cout << "• Accuracy of final grid: " << accuracy << "%\n";
        cout << "• Number of Messenger mistakes: " << countMessengerMistakes() << "\n";
        cout << "• Dispatcher clarity score: " << calculateClarityScore() << "/10\n";
        cout << "• Total messages exchanged: " << messagesUsed << "\n";
        cout << "• Turns taken: " << currentTurn << "\n";
        
        slowPrint("\nRewards:");
        slowPrint("+1 Communication Skill");
        slowPrint("+1 Pattern Complexity unlocked");
        slowPrint("\nNext episode: \"Episode 2 — Split Channels\"");
        
        pressAnyKey();
    }
    
private:
    int countMessengerMistakes() const {
        const vector<string>& sent = messenger->getSentMessages();
        int mistakes = 0;
        
        for (const auto& msg : sent) {
            if (msg.find("see") != string::npos || msg.find("sea") != string::npos || 
                msg.find("collum") != string::npos || msg.find("replay") != string::npos) {
                mistakes++;
            }
        }
        return mistakes;
    }
    
    int calculateClarityScore() const {
        const vector<string>& sent = dispatcher->getSentMessages();
        if (sent.empty()) return 0;
        
        int score = 0;
        string lastMessage = sent.back();
        
        if (lastMessage.find("row") != string::npos) score += 3;
        if (lastMessage.find("first") != string::npos || lastMessage.find("second") != string::npos ||
            lastMessage.find("third") != string::npos || lastMessage.find("fourth") != string::npos) {
            score += 2;
        }
        if (lastMessage.find("then") != string::npos || lastMessage.find("followed by") != string::npos) {
            score += 2;
        }
        if (lastMessage.length() < 100) score += 1;
        if (lastMessage.find("grid") != string::npos) score += 1;
        
        return min(10, score);
    }
};

// ==================== PATTERN GENERATOR ====================

class PatternGenerator {
public:
    static PatternGrid createEpisode1Pattern() {
        vector<vector<char>> pattern = {
            {'A', 'B', 'C', 'D'},
            {'A', 'A', 'D', 'D'},
            {'C', 'B', 'B', 'A'},
            {'D', 'C', 'C', 'C'}
        };
        return PatternGrid(pattern);
    }
};

// ==================== MAIN GAME LOOP ====================

int main() {
    clearScreen();
    slowPrint("🚚 Welcome to Clearline Dispatch - Episode 1: The First Job");
    
    PatternGrid target = PatternGenerator::createEpisode1Pattern();
    DispatchGame game(target, 0.2);
    game.playEpisode();
    
    cout << "\nPlay again? (y/n): ";
    string playAgain;
    getline(cin, playAgain);
    
    if (playAgain == "y" || playAgain == "Y") {
        main();
    }
    
    cout << "Thanks for playing Clearline Dispatch!\n";
    return 0;
}
