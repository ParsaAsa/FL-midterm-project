#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <cmath>
#include <unordered_set>
#include <queue>
#include <unordered_map>

using namespace std;

int counter;

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
        : alphabets(a), states(s), transitions(t), start(start), finalStates(f)
    {
    }
};

// this class won't be needed since a DFA can be made by NFA class
class DFA
{
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
        else if (i.to == "epsilon")
        {
            transitions.push_back(Transition(string(1, i.from), i.to, "F"));
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

// this method will produce landa closure
vector<string> getLandaClosure(vector<Transition> transitions, string from)
{
    vector<string> landaClosure;
    landaClosure.push_back(from);
    for (Transition j : transitions)
    {
        if (j.from == from && j.by == "epsilon")
        {
            landaClosure.push_back(j.to);
        }
    }
    return landaClosure;
}

// This will remove all landa Transitions and convert nfa with landa transition to nfa without landa transition
NFA noLanda(NFA nfa)
{
    vector<string> finalStates;

    for (string i : nfa.finalStates)
    {
        finalStates.push_back(i);
    }

    for (Transition i : nfa.transitions)
    {
        for (string j : finalStates)
        {
            if (i.by == "epsilon")
            {
                if (i.to == j)
                {
                    finalStates.push_back(i.from);
                }
            }
        }
    }
    // now we should produce delta prime (for deleting landa transitions)
    vector<Transition> transitions;
    for (string i : nfa.states)
    {
        vector<string> landaClosure = getLandaClosure(nfa.transitions, i);
        for (Transition j : nfa.transitions)
        {
            if (isStringInVector(landaClosure, j.from) && !(j.by == "epsilon"))
            {
                transitions.push_back(Transition(i, j.by, j.to));
            }
        }
    }
    return NFA(nfa.alphabets, nfa.states, transitions, nfa.start, finalStates);
}

// This method will convert a NFA with no landa transition to a DFA
NFA NFAtoDFA(NFA nfa)
{
    vector<string> states;
    string start;
    vector<string> finalStates;
    vector<vector<string>> powerset;
    vector<Transition> transitions;

    // 1. Generate all possible subsets (DFA states)
    int n = nfa.states.size();
    int numberOfSubsets = pow(2, n);

    // 2. Generate subsets and identify important states
    for (int i = 1; i < numberOfSubsets; i++)
    {
        vector<string> subset;
        for (int j = 0; j < n; j++)
        {
            if (i & (1 << j))
            {
                subset.push_back(nfa.states[j]);
            }
        }
        powerset.push_back(subset);
        string stateName = "q" + to_string(i - 1);
        states.push_back(stateName);

        // Identify start state (singleton containing NFA's start)
        if (subset.size() == 1 && subset[0] == nfa.start)
        {
            start = stateName;
        }

        // Mark as final if contains any NFA final state
        for (const string &fstate : nfa.finalStates)
        {
            if (find(subset.begin(), subset.end(), fstate) != subset.end())
            {
                finalStates.push_back(stateName);
                break;
            }
        }
    }

    // 3. Build transition table with BFS from start state
    unordered_set<string> reachableStates;
    queue<string> stateQueue;
    reachableStates.insert(start);
    stateQueue.push(start);

    while (!stateQueue.empty())
    {
        string current = stateQueue.front();
        stateQueue.pop();

        // Extract current subset index
        int currentIdx = stoi(current.substr(1));
        vector<string> currentSubset = powerset[currentIdx];

        for (char symbol : nfa.alphabets)
        {
            vector<string> nextState;
            bool allHaveTransition = true;

            // Check transitions for all subset members
            for (const string &state : currentSubset)
            {
                bool hasTransition = false;
                for (const Transition &t : nfa.transitions)
                {
                    if (t.from == state && t.by[0] == symbol)
                    {
                        hasTransition = true;
                        if (find(nextState.begin(), nextState.end(), t.to) == nextState.end())
                        {
                            nextState.push_back(t.to);
                        }
                    }
                }
                if (!hasTransition)
                {
                    allHaveTransition = false;
                    break;
                }
            }

            if (allHaveTransition && !nextState.empty())
            {
                // Find corresponding DFA state
                int nextStateIdx = -1;
                for (int j = 0; j < powerset.size(); j++)
                {
                    if (powerset[j] == nextState)
                    {
                        nextStateIdx = j;
                        break;
                    }
                }

                if (nextStateIdx != -1)
                {
                    string nextStateName = "q" + to_string(nextStateIdx);
                    transitions.push_back(Transition(current, string(1, symbol), nextStateName));

                    if (reachableStates.find(nextStateName) == reachableStates.end())
                    {
                        reachableStates.insert(nextStateName);
                        stateQueue.push(nextStateName);
                    }
                }
            }
        }
    }

    // 4. Filter unreachable states
    vector<string> newStates;
    vector<string> newFinalStates;
    vector<Transition> newTransitions;

    for (const string &state : states)
    {
        if (reachableStates.count(state))
        {
            newStates.push_back(state);
            if (find(finalStates.begin(), finalStates.end(), state) != finalStates.end())
            {
                newFinalStates.push_back(state);
            }
        }
    }

    for (const Transition &t : transitions)
    {
        if (reachableStates.count(t.from) && reachableStates.count(t.to))
        {
            newTransitions.push_back(t);
        }
    }

    return NFA(nfa.alphabets, newStates, newTransitions, start, newFinalStates);
}

bool noSuchTransition(vector<Transition> t, string state, char by)
{
    for (Transition i : t)
    {
        if (i.from == state && i.by == string(1, by))
        {
            return true;
        }
    }
    return false;
}

NFA minimize(NFA nfa)
{
    unordered_set<string> liveStates;
    unordered_map<string, vector<string>> reverseTransitions;

    // Build reverse transition map
    for (const Transition &t : nfa.transitions)
    {
        reverseTransitions[t.to].push_back(t.from);
    }

    // Start from final states
    queue<string> q;
    for (const string &f : nfa.finalStates)
    {
        liveStates.insert(f);
        q.push(f);
    }

    // Reverse BFS to find live states
    while (!q.empty())
    {
        string current = q.front();
        q.pop();

        for (const string &prev : reverseTransitions[current])
        {
            if (!liveStates.count(prev))
            {
                liveStates.insert(prev);
                q.push(prev);
            }
        }
    }

    // Filter transitions, states, and final states
    vector<string> newStates;
    vector<Transition> newTransitions;
    vector<string> newFinalStates;

    for (const string &s : nfa.states)
    {
        if (liveStates.count(s))
        {
            newStates.push_back(s);
        }
    }

    for (const Transition &t : nfa.transitions)
    {
        if (liveStates.count(t.from) && liveStates.count(t.to))
        {
            newTransitions.push_back(t);
        }
    }

    for (const string &f : nfa.finalStates)
    {
        if (liveStates.count(f))
        {
            newFinalStates.push_back(f);
        }
    }

    // If the start state is not live, return empty automaton
    if (!liveStates.count(nfa.start))
    {
        return NFA(nfa.alphabets, {}, {}, nfa.start, {});
    }

    return NFA(nfa.alphabets, newStates, newTransitions, nfa.start, newFinalStates);
}

NFA complementOP(NFA nfa)
{
    vector<Transition> transitions = nfa.transitions;
    vector<string> states = nfa.states;
    unordered_set<string> allStates(states.begin(), states.end());

    string trap = "T" + to_string(counter++);
    bool trapUsed = false;

    for (const string &state : states)
    {
        for (char c : nfa.alphabets)
        {
            bool hasTransition = false;
            for (const Transition &t : transitions)
            {
                if (t.from == state && t.by == string(1, c))
                {
                    hasTransition = true;
                    break;
                }
            }
            if (!hasTransition)
            {
                transitions.push_back(Transition(state, string(1, c), trap));
                trapUsed = true;
            }
        }
    }

    if (trapUsed)
    {
        for (char c : nfa.alphabets)
        {
            transitions.push_back(Transition(trap, string(1, c), trap));
        }
        states.push_back(trap);
    }

    vector<string> finalStates;
    for (const string &state : states)
    {
        if (!isStringInVector(nfa.finalStates, state))
        {
            finalStates.push_back(state);
        }
    }

    return minimize(NFA(nfa.alphabets, states, transitions, nfa.start, finalStates));
}

NFA unionOP(vector<NFA> DFAs)
{
    string start = "q0";
    vector<string> states;
    vector<Transition> transitions;
    vector<string> finalStates;

    states.push_back(start);

    for (int i = 0; i < DFAs.size(); i++)
    {
        // Rename states by appending DFA index
        for (string s : DFAs[i].states)
        {
            string newState = s + "_" + to_string(i);
            states.push_back(newState);
        }

        for (string f : DFAs[i].finalStates)
        {
            finalStates.push_back(f + "_" + to_string(i));
        }

        // Rename transitions
        for (Transition t : DFAs[i].transitions)
        {
            string newFrom = t.from + "_" + to_string(i);
            string newTo = t.to + "_" + to_string(i);
            transitions.push_back(Transition(newFrom, t.by, newTo));
        }

        // Add landa transition from new start to each DFA's renamed start
        string renamedStart = DFAs[i].start + "_" + to_string(i);
        transitions.push_back(Transition(start, "epsilon", renamedStart));
    }

    // Create NFA with landa transitions
    return minimize(NFAtoDFA(noLanda(NFA(DFAs[0].alphabets, states, transitions, start, finalStates))));
}

NFA intersectionOP(vector<NFA> DFAs)
{
    vector<NFA> comps;
    for (NFA i : DFAs)
    {
        comps.push_back(complementOP(i));
    }

    return minimize(complementOP(unionOP(comps)));
}

NFA operationHandling(string operation, vector<NFA> DFAs)
{
    if (operation == "Complement")
    {
        return complementOP(DFAs[0]);
    }
    else if (operation == "Union")
    {
        return unionOP(DFAs);
    }
    else if (operation == "Intersection")
    {
        return intersectionOP(DFAs);
    }
    return NFA();
}

int main()
{
    vector<NFA> DFAs;
    int q, trash; // number of testcases
    string temp;
    cin >> q;
    getline(cin, temp); // ignore \n
    for (int i = 0; i < q; i++)
    { // iterate over testcases
        string operation;
        cin >> trash;       // testcase number
        getline(cin, temp); // ignore \n
        while (true)
        {
            vector<char> alphabets;
            vector<char> variables;
            char start;
            vector<Rule> rules;
            getline(cin, temp); // Grammer name(not important) or "# Operation"

            if (temp == "# Operation")
            {
                getline(cin, temp);
                NFA dfa = operationHandling(temp, DFAs);
                for (Transition i : dfa.transitions)
                {
                    cout << i.from + " " + i.by + " " + i.to << endl;
                }
                for (string i : dfa.finalStates)
                {
                    cout << i + " ";
                }
                cout << endl;
                for (string i : dfa.states)
                {
                    cout << i + " ";
                }
                cout << endl
                     << dfa.start;
                DFAs.clear();
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
                    to = "epsilon"; // Normalize to simple 'epsilon' for consistent handling
                }

                rules.push_back(Rule(from, to));
                getline(cin, temp); // ignore \n
            }
            Grammar grammar(start, alphabets, variables, rules);
            NFA nfa = RGtoNFA(grammar);
            NFA nfa2 = noLanda(nfa);
            NFA dfa1 = NFAtoDFA(nfa2);
            DFAs.push_back(dfa1);
        }
    }
}