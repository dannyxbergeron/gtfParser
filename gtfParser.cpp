// #include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <string_view>
#include <stdio.h>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <sys/stat.h>

#include "robin_hood.h"

using namespace std;

vector<string> ATTRIBUTES{"gene_id", "gene_name", "gene_biotype",
                                "transcript_id", "transcript_name",
                                "transcript_support_level",
                                "exon_id", "exon_number",
                                "protein_id", "protein_version"};

vector<string> HEADER{"seqname", "source", "feature", "start", "end",
                        "score", "strand", "frame"};

string HELP = "This script is intended to parse a gtf file into a tsv format.\n"
              "You can either use it like this:\n\n\tgtfParser gtf_file.gtf\n\nThis will "
              "give you the parsed output with the base attributes which are listed "
              "at the bottom, or you can use your own list of attributes "
              "and use it like this:\n\n\tgtfParser parse_gtf gtf_file.gtf attributes.txt\n\n"
              "The attributes should have the same name in the gtf file "
              "and be one per line in the attribute file.\nThe order will "
              "be the one that you define in the attribute file.\nIf you "
              "want to find the list of all attributes in your gtf file, "
              "you can find them by using:\n\n\tgtfParser find_attributes gtf_file.gtf\n\n"
              "Or finally, you can find all attributes in your gtf file and "
              "parse it in one motion with:\n\n\tgtfParser find_parse gtf_file.gtf\n";

int main(int argc, char *argv[])

{
    void printHelp();
    void readAttributes(const string&);
    void readFile(const string&);
    void findAttributes(const string&, bool);

    char* instr = argv[1];
    // Display the help
    if (strcmp(instr,"--help") == 0) printHelp();
    else if (argc == 2)  readFile(argv[1]); // if no attribute file
    else if(argc == 4)
    {
        if (strcmp(instr,"parse_gtf") == 0) // parse the gtf with a attribute file
        {
            readAttributes(argv[3]);
            readFile(argv[2]);
        }
        else cout << "Non valid option !\n";
    }
    else if(argc == 3)
    {
        if(strcmp(instr,"find_attributes") == 0) // find attributes
        {
            findAttributes(argv[2], false);
        }
        else if(strcmp(instr,"find_parse") == 0) // find attributes
        {
            findAttributes(argv[2], true);
            readFile(argv[2]);
        }
        else cout << "Non valid option !\n";
    }
    else cout << "Non valid option !\n";

    return 0;

}

void printHelp()
{
    // print help string
    cout << HELP << endl;
    // print the base attributes
    cout << "Base attributes:\n";
    for(auto& a: ATTRIBUTES) cout << "\t" << a << "\n";
}

void readAttributes(const string& attFile)
{
    // Make sure that the file exist
    struct stat buffer;
    if((stat (attFile.c_str(), &buffer) == 0) == 0)
    {
        cout << "File does not exist !" << endl;
        exit(0);
    }

    // Declare the variables
    vector<string> newAttributes;
    ifstream fileStream(attFile);
    string line;

    // go line by line and extract the features
    while(getline(fileStream, line))
    {
        newAttributes.push_back(line);
    }

    // set the new attributes
    ATTRIBUTES = newAttributes;
}

void readFile(const string& file)
{
    void printHeader();

    // Make sure that the file exist
    struct stat buffer;
    if((stat (file.c_str(), &buffer) == 0) == 0)
    {
        cout << "File does not exist !" << endl;
        exit(0);
    }

    // print the header of the file
    printHeader();

    // Declare the variables
    ifstream fileStream(file);
    char _buffer[1024];
    fileStream.rdbuf()->pubsetbuf(_buffer, 16184);
    string line, attName, att;
    robin_hood::unordered_map<string, string> attributes;

    // initiate the position integers
    size_t found;
    size_t start;
    string_view sv;

    // go through the file line by line
    while(getline(fileStream, line))
    {
        sv = line;
        if(line[0] == '#') continue; // CHANGED
        start = 0; // reinitialize the first position
        found = 0;
        // go directly to the last column
        for(int i=0;i<8;i++)
        {
            found = line.find("\t", start+1);
            start = found + 1;
        }
        cout << sv.substr(0, found + 1);

        // find the position of the space affter the attribute name
        found = line.find(' ', start + 1);
        // iterate throught the rest of the column
        while(found != string::npos)
        {
            // get the attribute name
            attName = line.substr(start, found - start);

            // get the att
            start = line.find('\"', found+1) + 1;
            found = line.find('\"', start);
            att = line.substr(start, found - start);

            // skip the ; and the space
            start = line.find(';', found+1) + 2;
            found = line.find(' ', start);
            // cout << attName << "|" << att << endl;

            // put the attribute name and attribut in a map
            if(att == "") att = "NA"; // if the attribute is empty, put NA instead
            // if there is already an attribute for this column, append to it
            if(attributes.find(attName) != attributes.end())
                att = attributes[attName] + "|" + att;
            attributes[attName] = att;
        }

        // print all the attributes in the right order if they are present
        for (auto& x : ATTRIBUTES)
        {
            if(attributes.find(x) != attributes.end())
            {
                cout << attributes.at(x) << "\t";
            }
            else
            {
                cout << "NA\t";
            }
        }
        cout << endl;
        // reinitialize the map for each line
        attributes.clear();
    }
}

void printHeader()
{
    for(auto field: HEADER) cout << field << "\t";
    for(auto att: ATTRIBUTES) cout << att << "\t";
    cout << endl;
}

void findAttributes(const string& file, bool parse)
{
    // Make sure that the file exist
    struct stat buffer;
    if((stat (file.c_str(), &buffer) == 0) == 0)
    {
        cout << "File does not exist !" << endl;
        exit(0);
    }

    // Declare the variables
    ifstream fileStream(file);
    char _buffer[1024];
    fileStream.rdbuf()->pubsetbuf(_buffer, 16184);
    string line;
    unordered_set<string> allAtt;
    int i = 0;

    // initiate the position integers
    size_t found;
    size_t start;

    // iterate through each line
    while(getline(fileStream, line))
    {
        if(line[0] == '#') continue; // CHANGED
        start = 0; // reinitialize the first position
        // go directly to the last column
        for(int i=0;i<8;i++) start = line.find("\t", start+1);

        start++; // remove the \t

        // find the position of the space affter the attribute name
        found = line.find(' ', start + 1);
        // iterate throught the rest of the column
        while(found != string::npos)
        {
            // insert the attribute name substring in a set
            allAtt.insert(line.substr(start, found - start));
            if(line.substr(start, found - start) == "The") cout << line << '\n';
            // Skip the attribute value
            found = line.find('\"', found+1);
            found = line.find('\"', found+1);
            // skip the ; and the space
            start = line.find(';', found+1) + 2;
            found = line.find(' ', start+1);
        }
    }

    if(parse)
    {
            ATTRIBUTES.clear();
            for(auto a: allAtt)
            {
                ATTRIBUTES.push_back(a);
            }
    }
    else
    {
        // print all the attribute names
        for(auto a: allAtt) cout << a << endl;
    }
}
