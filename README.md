# 1093324_proj_2

## 影片連結 :
   
   https://youtu.be/J6T-7ZUDGqg \n
   註: modify部分有口誤，如果實際taken且state小於3那就 +1，反之如果實際not taken且大於0就 -1

## 程式 :
   
   輸入內容為組合語言的文件，執行過程中預測當前instruction會不會branch，並將預測過程和結果輸出檢查

## 執行程式 :

   將三個檔案放到同個資料夾，分別為程式碼(1093324_proj_2.cpp)、輸入文件(input.txt)及空白輸出文件(output.txt)，執行程式後，輸入entry數目，程式就會將結果輸出到output.txt中
   
## 程式碼解說 :

   先將文件讀入並輸入entry數後，將assembly code存入一個名為mycode的vector陣列，存成pc，operator，operands的格式
   ，如果是label就只存label，並且印出檢查
```cpp
   string tmp = "";
    vector<string> temp_vec;
    for (int i = 0; i < in.size(); i++) //把字串處理成 pc,operator
    {                                   //(對應operand、label)或單放label
        temp_vec.clear();
        for (int j = 0; j < in[i].size(); j++)
        {
            if (!isalnum(in[i][j]) && in[i][j] != '-')//負號not isalnum,會被吃掉
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
```
之後將mycode中的code去做處理，記錄他是第幾個instruction(拿來判斷用哪個entry)或是這個label在這組code的第幾行(之後模擬執行branch時跳的地方)存到map中
```cpp
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
```
開始判斷，TorNT存放預測回傳值，actual存放實際執行後的TorNT，再去依照兩著符不符合做出修改state跟history的動作
```cpp
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
        if (actual == 1)                                        //真的有跳
            i = record_labels[mycode[i][mycode[i].size() - 1]]; //跳到label繼續執行
```
實際執行時將值放到32格的array中，模擬32 regesters的操作
execute函式:
```cpp
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
            reg_rd = rd.substr(1, 1);
        else if (rd.length() == 3)
            reg_rd = rd.substr(1, 2);
        int reg_rd_num = stoi(reg_rd); //第幾個reg當作rd
        if (rs.length() == 2)
            reg_rs = rs.substr(1, 1);
        else if (rs.length() == 3)
            reg_rs = rs.substr(1, 2);
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
            reg_rs1 = rs1.substr(1, 1);
        else if (rs1.length() == 3)
            reg_rs1 = rs1.substr(1, 2);
        int reg_rs1_num = stoi(reg_rs1); //第幾個reg當作rs1
        if (rs2.length() == 2)
            reg_rs2 = rs2.substr(1, 1);
        else if (rs2.length() == 3)
            reg_rs2 = rs2.substr(1, 2);
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
            reg_rd = rd.substr(1, 1);
        else if (rd.length() == 3)
            reg_rd = rd.substr(1, 2);
        int reg_rd_num = stoi(reg_rd); //第幾個reg當作rd
        RegValues[reg_rd_num] = stoi(inst[3]);
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
            reg_rd = rd.substr(1, 1);
        else if (rd.length() == 3)
            reg_rd = rd.substr(1, 2);
        int reg_rd_num = stoi(reg_rd); //第幾個reg當作rd
        if (rs1.length() == 2)
            reg_rs1 = rs1.substr(1, 1);
        else if (rs1.length() == 3)
            reg_rs1 = rs1.substr(1, 2);
        int reg_rs1_num = stoi(reg_rs1); //第幾個reg當作rs
        if (rs2.length() == 2)
            reg_rs2 = rs2.substr(1, 1);
        else if (rs2.length() == 3)
            reg_rs2 = rs2.substr(1, 2);
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
            reg_rd = rd.substr(1, 1);
        else if (rd.length() == 3)
            reg_rd = rd.substr(1, 2);
        int reg_rd_num = stoi(reg_rd); //第幾個reg當作rd
        if (rs.length() == 2)
            reg_rs = rs.substr(1, 1);
        else if (rs.length() == 3)
            reg_rs = rs.substr(1, 2);
        int reg_rs_num = stoi(reg_rs); //第幾個reg當作rs
        cout << RegValues[reg_rs_num] << " & " << imm << " = ";
        RegValues[reg_rd_num] = RegValues[reg_rs_num] & imm;
        cout << RegValues[reg_rd_num] << endl;
        // cout << RegValues[reg_rd_num] << endl;
        T = 0; // and不會跳
    }
    return T;
}
```

    
    
   
  
  
