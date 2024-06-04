#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <cmath>
#include <TFile.h>
#include <TTree.h>
#include <glob.h>

using namespace std;

// Generate the root file from the csv file (names stored in a txt file)
vector<TTree*> read_csv(const string& directory, const string& txt, const string& treeName, const vector<string>& read_channel = {"CH1", "CH2", "CH4"}) {
    
    vector<TTree*> tree_vec = {};

    ifstream infile(directory+txt);    
    string csv_filename;       
    
    // Read the files one by one    
    int fileCount = 0;
    while (getline(infile, csv_filename)) {
        string filepath = directory + csv_filename;    
        filepath.erase(filepath.find_last_not_of(" \t\n\r\f\v") + 1);    
        cout << "Reading : " << filepath << endl;
        ifstream file(filepath);
        if (!file.is_open()) {
            cerr << "Could not open file: " << filepath << endl;
            return {};
        }
        
        // Define a tree for each file
        TTree *tree = new TTree(treeName.c_str(), "manchester_output");
        
        int event_number;
        tree->Branch("event_number", &event_number, "event_number/I");
        
        vector<float> *time = new vector<float>;
        tree->Branch("time", "vector<float>", &time);
        
        vector<vector<float>*> ch_vec(read_channel.size());
        for (int i = 0; i < read_channel.size(); i++) {
            ch_vec[i] = new vector<float>;
            tree->Branch((read_channel[i]).c_str(), "vector<float>", &ch_vec[i]);                
        }                                                    
        
        // Read the lines and find the columns of the desired variables
        string line, word;
        int time_column = -1;
        vector<int> ch_column_vec;
        for (int i = 0; i < read_channel.size(); i++) {
            int ch_column = -1;
            ch_column_vec.push_back(ch_column);
        }
        while (getline(file,line)) {
            if (line.find("TIME") != string::npos) {
                stringstream ss(line);                
                int column_count = 1;
                while (getline(ss, word, ',')) {
                    word.erase(remove_if(word.begin(), word.end(), [](unsigned char c) { return isspace(c); }), word.end());
                    if (word == "TIME" && time_column == -1)
                        time_column = column_count;
                    for (int i = 0; i < read_channel.size(); i++) {                            
                        if (word == read_channel[i] && ch_column_vec[i] == -1)
                            ch_column_vec[i] = column_count;
                    }                                                                        
                    column_count += 1;
                }
                break;
            }
        }
        if (time_column == -1 || (find(ch_column_vec.begin(), ch_column_vec.end(), -1) != ch_column_vec.end())) {
            cout << "Please check if the CSV file contains the variables you need." << endl;
            return {};
        }
 
        
        while (getline(file,line)) {
                
            stringstream ss(line);
            string value;
                        
            int column_count = 1;
            while (getline(ss, value, ',')) {                        
                if (column_count == time_column) {
                    if (std::isnan(stod(value)) || std::isinf(stod(value))) break;
                    time->push_back(stod(value));
                }
                
                bool need_break = false;
                for (int i = 0; i < read_channel.size(); i++) {
                    
                    if (column_count == ch_column_vec[i]) {
                        if (std::isnan(stod(value)) || std::isinf(stod(value))) {
                            need_break = true;
                            time->pop_back();
                            for (int j = 0; j < i; j++) {
                                ch_vec[j]->pop_back();
                            }
                            break;
                        }
                        ch_vec[i]->push_back(stod(value));
                    }
                }
                if (need_break) break;
                column_count += 1;
            }                     
        }
        
        event_number = fileCount;
        fileCount += 1;
        tree->Fill();
        tree_vec.push_back(tree);
        delete time;
        for (int i = 0; i < ch_vec.size(); i++) {
            delete ch_vec[i];
        }
        ch_vec.clear();
    }
    infile.close();
    return tree_vec;
}

