#include <iostream>
#include <cassert> 
#include <string>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

static string opers = "+-*/";
static string peren = "()";

class Solver {
    
    queue<string> oqu;
    stack<string> ost;
    vector<string> tokens;
    string expr; 

    // Determining precedence 
    int pre(string operStr) {
        
        char oper = operStr[0];
        switch (oper) {
            case '+':
                return 1;
            case '-':
                return 1;
            case '*':
                return 2;
            case '/':
                return 2;
            default:
                 throw invalid_argument("Invalid Operator");
                 exit(-1);
        }
    }

    // Making operation
    float operate(float a, float b, char oper) {
        
        switch (oper) {

            case '+':
                return a+b;
            case '-':
                return a-b;
            case '*':
                return a*b;
            case '/':
                if (b == 0) { cout << "Error: Division by 0" << endl; exit(-1); }
                return a/b;
            default:
                cout << "Error in operate(int a, int b, char oper) in line " << __LINE__ << endl;
                exit(-1);
        }
    }
 
public:

    Solver(string input) { expr = input; }

    // RPN Evaluation 
    float evalPostfix(void) {
        float a, b, temp;
        stack<float> s;

        while (!oqu.empty()) {
        // While tokens in reverse polish notation, if theres an operator, 2 elements (a,b) from stack are operated on
        // Then, that result is pushed onto the stack once more.
            if (opers.rfind(oqu.front()) != string::npos) {
                a = s.top();
                s.pop();
                b = s.top();
                s.pop();
                s.push(operate(b, a, oqu.front()[0]));

        // If it's a number, it's pushed up to the stack
            } else if ((temp = stoi(oqu.front()))) {
                s.push(temp);
            } else {
                cout << "Error in evalPostfix(void)" << endl;
                exit(-1);
            }
        // Finally, we pop the oper/num from queue
            oqu.pop();
        }
        // Finally Finally, we return the result, that was pushed ontop of stack in line 75 
        return s.top();
    }

    void parseTokens(void) {
        string token;
        string::size_type i = 0;
        // Loops through chars in std::string, if number of mult digits, each char pushed to token, then to vector<string>
        while (i < expr.length()) {

            // Skips over whitespace
            while (isspace(expr[i])) { i++; }

            // Checks whether operators are next to one another, (Invalid Expression Error) 
            if ((opers.rfind(expr[i]) != string::npos) && (opers.rfind(expr[i+1]) != string::npos)) {
                cout << "Error: Invalid Expression" << endl;
                exit(-1);
            }

            if (isdigit(expr[i])){
                while (isdigit(expr[i])) {
                    token += expr[i];
                    i++;
                }
                tokens.push_back(token);
                token.clear();
            } else if (opers.rfind(expr[i]) != string::npos) {
                token += expr[i];
                tokens.push_back(token);
                token.clear();
                i++;
            } else if (peren.rfind(expr[i]) != string::npos) {
                token += expr[i];
                tokens.push_back(token);
                token.clear();
                i++;
            } else {
                cout << "Error: Invalid Expression" << endl;
                exit(-1);
            }
        }

        // Checks if number of left and right parenthesis are equal
        int lpar = 0;
        int rpar = 0;
        for (auto token: tokens) {
            if (token == "(") { lpar++; }
            if (token == ")") { rpar++; }

        }
        if (lpar != rpar) {
            cout << "Error: Parenthesis do not match" << endl;
            exit(-1);
        }
    }

    void printTokens(void) {
        for (auto i: tokens) {
            cout << i << "  ";
        }
        cout << endl;
    }
   
    void qprint() {
        
        queue<string> copyQueue = oqu;

        while (!copyQueue.empty()) {
            cout << copyQueue.front() << "  ";
            copyQueue.pop();
        }
        cout << endl;
    }

   void shuntingYard(void) {
        for (auto token: tokens) {
            if (isdigit(token[0])) {
                oqu.push(token);
            } else if (opers.rfind(token) != string::npos) {

                // Seg fault 11 occuring in while loop
                while (!ost.empty() && ((ost.top() != "(") && ((pre(ost.top()) > pre(token)) || ((pre(ost.top()) == pre(token)) && (opers.rfind(token) != string::npos))))) {
                    string temp = ost.top();
                    oqu.push(temp);
                    ost.pop();
                } 
                ost.push(token);
            } else if (token == "(") {
                ost.push(token);
            } else if (token == ")") {
                while (ost.top() != "(") {
                    assert(!ost.empty());
                    string temp = ost.top();
                    oqu.push(temp);
                    ost.pop();
                }
                assert(ost.top() == "(");
                ost.pop();
                // for function implementation i'd add another if else statment
            } else {
                cout << "Something went wrong around line: " << __LINE__ << endl;
                exit(-1);
            }
        }
        while (!ost.empty()) {
            assert(ost.top() != "(");
            string temp = ost.top();
            oqu.push(temp);
            ost.pop();
        }
    }
};


int main(void) {
    string input;
    cout << "Input an expression: ";
    cin >> input;

    Solver ob(input);
    ob.parseTokens(); 

    cout << "Here are the tokens once parsed" << endl;
    ob.printTokens();

    cout << endl;
    cout << "Applying shuntingYard()" << endl;
    ob.shuntingYard();

    cout << "Shunting yard has been applied, new expression is below" << endl;
    ob.qprint();

    cout << endl;
    cout << "Below is the value of the input expression " << endl;
    cout << ob.evalPostfix() << endl;
    return 0;
}
