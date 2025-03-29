#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>

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

// Determine whether a char is in a vector or not
bool isCharInVector(const vector<char> &vec, char target)
{
    return find(vec.begin(), vec.end(), target) != vec.end();
}

// Determine whether a Grammar is Left Linear or right linear
bool isLeftLinear(vector<Rule> rules, vector<char> variables)
{
    for (Rule i : rules)
    {
        if (isCharInVector(variables, i.to[0]))
        {
            return 1;
        }
    }
    return 0;
}

class Transition
{
public:
    // Attributes
    string from;
    char by;
    string to;

    // Constructor
    Transition(string a, char b, string c)
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
    vector<string> states;
    vector<Transition> transitions;
    vector<char> alphabets;
    string start;
    string finalState;

    NFA() = default;
    NFA(vector<char> a, vector<string> s, vector<Transition> t, string start, string f)
        : alphabets(a), states(s), transitions(t)
    {
        start = start;
        finalState = f;
    }
};
class DFA
{
public:
    DFA() = default;
};

// converting a regular grammar to a Non-deterministic Finite Automata
NFA RGToNFA(Grammar g)
{
    vector<Transition> transitions;
    string finalState;
    vector<string> states;
    vector<char> alphabets;

    bool p = isLeftLinear(g.rules, g.variables);

    for (char i : g.variables)
    {
        states.push_back(string(1, i));
    }
    states.push_back("F");
    finalState = "F";

    for (Rule i : g.rules)
    {
        if (i.to.size() == 1)
        {
            transitions.push_back(Transition(string(1, i.from), i.to[0], "F"));
        }
        else
        {
            int index;
            if (p)
            {
                index = i.to.size() - 1;
                while (index > 0)
                {
                }
                if (isCharInVector(g.variables, i.to[0]))
                {
                }
                else
                {
                }
            }
            else
            {
                while (index < i.to.size() - 1)
                {
                }
                if (isCharInVector(g.variables, i.to[i.to.size() - 1]))
                {
                }
                else
                {
                }
            }
        }
    }
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
            vector<Rule> rules;
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

            iss.clear();
            iss.str(temp); // Create string stream from temp

            while (iss >> ch)
            { // Extract variables one by one
                variables.push_back(ch);
            }

            getline(cin, temp); // # Start
            cin >> start;
            getline(cin, temp); // ignore \n
            getline(cin, temp); // ignore "# Rules"
            while (true)
            {
                char from;
                cin >> from;
                if (from == '=')
                {
                    getline(cin, temp);
                    break;
                }
                string to;
                cin >> temp; // ignore '->' sign
                cin >> to;
                rules.push_back(Rule(from, to));
                getline(cin, temp); // ignore \n
            }
            Grammar grammar(start, alphabets, variables, rules);
        }
    }
}