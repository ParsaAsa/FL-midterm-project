#include <iostream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

// Class declarations

class Rule
{
public:
    // Attributes
    char from;
    string to;

    // Constructor
    Rule(char a, string b)
    {
        from = a;
        to = b;
    }
};

class Transition
{
public:
    // Attributes
    char from;
    char by;
    string to;

    // Constructor
    Transition(char a, char b, string c)
    {
        from = a;
        by = b;
        to = c;
    }
};

class Grammar
{
public:
    // Attributes
    const vector<Rule> rules;
    const vector<char> alphabets;
    const vector<char> variables;
    char start;
    // Constructor
    Grammar(char s, vector<char> a, vector<char> v, vector<Rule> r) : alphabets(a), variables(v), rules(r)
    {
        start = s;
    }
};
class NFA
{
public:
    NFA() = default;
};
class DFA
{
public:
    DFA() = default;
};

NFA RGToNFA(Grammar g)
{
    return NFA();
}

DFA NFAtoDFA(NFA f)
{
    return DFA();
}

DFA complementOP(DFA g)
{
    return DFA();
}

DFA unionOP(DFA g1, DFA g2)
{
    return DFA();
}

DFA intersectionOP(DFA g1, DFA g2)
{
    return DFA();
}

DFA operationHandling(string operation, DFA f1, DFA f2)
{
    if (operation == "Complement")
    {
        return complementOP(f1);
    }
    else if (operation == "Union")
    {
        return unionOP(f1, f2);
    }
    else if (operation == "Intersection")
    {
        return intersectionOP(f1, f2);
    }
    return DFA();
}

int main()
{
    int q; // number of testcases
    string temp;
    cin >> q;
    getline(cin, temp); // ignore \n
    for (int i = 0; i < q; i++)
    { // iterate over testcases
        vector<Grammar> grammars;
        vector<NFA> NFAs;
        vector<DFA> DFAs;
        string operation;
        getline(cin, temp); // testcase number
        while (true)
        {
            vector<char> alphabets;
            vector<char> variables;
            char start;
            getline(cin, temp); // Grammer name(not important) or "# Operation"

            if (temp == "# Operation")
            {
                break; // end the process of taking Grammars and go to operation phase
            }
            getline(cin, temp); // # Alphabet

            getline(cin, temp); // Read entire line

            istringstream iss(temp); // Create string stream from temp
            char ch;

            while (iss >> ch)
            { // Extract alphabets one by one
                alphabets.push_back(ch);
            }
            getline(cin, temp); // # Variables

            getline(cin, temp); // Read entire line

            istringstream iss2(temp); // Create string stream from temp

            while (iss2 >> ch)
            { // Extract variables one by one
                variables.push_back(ch);
            }

            getline(cin, temp); // # Start
            cin >> start;
            getline(cin, temp); // ignore \n

            getline(cin, temp); //=========
        }
    }
}