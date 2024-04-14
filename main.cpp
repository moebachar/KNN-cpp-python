#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <math.h>
#include <chrono>

using namespace std;
using namespace chrono;

void printTableHead(string** table, int n, int m) {
    for (int i = 0; i < min(n, 5); i++) {
        for (int j = 0; j < m; j++) {
            cout << table[i][j] << " ";
        }
        cout << endl;
    }
}

void printTableHead(double** table, int n, int m) {
    for (int i = 0; i < min(n, 5); i++) {
        for (int j = 0; j < m; j++) {
            cout << table[i][j] << " ";
        }
        cout << endl;
    }
}

bool isNumerical(const std::string& s) {
    
    if (s.empty()) return false;

    
    for (char c : s) {
        
        if (!isdigit(c) && c!='.') {
            return false;
        }
    }
    
    
    return true;
}

class CsvReader {
private:
    string filename;
    char delimiter;
    string** rawTable;
    string** textTable;
    double** numericalTable;

    
    void read_raw() {
        ifstream file(filename);
        string line;

        
        rawTable = new string*[n];
        for (int i = 0; i < n; i++) {
            rawTable[i] = new string[m];
        }

        int row = 0;
        while (getline(file, line)) {
            stringstream ss(line);
            string cell;
            int column = 0;
            while (getline(ss, cell, delimiter)) {
                rawTable[row][column++] = cell;
            }
            row++;
        }
        file.close();
    }

    
    void splitTable() {
        
        textTable = new string*[n];
        for (int i = 0; i < n; i++) {
            textTable[i] = new string[m_text];
        }

        numericalTable = new double*[n];
        for (int i = 0; i < n; i++) {
            numericalTable[i] = new double[m_numerical];
        }

        
        for (int i = 0; i < n; i++) {
            int textIndex = 0;
            int numericalIndex = 0;
            for (int j = 0; j < m; j++) {
                if (isNumerical(rawTable[i][j])) {
                    numericalTable[i][numericalIndex++] = stod(rawTable[i][j]);
                } else {
                    textTable[i][textIndex++] = rawTable[i][j];
                }
            }
        }
    }


    

public:
    int n;
    int m;
    int m_text;
    int m_numerical;
    set<string> unique;

    
    CsvReader(const string& filename, char delimiter = ',') : filename(filename), delimiter(delimiter) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Can't open file" << endl;
            return;
        }

        
        n = 0;
        m = 0;
        m_numerical = 0;
        m_text = 0;
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string cell;
            int columns_in_line = 0, text_columns_in_line = 0, numerical_columns_in_line = 0;
            while (getline(ss, cell, delimiter)) {
                columns_in_line++;
                if(isNumerical(cell)) numerical_columns_in_line++;
                else text_columns_in_line++;
            }
            m = max(m, columns_in_line);
            m_numerical = max(m_numerical, numerical_columns_in_line);
            m_text = max(m_text, text_columns_in_line);
            n++;
        }

        
        read_raw();

        
        splitTable();

        for(int i = 0; i < n; i++){
            unique.insert(rawTable[i][m - 1]);
        }

    }

    
    ~CsvReader() {
        for (int i = 0; i < n; i++) {
            delete[] rawTable[i];
        }
        delete[] rawTable;

        for (int i = 0; i < n; i++) {
            delete[] textTable[i];
        }
        delete[] textTable;

        for (int i = 0; i < n; i++) {
            delete[] numericalTable[i];
        }
        delete[] numericalTable;
    }

    
    string** getRawTable() {
        return rawTable;
    }

    
    string** getTextTable() {
        return textTable;
    }

    
    double** getNumericalTable() {
        return numericalTable;
    }

};


class KNN{

    public:

    int k;
    string filename;
    CsvReader reader;
    char label_type;
    char delimiter;
    KNN(int k, const string& filename, char delimiter = ','): k(k), delimiter(delimiter), reader(filename, delimiter), label_type(label_type){}

