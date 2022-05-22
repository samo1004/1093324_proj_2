#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
using namespace std;
int RegValues[32] = {0}; //存32registers value
int misprediction = 0;
int modify(int state_num, int mode)
{
    if (mode) // plus
    {
        if (state_num < 3)
            state_num++;
    }
    else // minus
    {
        if (state_num > 0)
            state_num--;
    }
    return state_num;
}

int execode(vector<string> inst) //丟入修整成vector的一排inst.
{
    int T = 0;
    if (inst[1] == "addi") // N
    {
        string rd = inst[2];
        string rs = inst[3];
        string reg_rd;
        string reg_rs;
        cout << inst[4] << endl;
        int imm = stoi(inst[4]);
        // stringstream ss;
        // ss << inst[4];
        // ss >> imm;
        // cout << "imm size" << inst[4].size() << endl;
        // if (inst[4].find('-'))
        // {
        //     imm = -imm;
        // }
        cout << "imm: " << imm << endl;
        if (rd.length() == 2)
        {
            reg_rd = rd.substr(1, 1);
            cout << "reg_rd:" << reg_rd << endl;
        }
        else if (rd.length() == 3)
        {
            reg_rd = rd.substr(1, 2);
            cout << "reg_rd:" << reg_rd << endl;
        }
        int reg_rd_num = stoi(reg_rd); //第幾個reg當作rd
        if (rs.length() == 2)
        {
            reg_rs = rs.substr(1, 1);
            cout << "reg_rs:" << reg_rs << endl;
        }
        else if (rs.length() == 3)
        {
            reg_rs = rs.substr(1, 2);
            cout << "reg_rs:" << reg_rs << endl;
        }
        int reg_rs_num = stoi(reg_rs); //第幾個reg當作rs
        cout << "rsvalue:" << RegValues[reg_rs_num] << endl;
        RegValues[reg_rd_num] = RegValues[reg_rs_num] + imm;
        cout << "rdvalue new:" << RegValues[reg_rd_num] << endl;
        T = 0;
        return T;
    }
    else if (inst[1] == "beq")
    {
        string rs1 = inst[2]; // r1
        string rs2 = inst[3]; // r2
        string reg_rs1;
        string reg_rs2;
        if (rs1.length() == 2)
        {
            reg_rs1 = rs1.substr(1, 1);
            cout << "rs1:" << reg_rs1 << endl;
        }
        else if (rs1.length() == 3)
        {
            reg_rs1 = rs1.substr(1, 2);
            cout << "rs1:" << reg_rs1 << endl;
        }
        int reg_rs1_num = stoi(reg_rs1); //第幾個reg當作rs1
        if (rs2.length() == 2)
        {
            reg_rs2 = rs2.substr(1, 1);
            cout << "rs2:" << reg_rs2 << endl;
        }
        else if (rs2.length() == 3)
        {
            reg_rs2 = rs2.substr(1, 2);
            cout << "rs2:" << reg_rs2 << endl;
        }
        int reg_rs2_num = stoi(reg_rs2); //第幾個reg當作rs2
        cout << "rs1value:" << RegValues[reg_rs1_num] << endl;
        cout << "rs2value:" << RegValues[reg_rs2_num] << endl;
        if (RegValues[reg_rs1_num] == RegValues[reg_rs2_num])
        {
            cout << "equal" << endl;
            T = 1;
        }
        else
        {
            cout << "not equal" << endl;
            T = 0;
        }
        return T;
    }
}

