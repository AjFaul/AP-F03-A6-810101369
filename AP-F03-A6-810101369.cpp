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
const string CMD_POST="POST";
const string CMD_GET="GET";
const string CMD_PUT="PUT";
const string CMD_DELETE="DELETE";
const char DELIMITER_SPACE=' ';
const char DELIMITER_COMMA=',';
const int LOGIN_ACTIVE=1;
const int LOGIN_DEACTIVE=0;


const int SIGNUP=11;
const int LOGIN=12;
const int LOGOUT=13;
const int SHOW_DISTRICT=21;
const int SHOW_DISTRICT_FULL=211;
const int MY_DISTRICTS=31;
const int UNKNOWN_CMD=-1;
const int SHOW_LIST_RESTAURANT=22;
const int SHOW_LIST_RESTAURANT_FULL=221;
const int RESTAURANT_DETAIL=23;
const int RESERVE_WITH_FOOD=14;
const int RESERVE_WITHOUT_FOOD=141;









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


int CMD_CONTROLLER(string line)
{
    vector<string> input_sections;
    input_sections=split_by_space(line,DELIMITER_SPACE);
    string output_type;
    if(input_sections[0]==CMD_POST)
        output_type+="1";
    if(input_sections[0]==CMD_GET)
        output_type+="2";
    if(input_sections[0]==CMD_PUT)
        output_type+="3";
    if(input_sections[0]==CMD_DELETE)
        output_type+="4";

    if(output_type.size()==0)
        return -1;
    

    if(input_sections[1]=="signup")
        output_type+="1";
    if(input_sections[1]=="login")
        output_type+="2";
    if(input_sections[1]=="logout")
        output_type+="3";
    if(input_sections[1]=="reserve")
    {
        output_type+="4"; 
        auto it =find(input_sections.begin() , input_sections.end() , "foods");
        if(it==input_sections.end())
            output_type+="1";
    }
    if(input_sections[1]=="districts")
    {
        output_type+="1";
        auto it=find(input_sections.begin(), input_sections.end(),"district");
        if(it!=input_sections.end())
            output_type+="1";
    }
    if(input_sections[1]=="restaurants")
    {
        output_type+="2";
        auto it=find(input_sections.begin(), input_sections.end(),"food_name");
        if(it==input_sections.end())
            output_type+="1";
    }
    if(input_sections[1]=="restaurant_detail")
        output_type+="3";
    if(input_sections[1]=="reserves")
    {
        output_type+="4";
        auto reserve_id=find(input_sections.begin(),input_sections.end(),"reserve_id");
        if(reserve_id!=input_sections.end())
            output_type+="3";
        else
            output_type+="2";
    }


    

    if(input_sections[1]=="my_district")
        output_type+="1";


    return stoi(output_type);     
}


class Reserve
{
private:
    int status;
public:
    string name_food;
    int start_time,end_time,reserve_id,price,table_num;
    string restaurants_name;
    string district_name;
    int num_of_reserve;

    Reserve(string name, int stime,int etime,int res_id, int price_order, int N_table,string rest_name,string district, int num_reserve)
    {
        name_food=name;
        start_time=stime;
        end_time=etime;
        reserve_id=res_id;
        price=price_order;
        table_num=N_table;
        restaurants_name=rest_name;
        district_name=district;
        num_of_reserve=num_reserve;
    }

    void Add_reserve(){num_of_reserve++;}

    bool conflict(Reserve order)
    {
        if(order.start_time >= this->start_time && order.start_time<=end_time && this->table_num==order.table_num)
            return false;
        if(order.end_time >= this->start_time && order.end_time<=end_time && this->table_num==order.table_num)
            return false;
        if(this->start_time >= order.start_time && this->start_time<=end_time && this->table_num==order.table_num) 
            return false;
        if(this->end_time >= order.start_time && this->end_time<=end_time && this->table_num==order.table_num)
            return false;
        return true;
    }
    template <typename T>
    bool conflict_restaurants_time(T rest)
    {
        if(this->start_time>=rest.get_open_time ()&& this->end_time <=rest.get_close_time())
            return true;
        return false;
    }

    template <typename J>
    bool conflict_by_own(J& user)
    {
        for(int i=0;i<user.reserves.size();i++)
        {
            if(this->conflict(user.reserves[i]))
                return 0;
        }
        return 1;
    }

