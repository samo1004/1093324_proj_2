#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
using namespace std;
int RegValues[32] = {0}; //存32registers value

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
string turn2bc(int n)
{
    string s;
    if (n == 0)
        s = "SN";
    else if (n == 1)
        s = "WN";
    else if (n == 2)
        s = "WT";
    else if (n == 3)
        s = "ST";
    return s;
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
        int imm = stoi(inst[4]);
        if (rd.length() == 2)
        {
            reg_rd = rd.substr(1, 1);
        }
        else if (rd.length() == 3)
        {
            reg_rd = rd.substr(1, 2);
        }
        int reg_rd_num = stoi(reg_rd); //第幾個reg當作rd
        if (rs.length() == 2)
        {
            reg_rs = rs.substr(1, 1);
        }
        else if (rs.length() == 3)
        {
            reg_rs = rs.substr(1, 2);
        }
        int reg_rs_num = stoi(reg_rs); //第幾個reg當作rs
        RegValues[reg_rd_num] = RegValues[reg_rs_num] + imm;
        T = 0; // add不會跳
    }
    else if (inst[1] == "beq" || inst[1] == "bne")
    {
        string rs1 = inst[2]; // r1
        string rs2 = inst[3]; // r2
        string reg_rs1;
        string reg_rs2;
        if (rs1.length() == 2)
        {
            reg_rs1 = rs1.substr(1, 1);
        }
        else if (rs1.length() == 3)
        {
            reg_rs1 = rs1.substr(1, 2);
        }
        int reg_rs1_num = stoi(reg_rs1); //第幾個reg當作rs1
        if (rs2.length() == 2)
        {
            reg_rs2 = rs2.substr(1, 1);
        }
        else if (rs2.length() == 3)
        {
            reg_rs2 = rs2.substr(1, 2);
        }
        int reg_rs2_num = stoi(reg_rs2); //第幾個reg當作rs2
        if (RegValues[reg_rs1_num] == RegValues[reg_rs2_num])
        {
            if (inst[1] == "beq")
                T = 1;
            else if (inst[1] == "bne")
                T = 0;
        }
        else
        {
            if (inst[1] == "beq")
                T = 0;
            else if (inst[1] == "bne")
                T = 1;
        }
    }
    else if (inst[1] == "li")
    {
        string rd = inst[2];
        string reg_rd;
        if (rd.length() == 2)
        {
            reg_rd = rd.substr(1, 1);
        }
        else if (rd.length() == 3)
        {
            reg_rd = rd.substr(1, 2);
        }
        int reg_rd_num = stoi(reg_rd); //第幾個reg當作rd
        // cout << "reg_rd_num:" << reg_rd_num << endl;
        RegValues[reg_rd_num] = stoi(inst[3]);
        // cout << "store value:" << RegValues[reg_rd_num] << endl;
        T = 0;
    }
    else if (inst[1] == "add")
    {
        string rd = inst[2];
        string rs1 = inst[3];
        string rs2 = inst[4];
        string reg_rd;
        string reg_rs1;
        string reg_rs2;
        if (rd.length() == 2)
        {
            reg_rd = rd.substr(1, 1);
        }
        else if (rd.length() == 3)
        {
            reg_rd = rd.substr(1, 2);
        }
        int reg_rd_num = stoi(reg_rd); //第幾個reg當作rd
        if (rs1.length() == 2)
        {
            reg_rs1 = rs1.substr(1, 1);
        }
        else if (rs1.length() == 3)
        {
            reg_rs1 = rs1.substr(1, 2);
        }
        int reg_rs1_num = stoi(reg_rs1); //第幾個reg當作rs
        if (rs2.length() == 2)
        {
            reg_rs2 = rs2.substr(1, 1);
        }
        else if (rs2.length() == 3)
        {
            reg_rs2 = rs2.substr(1, 2);
        }
        int reg_rs2_num = stoi(reg_rs2); //第幾個reg當作rs
        cout << RegValues[reg_rs1_num] << " + " << RegValues[reg_rs2_num] << " = ";
        RegValues[reg_rd_num] = RegValues[reg_rs1_num] + RegValues[reg_rs2_num];
        cout << RegValues[reg_rd_num] << endl;
        T = 0;
    }
    else if (inst[1] == "andi")
    {
        string rd = inst[2];
        string rs = inst[3];
        string reg_rd;
        string reg_rs;
        int imm = stoi(inst[4]);
        if (rd.length() == 2)
        {
            reg_rd = rd.substr(1, 1);
        }
        else if (rd.length() == 3)
        {
            reg_rd = rd.substr(1, 2);
        }
        int reg_rd_num = stoi(reg_rd); //第幾個reg當作rd
        if (rs.length() == 2)
        {
            reg_rs = rs.substr(1, 1);
        }
        else if (rs.length() == 3)
        {
            reg_rs = rs.substr(1, 2);
        }
        int reg_rs_num = stoi(reg_rs); //第幾個reg當作rs
        cout << RegValues[reg_rs_num] << " & " << imm << " = ";
        RegValues[reg_rd_num] = RegValues[reg_rs_num] & imm;
        cout << RegValues[reg_rd_num] << endl;
        // cout << RegValues[reg_rd_num] << endl;
        T = 0; // and不會跳
    }

    return T;
}

