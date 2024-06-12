#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip>
using namespace std;

#define DATA 1

map<std::string, char> registers = {
    {"$zero", '0'},
    {"$t0", '1'},
    {"$t1", '2'},
    {"$t2", '3'},
    {"$t3", '4'},
    {"$t4", '5'},
    {"$sp", '6'}};

map<char, std::string> serial = {
    {'A', "add"},
    {'B', "addi"},
    {'C', "sub"},
    {'D', "subi"},
    {'E', "and"},
    {'F', "andi"},
    {'G', "or"},
    {'H', "ori"},
    {'I', "sll"},
    {'J', "srl"},
    {'K', "nor"},
    {'L', "lw"},
    {'M', "sw"},
    {'N', "beq"},
    {'O', "bneq"},
    {'P', "j"},
};
map<std::string, char> type = {
    {"add", 'R'},  // add $rd, $rs, $rt
    {"addi", 'I'}, // add $rd, $rs, const
    {"sub", 'R'},
    {"subi", 'I'},
    {"and", 'R'},
    {"andi", 'I'},
    {"or", 'R'},
    {"ori", 'I'},
    {"sll", 'S'}, // add $rd, $rs, const
    {"srl", 'S'},
    {"nor", 'R'},
    {"lw", 'I'},
    {"sw", 'I'},
    {"beq", 'I'}, // beq $rs, $rt, label
    {"bneq", 'I'},
    {"j", 'J'}, // j label
};

map<std::string, int> label_address;

ofstream out;

string lower(string s)
{
    string ret;
    for (auto c : s)
    {
        ret += tolower(c);
    }
    return ret;
}

bool isDelim(char target, const string &delim)
{
    for (char c : delim)
    {
        if (c == target)
        {
            return true;
        }
    }
    return false;
}
vector<string> tokenize(const string &line, const string &delim = ", \t():")
{
    vector<string> tokens;
    std::string token;
    for (char c : line)
    {
        if (!isDelim(c, delim))
        {
            token += c;
        }
        else
        {
            if (!token.empty())
            {
                tokens.push_back(token);
            }
            token.clear();
        }
    }
    if (!token.empty())
    {
        tokens.push_back(token);
    }
    return tokens;
}
vector<string> tokenize(const string &line, char delim)
{
    vector<string> tokens;
    std::string token;
    std::stringstream ss(line);
    while (std::getline(ss, token, delim))
    {
        if (!token.empty())
        {
            tokens.push_back(token);
        }
    }
    return tokens;
}

string dec_to_hex(int decimal_value)
{
    stringstream ss;
    ss << hex << decimal_value;
    string res(ss.str());
    return res;
}
string dec_to_hex(string d_s)
{
    int decimal_value = stoi(d_s);
    stringstream ss;
    ss << hex << decimal_value;
    string res(ss.str());
    return res;
}

