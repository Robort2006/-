/*
     数字猜猜乐
这个程序是一个数字猜猜乐游戏，玩家需要猜一个5位数字，其中每个数字都不同。
代码由AI生成，我进行了测试调试，跟着代码一步步敲下来的，基本理解代码的逻辑。
这个还挺有意思的，益智小游戏
*/

#include <iostream>
#include <set>
#include <vector>
using namespace std;

// 函数声明
vector<int> generateRandomNumber();           // 生成5位不重复随机数
vector<int> getUserGuess();                   // 获取用户输入的5位数字
pair<int, int> checkGuess(const vector<int>& secret, const vector<int>& guess);  // 比较猜测与答案，返回A和B
void printGameState(int attempt, int maxAttempts, const vector<pair<int, int>>& history);  // 显示游戏状态
bool isValidGuess(const string& input);       // 验证输入是否为5位不重复数字
void printInstructions();                     // 打印游戏规则说明


int main() {
    srand(time(0));// 初始化随机种子

    int maxAttempts = 10;// 最大尝试次数
    int currentAttempt = 0;
    bool gameWon = false;

    // 生成随机数
    vector<int> secretNumber = generateRandomNumber();
    vector<pair<int,int>> history; // 存储每次猜测的结果 (A, B)

    cout << "? 数字猜猜乐 ?\n";
    cout << "===============\n\n";
    printInstructions();
    cout << "\n游戏开始！你有 " << maxAttempts << " 次机会。\n\n";

    // 调试用：显示答案（开发阶段可以打开查看）
    // cout << "（调试）正确答案: ";
    // for (int digit : secretNumber) cout << digit;
    // cout << "\n\n";

    // 游戏主循环
    while (currentAttempt < maxAttempts && !gameWon) {// 游戏还没结束
        currentAttempt++;// 尝试次数加1
        cout << "--- 第 " << currentAttempt << "/" << maxAttempts << " 次尝试 ---\n";

        // 获取用户输入
        vector<int> guess = getUserGuess();

        // 检查猜测结果
        pair<int, int> result = checkGuess(secretNumber, guess);
        //值为满足A/B条件的数 的数量
        int A = result.first;// 位置和数字都正确
        int B = result.second;// 数字正确但位置错误

        // 记录历史
        history.push_back(result);

        // 显示结果
        cout << "\n结果: " << A << "A" << B << "B\n";

        // 检查是否获胜
        if (A == 5) {
            gameWon=true;
            break;
        }
        cout<<endl;
    }
    // 游戏结束
    cout << "\n===============\n";
    if (gameWon) {
        cout << "? 恭喜！你在第 " << currentAttempt << " 次猜中了！\n";
    } else {
        cout << "? 游戏结束！机会用完了。\n";
        cout << "正确答案是: ";
        for (int digit : secretNumber) cout << digit;
        cout << "\n";
    }

    return 0;
}

// 生成5位不重复的随机数字（首位不为0）
vector<int> generateRandomNumber() {
    vector<int> digits(5);
    set<int> usedDigits;// 用于检查数字是否重复

    // 生成首位（1-9）
    digits[0] = rand() % 9 + 1;
    usedDigits.insert(digits[0]);

    // 生成剩余4位数字
    for (int i = 1; i < 5; i++) {
        int digit;
        do {
            digit = rand() % 10;
        } while (usedDigits.count(digit) > 0);
        digits[i] = digit;
        usedDigits.insert(digit);
    }
    return digits;
}

// 获取用户输入
vector<int> getUserGuess() {
    string input;
    bool valid = false;

    while (!valid) {
        cout << "请输入5位数字（0-9，不重复）: ";
        cin>>input;

        if (isValidGuess(input)) {
            valid = true;
        }else {
            cout << "? 输入无效！请确保输入5位不重复的数字（首位不为0）。\n";
        }
    }

    // 将字符串转换为数字数组

    vector<int> guess(5);
    for (int i = 0; i < 5; i++) {
        guess[i] = input[i] - '0';
    }

    return guess;
}

// 检查输入是否有效
bool isValidGuess(const string& input) {
    // 检查长度
    if (input.length() != 5) return false;
    // 检查是否都是数字
    for (char c : input) {
        if (!isdigit(c)) {
            return false;
        }
    }
    // 检查首位是否为0
    if (input[0] == '0') {
        return false;
    }
    // 检查是否有重复数字
    set<char> seen;
    for (char c : input) {
        if (seen.find(c) != seen.end()) {
            return false;// 重复
        }
        seen.insert(c);
    }
    return true;
}

// 检查猜测结果，返回(A, B)
pair<int, int> checkGuess(const vector<int>& secret, const vector<int>& guess) {
    int A = 0;  // 位置和数字都正确
    int B = 0;  // 数字正确但位置错误

    // 记录秘密数字和猜测数字中哪些位置已经匹配
    vector<bool> secretMatched(5, false);
    vector<bool> guessMatched(5, false);

    // 第一步：先找位置和数字都正确的（A）
    for (int i = 0; i < 5; i++) {
        if (secret[i] == guess[i]) {
            A++;
            secretMatched[i] = true;
            guessMatched[i] = true;
        }
    }
    // 第二步：找数字正确但位置错误的（B）
    for (int i = 0; i < 5; i++) {
        if (!guessMatched[i]) {// 这个位置还没匹配
            for (int j = 0; j < 5; j++) {
                // 如果找到相同数字且秘密数字的这个位置还没匹配
                if (!secretMatched[j] && secret[i] == guess[j]) {
                    B++;
                    secretMatched[j] = true;
                    break;
                }
            }
        }
    }
    return make_pair(A, B);
}

// 打印游戏状态和历史记录
void printGameState(int attempt, int maxAttempts, const vector<pair<int, int>>& history) {
    cout << "\n? 历史记录:\n";
    cout << "尝试\t猜测\t结果\n";
    cout << "----\t----\t----\n";

    for (int i = 0; i < history.size(); i++) {
        cout << "#" << (i + 1) << "\t";

        // 由于我们没有存储用户的每次猜测数字，这里只显示结果
        // 你可以修改代码来存储每次猜测的数字
        cout << "[用户猜测]\t" << history[i].first << "A" << history[i].second << "B\n";
    }

    cout << "\n剩余机会: " << (maxAttempts - attempt) << "\n";
}
// 打印游戏说明
void printInstructions() {
    cout << "? 游戏规则:\n";
    cout << "1. 程序会生成一个5位数字，每位数字不重复，首位不为0\n";
    cout << "2. 你有10次机会猜中这个数字\n";
    cout << "3. 每次猜测后，程序会给出反馈:\n";
    cout << "   - A: 位置和数字都正确的个数\n";
    cout << "   - B: 数字正确但位置错误的个数\n";
    cout << "4. 例如: 秘密数字是 12345\n";
    cout << "   - 猜测 13542 → 1A3B (1位置正确，3、4、5数字正确位置错误)\n";
    cout << "   - 猜测 54321 → 0A5B (所有数字正确但位置全错)\n";
}