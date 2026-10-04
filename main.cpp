//这是自己的用栈写法

class Solution {
public:
    bool checkValidString(string s) {
        if (s.empty()) return true;
        if(s[0]==')'||s[s.size()-1]=='(')
        return false;
        stack<int> st;
        stack<int> st_xing;
        int i=0;
        for(char ch:s)
        {
            if(ch=='(')
            {
            st.push(i);
            i++;
            }
            else if(ch=='*')
            {
               st_xing.push(i);
               i++; 
            }
            else if(ch==')')
            {
                if(st.empty()||s[st.top()]==')')
                {
                    st.push(i);
                    i++;//这里当时忘记改变i的值了
                }
                else{
                    st.pop();
                    i++;
                }
            }
        }
        while(!st.empty()&&!st_xing.empty())
        {
            if(s[st.top()]=='(')
            {
                if(st.top()<st_xing.top())
                {
                    st.pop();
                    st_xing.pop();
                }
                else return false;
            }
            else{//这里整个逻辑是有问题的，栈顶的可能匹配不了，但还有下面的
                while(!st_xing.empty()&&st.top()<st_xing.top())
                {
                    st_xing.pop();
                }
                if(st_xing.empty())
                return false;
                st.pop();
                st_xing.pop();
            }
        }
        bool a=st.empty();
        if(a)
        {
            return true;
        }
        else return false;
        
    }
};



//方法二（dp算法）
//笔记：先思考总状态，每个格子是什么状态，每一部为之后的状态有什么影响
class Solution {
public:
    bool checkValidString(string s) {
        int n = s.size();
        vector<vector<bool>> dp(n + 1, vector<bool>(n + 1, false));
        dp[0][0] = true;

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j <= n; ++j) {
                if (!dp[i][j]) continue;
                char c = s[i];
                if (c == '(') {
                    if (j + 1 <= n) dp[i + 1][j + 1] = true;
                } else if (c == ')') {
                    if (j - 1 >= 0) dp[i + 1][j - 1] = true;
                } else {
                    if (j + 1 <= n) dp[i + 1][j + 1] = true;
                    if (j - 1 >= 0) dp[i + 1][j - 1] = true;
                    dp[i + 1][j] = true;
                }
            }
        }
        return dp[n][0];
    }
};
// 二维 DP 详解
// 一、状态定义
// dp[i][j] 表示：只考虑字符串前 i 个字符（即 s[0..i-1]），是否存在一种把 * 解释成 (、) 或空串的方案，使得处理完后剩下 j 个未匹配的左括号 (。

// 换句话说：前 i 个字符能否被解释成「合法前缀 + 恰好 j 个还没配对的 (」。

// 二、为什么用“未匹配左括号个数”作为状态？
// 括号匹配问题的核心是：从左到右扫描时，未匹配的 ( 数量不能为负，且最终必须为 0。

// 遇到 ( → 未匹配数 +1

// 遇到 ) → 未匹配数 -1（前提是不能 < 0）

// 遇到 * → 三种可能，未匹配数 +1 / -1 / 不变

// 所以只要跟踪 j = 当前未匹配(数量，就能完整描述状态。

// 三、初始化
// cpp
// dp[0][0] = true;   // 前 0 个字符，0 个未匹配 '(' ，显然成立
// 其他 dp[0][j] = false：空串不可能有未匹配括号。

// 四、状态转移
// 遍历每个字符 c = s[i]，对每个可能的 j，从 dp[i][j] 推出 dp[i+1][*]：

// 情况 1：c == '('
// 只能让未匹配数 +1：

// cpp
// if (dp[i][j]) dp[i+1][j+1] = true;
// 情况 2：c == ')'
// 只能让未匹配数 -1（且 j-1 >= 0，否则会变成负数，非法）：

// cpp
// if (dp[i][j] && j-1 >= 0) dp[i+1][j-1] = true;
// 情况 3：c == '*'
// 三种选择，任一可达即可：

// cpp
// if (dp[i][j]) {
//     dp[i+1][j+1] = true;                       // '*' 当 '('
//     if (j-1 >= 0) dp[i+1][j-1] = true;         // '*' 当 ')'
//     dp[i+1][j]   = true;                       // '*' 当空串
// }
// 五、答案
// 处理完所有 n 个字符后，看 dp[n][0]：

// true → 整个字符串能被解释成合法括号串（最终 0 个未匹配）

// false → 无法做到

// cpp
// return dp[n][0];
// 六、走个小例子：s = "(*)"
// n = 3，dp 是 4 × 4 的表，只列出 j = 0..3。


//解法三：贪心算法
class Solution {
public:
    bool checkValidString(string s) {
        int lo = 0, hi = 0;
        for (char c : s) {
            if (c == '(') {
                lo++; hi++;
            } else if (c == ')') {
                lo--; hi--;
            } else { // '*'
                lo--; hi++;
            }
            if (hi < 0) return false;   // 全当 '(' 也不够
            lo = max(lo, 0);            // '*' 可以当空串
        }
        return lo == 0;
    }
};
// 思路
// 维护当前未匹配左括号 ( 的可能数量范围 [lo, hi]：

// 遇到 (：lo++, hi++

// 遇到 )：lo--, hi--

// 遇到 *：

// 当右括号用 → lo--

// 当左括号用 → hi++

// 当空串用 → 不影响

// 过程中：

// 如果 hi < 0，说明即使把所有 * 都当 ( 也不够匹配，直接 false

// 如果 lo < 0，说明 * 当 ) 太多，但 * 可以当空串，所以把 lo 拉回 0

// 最后只要 lo == 0，说明存在一种方案让所有括号匹配。