void SourceToBasic(vector<vector<string>> &ccode)
{
    cout << "\nBASIC:" << endl;
    for (int i = 0; i < ccode.size(); i++)
    {
        ccode[i][0] = lower(ccode[i][0]);
        cout << "0x" << setfill('0') << std::right << std::hex << setw(2) << (DATA * i) << ": ";
        switch (type[ccode[i][0]])
        {
        case 'R':
        {
            ccode[i][1] = registers[ccode[i][1]];
            ccode[i][2] = registers[ccode[i][2]];
            ccode[i][3] = registers[ccode[i][3]];
            cout << setfill(' ') << setw(4) << std::left << ccode[i][0] << " $" << ccode[i][1] << ", $" << ccode[i][2] << ", $" << ccode[i][3] << endl;
        }
        break;
        case 'S':
        {
            ccode[i][1] = registers[ccode[i][1]];
            ccode[i][2] = registers[ccode[i][2]];
            cout << setfill(' ') << setw(4) << std::left << ccode[i][0] << " $" << ccode[i][1] << ", $" << ccode[i][2] << ", "
                 << "0x" << setfill('0') << std::hex << setw(1) << stoi(ccode[i][3]) << endl;
        }
        break;
        case 'J':
        {
            string label = dec_to_hex(label_address[ccode[i][1]]);
            ccode[i][1] = label;
            cout << setfill(' ') << setw(4) << std::left << ccode[i][0] << " "
                 << "0x" << setfill('0') << setw(2) << ccode[i][1] << endl;
        }
        break;
        case 'I':
        {
            if (ccode[i][0] == "addi" || ccode[i][0] == "subi" || ccode[i][0] == "andi" || ccode[i][0] == "ori")
            {
                ccode[i][1] = registers[ccode[i][1]];
                ccode[i][2] = registers[ccode[i][2]];
                ccode[i][3] = dec_to_hex(ccode[i][3]);
                cout << setfill(' ') << setw(4) << std::left << ccode[i][0] << " $" << ccode[i][1] << ", $" << ccode[i][2] << ", "
                     << "0x" << setfill('0') << setw(1) << ccode[i][3] << endl;
            }
            else if (ccode[i][0] == "sw" || ccode[i][0] == "lw")
            {
                ccode[i][1] = registers[ccode[i][1]];
                ccode[i][2] = dec_to_hex(ccode[i][2]);
                ccode[i][3] = registers[ccode[i][3]];
                cout << setfill(' ') << setw(4) << std::left << ccode[i][0] << " $" << ccode[i][1] << ", "
                     << "0x" << setfill('0') << setw(1) << ccode[i][2] << "($" << ccode[i][3] << ")" << endl;
            }
            else if (ccode[i][0] == "beq" || ccode[i][0] == "bneq")
            {
                ccode[i][1] = registers[ccode[i][1]];
                ccode[i][2] = registers[ccode[i][2]];
                char label = 0;
                label = label_address[ccode[i][3]] - (i + 1);
                label = dec_to_hex(label).back();
                ccode[i][3] = label;
                cout << setfill(' ') << setw(4) << std::left << ccode[i][0] << " $" << ccode[i][1] << ", $" << ccode[i][2] << ", "
                     << "0x" << setfill('0') << setw(1) << ccode[i][3] << endl;
            }
        }
        break;
        }
    }
}

void BasicToCode(vector<vector<string>> &ccode)
{
    string sequence = "PGIOKMCBHDJLNFEA";

    map<string, char> opcode_map;
    for (int i = 0; i < sequence.size(); i++)
    {
        opcode_map[serial[sequence[i]]] = dec_to_hex(i).back();
    }
    cout << "\nCODE:" << endl;

    for (int i = 0; i < ccode.size(); i++)
    {
        char opcode = opcode_map[ccode[i][0]];
        cout << "0x" << setfill('0') << std::right << std::hex << setw(2) << (DATA * i) << ": ";
        switch (type[ccode[i][0]])
        {
        case 'R':
        {
            cout << "0x" << opcode << ccode[i][2] << ccode[i][3] << ccode[i][1] << endl;
        }
        break;

        case 'S':
        {
            cout << "0x" << opcode << ccode[i][2] << ccode[i][1] << ccode[i][3] << endl;
        }
        break;

        case 'J':
        {
            cout << "0x" << opcode << setfill('0') << setw(2) << ccode[i][1] << '0' << endl;
        }
        break;

        case 'I':
        {
            if (ccode[i][0] == "addi" || ccode[i][0] == "subi" || ccode[i][0] == "andi" || ccode[i][0] == "ori")
            {
                cout << "0x" << opcode << ccode[i][2] << ccode[i][1] << ccode[i][3] << endl;
            }
            else if (ccode[i][0] == "sw" || ccode[i][0] == "lw")
            {
                cout << "0x" << opcode << ccode[i][3] << ccode[i][1] << ccode[i][2] << endl;
            }
            else if (ccode[i][0] == "beq" || ccode[i][0] == "bneq")
            {
                cout << "0x" << opcode << ccode[i][1] << ccode[i][2] << ccode[i][3] << endl;
            }
        }
        break;

        default:
            break;
        }
    }
}
int main()
{
    ifstream in("input.asm");
    out.open("instruction.h");
    // Array of 16bit hex code

    string start, line;
    int line_count = 0;
    vector<string> code;
    vector<vector<string>> ccode;
    cout << "SOURCE:" << endl;
    while (in >> start)
    {
        getline(in, line);
        cout << start + line << endl;

        if (start.back() == ':') // Label
        {
            start.pop_back();
            label_address[start] = ccode.size();
        }
        else
        {
            line = start + line;
        }
        ccode.push_back(tokenize(line));
    }
    ofstream out("output.txt");
    SourceToBasic(ccode);
    BasicToCode(ccode);
    // for (char i = 'A'; i <= 'P'; i++)
    // {
    //     cout << opcode_map[serial[i]] << ",";
    // }
}