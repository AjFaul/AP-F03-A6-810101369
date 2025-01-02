#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <list>
#include <map>
#include <algorithm>
#include <bits/stdc++.h>
using namespace std;


class Output_msg
{
private:
    int status=0;
    void OK(){ cout<<"OK"<<endl;}
    void Empty(){cout<<"Empty"<<endl;}
    void Not_Found(){cout<<"Not Found"<<endl;}
    void Bad_Request(){cout<<"Bad_Request"<<endl;}
    void Permission_Denied(){cout<<"Permission_Denied"<<endl;}
public:
    Output_msg(){status=1;}
};


class Reserve
{
private:
    int status;
    string restaurant_name;
    int table_id;
    vector<string> foods_name;
    vector<int> prices;
    int reserve_id;
public:
    Reserve(/* args */){status=1;}
};


class District
{
private:
    int status;
    string name;
    vector<string> neighbors;
public:

    District(vector<string> data_line)
    {
        for(int i=0;i<data_line.size();i++)
        {
            if(i==0)
            {
                name=data_line[i];
                continue;
            }
            neighbors.push_back(data_line[i]);
        }
    }

    string get_name(){return name;}


};





class Restaurant
{
private:
    int status;
    string name;
    string district;
    vector<Reserve> reserves;
    int open_time,close_time, num_of_table;
    map<string,int>menu_item; 

public:
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


class Client
{
private:
    int status;
    string username;
    string password;
    vector<Reserve> reserves;
public:
    Client(/* args */){status=1;}

};







class App
{
private:
    int status;
    vector<Restaurant> restaurants;
    vector<Client> clients;
    vector<District> distritcs;


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

    void update_district()
    {
        vector<string> names_district;
        for(int i=0;i<distritcs.size();i++)
            names_district.push_back(distritcs[i].get_name());
        sort(names_district.begin(),names_district.end());

        vector<District> cpy_district;
        for(int i=0;i<names_district.size();i++)
        {
            for(int j=0;j<distritcs.size();j++)
            {
                if(distritcs[j].get_name()==names_district[i])
                    cpy_district.push_back(distritcs[j]);
            }
        }
        distritcs=cpy_district;
    }






public:
    App(){status=1;}
    
};











int main()
{

}