    template<typename M>
    bool find_food(M& restaurant)
    {
        for(auto it=restaurant.menu_item.begin() ; it!=restaurant.menu_item.end(); it++)
        {
            if(this->name_food== (*it).first)
                return true;
        }
        return false;
    }

};

class User
{
private:
    int status;
    string username;
    string password;
    string district;
public:
    vector<Reserve> reserves;
    User(){status=1;}
    User(string name,string pass)
    {
        status=1;
        username=name;
        password=pass;
    }
    string get_username(){return username;}
    string get_password(){return password;}
    void set_district(string ddstrict){district=ddstrict;}
    string get_district( ){return district;}
};

class Dirstrict
{
private:
    string name;
public:
    vector<Dirstrict> bfs_district;
    vector<string> neighbors;
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

class Restaurant
{
private:
    string name , district;
    int open_time,close_time, num_of_table;
public:
    vector<Reserve> reserves;
    map<string , int> menu_item;
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

    void show_info()
    {
        cout<<"Name: "<<name<<endl;
        cout<<"District: "<<district<<endl;
        cout<<"Time: "<<open_time<<"-"<<close_time<<endl;
        cout<<"Menu: ";
        for(auto it=menu_item.begin() ; it!=menu_item.end();it++)
        {
            if(it==menu_item.begin())
            {
                cout<<(*it).first<<"("<<(*it).second<<")";
                continue;
            }
            cout<<", "<<(*it).first<<"("<<(*it).second<<")";
        }
        cout<<endl;
    }


};

class App
{
private:
    int status_app;

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
        for(int i=0;i<dirstricts.size();i++)
            names_district.push_back(dirstricts[i].get_name());
        sort(names_district.begin(),names_district.end());

        vector<Dirstrict> cpy_district;
        for(int i=0;i<names_district.size();i++)
        {
            for(int j=0;j<dirstricts.size();j++)
            {
                if(dirstricts[j].get_name()==names_district[i])
                    cpy_district.push_back(dirstricts[j]);
            }
        }
        dirstricts=cpy_district;
    }

public:
    vector<User> users;
    vector<Restaurant> restaurants ;
    vector<Dirstrict> dirstricts;
    App(){status_app=1;}
    void Add_restaurant(Restaurant restaurant){restaurants.push_back(restaurant);}
    void Add_district(Dirstrict district){dirstricts.push_back(district);}
    void Add_user(string username , string password){users.push_back(User(username , password));}

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

    bool signup(string username , string password,App& app)
    {
        if(has_username(username, app.users))
        {
            Output_msg::Bad_Request();
            return false;
        }
        else
        {
            app.users.push_back(User(username,password));
            return true;
        }
    }