int en = 0; // entry數目
int which_entry(int instcount)
{
    return instcount % en;
}
int predict(string his, map<string, int> states) // return 0 or 1(0:N,1:T)
{
    if (states[his] == 0 || states[his] == 1)
    {
        return 0; // N
    }
    else if (states[his] == 2 || states[his] == 3)
    {
        return 1; // T
    }
}
int main()
{
    RegValues[0] = 0;
    vector<string> in;
    vector<vector<string>> mycode;
    fstream f("input.txt");
    cout << "enter the number of entries: ";
    cin >> en;
    map<string, int> states_of_entries[en];
    for (int i = 0; i < en; i++) // initialize each states_of_entries
    {
        states_of_entries[i] = {{"000", 0}, {"001", 0}, {"010", 0}, {"011", 0}, {"100", 0}, {"101", 0}, {"110", 0}, {"111", 0}};
    }
    string history[en];
    for (int i = 0; i < en; i++) // inityialize history for each entry
    {
        history[i] = "000";
    }

    string s;
    while (getline(f, s))
    {
        in.push_back(s);
    }
    for (auto i : in) //將註解部分移除
    {
        while (i.find(";") != string::npos)
        {
            i.erase(i.find(";"), i.find("\n"));
        }
        // cout << i << endl;
    }
    string tmp = "";
    vector<string> temp_vec;
    for (int i = 0; i < in.size(); i++) //把字串處理成 pc,operator
    {                                   //(對應operand、label)或單放label
        temp_vec.clear();
        for (int j = 0; j < in[i].size(); j++)
        {
            // if (in[i][j] == ',')
            //     in[i][j] = ' ';
            if (!isalnum(in[i][j]) && in[i][j] != '-')
                continue;
            else
            {
                tmp += in[i][j];
                if (!isalnum(in[i][j + 1])) //如果下一格不是alphabet or number
                {
                    temp_vec.push_back(tmp); //先把tmp存起來
                    tmp = "";                //清掉
                    continue;
                }
            }
        }
        mycode.push_back(temp_vec);
    }
    cout << "\n>>>>>modified code<<<<<\n\n";
    for (auto i : mycode)
    {
        for (auto j : i)
        {
            cout << "[" << j << "] ";
        }
        cout << endl;
    }
    int instc = 0;
    int lines = mycode.size(); //行數
    int instcount_or_label[lines];
    map<string, int> record_labels; // record_labels[label]會return
    for (int i = 0; i < lines; i++) // label的行數(branch的地方)
    {
        if (mycode[i][0][0] == '0' && mycode[i][0][1] == 'x' && isdigit(mycode[i][0][2])) // inst.
        {
            instcount_or_label[i] = instc; // inst.計數器
            instc++;                       // inst_count到時候可以
        }                                  //%entry_num去決定entry
        else                               // Label
        {
            instcount_or_label[i] = 999;     // not inst.
            record_labels[mycode[i][0]] = i; // label record
        }
    }
    /////////////////////////////////////////////
    cout << "\n>>>>>instcount_or_label<<<<<\n\n";
    cout << "inst.s:\n\n";
    for (int i = 0; i < lines; i++)
    {
        if (instcount_or_label[i] != 999)
        {
            cout << "inst. " << instcount_or_label[i] << ": ";
            for (int j = 0; j < mycode[i].size(); j++)
            {
                cout << mycode[i][j] << " ";
            }
            cout << endl;
        }
    }
    cout << "\nlabels:\n\n";
    for (auto i : record_labels)
    {
        cout << i.first << ": " << i.second << endl;
    }
    cout << endl;
    /////////////////////////////////////////////

    cout << "start>>\n\n";
    for (int i = 0; i < en; i++)
    {
        cout << "entry " << i << " initial history: " << history[i] << endl;
        cout << "entry " << i << " initial state: ";
        for (auto j : states_of_entries[i])
        {
            cout << "[" << j.first << "," << j.second << "] ";
        }
        cout << endl;
    }
    //開始預測
    // int count = 0;
    int TorNT;
    int actual;
    int count = 0;
    for (int i = 0; i < lines; i++ /*, count++*/) //如果沒跳就一直望下走
    {
        // if (count > 10)
        //     break;
        cout << "now code :";
        for (auto j : mycode[i])
        {
            cout << "[" << j << "] ";
        }
        cout << "using entry: " << which_entry(instcount_or_label[i]) << endl;
        // if (count > 10)
        //     break;
        if (mycode[i][0] == "End:")
        {
            cout << "complete\n";
            break;
        }
        else if (instcount_or_label[i] == 999) // label就直接跳過
        {
            // cout << "label: " << mycode[i][0] << endl;
            continue;
        }
        else // inst先predict
        {
            TorNT = predict(history[which_entry(instcount_or_label[i])],
                            states_of_entries[which_entry(instcount_or_label[i])]);
            cout << "predict: " << (TorNT == 1 ? "Taken" : "Not Taken") << endl;
            actual = execode(mycode[i]);
            cout << "actual: " << (actual == 1 ? "Taken" : "Not Taken") << endl;

            if (TorNT == actual)
            {
                cout << "correct\n";
                if (actual == 1)
                {
                    string temp = history[which_entry(instcount_or_label[i])].substr(1.2);
                    temp += "1";
                    history[which_entry(instcount_or_label[i])] = temp;
                }
                else if (actual == 0)
                {
                    string temp = history[which_entry(instcount_or_label[i])].substr(1.2);
                    temp += "0";
                    history[which_entry(instcount_or_label[i])] = temp;
                }
            }
            else
            {
                cout << "wrong\n";
                if (TorNT == 1 && actual == 0)
                {
                    states_of_entries[which_entry(instcount_or_label[i])][history[which_entry(instcount_or_label[i])]] =
                        modify(states_of_entries[which_entry(instcount_or_label[i])][history[which_entry(instcount_or_label[i])]], 0);
                    string temp = history[which_entry(instcount_or_label[i])].substr(1.2);
                    temp += "0";
                    history[which_entry(instcount_or_label[i])] = temp;
                }
                else if (TorNT == 0 && actual == 1)
                {
                    states_of_entries[which_entry(instcount_or_label[i])][history[which_entry(instcount_or_label[i])]] =
                        modify(states_of_entries[which_entry(instcount_or_label[i])][history[which_entry(instcount_or_label[i])]], 1);
                    string temp = history[which_entry(instcount_or_label[i])].substr(1.2);
                    temp += "1";
                    history[which_entry(instcount_or_label[i])] = temp;
                }
            }
            cout << " current state: ";
            for (auto j : states_of_entries[which_entry(instcount_or_label[i])])
            {
                cout << "[" << j.first << "," << j.second << "] ";
            }
            cout << "\n new history : " << history[which_entry(instcount_or_label[i])] << endl;
        }
        if (actual == 1) //真的有跳
        {
            i = record_labels[mycode[i][mycode[i].size() - 1]]; //跳到label繼續執行
            cout << "jump to label: " << mycode[i][mycode[i].size() - 1] << endl;
        }
    }
    system("pause");
    return 0;
}