int en = 0; // entry數目
int which_entry(int instcount)
{
    return instcount % en;
}
int predict(string his, map<string, int> states) // return 0 or 1(0:N,1:T)
{
    int re = 0;
    if (states[his] == 0 || states[his] == 1)
    {
        re = 0; // N
    }
    else if (states[his] == 2 || states[his] == 3)
    {
        re = 1; // T
    }
    return re;
}
int main()
{

    /////////////////////////////////
    RegValues[0] = 0;
    vector<string> in;
    vector<vector<string>> mycode;
    fstream f("input.txt");
    cout << "enter the number of entries(2^n,n>0): ";
    cin >> en;
    //
    streambuf *psbuf, *backup;
    ofstream file;
    file.open("output.txt");
    backup = cout.rdbuf();
    psbuf = file.rdbuf();
    cout.rdbuf(psbuf); // cout定向到文件
    //
    int misprediction[en];
    for (int i = 0; i < en; i++)
    {
        misprediction[i] = 0;
    }
    for (auto i : misprediction)
        cout << i << " ";
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
    for (int i = 0; i < in.size(); i++) //將註解部分移除
    {
        in[i] = in[i].substr(0, in[i].find(";"));
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
    cout << "\n>>>>>instcount_or_label<<<<<\n";
    cout << "inst.s:\n";
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
    cout << "\nlabels:\n";
    for (auto i : record_labels)
    {
        cout << i.first << ": " << i.second << endl;
    }

    /////////////////////////////////////////////
    // system("pause");
    cout << "start>>\n";
    for (int i = 0; i < en; i++)
    {
        cout << "entry " << i << " initial history: " << history[i] << endl;
        cout << "entry " << i << " initial state: ";
        for (auto j : states_of_entries[i])
        {
            cout << "[" << j.first << "," << turn2bc(j.second) << "] ";
        }
        cout << endl;
    }
    cout << "========================================================================================================\n";
    //開始預測
    int TorNT;
    int actual;
    for (int i = 0; i < lines; i++) //如果沒跳就一直望下走
    {
        if (instcount_or_label[i] == 999) // label就直接跳過
        {
            continue;
        }
        else // inst先predict
        {
            cout << in[i].substr(7, in[i].find(";")) << endl;
            // print each entry's state
            for (int j = 0; j < en; j++)
            {
                cout << "entry " << j << " :";
                cout << "[" << history[j];
                for (auto k : states_of_entries[j])
                {
                    cout << ", " << turn2bc(k.second);
                }
                cout << "]";
                cout << endl;
            }

            cout << "\nusing entry: " << which_entry(instcount_or_label[i]) << endl;
            TorNT = predict(history[which_entry(instcount_or_label[i])],
                            states_of_entries[which_entry(instcount_or_label[i])]);
            cout << "predict: " << (TorNT == 1 ? "T" : "N") << ", ";
            actual = execode(mycode[i]);
            cout << "actual: " << (actual == 1 ? "T" : "N") << " => ";

            if (TorNT == actual)
            {
                cout << "correct\n";
                if (actual == 1)
                {
                    states_of_entries[which_entry(instcount_or_label[i])][history[which_entry(instcount_or_label[i])]] =
                        modify(states_of_entries[which_entry(instcount_or_label[i])][history[which_entry(instcount_or_label[i])]], 1);
                    string temp;
                    temp = history[which_entry(instcount_or_label[i])].substr(1.2);
                    temp += "1";
                    history[which_entry(instcount_or_label[i])] = temp;
                }
                else if (actual == 0)
                {
                    states_of_entries[which_entry(instcount_or_label[i])][history[which_entry(instcount_or_label[i])]] =
                        modify(states_of_entries[which_entry(instcount_or_label[i])][history[which_entry(instcount_or_label[i])]], 0);
                    string temp;
                    temp = history[which_entry(instcount_or_label[i])].substr(1.2);
                    temp += "0";
                    history[which_entry(instcount_or_label[i])] = temp;
                }
            }
            else
            {
                cout << "wrong\n";
                misprediction[which_entry(instcount_or_label[i])]++;
                if (TorNT == 1 && actual == 0)
                {
                    states_of_entries[which_entry(instcount_or_label[i])][history[which_entry(instcount_or_label[i])]] =
                        modify(states_of_entries[which_entry(instcount_or_label[i])][history[which_entry(instcount_or_label[i])]], 0);
                    string temp;
                    temp = history[which_entry(instcount_or_label[i])].substr(1.2);
                    temp += "0";
                    history[which_entry(instcount_or_label[i])] = temp;
                }
                else if (TorNT == 0 && actual == 1)
                {
                    states_of_entries[which_entry(instcount_or_label[i])][history[which_entry(instcount_or_label[i])]] =
                        modify(states_of_entries[which_entry(instcount_or_label[i])][history[which_entry(instcount_or_label[i])]], 1);
                    string temp;
                    temp = history[which_entry(instcount_or_label[i])].substr(1.2);
                    temp += "1";
                    history[which_entry(instcount_or_label[i])] = temp;
                }
            }
            cout << "misprediction: " << misprediction[which_entry(instcount_or_label[i])] << endl;
            cout << "========================================================================================================\n";
        }
        if (actual == 1) //真的有跳
        {
            i = record_labels[mycode[i][mycode[i].size() - 1]]; //跳到label繼續執行
        }
    }
    //
    cout.rdbuf(backup); //恢复cout输出重定向到终端
    file.close();
    //
    return 0;
}
