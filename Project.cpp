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

// Determine whether a character is in a vector or not
bool isCharInVector(const vector<char> &vec, char target)
{
    return find(vec.begin(), vec.end(), target) != vec.end();
}

// Determine whether a string is in a vector or not
bool isStringInVector(vector<string> &vec, const string &target)
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
    string by;
    string to;

    // Constructor
    Transition(string a, string b, string c)
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
    vector<string> finalStates;

    NFA() = default;
    NFA(vector<char> a, vector<string> s, vector<Transition> t, string start, vector<string> f)
        : alphabets(a), states(s), transitions(t), finalStates(f)
    {
        start = start;
    }
};
class DFA
{
public:
    DFA() = default;
};

// converting a regular grammar to a Non-deterministic Finite Automata
NFA RGtoNFA(Grammar g)
{
    vector<Transition> transitions;
    vector<string> finalStates;
    vector<string> states;
    vector<char> alphabets;
    string start = string(1, g.start);
    bool p = isLeftLinear(g.rules, g.variables);

    // Adding grammar alphabets to automata alphabets
    for (char i : g.alphabets)
    {
        alphabets.push_back(i);
    }

    // Adding grammar variables to automata states
    for (char i : g.variables)
    {
        states.push_back(string(1, i));
    }
    states.push_back("F");

    finalStates.push_back("F");

    string currentState;

    for (Rule i : g.rules)
    {
        currentState = string(1, i.from);
        if (i.to.size() == 1)
        {
            transitions.push_back(Transition(string(1, i.from), string(1, i.to[0]), "F"));
        }
        else
        {
            int index;
            if (p)
            {
                index = i.to.size() - 1;
                while (index > 0)
                {
                    if (isCharInVector(g.variables, i.to[index - 1]))
                    {
                        transitions.push_back(Transition(currentState, string(1, i.to[index]), string(1, i.to[index - 1])));
                        currentState = string(1, i.to[index - 1]);
                    }
                    else
                    {
                        transitions.push_back(Transition(currentState, string(1, i.to[index]), "State" + to_string(states.size() + 1)));
                        states.push_back("State" + to_string(states.size() + 1));
                        currentState = "State" + to_string(states.size());
                    }

                    index--;
                }
                if (isCharInVector(g.variables, i.to[0]))
                {
                    // nothing to do
                }
                else
                {
                    transitions.push_back(Transition(currentState, string(1, i.to[1]), "F"));
                }
            }
            else
            {
                index = 0;
                while (index < i.to.size() - 1)
                {
                    if (isCharInVector(g.variables, i.to[index + 1]))
                    {
                        transitions.push_back(Transition(currentState, string(1, i.to[index]), string(1, i.to[index + 1])));
                        currentState = string(1, i.to[index + 1]);
                    }
                    else
                    {
                        transitions.push_back(Transition(currentState, string(1, i.to[index]), "State" + to_string(states.size() + 1)));
                        states.push_back("State" + to_string(states.size() + 1));
                        currentState = "State" + to_string(states.size());
                    }

                    index++;
                }
                if (isCharInVector(g.variables, i.to[i.to.size() - 1]))
                {
                    // nothing to do
                }
                else
                {
                    transitions.push_back(Transition(currentState, string(1, i.to[i.to.size() - 2]), "F"));
                }
            }
        }
    }
    return NFA(alphabets, states, transitions, start, finalStates);
}

// This will remove all landa Transitions and convert nfa with landa transition to nfa without landa transition
NFA noLanda(NFA nfa)
{
    vector<string> finalStates;

    finalStates.push_back("F");
    for (Transition i : nfa.transitions)
    {

        if (i.by == "e")
        {
            if (i.to == "F")
            {
                finalStates.push_back(i.from);
            }
        }
    }
    // this method is not finnished yet next commit it will be completed
    return NFA(nfa.alphabets, nfa.states, nfa.transitions, nfa.start, finalStates);
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

                if (to.size() == 1 && static_cast<unsigned char>(to[0]) == 0xEE)
                {
                    to = "e"; // Normalize to simple 'e' for consistent handling
                }

                rules.push_back(Rule(from, to));
                getline(cin, temp); // ignore \n
            }
            Grammar grammar(start, alphabets, variables, rules);
            NFA nfa = RGtoNFA(grammar);
            NFA nfa2 = noLanda(nfa);

            for (string i : nfa2.finalStates)
            {
                cout << i << endl;
            }
        }
    }
}