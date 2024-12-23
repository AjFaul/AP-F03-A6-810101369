#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <list>
#include <map>
using namespace std;

const char DELIMITER='/';


class Restaurant
{
private:
    string name , district;
    int open_time,close_time, num_of_table;
    map<string , int> menu_item;
public:

    string get_name(){return name;}

    Restaurant(vector <string> data)
    {
        int size=data.size();
        name=data[0];
        district=data[1];
        for(int i=2; i<(size - 3) ; i=i+2)
            menu_item[data[i]]=stoi(data[i+1]);
        open_time=stoi(data[size-3]);
        close_time=stoi(data[size-2]);
        num_of_table=stoi(data[size-1]);
    }
};


class App
{
private:
    int status_app;
public:
    vector<Restaurant> restaurants ;
    App(){status_app=1;}
    void Add_restaurant(Restaurant restaurant){restaurants.push_back(restaurant);}

};


void update_with_slash(string& line)
{
    for(int i=0;i<line.size() ; i++)
    {
        if(line[i] == ';' || line[i]== ',' || line[i]==':')
            line[i]='/';
    }

}

vector<string>  split(string Line,char DELIMITER)
{
    update_with_slash(Line);
    istringstream ss(Line);
    string token;
    vector<string> inputSection;
    while (getline(ss,token,DELIMITER)){
        inputSection.push_back(token);
    }
    return inputSection;
}


void handle_restaurants(App& app,ifstream& file_restaurants)
{
    string line;
    int num_line=0;
    vector<string>data_line;
    while (getline(file_restaurants,line))
    {
        if(num_line==0)
            continue;
        data_line=split(line,DELIMITER);
        Restaurant restaurant(data_line);
        app.Add_restaurant(restaurant);
    }
    file_restaurants.close();
}


void handle_districts(App& app){}


void handle_input_data(char* argv[],App& app)
{
    string name_file_restaurants=argv[1];
    // string name_file_districts=argv[2];
    
    ifstream file_restaurants(name_file_restaurants);
    // ifstream file_districts(name_file_districts);

    handle_restaurants(app,file_restaurants);
    // handle_districts(app);

}













int main(int argc , char* argv[])
{
    App app;
    handle_input_data(argv,app);
    // app.restaurants[0].get_name();


}
