#include<string>
#include<sstream>
#include<iostream>
#include<fstream>
#include<vector>

using namespace std;

int main()
{
    vector<string> words;

    string line;
    string token;
    ifstream is;
    

    is.open("./bb.trc");
    
    while(!is.eof()){
        getline(is, line);
        stringstream ss;
        ss.str(line);

        while (ss >> token) {
            cout << token << endl;
//             words.push_back(token);
        }
        
        // for(vector<string>::iterator p = words.begin();
        //     p != words.end(); ++p){
        //     cout << *p << endl;
        // }
    }
    return 0;
}