#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>

using namespace std;

class ProcessAskiiDoc
{
private:
    vector<string> lines;
    vector<string> headers;
    vector<string> urls;
    vector<string> paragraphs;

    regex headerPattern{ R"(^=+\s+(.+)$)" };
    regex urlPattern{ R"(https?://[a-zA-Z0-9-]+(\.[a-zA-Z0-9-]+)+([/?#][^\s\[]*)?)" };
    regex paragraphPattern{ R"(^[^\s=*+-.|>[].*)" };
    regex paragraphpPatternIV{ R"(^\s*(?!image::|video::).*)" };
    regex codeBlockPattern{ R"(^(----|....)$)" };
    bool codeBlock = false;
public:
    ProcessAskiiDoc(const string& fileName)
    {
        ifstream inputFile(fileName);
        if (!inputFile)
        {
            cout << "Error! Åhe file could not be opened!" << endl;
            return;
        }
        string line;

        while (getline(inputFile, line))
            lines.push_back(line);

        inputFile.close();
    }
    
    vector<string> getHeaders()
    {
        smatch matches;

        for (const auto& line : lines)
            if (regex_match(line, matches, headerPattern) && matches.size() > 1)
                headers.push_back(matches[1]);

        return headers;
    }

    vector<string> getURLs()
    {
        smatch matches;

        for (const auto& line : lines)
        {
            auto searchStart = line.cbegin();

            while (regex_search(searchStart, line.cend(), matches, urlPattern))
            {
                urls.push_back(matches[0]);
                searchStart = matches.suffix().first;
            }
        }

        return urls;
    }
    
    vector<string> getParagraphs()
    {
        for (const auto& line : lines)
        {
            if (regex_match(line, codeBlockPattern))
            {
                if (codeBlock == false)
                    codeBlock = true;
                else
                    codeBlock = false;
                continue;
            }

            if (codeBlock)
                continue;
                
            if (regex_match(line, paragraphPattern) && regex_match(line, paragraphpPatternIV))
                paragraphs.push_back(line);
        }

        return paragraphs;
    }

    void printHeaders()
    {
        getHeaders();

        for (auto it = headers.begin(); it != headers.end(); ++it)
            cout << "h: " << *it << endl;
    }

    void printURLs()
    {
        getURLs();

        for (auto it = urls.begin(); it != urls.end(); ++it)
            cout << "URL: " << *it << endl;
    }

    void printParagraphs()
    {
        getParagraphs();

        for (auto it = paragraphs.begin(); it != paragraphs.end(); ++it)
            cout << "p: " << *it << endl;
    }
};  



int main(int argc, char* argv[]) {
    if (argc < 2)
    {
        cout << "Error: No filename provided!" << endl;
        return 1;
    }
    ProcessAskiiDoc file(argv[1]);
   
    file.printHeaders();
    file.printURLs();
    file.printParagraphs();

    return 0;
}