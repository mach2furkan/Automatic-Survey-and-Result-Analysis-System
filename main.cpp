#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>


using namespace std;

// Utility function to split a string by delimiter
vector<string> split(const string &str, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(str);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Class to handle survey questions and responses
class Survey {
private:
    string questionsFile;
    string responsesFile;
    vector<string> questions;

public:
    Survey(const string &qFile, const string &rFile) : questionsFile(qFile), responsesFile(rFile) {}

    void createSurvey() {
        ofstream qFile(questionsFile, ios::app);
        if (!qFile.is_open()) {
            cerr << "Error opening file for writing questions." << endl;
            return;
        }

        int numQuestions;
        cout << "Enter the number of questions: ";
        cin >> numQuestions;
        cin.ignore();

        for (int i = 0; i < numQuestions; ++i) {
            string question;
            cout << "Enter question " << i + 1 << ": ";
            getline(cin, question);
            qFile << question << endl;
            questions.push_back(question);
        }

        qFile.close();
    }

    void loadQuestions() {
        ifstream qFile(questionsFile);
        if (!qFile.is_open()) {
            cerr << "Error opening file for reading questions." << endl;
            return;
        }

        string line;
        while (getline(qFile, line)) {
            questions.push_back(line);
        }

        qFile.close();
    }

    void startSurvey() {
        if (questions.empty()) {
            loadQuestions();
        }

        if (questions.empty()) {
            cout << "No questions available." << endl;
            return;
        }

        ofstream rFile(responsesFile, ios::app);
        if (!rFile.is_open()) {
            cerr << "Error opening file for writing responses." << endl;
            return;
        }

        for (const auto &question : questions) {
            cout << question << endl;
            string response;
            getline(cin, response);
            rFile << response << ",";
        }
        rFile << endl;

        rFile.close();
    }

    void analyzeResults() {
        ifstream rFile(responsesFile);
        if (!rFile.is_open()) {
            cerr << "Error opening file for reading responses." << endl;
            return;
        }

        map<string, int> responseCount;
        string line;
        while (getline(rFile, line)) {
            vector<string> responses = split(line, ',');
            for (const auto &response : responses) {
                responseCount[response]++;
            }
        }

        rFile.close();

        cout << "Survey Results Analysis:" << endl;
        for (const auto &[response, count] : responseCount) {
            cout << response << ": " << count << " responses" << endl;
        }
    }

    void generateReport() {
        ifstream rFile(responsesFile);
        if (!rFile.is_open()) {
            cerr << "Error opening file for reading responses." << endl;
            return;
        }

        map<string, int> responseCount;
        string line;
        vector<vector<string>> allResponses;
        while (getline(rFile, line)) {
            vector<string> responses = split(line, ',');
            allResponses.push_back(responses);
            for (const auto &response : responses) {
                responseCount[response]++;
            }
        }

        rFile.close();

        cout << "Survey Results Analysis:" << endl;
        for (const auto &[response, count] : responseCount) {
            cout << response << ": " << count << " responses" << endl;
        }

        // Generate detailed report
        ofstream reportFile("survey_report.csv");
        if (!reportFile.is_open()) {
            cerr << "Error opening file for writing report." << endl;
            return;
        }

        // Write header
        reportFile << "Question,Response,Count" << endl;
        for (const auto &[response, count] : responseCount) {
            reportFile << response << "," << count << endl;
        }

        reportFile.close();
        cout << "Report generated: survey_report.csv" << endl;
    }
};

void showMenu() {
    cout << "1. Create Survey" << endl;
    cout << "2. Start Survey" << endl;
    cout << "3. Analyze Results" << endl;
    cout << "4. Generate Report" << endl;
    cout << "5. Exit" << endl;
    cout << "Choose an option: ";
}

int main() {
    Survey survey("questions.csv", "responses.csv");

    while (true) {
        showMenu();
        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                survey.createSurvey();
                break;
            case 2:
                survey.startSurvey();
                break;
            case 3:
                survey.analyzeResults();
                break;
            case 4:
                survey.generateReport();
                break;
            case 5:
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
