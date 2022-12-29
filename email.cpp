//Objective: Get the input and output file names from the user (or default), output them, then read through the input file and output every valid and non-duplicate email address to the output file and console.
//Compiler: TDM MinGW
//Editor: MS NotePad

//libraries
#include <deque>
#include <fstream>
#include <iostream>
using namespace std;

//Defined functions
void introduction(string obj, string ins); //user introduction
string getFileName(string dFileName, string fileType); //get user-inputted file name
void readFile(deque<string>& nonDup, string iFileName, string oFileName); //read each line of file and call processLine to output line for each @ and count @'s
int processLine(deque<string>& nonDup, string lineFromFile); //output line for each @ and count @'s
bool isValidEmailChar (char c); //Returns true if c is a valid email address character, else false
bool isDuplicate (deque<string>& nonDup, string anEmail); //checks anEmail whether it is a duplicate email address using case sensitive comparison to create list of non-duplicate email addresses
string changeCase(string s); //converts string to all one case for case
void sortList(deque<string>& nonDup); //sort the emails in the list

//main program
int main()
{
  //Data
  string objective = "Get the input and output file names from the user (or default), output them, then read through the input file and output every valid and non-duplicate email address to the output file and console.\n"; //program objective
  string instructions = "Please enter a valid (.txt) input and output file name. The default filename is in brackets. If you would like to use the default filename, press ENTER with no input.\n"; //user instructions
  string iFileName; //user-inputted (or default) name of input file [Valid values: must end in ".txt"]
  string dFileName = "fileContainingEmails.txt"; //default file name
  string oFileName; //user-inputted (or default) name of output file [Valid values: must end in ".txt"]
  string fileType = "input"; //file type that we are looking for ("input" or "output")
  deque<string> nonDup;
  
  //User introduction
  introduction(objective, instructions);

  //Get file names
  iFileName = getFileName(dFileName, fileType);
  if(iFileName == dFileName) //set new default file name depending on user input
  {
    dFileName = "copyPasteMyEmails.txt";
  }
  else
  {
    dFileName = iFileName;
  } //set new default file name depending on user input
  fileType = "output";
  oFileName = getFileName(dFileName, fileType);

  //Output file names
  cout << endl << "Input file: " << iFileName << endl;
  cout << "Output file: " << oFileName << endl;
  cout << endl << "Press ENTER key to continue";
  cin.ignore(1000,10);

  //Read input file line by line, and output each valid email address found, along with the total number of valid email addresses
  readFile(nonDup, iFileName, oFileName);

}//main

//user introduction
void introduction(string obj, string ins)
{
  //Data
  //obj is the program objective
  //ins is the user instructions

  //User introduction
  cout << "Objective: " << obj; 
  cout << "Editor(s) used: Notepad\n"; 
  cout << "Compiler(s) used: TDM MinGW\n"; 
  cout << "File: " << __FILE__ << endl; 
  cout << "Compiled: " << __DATE__ << " at " << __TIME__ << endl; 
  cout << "Instructions/rules: " << ins << endl << endl;

}//introduction

//get user-inputted file name
string getFileName(string dFileName, string fileType)
{
  //Data
  //dFileName is the default file name
  //fileType is the file type of the filename that we are looking for (.txt)
  string fileName; //stores file name, returned by subprogram

  cout << "Enter " << fileType << " filename [default: " << dFileName << "]: ";
  while(true) //while loop to ensure valid input
  {
    getline(cin,fileName);
    
    if(fileName.length() > 0 && ((fileName.length() < 5) || (fileName.substr(fileName.length()-4, 4) != ".txt"))) //prompt user again if invalid input
    {
      cout << "Please enter a valid file name (valid .txt file): ";
    }
    else
    {
      break;
    } //prompt user again if invalid input
  } //while loop to ensure valid input
  if(fileName.length() == 0) //set equal to default if user left blank
  {
    fileName = dFileName;
  } //set equal to default if user left blank

  return fileName;
} //getFileName

//read each line of file and call processLine to output line for each @ and count @'s
void readFile(deque<string>& nonDup, string iFileName, string oFileName)
{
  //Data
  //nonDup is the list of valid, non-duplicate email addresses
  //iFileName is the name of the file
  string lineFromFile; //current read-in line from input file
  ifstream fin; //object definition to take input from a file
  ofstream fout; //object definition to output to a file
  int nEmails = 0; //number of valid output emails
  int outputEmails; //index to loops through email list and correctly output each one
  
  fin.open(iFileName.c_str());
  while(fin.good()) //process and look for valid emails in each line of file, until EOF is reached
  {
    getline(fin, lineFromFile);
    nEmails = nEmails + processLine(nonDup, lineFromFile);
  } //process and look for valid emails in each line of file, until EOF is reached
  fin.close();
  fin.clear();
  if(nonDup.size() > 0) //if email is not empty, output each valid, non-duplicate email to file and console
  {
    fout.open(oFileName.c_str());
    for(outputEmails = 0; outputEmails < nonDup.size(); outputEmails = outputEmails + 1) //output each email
    {
      if(outputEmails < nonDup.size() - 1)  //output each email with a semicolon except the last email
      {
        fout << nonDup[outputEmails] << "; ";
        cout << endl << nonDup[outputEmails] << ';';
      }  //output each email with a semicolon except the last email
      else //output last email without semicolon
      {
        fout << nonDup[outputEmails];
        cout << endl << nonDup[outputEmails] << endl;
      } //output last email without semicolon
    } //output each email
    fout.close();
    cout << endl << nEmails << " email addresses were found, and copied to the file " << oFileName << "." << endl;
  }//if email is not empty, output each valid, non-duplicate email to file and console
  else //if list is empty, output error message instead
  {
    cout << endl << "Sorry, no email addresses were found in the file " << iFileName << '.' << endl;
  } //if list is empty, output error message instead
} //readFile