    int login(string username, string password,App& app)
    {
        int has_user=has_username(username, app.users);
        if(has_user)   
        {
            if(app.users[has_user-1].get_password() == password)
                return (has_user);
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

template <typename I, typename T>
bool has_food(I onething , T something)
{
    for(auto it=something.begin();it!=something.end();it++)
    {
        if(onething==(*it)->first)
            return true;
    }
    return false;
}



class GET : public Output_msg
{
private:
    int status=0;

    bool find_restaurants_by_place(string name_place, App& app,int type_cmd,string food_name)
    {
        
        for(int i=0;i<app.restaurants.size();i++)
        {
            // cout<<app.restaurants[i].menu_item[food_name]<<endl;
            if(name_place==app.restaurants[i].get_district() && (!type_cmd || (app.restaurants[i].menu_item[food_name]!=0)))
                cout<<app.restaurants[i].get_name()<<" ("<<name_place<<")"<<endl;
        }
        return 1;

    }


    Dirstrict find_district_by_name(vector<Dirstrict> districts,string my_dis)
    {
        for(int i=0;i<districts.size();i++)
        {
            if(districts[i].get_name()==my_dis)
                return districts[i];
        }


    }


    bool find_food_in_restaurants(string name_food , map<string ,int> food_price)
    {
        for(auto it=food_price.begin(); it!=food_price.end() ; it++)
        {
            if((*it).first==name_food)
                return 1;
        }
        return 0;
    }


    void write_restaurant_place(Restaurant& my_res)
    {
        cout<<my_res.get_name()<<" ("<<my_res.get_district()<<")"<<endl;
    }

    void show_restaurants_places(vector<string> visited , App& app,string name_food,bool type_cmd)
    {
        for(int i=0;i<visited.size();i++)
        {
            for(int j=0;j<app.restaurants.size();j++)
            {
                if(app.restaurants[j].get_district()==visited[i])
                {
                    if(find_food_in_restaurants(name_food , app.restaurants[j].menu_item) || (!type_cmd))
                        write_restaurant_place(app.restaurants[j]);
                }
            }
        }
        
    }

public:
    GET(){status=1;}

    void districts(string name_district,App& app)
    {
        if(name_district=="NULL")
        {
            for(int i=0;i<app.dirstricts.size();i++)
                app.dirstricts[i].show_neighbor();
        }
        else
        {
            int status_districts=has_district(name_district,app.dirstricts);
            if(status_districts)
                app.dirstricts[status_districts-1].show_neighbor();
            else
                Output_msg::Not_Found();
        }
    }

    void restaurants(vector<string> visited, App& app,string food_name){
        if(food_name=="NULL")
        {
            show_restaurants_places(visited,app,food_name,false);
        }
        else
        {
            show_restaurants_places(visited,app,food_name,true);
        }

    }

    int show_info_restaurants(string name, App &app)
    {
        for(int i=0;i<app.restaurants.size();i++)
        {
            if(name==app.restaurants[i].get_name())
            {
                app.restaurants[i].show_info();
                return 1;
            }
        }
        Output_msg::Not_Found();
        return 0;

    }

    bool reserve(string rest_name , int table_id, int s_time, int e_time ,string food_name, User& user, App& app,int res_id, int price,string district, int num_reserve)
    {
        Reserve new_res=Reserve(food_name,s_time,e_time,res_id,price ,table_id, rest_name, district, num_reserve);
        int i;
        for (i=0;i<app.restaurants.size();i++)
        {
            if( rest_name== app.restaurants[i].get_name())
            {
                if(!(new_res.find_food(app.restaurants[i])))
                {
                    Output_msg::Not_Found();
                    return false;
                }

                if(new_res.conflict_restaurants_time(app.restaurants[i]))
                {
                    Output_msg::Permission_Denied();
                    return false;
                }

                for(int j=0; j<app.restaurants[i].reserves.size(); j++)
                {
                    if(new_res.conflict(app.restaurants[i].reserves[j]))
                    {
                        Output_msg::Permission_Denied();
                        return false;
                    }
                }
                break;
            }
        }
        if(new_res.conflict_by_own(user))
        {
            Output_msg::Permission_Denied();
            return false;
        }
        app.restaurants[i].reserves.push_back(new_res);
        cout<<"Reserve ID: "<<res_id<<endl;
        cout<<"Table "<<table_id<<" for "<<s_time<<" to "<<e_time<<" in "<<rest_name<<endl;
        cout<<"Price: "<<price<<endl;
        return 1;
    }


    void show_reserve_info(Reserve meine)
    {
        cout<<meine.reserve_id<<": "<<meine.district_name<<" ";
        cout<<meine.table_num<<" "<<meine.start_time<<"-"<<meine.end_time<<endl;
        // cout<<" "<<meine.name_food<<"("<<meine.num_of_reserve<<")"<<endl;
    }


    void show_user_reserve(User& user)
    {
        if(user.reserves.size()==0)
            Output_msg::Empty();
        else
        {
            for(int i=0;i<user.reserves.size();i++)
                show_reserve_info(user.reserves[i]);
        }
    }

    bool show_res(string rest_name , int ID , User& user,App& app)
    {
        for(int i=0;i<user.reserves.size();i++)
        {
            if(!(user.reserves[i].restaurants_name== rest_name)|| !(user.reserves[i].reserve_id==ID) )
                {
                    Output_msg::Permission_Denied();
                    return false;
                }
        }


        int status=0;
        for(int i=0;i<app.restaurants.size();i++)
        {
            if(status==1)
                break;
            if(app.restaurants[i].get_name()==rest_name)
            {
                for(int j=0 ; j<app.restaurants[i].reserves.size();j++)
                {
                    if(app.restaurants[i].reserves[j].reserve_id == ID)
                    {
                        status=1;
                        break;
                    }
                }
                return false;
            }
        }
        show_user_reserve(user);


    }


};

class PUT : public Output_msg
{
private:
    int status;
public:
    PUT(){status=1;}

    bool my_districts(string name_district , User& user,App& app)
    {
        int status_districts=has_district(name_district,app.dirstricts);
        if(status_districts)
        {
            Output_msg::OK();
            user.set_district(name_district);
            return true;
        }
        else
        {
            Output_msg::Not_Found();
            return false;
        }
    }

};

class DELETE
{
private:
    int status;
public:
    DELETE(){status=1;}

    void delete_func(User &user,App& app)
    {
        user.reserves.pop_back();
    }
};





template<typename T>
void show_vector(vector<T> ex)
{
    for(int i=0;i<ex.size();i++)
        cout<<ex[i]<<endl;
}


class Analysis_input
{
private:
    int status;


    bool check_type(vector<string> name_list,string next_name)
    {
        for(int i=0;i<name_list.size();i++)
        {
            if(next_name==name_list[i])
                return 0;
        }
        return 1;
    }


public:

    Analysis_input(){status=1;}

    vector<string> Analysis_username_pass(string& line)
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

    vector<string> two_cmd(string& line)
    {
        vector<string> output;
        output=split_by_space(line,DELIMITER_SPACE);
        return output;
    }

    string Analysis_district(string &line)
    {
        string output;
        vector<string> line_token= split_by_space(line,DELIMITER_SPACE);
        if(line_token.size()==3)
        {
            output=("NULL");
            return output;
        }
        else
        {
            auto it=find(line_token.begin(),line_token.end(),"district");
            it++;
            output=(*it);
            return output;
        }
    }

    string Analysis_food_name(string & line)
    {
        string output;
        vector<string> line_token= split_by_space(line,DELIMITER_SPACE);
        if(line_token.size()==3)
        {
            output=("NULL");
            return output;
        }
        else
        {
            auto it=find(line_token.begin(),line_token.end(),"food_name");
            it++;
            output=(*it);
            return output;
        }
    }



    vector<string> Analysis_on_food(string foods)
    {
        vector<string> output=split_by_space(foods,DELIMITER_COMMA);
        return output;
    }
    


    vector<string> Analysis_reserve(string& line)
    {
        vector<string>output;
        vector<string> token;
        token=split_by_space(line,DELIMITER_SPACE);
        string extra;
        vector<string> list_name={"reserve","restaurant_name","table_id","start_time","end_time","foods"};
        auto it=find(token.begin(), token.end(),"restaurant_name");
        it++;
        while (check_type(list_name,(*it)))
        {
            extra+=(*it);
            it++;
            if(check_type(list_name,(*it)))
                extra+=" ";
            if(it==token.end())
                break;
        }
        output.push_back(extra);
        extra="";

        
        it=find(token.begin(), token.end(),"table_id");
        it++;
        while (check_type(list_name,(*it)))
        {
            extra+=(*it);
            it++;
            if(it==token.end())
                break;
        }
        output.push_back(extra);
        extra="";


        it=find(token.begin(), token.end(),"start_time");
        it++;
        while (check_type(list_name,(*it)))
        {
            extra+=(*it);
            it++;
            if(it==token.end())
                break;
        }
        output.push_back(extra);
        extra="";



        it=find(token.begin(), token.end(),"end_time");
        it++;
        while (check_type(list_name,(*it)))
        {
            extra+=(*it);
            it++;
            if(it==token.end())
                break;
        }
        output.push_back(extra);
        extra="";

        it=find(token.begin(), token.end(),"foods");
        if(it==token.end())
            return output;
        it++;

        while (check_type(list_name,(*it)))
        {
            extra+=(*it);
            it++;
            if(it==token.end())
                break;
        }


        vector<string> foods_list=Analysis_on_food(extra);
        for(int i=0;i<foods_list.size();i++)
        {
            output.push_back(foods_list[i]);
        }
        return output;

    }





};


Dirstrict find_by_name(string name,App& app)
{
    for(int i=0;i<app.dirstricts.size();i++)
    {
        if(app.dirstricts[i].get_name()==name)
            return app.dirstricts[i];
    }
    vector<string> data_dummy={name};
    Dirstrict dummy(data_dummy);
    return dummy;
}

bool check_visited(string name, vector<string>& visited)
{
    for(int i=0;i<visited.size();i++)
    {
        if(name==visited[i])
            return 0;
    }
    return 1;
}


vector<string> bfs_function (vector<string> neighbors,vector<string>& visited,App& app){
    int number_of_new_visited=0;
    vector <string> new_neighbor;
    for(int i=0;i<neighbors.size();i++)
    {
        Dirstrict cur_dis=find_by_name(neighbors[i],app);
        for(int j=0;j<cur_dis.neighbors.size();j++)
        {
            if(check_visited(cur_dis.neighbors[j],visited))
            {
                new_neighbor.push_back(cur_dis.neighbors[j]);
                visited.push_back(cur_dis.neighbors[j]);
                number_of_new_visited++;
            }
        }
    }
    if(number_of_new_visited==0)
        return visited;
    return bfs_function(new_neighbor,visited,app);
}




void control_structure(char* argv[])
{
    App app;
    app.handle_input_data(argv,app);

    int status_login=0;

    string line;
    int type_of_CMD;

    POST post;
    GET get;
    User cur_user;
    PUT put;

    Output_msg Errormsg;
    Analysis_input analysis;


    string district_name;
    string food_name;
    vector<string> line_tokens;

    while (getline(cin,line))
    {

        vector<string> visited;
        type_of_CMD=CMD_CONTROLLER(line);
        
        if(type_of_CMD==UNKNOWN_CMD)
        {
            Errormsg.Bad_Request();
            continue;
        }




        switch (type_of_CMD)
        {
        case SIGNUP:

            if(status_login==LOGIN_ACTIVE)
                Errormsg.Permission_Denied();
            else
            {
                vector<string> username_pass=analysis.Analysis_username_pass(line);
                if(post.signup(username_pass[0],username_pass[1],app))
                {
                    cur_user=app.users[app.users.size()-1];
                    status_login=1;
                    Errormsg.OK();
                }
                else
                    break;
            }
            break;
        
        case LOGIN:
            if(status_login==1)
            {
                Errormsg.Permission_Denied();
                break;
            }
            else
            {
                vector<string> username_pass=analysis.Analysis_username_pass(line);
                int number=post.login(username_pass[0],username_pass[1],app);
                if(number)
                {
                    cur_user=app.users[number-1];
                    status_login=1;
                    Errormsg.OK();
                }
                
            }
            break;
        case LOGOUT:
            if(status_login=0)
                Errormsg.Permission_Denied();
            else
                {
                    vector<string> logout_line=analysis.two_cmd(line);
                    if(logout_line[1]=="logout")
                    {
                        status_login=0;
                        Errormsg.OK();
                        break;
                    }else
                        Errormsg.Bad_Request(); 
                }
            break;
        default:
            break;
        }

        if(status_login==0)
            continue;



        // cout<<type_of_CMD<<endl;





        switch (type_of_CMD)
        {
        case (SHOW_DISTRICT) : 
            if(app.dirstricts.size()==0)
                Errormsg.Empty();
            else
                get.districts("NULL",app);
            break;
        

        case (SHOW_DISTRICT_FULL):
            if(app.dirstricts.size()==0)
                Errormsg.Empty();
            else
                get.districts(analysis.Analysis_district(line),app);
            break;

        case (MY_DISTRICTS):
            district_name=analysis.Analysis_district(line);   
            put.my_districts(district_name,cur_user,app);
            break;

        case (SHOW_LIST_RESTAURANT):
            if(app.restaurants.size()==0)
            {
                Errormsg.Empty();
                break;
            }

            if(cur_user.get_district().size()==0)
            {
                Errormsg.Not_Found();
                break;
            }else
            {
                visited.push_back(cur_user.get_district());
                food_name=analysis.Analysis_food_name(line);
                bfs_function({cur_user.get_district()} , visited, app);
                get.restaurants(visited,app,food_name);
                break;
            }

            break;


        case (SHOW_LIST_RESTAURANT_FULL):
            if(app.restaurants.size()==0)
            {
                Errormsg.Empty();
                break;
            }

            if(cur_user.get_district().size()==0)
            {
                Errormsg.Not_Found();
                break;
            }else
            {
                visited.push_back(cur_user.get_district());
                food_name=analysis.Analysis_food_name(line);
                bfs_function({cur_user.get_district()} , visited, app);
                get.restaurants(visited,app,food_name);
                break;
            }
            break;

        // case(RESTAURANT_DETAIL):

        //     break;


        case(RESERVE_WITH_FOOD):
            line_tokens=analysis.Analysis_reserve(line);


            break;




        default:
            break;
        }
        

        
        



    }
    



}

















int main(int argc , char* argv[])
{
    control_structure(argv);
    


}
