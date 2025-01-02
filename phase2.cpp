#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <list>
#include <map>
#include <algorithm>
#include <bits/stdc++.h>
using namespace std;
const char DELIMITER='/';
const char DELIMITER_SPACE=' ';
const char DELIMITER_COMMA=',';
const string CMD_POST="POST";
const string CMD_GET="GET";
const string CMD_PUT="PUT";
const string CMD_DELETE="DELETE";

const int DE_ACTIVE=0;
const int ACTIVE=1;

const int WRONG_CMD=-1;
const int CMD_SIGNUP=11;
const int CMD_LOGIN=12;
const int CMD_LOGOUT=13;
const int CMD_FULL_DISTRICTS=21;
const int CMD_DISTRICTS=211;
const int CMD_PUT_DISTRICT=31;






















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

vector<string>  split_by_space(string Line,char DELIMITER)
{
    istringstream ss(Line);
    string token;
    vector<string> inputSection;
    while (getline(ss,token,DELIMITER)){
        inputSection.push_back(token);
    }
    return inputSection;
}


class Output_msg
{
private:
    int status=0;
    void OK(){ cout<<"OK"<<endl;}
    void Empty(){cout<<"Empty"<<endl;}
    void Not_Found(){cout<<"Not Found"<<endl;}
    void Bad_Request(){cout<<"Bad_Request"<<endl;}
    void Permission_Denied(){cout<<"Permission_Denied"<<endl;}
    friend class Analysis_input;
    friend class Server;
    friend class Check;
    friend class GET;
    friend class PUT;
public:
    Output_msg(){status=1;}
};



class Analysis_input
{
private:
    int status;
    Output_msg err;
public:
    Analysis_input(){status=1;}

    int CMD_CONTROLLER(string &line)
    {
        vector<string> input_sections;
        input_sections=split_by_space(line,DELIMITER_SPACE);
        string output_type;


        // lev set 0
        if(input_sections[0]==CMD_POST)
            output_type+="1";
        if(input_sections[0]==CMD_GET)
            output_type+="2";
        if(input_sections[0]==CMD_PUT)
            output_type+="3";

        // lev set 0 is ok?
        if(output_type.size()==0)
        {
            err.Not_Found();
            return -1;
        }


        // lev set 1
        if(input_sections[1]=="signup")
            output_type+="1";
        if(input_sections[1]=="login")
            output_type+="2";
        if(input_sections[1]=="logout")
            output_type+="3";
        if(input_sections[1]=="districts")
        {
            output_type+="1";
            auto it=find(input_sections.begin(), input_sections.end(),"district");
            if(it!=input_sections.end())
                output_type+="1";
        }
        if(input_sections[1]=="my_district")
            output_type+="1";





        return stoi(output_type);
    }



    vector<string> Analysis_username_password(string& line)
    {
        vector<string> output;
        vector<string> line_token= split_by_space(line,DELIMITER_SPACE);

        auto it=find(line_token.begin() , line_token.end(),"username");
        it++;
        output.push_back((*it));
        it=find(line_token.begin(),line_token.end(),"password");
        it++;
        output.push_back((*it));
        return output;
    }

    string get_district_name(vector<string>& line_token)
    {
        string output;
        for(int i=0;i<line_token.size();i++)
        {
            if(line_token[i]=="district")
            {
                for(int j=(i+1) ; j<line_token.size();j++)
                {
                    output+=line_token[j];
                    if((j+1)!=line_token.size())
                        output+=" ";
                }
            }
        }
        return output;
    }





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
    friend class App;
    friend class Check;
    friend class GET;
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
    friend class App;
    friend class Check;

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
    string district;
    vector<Reserve> reserves;
    friend class App;
    friend class Check;
    friend class Server;
public:
    Client(){}
    Client(string name, string pass)
    {
        username=name;
        password=pass;
    }

    void set_client(string name , string password_)
    {
        username=name;
        password=password_;
    }

    void set_district(string district_name){district=district_name;}
    // string get_name(){return username;}

};