//output line for each @ and count @'s
int processLine(deque<string>& nonDup, string lineFromFile)
{
  //Data
  //nonDup is the list of valid, non-duplicate email addresses
  //lineFromFile is the current read-in line from the input file
  int i; //traversal index to find @'s
  int s; //traversal index for characters before the @
  int e; //traversal index for characters after the @
  bool hasDot; //true if the characters after the @ has a dot, false if it does not
  int dotPos; //stores the index of the dot
  string anEmail; //stores the full valid email to output
  int nATs = 0; //stores how many @'s were found (valid emails)
  bool dup; //stores whether current email was a duplicate or not

  for(i = 0; i < lineFromFile.length(); i = i + 1) //traverse through each character of string
  {
    if(lineFromFile[i] == '@') //check for valid email if an '@' is found
    {
      s = i;
      while(true) //check characters before '@' for validity, stop when invalid character is found
      {
        s--;
        if(s < 0) break;
        if(isValidEmailChar(lineFromFile[s]) == false) break; //check if the current char is invalid
      } //check characters before '@' for validity
      s++;
      e = i;
      while(true) //check characters after '@' for validity, stop when invalid character is found
      {
        e++;
        if(e == lineFromFile.length()) break;
        if(lineFromFile[e] == '.') //check if current character is a dot
        {
          hasDot = true;
          dotPos = e;
        } //check if current character is a dot
        if(isValidEmailChar(lineFromFile[e]) == false) break; //check if current char is invalid
      } //check characters after '@' for validity, stop when invalid character is found
      if((s < i) && (e > i) && (hasDot == true) && (dotPos > (i + 1))) //check if conditions for "valid" emails are met
      {
        anEmail = lineFromFile.substr(s, e-s);
        dup = isDuplicate(nonDup, anEmail);
        if(dup == false) //check if email is a duplicate
        {
          nATs = nATs + 1;
        } //check if email is a duplicate
      } //check if conditions for "valid" emails are met
    } //check for valid email if an '@' is found
  } //traverse through each character of string
  return nATs;
} //processLine

//Returns true if c is a valid email address character, else false
bool isValidEmailChar(char c)
{
  //Data
  //c is the current character from the line read from the file

  if((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || (c >= 48 && c <= 57) || (c == 95 || c == '.' || c == 45  || c == 43)) //return true if the character is one of the listed valid characters
  {
    return true;
  } //return true if the character is one of the listed valid characters
  
  return false;
} //isValidEmailChar

//sort the emails in the list
void sortList(deque<string>& nonDup)
{
  //Data
  //nonDup is the list of valid, non-duplicate email addresses
  int o; //outer index for sorting loop
  int p; //inner index for sorting loop
  string temp; //stores temporary string for sorting

  for(o = 0; o < nonDup.size(); o = o + 1) //sort each email
  {
    for(p = o + 1; p < nonDup.size(); p = p + 1) //compare to every other email in list
    {
      if (changeCase(nonDup[o]) > changeCase(nonDup[p])) //switch emails if greater
      {
        temp = nonDup[o];
        nonDup[o] = nonDup[p];
        nonDup[p] = temp;
      } //switch emails if greater
    } //for j
  } //for o
} //sortList

//checks anEmail whether it is a duplicate email address using case sensitive comparison to create list of non-duplicate email addresses
bool isDuplicate (deque<string>& nonDup, string anEmail)
{
  //Data
  //nonDup is the list of valid, non-duplicate email addresses
  //anEmail is the current valid email address being checked for duplicates
  int m; //index to traverse through list of non-duplicate email addresses
  string tempCaseAnEmail = changeCase(anEmail); //email converted to all uppercase for comparison
  string tempCaseListEmail; //email from list converted to all uppercase for comparison
  bool isDuplicate = false; //stores whether email is a duplicate or not
  
  for(m = 0; m < nonDup.size(); m = m + 1) //traverse through list, checking for duplicates
  {
    tempCaseListEmail = changeCase(nonDup[m]);
    if(tempCaseAnEmail == tempCaseListEmail) //check if emails are duplicates
    {
      isDuplicate = true;
      return true;
    } //check if emails are duplicates
  } //traverse through list, checking for duplicates
  if(isDuplicate == false) //if email is not a duplicate, add it to the list
  {
    nonDup.push_back(anEmail);
    sortList(nonDup);
  } //if email is not a duplicate, add it to the list
  return false;
} //isDuplicate


//converts string to all one case for case
string changeCase(string s)
{
  //Data
  //s is the string to be converted to all uppercase for use in sorting
  int caseChange; //index to traverse string and change to all uppercase
  string result = s; //resulting string from changing s to all uppercase
  
  for(caseChange = 0; caseChange < s.length(); caseChange = caseChange + 1) //change each letter to uppercase
  {
    result[caseChange] = toupper(result[caseChange]);
  } //change each letter to uppercase
  
  return result;
} //changeCase