    string predict(string line){
        vector<string> text;
        vector<double> numerical;
        stringstream ss(line);
            string cell;
            while (getline(ss, cell, delimiter)) {
                if(isNumerical(cell)) numerical.push_back(stod(cell));
                else text.push_back(cell);
        }
        int n = reader.n;
        int m_text = reader.m_text;
        int m_numerical = reader.m_numerical;

        if(!isNumerical(reader.getRawTable()[0][reader.m - 1])){
            m_text--;
        }
        else{
            m_numerical--;
        }


        vector<pair<string, double>> distances(n);
        double d;
        for(int i = 0; i < n; i++){
            d = 0;
            if(m_text > 0){
                d += textDistance(text, i, m_text);
            }
            if(m_numerical > 0){
                d += euclideanDistance(numerical, i, m_numerical);
            }
            distances[i] = {reader.getRawTable()[i][reader.m - 1], d};
        }

        sort(distances.begin(), distances.end(), comparePairs);

        vector<string> votes(k);
        for(int i = 0; i < k; i++){
            votes[i] = distances[i].first;
        }
        string label ;
        int maximum = 0;
        for(string s : reader.unique){
            int occ = countOccurrences(votes, s);
            if(occ > maximum){
                maximum = occ;
                label = s;
            }
        }

        return label;

    }

    private:

    double textDistance(vector<string>& text,int i, int m_text){
        double occ = 0;
        for(int j = 0; j < m_text; j++){
            if(reader.getTextTable()[i][j] == text[j]) occ++;
        }
        return (double) (m_text - occ)/m_text;
    }

    double euclideanDistance(vector<double>& numerical, int i, int m_numerical){
        double maximum = 0;
        double distance = 0;
        for(int j = 0; j < m_numerical; j++){
            distance += pow(reader.getNumericalTable()[i][j] - numerical[j], 2);
            maximum = max(maximum, abs(reader.getNumericalTable()[i][j]));
        }
        return (double) sqrt(distance)/maximum;
    }

    static bool comparePairs(pair<string, double> a, pair<string, double> b) {
    return a.second < b.second;
    }

    int countOccurrences(const vector<string> &vec, string element) {
    int count = 0;
    for (string x : vec) {
        if (x == element) {
            count++;
        }
    }
    return count;
}


};

int main() {
    cout << "Enter filename (example : data.csv) : ";
    string filename;
    cin >> filename;

    cout << "Enter delimiter character (example : , ) : ";
    char delimiter;
    cin >> delimiter;
    
    // CsvReader reader(filename, delimiter);
 
    
    // string** rawTable = reader.getRawTable();
    // int n = reader.n;
    // int m = reader.m;
    // int m_text = reader.m_text;
    // int m_numerical = reader.m_numerical;

    
    // cout << "Raw Table:" << endl;
    // printTableHead(rawTable, n, m);
    // cout << endl;

    
    // string** textTable = reader.getTextTable();

    
    // cout << "Text Table:" << endl;
    // printTableHead(textTable, n, m_text);
    // cout << endl;

    
    // double** numericalTable = reader.getNumericalTable();

    
    // cout << "Numerical Table:" << endl;
    // printTableHead(numericalTable, n, m_numerical);
    // cout << endl;

    auto start = steady_clock::now();

    int k;
    cout << "Enter the K : ";
    cin >> k;

    KNN knn_classifier(k, filename, delimiter);

    cout << "Enter line to predict : ";
    string line;
    cin >> line;

    cout << "Predicted class : " << knn_classifier.predict(line) << endl;
    
    /*
    To test (data.csv) :

    6,190,92,0,0,35.5,0.278,66      1
    2,88,58,26,16,28.4,0.766,22     0
    9,170,74,31,0,44,0.403,43       1
    9,89,62,0,0,22.5,0.142,33       0
    10,101,76,48,180,32.9,0.171,63  0
    2,122,70,27,0,36.8,0.34,27      0
    5,121,72,23,112,26.2,0.245,30   0
    1,126,60,0,0,30.1,0.349,47      1
    1,93,70,31,0,30.4,0.315,23      0
    
    */


   auto end = steady_clock::now();
   auto duration = duration_cast<seconds>(end - start);
   cout << "Runtime: " << duration.count() << " Seconds" << endl;

   
    return 0;
}