// Generate the root file from the csv file (directly put the paths to the arguments)
vector<TTree*> read_csv(const vector<string>& csv_filenames, const string& treeName, const vector<string>& read_channel = {"CH1", "CH2", "CH4"}) {
    
    vector<TTree*> tree_vec = {};
    
    for (const auto& csv_pattern : csv_filenames) {
    
        glob_t glob_result;
        int glob_status = glob(csv_pattern.c_str(), GLOB_TILDE, nullptr, &glob_result);
        if (glob_status != 0) {
            continue;
        }
        
        int fileCount = 0;
        for (size_t i = 0; i < glob_result.gl_pathc; ++i) {
        
            string csv_filename = glob_result.gl_pathv[i];
            
            cout << "Reading : " << csv_filename << endl;

            ifstream file(csv_filename);
            if (!file.is_open()) {
                cerr << "Could not open file: " << csv_filename << endl;
                return {};
            }
            
            // Define a tree for each file
            TTree *tree = new TTree(treeName.c_str(), "manchester_output");
            
            int event_number;
            tree->Branch("event_number", &event_number, "event_number/I");
            
            vector<float> *time = new vector<float>;
            tree->Branch("time", "vector<float>", &time);
            
            vector<vector<float>*> ch_vec(read_channel.size());
            for (int i = 0; i < read_channel.size(); i++) {
                ch_vec[i] = new vector<float>;
                tree->Branch((read_channel[i]).c_str(), "vector<float>", &ch_vec[i]);                
            }                                                    
            
            // Read the lines and find the columns of the desired variables
            string line, word;
            int time_column = -1;
            vector<int> ch_column_vec;
            for (int i = 0; i < read_channel.size(); i++) {
                int ch_column = -1;
                ch_column_vec.push_back(ch_column);
            } 
            while (getline(file,line)) {
                if (line.find("TIME") != string::npos) {
                    stringstream ss(line);                
                    int column_count = 1;
                    while (getline(ss, word, ',')) {
                        word.erase(remove_if(word.begin(), word.end(), [](unsigned char c) { return isspace(c); }), word.end());
                        if (word == "TIME" && time_column == -1)
                            time_column = column_count;
                        for (int i = 0; i < read_channel.size(); i++) {                            
                            if (word == read_channel[i] && ch_column_vec[i] == -1)
                                ch_column_vec[i] = column_count;
                        }                                                                        
                        column_count += 1;
                    }
                    break;
                }
            }
            if (time_column == -1 || (find(ch_column_vec.begin(), ch_column_vec.end(), -1) != ch_column_vec.end())) {
                cout << "Please check if the CSV file contains the variables you need." << endl;
                return {};
            }
     
            
            while (getline(file,line)) {
                    
                stringstream ss(line);
                string value;
                            
                int column_count = 1;
                while (getline(ss, value, ',')) {                        
                    if (column_count == time_column) {
                        if (std::isnan(stod(value)) || std::isinf(stod(value))) break;
                        time->push_back(stod(value));
                    }
                    
                    bool need_break = false;
                    for (int i = 0; i < read_channel.size(); i++) {
                        
                        if (column_count == ch_column_vec[i]) {
                            if (std::isnan(stod(value)) || std::isinf(stod(value))) {
                                need_break = true;
                                time->pop_back();
                                for (int j = 0; j < i; j++) {
                                    ch_vec[j]->pop_back();
                                }
                                break;
                            }
                            ch_vec[i]->push_back(stod(value));
                        }
                    }
                    if (need_break) break;
                    column_count += 1;
                }                         
            }
            
            event_number = fileCount;
            fileCount += 1;
            tree->Fill();
            tree_vec.push_back(tree);
            delete time;
            for (int i = 0; i < ch_vec.size(); i++) {
                delete ch_vec[i];
            }
            ch_vec.clear();   
        }
    }   
    
    return tree_vec;
}

void mergeTrees(const string& outputName, const string& treeName, const vector<TTree*>& trees, const vector<string>& read_channel = {"CH1", "CH2", "CH4"}) {

    TFile *mergedFile = new TFile(outputName.c_str(), "RECREATE");
    TTree* mergedTree = new TTree(treeName.c_str(), "Merged Tree");  
    
    int event_number;
    mergedTree->Branch("event_number", &event_number, "event_number/I");
    vector<float>* time = new vector<float>;
    mergedTree->Branch("time", "vector<float>", &time);        
    vector<vector<float>*> ch_vec(read_channel.size());
    for (int i = 0; i < read_channel.size(); i++) {
        ch_vec[i] = new vector<float>;
        mergedTree->Branch((read_channel[i]).c_str(), "vector<float>", &ch_vec[i]);
    }
            
    for (const auto& tree : trees) {
        tree->SetBranchAddress("event_number", &event_number);
        tree->SetBranchAddress("time", &time);
        for (int i = 0; i < read_channel.size(); i++) {
            tree->SetBranchAddress((read_channel[i]).c_str(), &ch_vec[i]);
        }

        Long64_t nEntries = tree->GetEntries();        
        for (Long64_t iEntry = 0; iEntry < nEntries; ++iEntry) {
            tree->GetEntry(iEntry);      
            mergedTree->Fill();
        }
    }
    
    delete time;
    for (int i = 0; i < ch_vec.size(); i++) {
        delete ch_vec[i];
    } 
    
    mergedFile->cd();
    mergedTree->Write();
    mergedFile->Close();
}

int csv_to_root() {
    
    string directory = "./";
    string txtfile = "csvFileList.txt";  
    vector<string> read_channel = {"CH1", "CH2", "CH3", "CH4"};  

    vector<TTree*> trees = read_csv(directory, txtfile, "tree", read_channel);     
    mergeTrees("merged_data.root", "tree", trees, read_channel);
    
    return 0;
}