class App
{
private:
    int status;
    vector<Restaurant> restaurants;
    vector<Client> clients;
    vector<District> distritcs;
    void Add_district(District district){distritcs.push_back(district);}
    void Add_restaurant(Restaurant restaurant){restaurants.push_back(restaurant);}


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
            District dirstrict(data_line);
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

    friend class Check;
    friend class GET;
    friend class Server;


public:
    App(){status=1;}
    
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
        update_district();
    }

    void Add_client(Client client)
    {
        clients.push_back(client);
    }

};


class Check
{
private:
    int status;
    Output_msg err;
public:
    Check(){status=1;}


    bool name_is_repeatly(string name, App& app)
    {
        vector<string> usernames;
        for(int i=0;i<app.clients.size();i++)
            usernames.push_back(app.clients[i].username);
        for(int i=0; i<usernames.size();i++)
        {
            if(usernames[i]==name)
                return true;
        }
        return false;
    }

    bool check_user_pass(vector<string> user_pass , App &app)
    {
        for(int i=0;i<app.clients.size();i++)
        {
            if(app.clients[i].username==user_pass[0] && app.clients[i].password!=user_pass[1])
            {
                err.Not_Found();
                return false;
            }
            if(app.clients[i].username==user_pass[0] && app.clients[i].password==user_pass[1])
                return true;
        }
        err.Not_Found();
        return false;
    }

    bool check_district_name(vector<District> districts ,string name)
    {
        for(int i=0;i<districts.size();i++)
        {
            if(name==districts[i].name)
            {
                err.OK();
                return true;
            }
        }
        err.Not_Found();
        return false;
    }

    bool check_visited(string name, vector<string>& visited)
    {
        for(int i=0;i<visited.size();i++)
        {
            if(name==visited[i])
                return true;
        }
        return false;
    }

    District find_district(string name, App& app)
    {
        for(int i=0;i<app.distritcs.size();i++)
        {
            if(app.distritcs[i].get_name()==name)
                return app.distritcs[i];
        }
        vector<string> data_dummy={name};
        District dummy(data_dummy);
        return dummy;
    }

    vector<string> bfs_function(vector<string>& neighbors , vector<string>&visited , App &app)
    {
        int number_of_new_visited=0;
        vector<string> new_neighbors;
        for(int i=0;i<neighbors.size();i++)
        {
            District cur_district=find_district(neighbors[i],app);
            for(int j=0;j<cur_district.neighbors.size();j++)
            {
                if(check_visited(cur_district.neighbors[j],visited))
                {
                    new_neighbors.push_back(cur_district.neighbors[j]);
                    visited.push_back(cur_district.neighbors[j]);
                    number_of_new_visited++;
                }
            }
        }
        if(number_of_new_visited==0)
            return visited;
        return bfs_function(new_neighbors,visited,app);
    }



};



class POST
{
private:
    int status;

    void signup(vector<string> username_password,App& app)
    {
        Client client(username_password[0], username_password[1]);
        app.Add_client(client);
    }

    Client login(vector<string> user_pass)
    {
        Client client(user_pass[0], user_pass[1]);
        return client;
    }






    friend class Server;
public:
    POST(/* args */){status=1;}

};

class GET
{
private:
    Output_msg err;

    void districts(string district_name,App& app)
    {
        if(app.distritcs.size()==0)
        {
            err.Empty();
            return;
        }

        if(district_name=="NULL")
        {
            for(int i=0 ; i<app.distritcs.size();i++)
            {
                cout<<app.distritcs[i].get_name()<<":";
                for(int j=0; j<app.distritcs[i].neighbors.size();j++)
                {
                    cout<<" "<<app.distritcs[i].neighbors[j];
                    if((j+1)!=app.distritcs[i].neighbors.size())
                        cout<<",";
                }
                cout<<endl;
            }
            

        }else
        {
            for(int i=0;i<app.distritcs.size();i++)
            {
                if(app.distritcs[i].get_name()==district_name)
                {
                    cout<<district_name<<":";
                    for(int j=0;j<app.distritcs[i].neighbors.size();j++)
                    {
                        cout<<" "<<app.distritcs[i].neighbors[j];
                        if((j+1)!=app.distritcs[i].neighbors.size())
                            cout<<",";
                    }
                    cout<<endl;
                    return;
                }
            }
            err.Not_Found();
        }
    }



