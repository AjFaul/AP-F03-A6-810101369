#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <list>
#include <map>
using namespace std;

const char DELIMITER='/';

class User
{
private:
    int status;
    string username;
    string password;
public:
    User(){status=1;}
    User(string name,string pass)
    {
        status=1;
        username=name;
        password=pass;
    }
    string get_username(){return username;}
    string get_password(){return password;}
};

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
            neighbors.push_back(data_line[i]);
        }
    }

    string get_name(){return name;}
    void show_neighbor()
    {
        cout<<name<<":";
        for(int i=0;i<neighbors.size()-1;i++)
            cout<<" "<<neighbors[i]<<",";
        cout<<" "<<neighbors[neighbors.size()-1]<<endl;
    }
};

class App
{
private:
    int status_app;
public:
    vector<User> users;
    vector<Restaurant> restaurants ;
    vector<Dirstrict> dirstricts;
    App(){status_app=1;}
    void Add_restaurant(Restaurant restaurant){restaurants.push_back(restaurant);}
    void Add_district(Dirstrict district){dirstricts.push_back(district);}
    void Add_user(string username , string password){users.push_back(User(username , password));}
};

class Output_msg
{
private:
    int status=0;
public:
    Output_msg(){status=1;}
    void OK(){ cout<<"OK"<<endl;}
    void Empty(){cout<<"Empty"<<endl;}
    void Not_Found(){cout<<"Not Found"<<endl;}
    void Bad_Request(){cout<<"Bad_Request"<<endl;}
    void Permission_Denied(){cout<<"Permission_Denied"<<endl;}
};

template <typename I,typename T>
int has_username(I onething , T something)
{
    for(int i=0 ; i<something.size(); i++)
    {
        if(onething == something[i].get_username())
            return (i+1);
    }
    return 0;
}

class POST :public Output_msg
{
private:
    int status_post=0;
public:
    POST(){status_post=1;}

    void signup(string username , string password,App& app)
    {
        if(has_username(username, app.users))
            Output_msg::Bad_Request();
        else
            app.users.push_back(User(username,password));
    }

    int login(string username, string password,App& app)
    {
        int has_user=has_username(username, app.users);
        if(has_user)   
        {
            if(app.users[has_user-1].get_password() == password)
                return 1;
            else
                {
                    Output_msg::Permission_Denied();
                    return 0;
                }
        }else
            {
                Output_msg::Not_Found();
                return 0;
            }
    }

};



template <typename I, typename T>
int has_district(I onething , T something)
{
    for(int i=0; i<something.size() ;i++)
    {
        if(onething==something[i].get_name())
            return i+1;
    }
    return 0;
}

class GET : public Output_msg
{
private:
    int status=0;
public:
    GET(){status=1;}

    void districts(string name_district,App& app)
    {
        if(name_district=="NULL")
            Output_msg::Empty();
        else
        {
            int status_districts=has_district(name_district,app.dirstricts);
            if(status_districts)
                app.dirstricts[status_districts-1].show_neighbor();
            else
                Output_msg::Not_Found();
        }
    }

    void 
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
    string name_file_districts=argv[2];
    
    ifstream file_restaurants(name_file_restaurants);
    if(!file_restaurants.is_open())
        cout<<"file is not open"<<endl;

    ifstream file_districts(name_file_districts);
    if(!file_districts.is_open())
        cout<<"file is not open"<<endl;

    handle_restaurants(app,file_restaurants);
    handle_districts(app,file_districts);


}






int main(int argc , char* argv[])
{
    App app;
    handle_input_data(argv,app);
    
    GET get;
    get.districts("Enghelab",app);


}
