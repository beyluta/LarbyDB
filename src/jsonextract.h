#pragma once
#include <iostream>
#include <vector>
using namespace std;

/*
    This method separates all objects in a JSON string into their own string
*/
vector<string> SplitJSONStringObjects(string json)
{
    vector<string> objects;
    string currentObject = "";
    bool quoteEnclosed = false;
    for (int i = 0; i < json.size(); i++)
    {
        currentObject += json[i];
        if (json[i] == '"' && !quoteEnclosed)
            quoteEnclosed = true;
        else if (json[i] == '"' && quoteEnclosed)
            quoteEnclosed = false;
        else if (!quoteEnclosed && (json[i] == ',' || json.size() - 1 == i))
        {
            objects.push_back(currentObject);
            currentObject = "";
        }
    }
    return objects;
}

/*
    This method extracts JSON data by passing the json string and the desired field
*/
string GetJSONFieldValues(string json, string field)
{
    bool quoteEnclosed = false;  // Is the current substring enclosed in quotes?
    bool braceEnclosed = false;  // Is the current substring enclosed in braces?
    bool nextFieldValue = false; // Is the current substring a field value?
    bool fieldFound = false;     // Has the desired field been found?
    string currentField = "";
    string currentValue = "";

    for (int i = 0; i < json.size(); i++)
    {
        if (json[i] == '"' && !quoteEnclosed && !braceEnclosed)
        {
            quoteEnclosed = true;
        }
        else if (json[i] == '"' && quoteEnclosed && !braceEnclosed)
        {
            quoteEnclosed = false;
        }
        else if (json[i] != '"' && quoteEnclosed && !nextFieldValue && !braceEnclosed)
        {
            currentField += json[i];
            if (currentField == field)
            {
                fieldFound = true;
            }
        }
        else if (json[i] == ':' && !quoteEnclosed && !braceEnclosed)
        {
            nextFieldValue = true;
        }
        else if (nextFieldValue && quoteEnclosed && !braceEnclosed)
        {
            currentValue += json[i];
        }
        else if (nextFieldValue && !quoteEnclosed && (json[i] == ',' || !quoteEnclosed && json[i] == '}') && !braceEnclosed)
        {
            nextFieldValue = false;
            if (fieldFound)
            {
                return currentValue;
            }
            else
            {
                currentField = "";
                currentValue = "";
            }
        }
        else if (!quoteEnclosed && json[i] == '[' && !braceEnclosed)
        {
            braceEnclosed = true;
        }
        else if (!quoteEnclosed && json[i] == ']' && braceEnclosed)
        {
            braceEnclosed = false;
        }
        else if (nextFieldValue && !quoteEnclosed && braceEnclosed)
        {
            currentValue += json[i];
        }
    }
    return "";
}