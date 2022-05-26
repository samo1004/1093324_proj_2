# 1093324_proj_2

## 程式 :
   
   輸入內容為組合語言的文件，執行過程中預測當前instruction會不會branch，並將預測過程和結果輸出檢查

## 執行程式 :

   將三個檔案放到同個資料夾，分別為程式碼(1093324_proj_2.cpp)、輸入文件(input.txt)及輸出文件(output.txt)，執行程式後，輸入entry數目，程式就會將結果輸出到output.txt中
   
## 程式碼解說 :
   
   先將文件讀入並輸入entry數後，
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
   
  
  