    friend class Server;
public:
    GET(){}
};


class PUT
{
private:
    Output_msg err;
    void my_district(string name,Client client)
    {
        client.set_district(name);
    }

    friend class Server;
public:
    PUT(){}

};






class Server : public Check
{
private:
    int type_cmd;
    string line;
    Output_msg err;
    Analysis_input analysis;
    Client cur_client;
    void update_client(App&app)
    {
        for(int i=0;i<app.clients.size();i++)
        {
            if(app.clients[i].username== cur_client.username)
                app.clients[i]=cur_client;
        }
    }

    bool signup_server(App& app)
    {
        POST post;
        vector<string> line_tokens=analysis.Analysis_username_password(line);
        if(name_is_repeatly(line_tokens[0],app))
        {
            err.Bad_Request();
            return false;
        }
        else
        {
            cur_client.set_client(line_tokens[0], line_tokens[1]);
            post.signup(line_tokens,app);
            err.OK();
            return true;
        }
    }

    void login_server(App& app,int& login_status)
    {
        vector<string> line_tokens=analysis.Analysis_username_password(line);
        bool login_output=Check :: check_user_pass(line_tokens,app);
        login_status=(login_output)?ACTIVE:DE_ACTIVE;
        POST post;
        if(login_status)
        {
            cur_client=post.login(line_tokens);
            err.OK();
        }
    }

    void district_server(App& app)
    {
        GET get;
        vector<string> line_tokens=split_by_space(line,DELIMITER_SPACE);
        if(line_tokens.size()==3)
            get.districts("NULL",app);
        else
            get.districts(analysis.get_district_name(line_tokens),app);
    }

    void my_district_server(App& app)
    {
        PUT put;
        vector<string> line_tokens=split_by_space(line,DELIMITER_SPACE);
        string name = analysis.get_district_name(line_tokens);
        if(check_district_name(app.distritcs , name))
            put.my_district(name , cur_client);
        update_client(app);
    }


public:
    Server(){}

    void set_Server(int t_cmd, string line_input)
    {
        type_cmd=t_cmd;
        line=line_input;
    }

    void check(App& app, int &login_status)
    {
        // cout<<login_status<<"   "<<type_cmd<<endl;
        if(login_status==DE_ACTIVE)
        {

            if(type_cmd==CMD_SIGNUP)
            {
                if(signup_server(app))
                    login_status=ACTIVE;
                return;
            }

            if(type_cmd==CMD_LOGIN)
            {
                login_server(app,login_status);
                return;
            }

            if(type_cmd==CMD_LOGOUT)
            {
                err.Permission_Denied();
                return;
            }
            
            err.Permission_Denied();
        }else
        {

            if(type_cmd==CMD_SIGNUP)
                err.Permission_Denied();

            if(type_cmd==CMD_LOGIN)
                err.Permission_Denied();
            
            if(type_cmd==CMD_LOGOUT)
            {
                login_status=DE_ACTIVE;
                err.OK();
            }

            if(type_cmd==CMD_DISTRICTS)
                district_server(app);
            
            if(type_cmd==CMD_FULL_DISTRICTS)
                district_server(app);
            
            if(type_cmd==CMD_PUT_DISTRICT)
                my_district_server(app);
            
        }
        
    }


};








void control_structure(char* argv[])
{
    App app;
    string line;
    app.handle_input_data(argv,app);
    Analysis_input analysis;
    Server server;
    int cmd_type;
    int login_status=0;
    while (getline(cin,line))
    {
        cmd_type=analysis.CMD_CONTROLLER(line);
        // cout<<cmd_type<<endl;
        if(cmd_type==WRONG_CMD)
            break;
        server.set_Server(cmd_type,line);
        server.check(app,login_status);
    }   
}

















int main(int argc , char* argv[])
{
    control_structure(argv);
}
