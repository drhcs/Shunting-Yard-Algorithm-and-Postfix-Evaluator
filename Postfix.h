#ifndef POSTFIX
#define POSTFIX

#include <iostream>
#include <stdexcept>
#include <exception>
#include <vector>
#include <string>
#include <stack>
#include <queue>

class Postfix {

public:

    static std::vector<std::string> Tokenize(std::string expression) {
        std::vector<std::string> tokens;
        std::string token;
        for (const auto & letter : expression)
        {
            if (!token.empty() && isblank(letter))
            {
                tokens.push_back(token);
                token.clear();
            }
            else
            {
                token += letter;
            }
        }

        if (!token.empty())
        {
            tokens.push_back(token);
        }

        return tokens;
    }

    static bool IsNumber(std::string token) {
        for (const auto & digit : token)
        {
            if (!isdigit(digit))
            {
                return false;
            }
        }
        return true;
    }

    static bool IsOperator(std::string op) {
        return op == "+" || op == "-" || op == "*" || op == "/";
    }

    static int Precedence(std::string op) {
        return op == "*" || op == "/" ? 1 : 0;
    }

    static std::string ShuntingYard(std::string infix) {
        std::queue<std::string> postfix;
        std::stack<std::string> ops;
        std::vector<std::string> tokens = Tokenize(infix);
        for (const auto & token : tokens)
        {
            if (IsNumber(token))
            {
                postfix.push(token);
            }
            else if (IsOperator(token))
            {
                while (!ops.empty() && IsOperator(ops.top()) && ops.top() != "(" && Precedence(ops.top()) >= Precedence(token))
                {
                    postfix.push(ops.top());
                    ops.pop();
                }
                ops.push(token);
            }
            else if (token == "(")
            {
                ops.push(token);
            }
            else if (token == ")")
            {
                while (!ops.empty() && ops.top() != "(")
                {
                    postfix.push(ops.top());
                    ops.pop();
                }

                if (ops.empty())
                {
                    throw std::invalid_argument("Mismatched parentheses.");
                }

                ops.pop();
            }
        }

        while (!ops.empty())
        {
            if (ops.top() == "(")
            {
                throw std::invalid_argument("Mismatched parentheses.");
            }
            postfix.push(ops.top());
            ops.pop();
        }

        std::string expression = postfix.front();
        postfix.pop();
        while (!postfix.empty())
        {
            expression += " " + postfix.front();
            postfix.pop();
        }

        return expression;
    }

    static int EvaluatePostfix(std::string postfix) {
        std::vector<std::string> tokens = Tokenize(postfix);
        std::stack<int> ops;
        for (const auto & token : tokens)
        {
            if (IsOperator(token))
            {
                if (ops.size() < 2)
                {
                    throw std::invalid_argument("Too few operands.");
                }

                int right_val = ops.top();
                ops.pop();

                int left_val = ops.top();
                ops.pop();

                char op = token[0];
                switch (op)
                {
                    case '+':
                    {
                        ops.push(left_val + right_val);
                        break;
                    }
                    case '-':
                    {
                        ops.push(left_val - right_val);
                        break;
                    }
                    case '*':
                    {
                        ops.push(left_val * right_val);
                        break;
                    }
                    case '/':
                    {
                        if (right_val == 0)
                        {
                            throw std::runtime_error("Division by zero.");
                        }

                        ops.push(left_val / right_val);
                        break;
                    }
                }
            }
            else
            {
                int val = std::stoi(token);
                ops.push(val);
            }
        }

        if (ops.size() != 1)
        {
            throw std::invalid_argument("Too few operators.");
        }

        return ops.top();
    }

};

#endif
