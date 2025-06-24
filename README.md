# 簡易線上評測系統 (Simple Online Judge)
>Developed by 蘇俊誠  
  
- 一個以 C++ 開發的命令列版 Online Judge，使用者可以註冊／登入、瀏覽題目、每日推題、提交程式並自動批改。  
- 管理者可以新增題目和執行所有使用者的功能。

---

## 💻 主要功能

1. **註冊 / 登入**
   
3. **查詢當前使用者** （選項 1）  
4. **查詢系統版本** （選項 2）  
5. **列出所有題目** （選項 3）  
6. **每日推薦題目** （選項 4）  
7. **提交程式碼批改** （選項 5）  
   - Compiler Error  
   - Runtime Error  
   - Wrong Answer  
   - Accepted  
8. **新增題目（僅限管理員）** （選項 6）  
9. **結束程式** （選項 7）  

---


## 🔑 註冊與登入流程

- **首次使用**  
  選擇「-1」進入註冊流程，輸入帳號、密碼（需重複確認），系統儲存至 `user.csv`。

- **登入**  
  輸入已註冊帳號，輸入密碼（最多三次失敗），成功後顯示  
  `Login Success!!! welcome aboard`

- **登入後**  
  顯示主選單（1–7），可自由使用各項功能。

---

## 📝 題目格式

`problem.csv` 每行為：  
- `magic_number` 可作為「每日推薦」演算法的種子。

  **範例**  
  ```csv
  A+B,./data/problem/testcase/A+B/input.txt,./data/problem/testcase/A+B/ans.txt,10
  GCD,./data/problem/testcase/GCD/input.txt,./data/problem/testcase/GCD/ans.txt,20

---

##  🔁每日推薦題目（選項 4)  
以「今日日期 + magic number」做雜湊排序，穩定選出當日題目

顯示題目標題、簡短說明題目（A+B、A-B、GCD、LCM），提示使用者可用選項 5 提交。

---

##  🚀 提交程式與批改（選項 5）
- 輸入題目 ID

- 輸入程式檔案路徑

- 編譯

- 回傳
  - Compiler Error  
   - Runtime Error  
   - Wrong Answer  
   - Accepted

---

## ⚙️ 新增題目（選項 6，限 admin）
- 輸入完整的 Input 檔路徑  

- 輸入完整的 Output 檔路徑  

- 輸入 Magic Number  

- 輸入 題目標題  

- 系統寫入 problem.csv
