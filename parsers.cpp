#include <bits/stdc++.h>
using namespace std;
using ll=long long;

ll N,n,m,totalStates,currState;
char Strting;
map<char,vector<string>>productions;
map<char,set<char>>First,Follow,Leading,Trailing;
set<char>terminal,variable;
map<pair<char,string>,ll>mp;
map<char,ll>posterminal;
map<ll,string>Reduce;
map<vector<string>,ll>states;
map<ll,vector<string>>productionsInState;
map<pair<ll,string>,set<char>>LookAhead;
map<pair<set<char>,vector<string>>,ll>LAstates;
vector<vector<ll>>combineStates;
vector<ll>vis;
map<pair<ll,char>,ll>Goto;

bool is_terminal(char ch)
{
    if(ch>='A' && ch<='Z')
    return false;
    return true;
}
set<char> find_first(char ch,char prev,ll maxi)
{
    if(ch==prev || maxi==113)
    return {};
    if(is_terminal(ch))
    return First[ch]={ch};
    for(auto x:productions)
    {
        if(x.first==ch)
        for(auto y:x.second)
        {
            ll k=0;
            while(k<(ll)y.size())
            {
                bool flag=false;
                set<char>temp=find_first(y[k],ch,maxi+1);
                for(auto z:temp)
                {
                    if(z=='#')
                    flag=true;
                    First[ch].insert(z);
                }
                if(flag)
                k++;
                else
                break;
            }
        }
    }
    return First[ch];
}
void find_follow_rule1()
{
    string s;
    ll ss,i;
    for(auto x:productions)
    {
        for(auto y:x.second)
        {
            s=y;
            ss=s.size();
            for(i=0;i<ss-1;i++)
            {
                if(!is_terminal(s[i]))
                {
                    if(is_terminal(s[i+1]))
                    {
                        Follow[s[i]].insert(s[i+1]);
                        continue;
                    }
                    for(auto z:First[s[i+1]])
                    {
                        if(z!='#')
                        Follow[s[i]].insert(z);
                    }
                }
            }
            
        }
    }
}
void find_follow_rule2()
{
    ll i,ss;
    string s;
    for(auto x:productions)
    {
        for(string y:x.second)
        {
            s=y;
            ss=s.size();
            for(i=0;i<ss;i++)
            if(!is_terminal(s[i]) && (i==ss-1 || (!is_terminal(s[i+1]) && First[s[i+1]].find('#')!=First[s[i+1]].end())))
            {
                for(auto f:Follow[x.first])
                Follow[s[i]].insert(f);
            }
        }
    }
}
void findFirstFollow()
{
    for(auto x:productions)
    find_first(x.first,'!',0);
    Follow[Strting].insert('$');
    terminal.insert('$');
    find_follow_rule1();
    find_follow_rule2();
}
void printFirstFollow()
{
    cout<<"First of the variables are\n\n";
    for(auto x:productions)
    {
        cout<<x.first<<" -> { ";
        for(auto y:First[x.first])
        cout<<y<<" ,";
        cout<<" }\n";
    }
    cout<<"\nFollow of the variables are\n\n";
    for(auto x:productions)
    {
        cout<<x.first<<" -> { ";
        for(auto y:Follow[x.first])
        cout<<y<<" ,";
        cout<<" }\n";
    }
    cout<<"\n";
}
set<char> find_leading(char ch,char prev)
{
    if(ch==prev)
    return {};
    if(is_terminal(ch))
    return Leading[ch]={ch};
    for(auto x:productions)
    {
        if(x.first==ch)
        {
            for(auto y:x.second)
            {
                for(auto z:y)
                if(is_terminal(z))
                {
                    Leading[ch].insert(z);
                    break;
                }
            }
            for(auto y:x.second)
            {
                ll k=0;
                while(k<(ll)y.size())
                {
                    bool flag=false;
                    set<char>temp=find_leading(y[k],ch);
                    for(auto z:temp)
                    {
                        if(z=='#')
                        flag=true;
                        Leading[ch].insert(z);
                    }
                    if(flag)
                    k++;
                    else
                    break;
                }
            }
        }
    }
    return Leading[ch];
}
set<char> find_trailing(char ch,char prev)
{
    if(ch==prev)
    return {};
    if(is_terminal(ch))
    return Trailing[ch]={ch};
    for(auto x:productions)
    {
        if(x.first==ch)
        {
            for(auto y:x.second)
            {
                string s=y;
                reverse(s.begin(),s.end());
                for(auto z:s)
                if(is_terminal(z))
                {
                    Trailing[ch].insert(z);
                    break;
                }
            }
            for(auto y:x.second)
            {
                ll k=(ll)y.size()-1;
                while(k>=0)
                {
                    bool flag=false;
                    set<char>temp=find_trailing(y[k],ch);
                    for(auto z:temp)
                    {
                        if(z=='#')
                        flag=true;
                        Trailing[ch].insert(z);
                    }
                    if(flag)
                    k--;
                    else
                    break;
                }
            }
        }
    }
    return Trailing[ch];
}
void findLeadingtrailing()
{
    for(auto x:productions)
    find_leading(x.first,'!');
    for(auto x:productions)
    find_trailing(x.first,'!');
}
void printLeadingTrailing()
{
    cout<<"Leading of the variables are\n\n";
    for(auto x:productions)
    {
        cout<<x.first<<" -> { ";
        for(auto y:Leading[x.first])
        cout<<y<<" ,";
        cout<<" }\n";
    }
    cout<<"\nTrailing of the variables are\n\n";
    for(auto x:productions)
    {
        cout<<x.first<<" -> { ";
        for(auto y:Trailing[x.first])
        cout<<y<<" ,";
        cout<<" }\n";
    }
    cout<<"\n";
}
void printOperatorPrecedenceParseTable()
{
    vector<vector<string>>OPparse_table(terminal.size()+1,vector<string>(terminal.size()+1," "));
    ll i,j,k;
    auto x=terminal.begin();
    map<char,ll>mpchar;
    string ss;
    for(i=1;i<=terminal.size();i++)
    {
        ss="";
        ss+=(*x);
        ss+=" ,";
        OPparse_table[i][0]=OPparse_table[0][i]=ss;
        mpchar[*x]=i;
        x++;
    }
    // LLL left to right,leading of var right to operator,fill <.
    for(auto x:productions)
    {
        for(string y:x.second)
        {
            k=y.size();
            for(i=1;i<k-1;i++)
            {
                if(is_terminal(y[i]))
                {
                    for(auto z:Leading[y[i+1]])
                    OPparse_table[mpchar[y[i]]][mpchar[z]]+="<. ,";
                    for(auto z:Trailing[y[i-1]])
                    OPparse_table[mpchar[z]][mpchar[y[i]]]+=".> ,";
                }
            }
        }
    }
    for(i=0;i<=terminal.size();i++)
    {
        cout<<"|";
        for(j=0;j<=terminal.size();j++)
        {
            OPparse_table[i][j].pop_back();
            cout<<" "<<OPparse_table[i][j];
            for(k=0;k<12-OPparse_table[i][j].size();k++)
            cout<<" ";
            cout<<"|";
        }
        cout<<"\n";
    }
    cout<<"\n";
}
void printLL1_parseTable()
{
    ll i,j,k,production_mapping;
    cout<<"Assuming the productions to be numebered as follows then parse table will be-\n\n";
    for(auto x:mp)
    cout<<x.second<<" -> "<<x.first.first<<" "<<x.first.second<<"\n";
    cout<<"\n";
    n=variable.size();
    m=terminal.size();
    vector<vector<string>>parse_table;
    parse_table.assign(n+1,vector<string>(m+1," "));
    k=1;
    for(auto x:terminal)
    {
        posterminal[x]=k;
        k++;
    }
    string temps;
    k=1;
    for(auto x:productions)
    {
        for(auto y:x.second)
        {
            production_mapping=mp[{x.first,y}];
            if(y[0]=='#')
            {
                for(auto f:Follow[x.first])
                {
                    temps=to_string(production_mapping);
                    temps+=",";
                    parse_table[k][posterminal[f]]=temps;
                }
                continue;
            }
            for(auto z:First[y[0]])
            {
                if(z=='#')
                {
                    for(auto f:Follow[x.first])
                    {
                        temps=to_string(production_mapping);
                        temps+=",";
                        parse_table[k][posterminal[f]]=temps;
                    }
                }
                else
                {
                    temps=to_string(production_mapping);
                    temps+=",";
                    parse_table[k][posterminal[z]]=temps;
                }
            }
        }
        k++;
    }
    auto x=productions.begin();
    for(i=1;i<=n;i++)
    {
        parse_table[i][0]=(*x).first;
        x++;
    }
    auto y=terminal.begin();
    for(i=1;i<=m;i++)
    {
        parse_table[0][i]=(*y);
        y++;
    }
    for(i=0;i<=n;i++)
    {
        cout<<"|";
        for(j=0;j<=m;j++)
        {
            if(parse_table[i][j].back()==',')
            parse_table[i][j].pop_back();
            cout<<" "<<parse_table[i][j];
            for(k=0;k<5-parse_table[i][j].size();k++)
            cout<<" ";
            cout<<"|";
        }
        cout<<"\n";
    }
    cout<<"\n";
}
void inputAndMappingOfProductions()
{
    LookAhead.clear();
    LAstates.clear();
    combineStates.clear();
    vis.clear();
    productions.clear();
    variable.clear();
    terminal.clear();
    First.clear();
    Follow.clear();
    mp.clear();
    posterminal.clear();
    Reduce.clear();
    productionsInState.clear();
    states.clear();
    Goto.clear();
    ll i,k;
    string s;
    char ch;
    cin>>N;
    k=0;
    for(i=0;i<N;i++)
    {
        cin>>ch>>s;
        mp[{ch,s}]=k;
        k++;
        variable.insert(ch);
        for(auto x:s)
        {
            if(x=='#')
            continue;
            if(is_terminal(x))
            terminal.insert(x);
            else
            variable.insert(x);
        }
        if(i==0)
        Strting=ch;
        productions[ch].push_back(s);
    }
    terminal.insert('$');
}
void printLR1_ParseTable()
{
    cout<<"\nAssuming the productions to be numebered as follows then parse table will be-\n\n";
    for(auto x:mp)
    cout<<x.second<<" -> "<<x.first.first<<" "<<x.first.second<<"\n";
    cout<<"\n";
    ll i,j,k,m;
    m=terminal.size();
    m+=variable.size();
    vector<vector<string>>Lr1ParseTable(totalStates+2,vector<string>(m+1," "));
    for(i=1;i<=totalStates+1;i++)
    Lr1ParseTable[i][0]=to_string(i-1);
    k=1;
    string firs="@ ",temp;
    firs+=Strting;
    firs+=".";
    auto term=terminal.begin();
    auto var=variable.begin();
    map<ll,char>mapchar;
    map<char,ll>mapll;
    for(j=1;j<=terminal.size();j++)
    {
        Lr1ParseTable[0][j]=*term;
        mapchar[j]=*term;
        mapll[*term]=j;
        term++;
    }
    for(i=j;i<=terminal.size()+variable.size();i++)
    {
        Lr1ParseTable[0][i]=*var;
        mapchar[i]=*var;
        var++;
    }
    for(i=1;i<=totalStates+1;i++)
    {
        for(j=1;j<=m;j++)
        {
            if(Goto.find({i-1,mapchar[j]})!=Goto.end())
            {
                if(is_terminal(mapchar[j]))
                Lr1ParseTable[i][j]="S"+to_string(Goto[{i-1,mapchar[j]}]);
                else
                Lr1ParseTable[i][j]=to_string(Goto[{i-1,mapchar[j]}]);
            }
        }
        if(Reduce.find(i-1)!=Reduce.end())
        {
            if(Reduce[i-1]==firs)
            {
                Lr1ParseTable[i][mapll['$']]="Acc";
                continue;
            }
            // for lr1 
            for(auto x:Follow[Reduce[i-1][0]])
            {
                temp=Reduce[i-1].substr(2);
                temp.pop_back();
                Lr1ParseTable[i][mapll[x]]="R"+to_string(mp[{Reduce[i-1][0],temp}]);
            }
        }
    }
    for(i=0;i<=totalStates+1;i++)
    {
        cout<<"|";
        for(j=0;j<=m;j++)
        {
            cout<<"  "<<Lr1ParseTable[i][j];
            for(k=0;k<4-Lr1ParseTable[i][j].size();k++)
            cout<<" ";
            cout<<"|";
        }
        cout<<"\n";
    }
    cout<<"\n";
}
void printCLR1_ParseTable()
{
    cout<<"\nAssuming the productions to be numebered as follows then parse table will be-\n\n";
    for(auto x:mp)
    cout<<x.second<<" -> "<<x.first.first<<" "<<x.first.second<<"\n";
    cout<<"\n";
    ll i,j,k,m;
    m=terminal.size();
    m+=variable.size();
    vector<vector<string>>CLr1ParseTable(totalStates+2,vector<string>(m+1," "));
    for(i=1;i<=totalStates+1;i++)
    CLr1ParseTable[i][0]=to_string(i-1);
    k=1;
    string firs="@ ",temp;
    firs+=Strting;
    firs+=".";
    auto term=terminal.begin();
    auto var=variable.begin();
    map<ll,char>mapchar;
    map<char,ll>mapll;
    for(j=1;j<=terminal.size();j++)
    {
        CLr1ParseTable[0][j]=*term;
        mapchar[j]=*term;
        mapll[*term]=j;
        term++;
    }
    for(i=j;i<=terminal.size()+variable.size();i++)
    {
        CLr1ParseTable[0][i]=*var;
        mapchar[i]=*var;
        var++;
    }
    for(i=1;i<=totalStates+1;i++)
    {
        for(j=1;j<=m;j++)
        {
            if(Goto.find({i-1,mapchar[j]})!=Goto.end())
            {
                if(is_terminal(mapchar[j]))
                CLr1ParseTable[i][j]="S"+to_string(Goto[{i-1,mapchar[j]}]);
                else
                CLr1ParseTable[i][j]=to_string(Goto[{i-1,mapchar[j]}]);
            }
        }
        if(Reduce.find(i-1)!=Reduce.end())
        {
            if(Reduce[i-1]==firs)
            {
                CLr1ParseTable[i][mapll['$']]="Acc";
                continue;
            }
            // for Clr1 
            for(auto x:LookAhead[{i-1,Reduce[i-1]}])
            {
                temp=Reduce[i-1].substr(2);
                temp.pop_back();
                CLr1ParseTable[i][mapll[x]]="R"+to_string(mp[{Reduce[i-1][0],temp}]);
            }
        }
    }
    for(i=0;i<=totalStates+1;i++)
    {
        cout<<"|";
        for(j=0;j<=m;j++)
        {
            cout<<"  "<<CLr1ParseTable[i][j];
            for(k=0;k<4-CLr1ParseTable[i][j].size();k++)
            cout<<" ";
            cout<<"|";
        }
        cout<<"\n";
    }
    cout<<"\n";
}
string combine(char ch)
{
    string ans="";
    for(auto x:combineStates)
    {
        if(find(x.begin(),x.end(),(ch-'0'))!=x.end())
        for(auto y:x)
        ans+=(char)(y+'0');
    }
    return ans;
}
void printLALR1_ParseTable()
{
    cout<<"\nAssuming the productions to be numebered as follows and after combining the common core, parse table will be-\n\n";
    for(auto x:mp)
    cout<<x.second<<" -> "<<x.first.first<<" "<<x.first.second<<"\n";
    cout<<"\n";
    ll i,j,k,m;
    m=terminal.size();
    m+=variable.size();
    vector<vector<string>>CLr1ParseTable(totalStates+2,vector<string>(m+1,""));
    for(i=1;i<=totalStates+1;i++)
    for(auto x:combineStates[i-1])
    CLr1ParseTable[i][0]+=to_string(x);
    k=1;
    string firs="@ ",temp;
    firs+=Strting;
    firs+=".";
    auto term=terminal.begin();
    auto var=variable.begin();
    map<ll,char>mapchar;
    map<char,ll>mapll;
    for(j=1;j<=terminal.size();j++)
    {
        CLr1ParseTable[0][j]=*term;
        mapchar[j]=*term;
        mapll[*term]=j;
        term++;
    }
    for(i=j;i<=terminal.size()+variable.size();i++)
    {
        CLr1ParseTable[0][i]=*var;
        mapchar[i]=*var;
        var++;
    }
    for(i=1;i<=totalStates+1;i++)
    {
        if(vis[i-1])
        continue;
        for(auto z:combineStates[i-1])
        {
            for(j=1;j<=m;j++)
            {
                if(Goto.find({z,mapchar[j]})!=Goto.end())
                {
                    if(is_terminal(mapchar[j]))
                    CLr1ParseTable[i][j]+=to_string(Goto[{z,mapchar[j]}]);
                    else
                    CLr1ParseTable[i][j]+=to_string(Goto[{z,mapchar[j]}]);
                }
            }
            if(Reduce.find(z)!=Reduce.end())
            {
                if(Reduce[z]==firs)
                {
                    CLr1ParseTable[i][mapll['$']]="Acc";
                    continue;
                }
                for(auto x:LookAhead[{z,Reduce[z]}])
                {
                    temp=Reduce[z].substr(2);
                    temp.pop_back();
                    CLr1ParseTable[i][mapll[x]]="R"+to_string(mp[{Reduce[z][0],temp}]);
                }
            }
        }
    }
    for(i=0;i<=totalStates+1;i++)
    {
        if(i>0 && vis[i-1])
        continue;
        cout<<"|";
        for(j=0;j<=m;j++)
        {
            cout<<"  ";
            if(i>0 && j>0 && CLr1ParseTable[i][j]!="")
            {
                if(CLr1ParseTable[i][j][0]!='R' &&  CLr1ParseTable[i][j][0]!='A')
                {
                    CLr1ParseTable[i][j]=combine(CLr1ParseTable[i][j][0]);
                    if(i>0 && j>0 && j<=terminal.size())
                    CLr1ParseTable[i][j]="S"+CLr1ParseTable[i][j];
                }
            }
            cout<<CLr1ParseTable[i][j];
            for(k=0;k<5-CLr1ParseTable[i][j].size();k++)
            cout<<" ";
            cout<<"|";
        }
        cout<<"\n";
    }
    cout<<"\n";
}
void printLR0_ParseTable()
{
    cout<<"\nAssuming the productions to be numebered as follows then parse table will be-\n\n";
    for(auto x:mp)
    cout<<x.second<<" -> "<<x.first.first<<" "<<x.first.second<<"\n";
    cout<<"\n";
    ll i,j,k,m;
    m=terminal.size();
    m+=variable.size();
    vector<vector<string>>Lr1ParseTable(totalStates+2,vector<string>(m+1," "));
    for(i=1;i<=totalStates+1;i++)
    Lr1ParseTable[i][0]=to_string(i-1)+",";
    k=1;
    string firs="@ ",temp;
    firs+=Strting;
    firs+=".";
    auto term=terminal.begin();
    auto var=variable.begin();
    map<ll,char>mapchar;
    map<char,ll>mapll;
    for(j=1;j<=terminal.size();j++)
    {
        Lr1ParseTable[0][j]=*term;
        Lr1ParseTable[0][j]+=",";
        mapchar[j]=*term;
        mapll[*term]=j;
        term++;
    }
    for(i=j;i<=terminal.size()+variable.size();i++)
    {
        Lr1ParseTable[0][i]=*var;
        Lr1ParseTable[0][i]+=",";
        mapchar[i]=*var;
        var++;
    }
    string h;
    for(i=1;i<=totalStates+1;i++)
    {
        for(j=1;j<=m;j++)
        {
            if(Goto.find({i-1,mapchar[j]})!=Goto.end())
            {
                if(is_terminal(mapchar[j]))
                {
                    h=("S"+to_string(Goto[{i-1,mapchar[j]}])+",");
                    Lr1ParseTable[i][j]+=h;
                }
                else
                {
                    h=(to_string(Goto[{i-1,mapchar[j]}])+",");
                    Lr1ParseTable[i][j]+=h;
                }
            }
        }
        if(Reduce.find(i-1)!=Reduce.end())
        {
            if(Reduce[i-1]==firs)
            {
                for(j=1;j<=terminal.size();j++)
                {
                    h="Acc,";
                    Lr1ParseTable[i][j]+=h;
                }
                continue;
            }
            // for lr0 
            for(j=1;j<=terminal.size();j++)
            {
                temp=Reduce[i-1].substr(2);
                temp.pop_back();
                h=("R"+to_string(mp[{Reduce[i-1][0],temp}])+",");
                Lr1ParseTable[i][j]+=h;
            }
        }
    }
    for(i=0;i<=totalStates+1;i++)
    {
        cout<<"|";
        for(j=0;j<=m;j++)
        {
            Lr1ParseTable[i][j].pop_back();
            cout<<"  "<<Lr1ParseTable[i][j];
            for(k=0;k<8-Lr1ParseTable[i][j].size();k++)
            cout<<" ";
            cout<<"|";
        }
        cout<<"\n";
    }
    cout<<"\n";
}
void completeStatesClosure(vector<string>&stringsInState)
{
    set<string>vs;
    for(auto x:stringsInState)
    vs.insert(x);
    ll i,n,ss,j;
    string temps;
    bool flag;
    n=stringsInState.size();
    for(i=0;i<n;i++)
    {
        ss=stringsInState[i].size();
        for(j=0;j<ss-1;j++)
        {
            if(stringsInState[i][j]=='.' && !is_terminal(stringsInState[i][j+1]))
            {
                char f=stringsInState[i][j+1];
                flag=false;
                for(string x:productions[f])
                {
                    temps=f;
                    temps+=" .";
                    temps+=x;
                    if(vs.find(temps)==vs.end())
                    {
                        stringsInState.push_back(temps);
                        flag=true;
                    }
                }
                if(flag)
                completeStatesClosure(stringsInState);
                break;
            }
        }
    }
}
void completeStatesClosurePlusLookAhead(vector<string>&stringsInState)
{
    set<string>vs;
    for(auto x:stringsInState)
    vs.insert(x);
    ll i,n,ss,j;
    string temps;
    bool flag;
    n=stringsInState.size();
    for(i=0;i<n;i++)
    {
        ss=stringsInState[i].size();
        for(j=0;j<ss-1;j++)
        {
            if(stringsInState[i][j]=='.' && !is_terminal(stringsInState[i][j+1]))
            {
                char f=stringsInState[i][j+1];
                flag=false;
                for(string x:productions[f])
                {
                    temps=f;
                    temps+=" .";
                    temps+=x;
                    if(vs.find(temps)==vs.end())
                    {
                        stringsInState.push_back(temps);
                        if(j+2<stringsInState[i].size())
                        {
                            for(auto z:First[stringsInState[i][j+2]])
                            LookAhead[{totalStates,temps}].insert(z);
                        }
                        else
                        {
                            for(auto z:LookAhead[{totalStates,stringsInState[i]}])
                            LookAhead[{totalStates,temps}].insert(z);
                        }
                        flag=true;
                    }
                }
                if(flag)
                completeStatesClosurePlusLookAhead(stringsInState);
                break;
            }
        }
    }
}
void makeNewState(ll state)
{
    bool flag;
    ll i,ss;
    map<char,ll>mpp;
    for(string x:productionsInState[currState])
    {
        ss=x.size();
        for(i=0;i<ss-1;i++)
        if(x[i]=='.')
        mpp[x[i+1]]++;
        if(x.back()=='.')
        Reduce[currState]=x;
    }
    string str;
    for(auto x:mpp)
    {
        vector<string>temp,srttemp;
        for(string y:productionsInState[currState])
        {
            ss=y.size();
            for(i=0;i<ss-1;i++)
            if(y[i]=='.' && y[i+1]==x.first)
            {
                str=y;
                swap(str[i],str[i+1]);
                temp.push_back(str);
                break;
            }
            if(y.back()=='.')
            Reduce[currState]=y;
        }
        if(temp.size()==0)
        continue;
        completeStatesClosure(temp);
        srttemp=temp;
        sort(srttemp.begin(),srttemp.end());
        if(states.find(srttemp)!=states.end())
        {
            Goto[{currState,x.first}]=states[srttemp];
            continue;
        }
        flag=true;
        totalStates++;
        productionsInState[totalStates]=temp;
        Goto[{currState,x.first}]=totalStates;
        states[srttemp]=totalStates;
    }
    currState++;
    if(currState<=totalStates)
    makeNewState(currState);
}
void makeCLRNewState(ll state)
{
    bool flag,ff;
    ll i,ss;
    map<char,ll>mpp;
    for(string x:productionsInState[currState])
    {
        ss=x.size();
        for(i=0;i<ss-1;i++)
        if(x[i]=='.')
        mpp[x[i+1]]++;
        if(x.back()=='.')
        Reduce[currState]=x;
    }
    string str;
    for(auto x:mpp)
    {
        vector<string>temp,srttemp;
        string yy="!";
        for(string y:productionsInState[currState])
        {
            ss=y.size();
            for(i=0;i<ss-1;i++)
            if(y[i]=='.' && y[i+1]==x.first)
            {
                yy=y;
                str=y;
                swap(str[i],str[i+1]);
                temp.push_back(str);
                break;
            }
            if(y.back()=='.')
            Reduce[currState]=y;
        }
        if(temp.size()==0)
        continue;
        totalStates++;
        LookAhead[{totalStates,str}]=LookAhead[{currState,yy}];
        completeStatesClosurePlusLookAhead(temp);
        srttemp=temp;
        sort(srttemp.begin(),srttemp.end());
        if(LAstates.find({LookAhead[{totalStates,str}],srttemp})!=LAstates.end())
        {
            Goto[{currState,x.first}]=LAstates[{LookAhead[{totalStates,str}],srttemp}];
            LookAhead[{totalStates,str}].clear();
            totalStates--;
            continue;
        }
        flag=true;
        productionsInState[totalStates]=temp;
        Goto[{currState,x.first}]=totalStates;
        LAstates[{LookAhead[{totalStates,str}],srttemp}]=totalStates;
    }
    currState++;
    if(currState<=totalStates)
    makeCLRNewState(currState);
}
void printLR1ItemSet()
{
    cout<<"LR1 itemset, total states-> "<<totalStates+1<<"\n\n";
    for(ll i=0;i<=totalStates;i++)
    {
        cout<<"State->"<<i<<"\n";
        for(auto x:productionsInState[i])
        cout<<x<<"\n";
        cout<<"\n";
    }
}
void printCLR1ItemSet()
{
    cout<<"CLR1 itemset, total states-> "<<totalStates+1<<"\n\n";
    for(ll i=0;i<=totalStates;i++)
    {
        cout<<"State->"<<i<<"\n";
        cout<<"productions    lookahead\n";
        for(auto x:productionsInState[i])
        {
            cout<<x<<"          ";
            for(auto y:LookAhead[{i,x}])
            cout<<y<<",";
            cout<<"\n";
        }
        cout<<"\n";
    }
}
void printLALR1ItemSet()
{
    ll i,j,no_states,k;
    no_states=0;
    vis.assign(totalStates+1,false);
    combineStates.assign(totalStates+1,vector<ll>());
    for(i=0;i<=totalStates;i++)
    {
        if(!vis[i])
        {
            no_states++;
            combineStates[i].push_back(i);
            for(j=i+1;j<=totalStates;j++)
            {
                if(!vis[j])
                if(productionsInState[i]==productionsInState[j])
                {
                    for(auto x:productionsInState[i])
                    {
                        for(auto y:LookAhead[{j,x}])
                        LookAhead[{i,x}].insert(y);
                    }
                    vis[j]=true;
                    combineStates[i].push_back(j);
                }
            }
        }
    }
    cout<<"LALR1 itemset, total states-> "<<no_states<<"\n\n";
    k=0;
    for(i=0;i<=totalStates;i++)
    {
        if(vis[i])
        continue;
        cout<<"State->";
        for(auto x:combineStates[i])
        cout<<x;
        cout<<"\n";
        cout<<"productions    lookahead\n";
        for(auto x:productionsInState[i])
        {
            cout<<x<<"          ";
            for(auto y:LookAhead[{i,x}])
            cout<<y<<",";
            cout<<"\n";
        }
        cout<<"\n";
        k++;
    }
}
void printGotoInformation()
{
    cout<<"Information regarding goto\n";
    cout<<"\n| From | variable/terminal | To |\n";
    for(auto x:Goto)
    {
        cout<<"|  "<<x.first.first;
        if(x.first.first<10)
        cout<<"   |         ";
        else
        cout<<"  |         ";
        cout<<x.first.second<<"         | "<<x.second;
        if(x.second<10)
        cout<<"  |\n";
        else
        cout<<" |\n";
    }
}
void printReduceInformation()
{
    cout<<"\nInformation regarding reduce\n\n";
    for(auto x:Reduce)
    cout<<"state-> "<<x.first<<" ,Reduced-> "<<x.second<<"\n";
}
void LR0itemset()
{
    findFirstFollow();
    totalStates=0;
    currState=0;
    string s="@ .";
    s+=Strting;
    productionsInState[0].push_back(s);
    completeStatesClosure(productionsInState[0]);
    makeNewState(0);
}
void CLR1itemset()
{
    findFirstFollow();
    totalStates=0;
    currState=0;
    string s="@ .";
    s+=Strting;
    productionsInState[0].push_back(s);
    LookAhead[{0,s}].insert('$');
    completeStatesClosurePlusLookAhead(productionsInState[0]);
    makeCLRNewState(0);
}
void LL1()
{
    cout<<"LL1 parse table\n\n";
    inputAndMappingOfProductions();
    findFirstFollow();
    printFirstFollow();
    printLL1_parseTable();
    for(ll i=0;i<120;i++)
    cout<<"*";
    cout<<"\n";
}
void LR1()
{
    cout<<"LR1 parse table\n\n";
    inputAndMappingOfProductions();
    LR0itemset();
    printLR1ItemSet();
    printGotoInformation();
    printReduceInformation();
    printLR1_ParseTable();
    for(ll i=0;i<120;i++)
    cout<<"*";
    cout<<"\n";
}
void LR0()
{
    cout<<"LR0 parse table\n\n";
    inputAndMappingOfProductions();
    LR0itemset();
    printLR1ItemSet();
    printGotoInformation();
    printLR0_ParseTable();
    for(ll i=0;i<120;i++)
    cout<<"*";
    cout<<"\n";
}
void CLR1()
{
    cout<<"CLR1 parse table\n\n";
    inputAndMappingOfProductions();
    CLR1itemset();
    printFirstFollow();
    printCLR1ItemSet();
    printGotoInformation();
    printReduceInformation();
    printCLR1_ParseTable();
    for(ll i=0;i<120;i++)
    cout<<"*";
    cout<<"\n";
}
void LALR1()
{
    cout<<"LALR1 parse table\n\n";
    inputAndMappingOfProductions();
    CLR1itemset();
    printFirstFollow();
    printLALR1ItemSet();
    printGotoInformation();
    printReduceInformation();
    printLALR1_ParseTable();
    for(ll i=0;i<120;i++)
    cout<<"*";
    cout<<"\n";
}
void operatorPrecedence()
{
    cout<<"Operator precedence parse table\n\n";
    inputAndMappingOfProductions();
    findLeadingtrailing();
    printLeadingTrailing();
    printOperatorPrecedenceParseTable();
    for(ll i=0;i<120;i++)
    cout<<"*";
    cout<<"\n";
}
int main(void)
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    freopen("parsein.txt","r",stdin);
    freopen("parseout.txt","w",stdout);
    LL1();
    LR0();
    LR1();
    operatorPrecedence();
    CLR1();
    LALR1();
    return 0;
}