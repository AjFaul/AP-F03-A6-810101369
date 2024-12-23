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
    string get_district(){return district;}
    int get_open_time(){return open_time;}
    int get_close_time(){return close_time;}
    int get_num_of_table(){return num_of_table;}
    void print_menu(){
        for(auto it=menu_item.begin() ; it!=menu_item.end();it++){
            cout<<(*it).first<< ":"<<(*it).second<<endl;
        }
    }

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


class Dirstrict
{
private:
    string name;
    vector<string> neighbors;
public:
    Dirstrict(vector<string> data_line)
    {
        for(int i=0;i<data_line.size();i++)
        {
            if(i==0)
            {
                name=data_line[i];
                continue;
            }
            neighbors[i-1]=data_line[i];
        }
    }
};







class App
{
private:
    int status_app;
public:
    vector<Restaurant> restaurants ;
    vector<Dirstrict> dirstricts;
    App(){status_app=1;}
    void Add_restaurant(Restaurant restaurant){restaurants.push_back(restaurant);}
    void Add_district(Dirstrict district){dirstricts.push_back(district);}
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
        {
            num_line++;
            continue;
        }
        data_line=split(line,DELIMITER);
        Restaurant restaurant(data_line);
        app.Add_restaurant(restaurant);

    }
    file_restaurants.close();
}


void handle_districts(App& app,ifstream& file_district){
    string line;
    int num_line=0;
    vector<string> data_line;
    while (getline(file_district,line))
    {
        if(num_line==0)
        {
            num_line++;
            continue;
        }
        data_line=split(line,DELIMITER);
        Dirstrict dirstrict(data_line);
        app.Add_district(dirstrict);
    }
    file_district.close();
    
}


void handle_input_data(char* argv[],App& app)
{
    string name_file_restaurants=argv[1];
    // string name_file_districts=argv[2];
    
    ifstream file_restaurants(name_file_restaurants);
    if(!file_restaurants.is_open())
        cout<<"file is not open"<<endl;

    // ifstream file_districts(name_file_districts);

    handle_restaurants(app,file_restaurants);
    // handle_districts(app);

}













int main(int argc , char* argv[])
{
    App app;
    handle_input_data(argv,app);
    app.restaurants[1].print_menu();


}
