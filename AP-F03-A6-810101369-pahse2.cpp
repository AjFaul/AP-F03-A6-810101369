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
const char DELIMITER_SIMIC=';';
const char DELIMITER_YA='|';
const char DELIMITER_2_DOT=':';
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
const int CMD_RESTAURANTS_WITHOUT_FOODNAME=221;
const int CMD_RESTAURANTS_WITH_FOODNAME=22;
const int CMD_RESERVE_WITHOUT_FOOD=141;
const int CMD_RESERVE_WITH_FOOD=14;
const int CMD_RESTAURANT_DETAIL=23;
const int CMD_INFO_CLIENT=26;
const int CMD_DELETE_M=4;
const int CMD_INCREASE_BUDGET=115;
const int CMD_SHOW_BUDGET=2111;







struct discount
{
    string type;
    string name_food;
    string value;
};

struct total_discount
{
    string type;
    string floor_price;
    string value;
};

struct first_discount
{
    string type;
    string value;
};




void show_vector(vector<string> salam)
{
    for(auto it=salam.begin(); it!=salam.end();it++)
        cout<<(*it)<<endl;
}







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
    friend class POST;
    friend class Restaurant;
    friend class Client;
    friend class DELETE;
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
        if(input_sections[0]==CMD_DELETE)
        {
            output_type+="4";
            return stoi(output_type);
        }

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
        if(input_sections[1]=="restaurants")
        {
            output_type+="2";
            auto it=find(input_sections.begin(), input_sections.end(),"food_name");
            if(it==input_sections.end())
                output_type+="1";
        }

        if(input_sections[1]=="reserve")
        {
            output_type+="4"; 
            auto it =find(input_sections.begin() , input_sections.end() , "foods");
            if(it==input_sections.end())
                output_type+="1";
        }
        if(input_sections[1]=="restaurant_detail")
            output_type+="3";
        
        if(input_sections[1]=="reserves")
            output_type+="6";
        
        if(input_sections[1]=="increase_budget")
            output_type+="15";

        if(input_sections[1]=="show_budget")
            output_type+="111";
        






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

    string get_food_name(vector<string>& line_token)
    {
        string output;
        for(int i=0;i<line_token.size();i++)
        {
            if(line_token[i]=="food_name")
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


    // in 3 ta baraye tahlil reserve hast
    bool check_type(vector<string> name_list,string next_name)
    {
        for(int i=0;i<name_list.size();i++)
        {
            if(next_name==name_list[i])
                return 0;
        }
        return 1;
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
            if(it==token.end())
                break;
            if(check_type(list_name,(*it)))
                extra+=" ";
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
            if(check_type(list_name,(*it)))
                extra+=" ";
        }


        vector<string> foods_list=Analysis_on_food(extra);
        for(int i=0;i<foods_list.size();i++)
        {
            output.push_back(foods_list[i]);
        }
        return output;

    }

    vector<string> separate_food(vector<string> words)
    {
        vector<string> output;
        for(int i=4;i<words.size();i++)
            output.push_back(words[i]);
        return output;
    }

    string restaurant_name(vector<string> words)
    {
        string output;
        for(int i=0;i<words.size();i++)
        {
            if(words[i]=="restaurant_name")
            {
                for(int j=(i+1); j<words.size();j++)
                {
                    output+=words[j];
                }
            }
        }
        return output;
    }

    string search_words(string& line, string& key , vector<string>& keys)
    {
        vector<string> tokens=split_by_space(line,DELIMITER_SPACE);
        string output="";
        for(int i=0;i<tokens.size();i++)
        {
            if(tokens[i]==key)
            {
                for(int j=(i+1);j<tokens.size();j++)
                {
                    if(find(keys.begin(),keys.end(),tokens[j]) == keys.end())
                    {
                        output+=tokens[j];
                        output+=" ";
                    }else
                        break;;
                }
                break;
            }
        }
        if(output.size()!=0)
            output.pop_back();
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
    int start_time,end_time;
    int final_price=0;
    friend class Client;
    
public:
    Reserve(){status=1;}

    Reserve(string rest_name,int t_id,int s_time,int e_time,vector<string>foodd_name)
    {
        status=1;
        restaurant_name=rest_name;
        table_id=t_id;
        start_time=s_time;
        end_time=e_time;
        foods_name=foodd_name;
    }

    void set_price(vector<int> price_foods)
    {
        prices=price_foods;
    }

    int delete_reserve()
    {
        status=0;
        return final_price;
    }

    int get_status(){return status;}

    void set_reserve_id(int id){reserve_id=id;}

    int get_table_id(){return table_id;}

    int get_start_time(){return start_time;}
    
    int get_end_time(){return end_time;}

    int get_reserve_id(){return reserve_id;}

    vector<int> get_prices(){return prices;}

    int get_price()
    {
        int x=0;
        for(int i=0;i<prices.size();i++)
        {
            x+=prices[i];   
        }
        return x;
    }
    
    string get_restaurant_name(){return restaurant_name;}

    void set_final_price(int x)
    {
        final_price=x;
    }

    int get_final_price(){return final_price;}

    int get_inital_price()
    {
        int x=0; 
        for(int i=0;i<prices.size();i++)
        {
            x+=prices[i];
        }
        return x;
    }
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



class Restaurant : public Reserve
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
    friend class GET;
    friend class POST;
    friend class Server;
    friend class DELETE;
    friend class Client;
    Output_msg err;

    vector<discount> discounts;
    total_discount total_price_discount;
    first_discount first_order_discount;





    bool conflict_time(int s1,int e1,int s2 , int e2)
    {
        if(s1<1 || e1>24)
            return false;
        if(e1>s2 && e1<e2)
            return false;
        if(s1>s2 && s1<e2)
            return false;
        if(e2>s1 && e2<e1)
            return false;
        if(s2>s1 && s2<e1)
            return false;
        return true;
    }



    bool Add_reserve(int table_id, int s_time ,int e_time, vector<string> foods,int budget=0)
    {
        Reserve reserve(name,table_id,s_time,e_time,foods);
        vector<int> prices;
        
        if(table_id>num_of_table)
        {
            err.Not_Found();
            return false;
        }

        for(int i=0;i<reserves.size();i++)
        {
            if(reserves[i].get_table_id()==table_id)
            {
                if(!conflict_time(s_time,e_time,reserves[i].get_start_time(),reserves[i].get_end_time()))
                {
                    
                    err.Permission_Denied();
                    return false;
                }
            }
        }
        

        for(int i=0;i<foods.size();i++)
        {
            if(menu_item[foods[i]]==0)
            {
                err.Not_Found();
                return false;
            }
            prices.push_back(menu_item[foods[i]]);
        }
        int sum_all=0;
        for(int i=0;i<prices.size();i++)
            sum_all+=prices[i];


        int takhfif=0;
        for(int i=0;i<foods.size();i++)
        {

            for(int j=0;j<foods.size();j++)
            {
                if(discounts[i].name_food==foods[j])
                {
                    if(discounts[i].type=="amount")
                    {
                        takhfif+=stoi(discounts[i].value);
                    }
                
                    if(discounts[i].type=="percent")
                    {
                        takhfif+= stoi(discounts[i].value)*prices[i];
                    }
                }
            }
        }
        if(first_order_discount.type=="amount")
        {
            takhfif+=stoi(first_order_discount.value);
        }
        if(first_order_discount.type=="percent")
        {
            takhfif+=stoi(first_order_discount.value) * (sum_all-takhfif)/100;
        }


        if((sum_all-takhfif)>stoi(total_price_discount.floor_price))
        {
            if(total_price_discount.type=="amount")
            {
                takhfif+=stoi(total_price_discount.value);
            }
            if(total_price_discount.type=="percent")
            {
                takhfif+=stof(total_price_discount.value) * (sum_all-takhfif)/100;
            }
        }
         
        
        if(budget<(sum_all-takhfif))
        {
            err.Bad_Request();
            return false;
        }

        reserve.set_final_price(sum_all-takhfif);
        reserve.set_price(prices);
        reserve.set_reserve_id(reserves.size()+1);
        reserves.push_back(reserve);
        return true;
    }


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


    void show_restaurant_info()
    {
        cout<<"Name: "<<name<<endl;
        cout<<"District: "<<district<<endl;
        cout<<"Time: "<<open_time<<"-"<<close_time<<endl;
        cout<<"Menu:";
        string foods_name_price;
        for(auto it=menu_item.begin();it!=menu_item.end();it++)
        {
            if((*it).second!=0)
            {
                foods_name_price+=" ";
                foods_name_price+=(*it).first;
                foods_name_price+="(";
                foods_name_price+=to_string(((*it).second));
                foods_name_price+="),"; 
            }
            
        }
        cout<<foods_name_price.substr(0,foods_name_price.size()-1)<<endl;


        map<int , string > output;
        vector<int> num_of_table_id;


        for(int i=0;i<num_of_table;i++)
        {
            num_of_table_id.push_back(i+1);
        }







        string itt;
        string xx;

        for(int i=0;i<num_of_table_id.size();i++)
        {
            xx="";
            itt="";
            vector<string> irrr;
            for(int j=0;j<reserves.size();j++)
            {
                if(reserves[j].get_table_id()== num_of_table_id[i] && reserves[j].get_status()==1)
                {
                    itt+=" ";
                    itt+="(";
                    itt+=to_string(reserves[j].get_start_time());
                    itt+="-";
                    itt+=to_string(reserves[j].get_end_time());
                    itt+=")";
                    itt+=",";
                }
                irrr.push_back(itt);
                itt="";
            }
            sort(irrr.begin(),irrr.end());
            for(int k=0;k<irrr.size();k++)
            {
                xx+=irrr[k];
            }
            if(xx.size()!=0)
                xx.pop_back();
            output[num_of_table_id[i]]=xx;
        }
        
        for(auto it=output.begin(); it!=output.end() ; it++)
        {
            cout<<(*it).first<<":";
            cout<<(*it).second<<endl;
        }


    }


    void Add_total_price(string words)
    {
        vector<string> data=split_by_space(words,DELIMITER_SIMIC);

        if(data[0]=="none")
        {
            total_price_discount.floor_price="none";
            total_price_discount.type="none";
            total_price_discount.value="none";
            return;
        }

        total_price_discount.type=data[0];
        total_price_discount.floor_price=data[1];
        total_price_discount.value=data[2];
        return;
    }

    void Add_first_order(string words)
    {
        vector<string>data=split_by_space(words,DELIMITER_SIMIC);

        if(data[0]=="none")
        {
            first_order_discount.type="none";
            first_order_discount.value="none";
            return;
        }

        first_order_discount.type=data[0];
        first_order_discount.value=data[1];
        return;
    }

    void Add_foods_discounts(string words)
    {
        vector<string> section_food=split_by_space(words,DELIMITER_YA);

        if(section_food[0]=="none")
        {
            discount dis;
            dis.name_food="none";
            dis.type="none";
            dis.value="none";
            discounts.push_back(dis);
            return;
        }

        for(int i=0;i<section_food.size();i++)
        {
            vector<string> data_discount=split_by_space(section_food[i],DELIMITER_SIMIC);
            discount dis;
            dis.type=data_discount[0];
            vector<string>x=split_by_space(data_discount[1],DELIMITER_2_DOT);
            dis.name_food=x[0];
            dis.value=x[1];
            discounts.push_back(dis);
        }

    }

    void show_order_amount_discount()
    {
        if(total_price_discount.type=="none")
            return;
        cout<<"Order Amount Discount: "<<total_price_discount.type<<", ";
        cout<<total_price_discount.floor_price<<", "<<total_price_discount.value<<endl;  
    }

    void show_food_discount()
    {
        if(discounts.size()==0)
            return;

        cout<<"Item Specific Discount:";
        string x;
        for(int i=0;i<discounts.size();i++)
        {
            x+=" ";
            x+=discounts[i].name_food;
            x+="(";
            x+=discounts[i].type;
            x+=": ";
            x+=discounts[i].value;
            x+="),";
        }
        if(x.size()!=0)
            x.pop_back();
        cout<<x<<endl;
    }

    void show_first_discount()
    {
        if(first_order_discount.value=="none")
            return;
        cout<<"First Order Discount: ";
        cout<<first_order_discount.type<<", ";
        cout<<first_order_discount.value<<endl;
    }


    void show_discount()
    {
        show_order_amount_discount();
        show_food_discount();
        show_first_discount();
    }

};



class Client
{
private:
    int budget=0;
    int num_of_reserve=0;
    int status;
    string username;
    string password;
    string district;
    vector<Reserve> reserves;
    Analysis_input aaann;
    friend class App;
    friend class Check;
    friend class Server;
    friend class GET;
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

    bool conflict_time_client(int s1,int e1,int s2 , int e2)
    {
        if(s1<1 || e1>24)
            return false;
        if(e1>s2 && e1<=e2)
            return false;
        if(s1>s2 && s1<e2)//
            return false;
        if(e2>s1 && e2<=e1)
            return false;
        if(s2>s1 && s2<e1)
            return false;
        return true;
    }


    bool Add_reserve(vector<string>&words)
    {
        int start_time=stoi(words[2]);
        int end_time=stoi(words[3]);
        for(int i=0;i<reserves.size();i++)
        {
            if(!conflict_time_client(start_time,end_time,reserves[i].get_start_time(),reserves[i].get_end_time()))
            {
                Output_msg err;
                err.Permission_Denied();
                return false;
            }
        }
        return true;
    }


    int find_price(string name, map<string , int > menu_item)
    {
        for(auto it=menu_item.begin();it!=menu_item.end();it++)
        {
            if((*it).first==name)
                return (*it).second;
        }
        return 0;
    }

    bool Add_reservetion(vector<string> words)
    {
        int start_time=stoi(words[2]);
        int end_time=stoi(words[3]);
        if(Add_reserve(words))
        {
            Reserve reserve(words[0],stoi(words[1]),start_time,end_time,aaann.separate_food(words));
            reserves.push_back(reserve);
            return true;
        }
        return false;
    }



    void show_info(string restaurant_name , string reserve_id,bool type_mode)
    {
        vector<int> perm;
        for(int i=0;i<reserves.size();i++)
        {
            if(  (restaurant_name==reserves[i].get_restaurant_name() && reserve_id==to_string(reserves[i].get_reserve_id())) || type_mode )
            {
                if(reserves[i].get_status()==1)
                    perm.push_back(reserves[i].get_start_time()+100*i);
            }
        }
        for(int i=0;i<perm.size();i++)
        {
            int index=perm[i]/100;
            cout<<reserves[index].get_reserve_id()<<": ";
            cout<<reserves[index].get_restaurant_name()<<" ";
            cout<<reserves[index].get_table_id()<<" ";
            cout<<reserves[index].get_start_time()<<" "<<reserves[index].get_end_time()<<" ";
            vector<string>visited;
            for(int j=0;j<reserves[index].foods_name.size();j++)
            {
                if(count(visited.begin(),visited.end(),reserves[index].foods_name[j])==0 )
                {
                    visited.push_back(reserves[index].foods_name[j]);
                    cout<<reserves[index].foods_name[j]<<"(";
                    cout<<count(reserves[index].foods_name.begin(),reserves[index].foods_name.end(),reserves[index].foods_name[j])<<") ";
                }
            }
            cout<<" "<<reserves[index].get_inital_price()<<" "<<reserves[index].get_final_price();

            cout<<endl;
        }
    }

    void Add_budget(int amount)
    {
        budget+=amount;
    }

    int get_budget(){return budget;}

    void decrease_budget(int num){budget=budget-num;}

    void set_num_of_reserve(int x){num_of_reserve=num_of_reserve+x;}


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


    void handle_discounts(App& app ,ifstream& file_discounts)
    {
        string line;
        int num_line=0;
        vector<string>data_line;
        while (getline(file_discounts,line))
        {
            if(num_line==0)
            {
                num_line++;
                continue;
            }
            data_line=split_by_space(line,DELIMITER_COMMA);

            for(int i=0;i<app.restaurants.size();i++)
            {
                if(app.restaurants[i].name==data_line[0])
                {
                    app.restaurants[i].Add_total_price(data_line[1]);
                    app.restaurants[i].Add_first_order(data_line[2]);
                    app.restaurants[i].Add_foods_discounts(data_line[3]);
                }
            }
        }
        file_discounts.close();
    }



    friend class Check;
    friend class GET;
    friend class Server;
    friend class POST;
    friend class DELETE;
    friend class Client;

public:
    App(){status=1;}
    
    void handle_input_data(char* argv[],App& app)
    {
        string name_file_restaurants=argv[1];
        string name_file_districts=argv[2];
        string name_file_discounts=argv[3];

        ifstream file_restaurants(name_file_restaurants);
        if(!file_restaurants.is_open())
            cout<<"file is not open"<<endl;
        
        ifstream file_districts(name_file_districts);
        if(!file_districts.is_open())
            cout<<"file is not open"<<endl;
        
        ifstream file_discount(name_file_discounts);
        if(!file_discount.is_open())
            cout<<"file is not open"<<endl;
        
        
        handle_restaurants(app,file_restaurants);
        handle_districts(app,file_districts);
        update_district();
        handle_discounts(app,file_discount);
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
                err.Permission_Denied();
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
            // show_vector(cur_district.neighbors);
            for(int j=0;j<cur_district.neighbors.size();j++)
            {
                if(!check_visited(cur_district.neighbors[j],visited))
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
    Output_msg err;
    Analysis_input analysis;
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

    bool reserve(vector<string> words , App& app,Client& client)
    {
        for(int i=0;i<app.restaurants.size();i++)
        {
            if(words[0]==app.restaurants[i].name)
            {
                vector<string>foods=analysis.separate_food(words);
                if(app.restaurants[i].Add_reserve(stoi(words[1]),stoi(words[2]),stoi(words[3]),foods,client.get_budget()))
                {
                    return true;
                }
            }
        }
        err.Not_Found();
        return false;
    }


    friend class Server;
public:
    POST(){status=1;}

};



class GET
{
private:
    Output_msg err;
    Check check;

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

    void restaurants(string food_name, App& app,Client client)
    {
        if(client.district.size()==0)
        {
            err.Not_Found();
            return;
        }

        if(app.restaurants.size()==0)
        {
            err.Empty();
            return;
        }
        vector<string> visited;
        visited.push_back(client.district);
        vector<string> neighbors;
        neighbors.push_back(client.district);
        visited=check.bfs_function(neighbors,visited,app);
        bool cmd_type=(food_name=="NULL")?true:false;
        int num=0;
        for(int i=0;i<visited.size();i++)
        {
            for(int j=0;j<app.restaurants.size();j++)
            {
                if(app.restaurants[j].district==visited[i])
                {
                    if(app.restaurants[j].menu_item[food_name]!=0 || cmd_type)
                    {
                        cout<<app.restaurants[j].name<<" ("<<app.restaurants[j].district<<")"<<endl;
                        num++;
                    }
                }
            }
        }
        if(num==0)
            err.Empty();
    }

    friend class Server;
public:
    GET(){}
};


class PUT
{
private:
    Output_msg err;
    void my_district(string name,Client& client)
    {
        client.set_district(name);
    }

    friend class Server;
public:
    PUT(){}

};



class DELETE
{
private:
    Output_msg err;
public:
    DELETE(){}

    int del_func(string restaurant_name, int reserve_id,App& app)
    {
        int x;
        for(int i=0;i<app.restaurants.size();i++)
        {
            if(app.restaurants[i].name ==restaurant_name)
            {
                for(int j=0;j<app.restaurants[i].reserves.size();j++)
                {
                    if(app.restaurants[i].reserves[j].get_reserve_id()==reserve_id)
                    {
                        x=app.restaurants[i].reserves[j].delete_reserve();
                        return x;
                    }
                }
                err.Not_Found();
                return false;
            }
        }
        err.Not_Found();
        return false;
    }


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

    void restaurants_server(App& app)
    {
        GET get;
        vector<string> line_tokens=split_by_space(line,DELIMITER_SPACE);
        if(line_tokens.size()==3)
            get.restaurants("NULL",app,cur_client);
        else
            get.restaurants(analysis.get_food_name(line_tokens) , app , cur_client);
    }


    float show_order_amount_discount(App&app , string restaurant_name,int original_price)
    {
        for(int i=0;i<app.restaurants.size();i++)
        {
            if(app.restaurants[i].name==restaurant_name)
            {
                cout<<"Order Amount Discount: ";
                if(app.restaurants[i].total_price_discount.type=="percent")
                {
                    cout<<stoi(app.restaurants[i].total_price_discount.value)*original_price/100<<endl;
                    return stof(app.restaurants[i].total_price_discount.value)*original_price/100;

                }else
                {
                    cout<<stoi(app.restaurants[i].total_price_discount.value)<<endl;
                    return stof(app.restaurants[i].total_price_discount.value);
                }
                
            }
        }
        return 0;
    }


    float show_total_item(App&app , string restaurant_name,vector<string> foods)
    {
        float output=0;
        for(int i=0;i<app.restaurants.size();i++)
        {
            if(app.restaurants[i].name==restaurant_name)
            {
                for(int j=0;j<app.restaurants[i].discounts.size();j++)
                {
                    if(find(foods.begin(),foods.end(),app.restaurants[i].discounts[j].name_food)!=foods.end())
                    {
                        output+=stof(app.restaurants[i].discounts[j].value);
                    }
                }
            }
        }
        
        if(output!=0)
        {
            cout<<"Total Item Specific Discount: "<<output<<endl;
            return output;
        }
        return output;
    }

    float show_first_discount(App &app , string restaurant_name)
    {
        if(cur_client.num_of_reserve>1)
            return 0;
        float output=0;
        for(int i=0;i<app.restaurants.size();i++)
        {
            if(app.restaurants[i].name==restaurant_name)
            {
                if(app.restaurants[i].first_order_discount.type!="none")
                {
                    cout<<"First Order Discount: "<<app.restaurants[i].first_order_discount.value<<endl;
                    output=stof(app.restaurants[i].first_order_discount.value);
                    return output;
                }
            }
        }
        return 0;
    }

    float show_info_reserve(vector<string> words,App& app)
    {
        cout<<"Reserve ID: "<<cur_client.reserves[cur_client.reserves.size()-1].get_reserve_id()<<endl;
        cout<<"Table "<<words[1]<<" for "<<words[2]<<" to "<<words[3]<<" in "<<words[0]<<endl;
        int price=cur_client.reserves[cur_client.reserves.size()-1].get_price();
        cout<<"Original Price: "<<price<<endl;

        float l1,l2,l3;


        l1=show_order_amount_discount(app,words[0],price);
        vector<string>foods;
        foods=analysis.separate_food(words);
        l2=show_total_item(app,words[0],foods);
        l3=show_first_discount(app,words[0]);
        cout<<"Total Discount: ";
        cout<<(l1+l2+l3)<<endl;
        cout<<"Total Price: "<<(price-(l1+l2+l3))<<endl;




        // vector<int>priceeee;
        // for(int i=0;i<app.restaurants.size();i++)
        // {
        //     if(app.restaurants[i].name==words[0])
        //     {
        //         for(auto it=app.restaurants[i].menu_item.begin() ;it!=app.restaurants[i].menu_item.end(); it++)
        //         {
        //             if(find(foods.begin(),foods.end(), (*it).first )!=foods.end())
        //             {
        //                 priceeee.push_back((*it).second);
        //             }
        //         }
        //     }
        // }


        // int takhfif;
        // for(int i=0;i<app.restaurants.size();i++)
        // {
        //     if(app.restaurants[i].name == words[0])
        //     {
        //         for(int j=0;j<foods.size();j++)
        //         {
        //             for(int k=0;k<app.restaurants[i].discounts.size();k++)
        //             {
        //                 if(app.restaurants[i].discounts[k].name_food==foods[j])
        //                 {
        //                     if(app.restaurants[i].discounts[k].type=="amount")
        //                     {
        //                         takhfif+=stoi(app.restaurants[i].discounts[k].value);
        //                     }            
        //                     if(app.restaurants[i].discounts[k].type=="percent")
        //                     {
        //                         takhfif+=stoi(app.restaurants[i].discounts[k].value) * priceeee[j];
        //                     }
        //                 }
        //             }
        //         }
        //     }
        // }



        return (price-(l1+l2+l3));


    }


    void update_client(Client& client , vector<string>words , App& app)
    {
        for(int i=0;i<app.restaurants.size();i++)
        {
            if(app.restaurants[i].name==words[0])
            {
                client.reserves[client.reserves.size()-1].set_reserve_id(app.restaurants[i].reserves[app.restaurants[i].reserves.size()-1].get_reserve_id());
                client.reserves[client.reserves.size()-1].set_price(app.restaurants[i].reserves[app.restaurants[i].reserves.size()-1].get_prices());
                client.reserves[client.reserves.size()-1].set_final_price(app.restaurants[i].reserves[app.restaurants[i].reserves.size()-1].get_final_price());
            }
        }
    }

    bool reserve_server(App& app)
    {
        vector<string> words=analysis.Analysis_reserve(line);
        POST post;
        if(post.reserve(words,app,cur_client) && cur_client.Add_reservetion(words))
        {
            int l;
            update_client(cur_client,words,app);
            l=show_info_reserve(words,app);
            cur_client.decrease_budget(l);
            cur_client.set_num_of_reserve(1);
            return true;
        }else
            return false;


    }

    void show_restaurant_detail_server(App& app)
    {
        vector<string> keys={"restaurant_name"};
        string rest_name=analysis.search_words(line,keys[0],keys);
        cout<<rest_name<<endl;
        for(int i=0;i<app.restaurants.size();i++)
        {
            if( rest_name==app.restaurants[i].name)
            {
                app.restaurants[i].show_restaurant_info();
                app.restaurants[i].show_discount();
                return;
            }
        }
        err.Not_Found();
        return;
    }

    void show_client_info(App& app)
    {
        vector<string> keys={"restaurant_name","reserve_id"};
        string restaurant_name=analysis.search_words(line,keys[0],keys);
        string reserve_id=analysis.search_words(line,keys[1],keys);
        if(cur_client.reserves.size()==0)
        {
            err.Empty();
            return;
        }
        if(reserve_id.size()>0 && restaurant_name.size()==0)
        {
            err.Bad_Request();
            return;
        }
        bool type_mode;
        if(reserve_id.size()==0 && restaurant_name.size()==0)
            type_mode=true;
        cur_client.show_info(restaurant_name,reserve_id,type_mode);
        
    }

    void delete_server(App& app)
    {
        DELETE del;
        vector<string> keys={"restaurant_name","reserve_id"};
        string restaurant_name=analysis.search_words(line,keys[0],keys);
        string reserve_id=analysis.search_words(line,keys[1],keys);
        for(int i=0;i<cur_client.reserves.size();i++)
        {
            if(cur_client.reserves[i].get_restaurant_name()==restaurant_name)
            {
                if(to_string(cur_client.reserves[i].get_reserve_id())==reserve_id)
                {
                    int x;
                    cur_client.reserves[i].delete_reserve();
                    x=del.del_func(restaurant_name,stoi(reserve_id),app);
                    // cout<<"salam man injam   "<<x<<endl;
                    cur_client.decrease_budget(-1 * x* 0.6);
                    err.OK();
                    return;
                }else
                {
                    err.Not_Found();
                    return;
                }
            }
        }
        err.Permission_Denied();
        return;
    }


    void Add_budget_server(App& app)
    {
        vector<string>keys={"amount"};
        string amount=analysis.search_words(line,keys[0],keys);
        int x;
        try
        {
            x=stoi(amount);
            cur_client.Add_budget(x);
            err.OK();
            return;
        }
        catch(...)
        {
            err.Bad_Request();
            return;
        }
        
    }

    void show_budget_server(App& app)
    {
        cout<<cur_client.get_budget()<<endl;
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
            
            if(type_cmd==CMD_RESTAURANTS_WITHOUT_FOODNAME)
                restaurants_server(app);
            
            if(type_cmd==CMD_RESTAURANTS_WITH_FOODNAME)
                restaurants_server(app);

            if(type_cmd==CMD_RESERVE_WITH_FOOD)
                reserve_server(app);
            
            if(type_cmd==CMD_RESERVE_WITHOUT_FOOD)
                reserve_server(app);
            
            if(type_cmd==CMD_RESTAURANT_DETAIL)
                show_restaurant_detail_server(app);

            if(type_cmd==CMD_INFO_CLIENT)
                show_client_info(app);

            if(type_cmd==CMD_DELETE_M)
                delete_server(app);
            
            if(type_cmd==CMD_INCREASE_BUDGET)
                Add_budget_server(app);

            if(type_cmd==CMD_SHOW_BUDGET)
                show_budget_server(app);



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
