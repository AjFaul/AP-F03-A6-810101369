#include <iostream>
#include <vector>
#include <sstream>


using namespace std;




vector<string>  split(string Line,char DELIMITER)
{
    istringstream ss(Line);
    string token;
    vector<string> inputSection;
    while (getline(ss,token,DELIMITER)){
        inputSection.push_back(token);
    }
    return inputSection;
}