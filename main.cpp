#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <list>
#include <map>


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

void handle_restaurants(){}

void handle_districts(){}




void handle_input_data(char* argv[])
{
    string name_file_restaurants=argv[1];
    string name_file_districts=argv[2];
    
    ifstream file_restaurants(name_file_restaurants);
    ifstream file_districts(name_file_districts);

    handle_restaurants();
    handle_districts();

}













int main(int argc , char* argv[])
{

}
