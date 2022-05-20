#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;
map<string, int> state = {{"000", 0}, {"001", 0}, {"010", 0}, {"011", 0}, {"100", 0}, {"101", 0}, {"110", 0}, {"111", 0}};
string history = "000";

int modify(int statenum, int mode)
{
    if (mode) // plus
    {
        if (statenum < 3)
            statenum++;
    }
    else // minus
    {
        if (statenum > 0)
            statenum--;
    }
    return statenum;
}

string turn_2bc(int n)
{
    if (n == 0)
        return "SN";
    else if (n == 1)
        return "WN";
    else if (n == 2)
        return "WT";
    else if (n == 3)
        return "ST";
    else
        return "error";
}

int main()
{
    cout << " initial state : ";
    for (auto i : state)
    {
        cout << "[" << i.first << ":" << turn_2bc(i.second) << "] ";
    }
    cout << endl;
    cout << " initial history : " << history << endl;
    string s;
    fstream f("test.txt");
    getline(f, s);
    // cout << " input : " << s << endl;
    f.close();
    cout << " ==========start==========\n";
    for (int i = 0; i < s.length(); i++)
    {
        int curstate = state[history];
        if (curstate == 0 || curstate == 1)
        {
            cout << " predict : not taken\n";
            if (s[i] == 'N')
            {
                cout << " actual : not taken -> correct\n";
                string temp = history.substr(1, 2);
                temp += "0";
                history = temp;
            }
            else
            {
                cout << " actual : taken -> wrong\n";
                state[history] = modify(state[history], 1); // modify the state
                string temp = history.substr(1, 2);
                temp += "1";
                history = temp;
            }
        }
        else // current state==2,3
        {
            cout << " predict : taken\n";
            if (s[i] == 'T')
            {
                cout << " actual : taken -> correct\n";
                string temp = history.substr(1, 2);
                temp += "1";
                history = temp;
            }
            else
            {
                cout << " actual : not taken -> wrong\n";
                state[history] = modify(state[history], 0); // modify the state
                string temp = history.substr(1, 2);
                temp += "0";
                history = temp;
            }
        }
        cout << " current state : ";
        for (auto i : state)
        {
            cout << "[" << i.first << ":" << turn_2bc(i.second) << "] ";
        }
        cout << endl;
        cout << " new history : " << history << endl;
    }
    system("pause");
    return 0;
}
