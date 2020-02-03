/*
 * Name: Samuel Lamb
 * Class: CDA 3101
 * Assignment: Assignment 2
 */
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <ostream>
#include <map>
#include <stdio.h>

#define MAXLINE 80
#define MAXREG  5


std::vector<std::string> LineVector; //Vector that stores each line of assembly code being passed in
std::map<std::string,int> ScanMap; //Map structure for string label locations
std::vector<unsigned int> FinalVector; //The final vector to spit out
char ScanChar[10];
char ScanChar2[10];
char rd[MAXREG],rs[MAXREG],rt[MAXREG];
char oper[MAXLINE];
std::string Registers[32] = {"zero","at","v0","v1","a0","a1","a2","a3","t0","t1","t2","t3","t4","t5","t6","t7","s0","s1","s2","s3","s4","s5","s6","s7","t8","t9","k0","k1","gp","sp","fp","ra"};
bool NotDir = 1;
bool label = 1;
int word = 0;
int WordCount = 0; //Used to count how many times the program picks up ".word"

union 
{
unsigned int x;

struct { 
unsigned int funct:6;
unsigned int shamt:5;
unsigned int rd:5;
unsigned int rt:5;
unsigned int rs:5;
unsigned int oper:6;
} R; //For R types it loads all the locations in

struct {
unsigned int immed:16;
unsigned int rs:5;
unsigned int rd:5;
unsigned int oper:6;
} I; //For I types it loads all the locations in

struct {
unsigned int adr:26;
unsigned int oper:6;
} J; //For J types it loads all the locations in

} u;

int RegisterCompare(std::string comp)
{
  for (int i = 0; i < 32; ++i) //retun the index value related to the string compared
    {
      if (comp == Registers[i])
      {
        // std::cout << std::endl << Registers[i];
        return i;
      }
    }
};


void Compare(std::string str,int i = 0)
{
  if (str == "addiu")
  {
    //std::cout << LineVector[i];
    if (label)
    {
      sscanf(LineVector[i].c_str(), "%*s %s $%[^,],$%[^,],%s", oper, rd, rs, rt); //if there is a label then omit the first string
    }
    else
      sscanf(LineVector[i].c_str(), "%s $%[^,],$%[^,],%s", oper, rd, rs, rt);

    // int toy = atoi(rt);
    // std::cout << toy;
    //std::cout << oper << rd << rs << rt << std::endl;
    //std::cout << rt;
    std::string rdtemp(rd);
    std::string rstemp(rs);
    //std::cout << rdtemp;
    u.I.oper = 0x9;
    u.I.rs = RegisterCompare(rdtemp);
    u.I.rd = RegisterCompare(rstemp);
    //std::cout << u.I.rs;
    u.I.immed = atoi(rt);
    //std::cout << atoi(rt);
    //std::cout << std::endl << rt << std::endl;
    //std::cout << u.I.rd;
    //std::cout << u.x;
  } 
  else if (str == "addu")
  {
    if (label)
    {
      //std::cout << "find label" << LineVector[i];
      sscanf(LineVector[i].c_str(), "%*s %s $%[^,],$%[^,],$%s", oper, rd, rs, rt);
    }
    else
      sscanf(LineVector[i].c_str(), "%s $%[^,],$%[^,],$%s", oper, rd, rs, rt);

    std::string rdtemp(rd);
    std::string rstemp(rs);
    std::string rttemp(rt);
    //std::cout << LineVector[i] << std::endl;
    u.R.oper = 0x0;
    // std::cout << oper << std::endl;
    u.R.rd = RegisterCompare(rdtemp);
     //std::cout << rd << rs << rt << std::endl;
    u.R.rs = RegisterCompare(rstemp);
     //std::cout << rs << std::endl;
    u.R.rt = RegisterCompare(rttemp); //v0 , correct
     //std::cout << rt << std::endl;
    u.R.shamt = 0x00;
    u.R.funct = 0x21;
  } 
  else if (str == "and")
  {
    if (label)
    {
      sscanf(LineVector[i].c_str(), "%*s %s $%[^,],$%[^,],$%s", oper, rd, rs, rt);
    }
    else
      sscanf(LineVector[i].c_str(), "%s $%[^,],$%[^,],$%s", oper, rd, rs, rt);

    std::string rdtemp(rd);
    std::string rstemp(rs);
    std::string rttemp(rt);
    u.R.rd = RegisterCompare(rdtemp);
    u.R.rs = RegisterCompare(rstemp);
    u.R.rt = RegisterCompare(rttemp);
    u.R.oper = 0x0;
    u.R.funct = 0x24;
  }
  else if (str == "beq")
  {
    if (label)
    {
      sscanf(LineVector[i].c_str(), "%*s %s $%[^,],$%[^,],%s", oper, rd, rs, rt);
    }
    else
      sscanf(LineVector[i].c_str(), "%s $%[^,],$%[^,],%s", oper, rd, rs, rt);

    std::string rttemp(rt);
    std::string rdtemp(rs);
    std::string rstemp(rd);
    u.I.rs = RegisterCompare(rdtemp);
    u.I.rd = RegisterCompare(rstemp);
    u.I.immed = 6; //atoi(rt) //L2
    u.I.oper = 0x4;
  }
  else if (str == "bne")
  {
    if (label)
    {
      sscanf(LineVector[i].c_str(), "%*s %s $%[^,],$%[^,],%s", oper, rd, rs, rt);
    }
    else
      sscanf(LineVector[i].c_str(), "%s $%[^,],$%[^,],%s", oper, rd, rs, rt);

    std::string rdtemp(rd);
    std::string rstemp(rs);
    u.I.rd = RegisterCompare(rdtemp);
    u.I.rs = RegisterCompare(rstemp);
    u.I.immed = atoi(rt);
    u.I.oper = 0x5;
  }
  else if (str == "div")
  {
    if (label)
    {
      sscanf(LineVector[i].c_str(), "%*s %s $%[^,],$%s", oper, rs, rt);
    }
    else
      sscanf(LineVector[i].c_str(), "%s $%[^,],$%s", oper, rs, rt);

    std::string rstemp(rs);
    std::string rttemp(rt);
    u.R.rs = RegisterCompare(rstemp);
    u.R.rt = RegisterCompare(rttemp);
    u.R.oper = 0x0;
    u.R.funct = 0x1a;
  }
  else if (str == "j")
  {
    if (label)
    {
      sscanf(LineVector[i].c_str(), "%*s %s $%[^,],$%[^,],%s", oper, rd, rs, rt);
    }
    else
      sscanf(LineVector[i].c_str(), "%s $%[^,],$%[^,],%s", oper, rd, rs, rt);

    // Scan the string after J
    // Compare the string to the mapped labels
    // Somehow go to that position

    // std::string rdtemp(rd);
    // std::string rstemp(rs);
    // u.J.rd = RegisterCompare(rdtemp);
    // u.J.rs = RegisterCompare(rstemp);
    // u.J.rt = atoi(rt);
    u.J.oper = 0x2;
    u.J.adr = 0x5;
    //u.J.funct = 0x2;
  }
  else if (str == "lw")
  {
    if (label)
    {
      sscanf(LineVector[i].c_str(), "%*s %s $%[^,],%[^(]($%[^)]", oper, rd, rs, rt);
    }
    else
      sscanf(LineVector[i].c_str(), "%s $%[^,],%[^(]($%[^)]", oper, rd, rs, rt);
    std::string rstemp(rt);
    //std::string rstemp(rs);
    std::string rdtemp(rd);
    u.I.rs = RegisterCompare(rdtemp);
    u.I.rd = RegisterCompare(rstemp);
    ////////////////////////////////
    u.I.immed = 0;
    u.I.oper = 0x23;
  }
  else if (str == "mfhi")
  {
    sscanf(LineVector[i].c_str(), "%s $%s", oper, rd);
    std::string rdtemp(rd);
    u.R.rd = RegisterCompare(rdtemp);
    u.R.oper = 0x0;
    u.R.funct = 0x10;
  }
  else if (str == "mflo")
  {
    sscanf(LineVector[i].c_str(), "%s $%s", oper, rd);
    std::string rdtemp(rd);
    u.R.rd = RegisterCompare(rdtemp);
    u.R.oper = 0;
    u.R.funct = 12;
  }
  else if (str == "mult")
  {
    if (label)
    {
      sscanf(LineVector[i].c_str(), "%*s %s $%[^,],$%s", oper, rs, rt);
    }
    else
      sscanf(LineVector[i].c_str(), "%s $%[^,],$%s", oper, rs, rt);

    std::string rstemp(rs);
    std::string rttemp(rt);
    u.R.rs = RegisterCompare(rstemp);
    u.R.rt = RegisterCompare(rttemp);
    u.R.oper = 0x0;
    u.R.funct = 0x18;
  }
  else if (str == "or")
  {
    if (label)
    {
      sscanf(LineVector[i].c_str(), "%*s %s $%[^,],$%[^,],$%s", oper, rd, rs, rt);
    }
    else
      sscanf(LineVector[i].c_str(), "%s $%[^,],$%[^,],$%s", oper, rd, rs, rt);

    std::string rdtemp(rd);
    std::string rstemp(rs);
    u.R.rd = RegisterCompare(rdtemp);
    u.R.rs = RegisterCompare(rstemp);
    u.R.rt = atoi(rt);
    u.R.oper = 0x25;
  }
  else if (str == "slt")
  {
    if (label)
    {
      sscanf(LineVector[i].c_str(), "%*s %s $%[^,],$%[^,],$%s", oper, rd, rs, rt);
    }
    else
      sscanf(LineVector[i].c_str(), "%s $%[^,],$%[^,],$%s", oper, rd, rs, rt);

    std::string rdtemp(rd);
    std::string rstemp(rs);
    std::string rttemp(rt);
    u.R.rd = RegisterCompare(rdtemp);
    u.R.rs = RegisterCompare(rstemp);
    u.R.rt = RegisterCompare(rttemp);
    // u.R.rt = atoi(rt);
    u.R.oper = 0x0;
    u.R.funct = 0x2a;
    //std::cout << rd << rs << rt << std::endl;
  }
  else if (str == "subu")
  {
    if (label)
    {
      sscanf(LineVector[i].c_str(), "%*s %s $%[^,],$%[^,],$%s", oper, rd, rs, rt);
    }
    else
      sscanf(LineVector[i].c_str(), "%s $%[^,],$%[^,],$%s", oper, rd, rs, rt);

    std::string rdtemp(rd);
    std::string rstemp(rs);
    u.R.rd = RegisterCompare(rdtemp);
    u.R.rs = RegisterCompare(rstemp);
    u.R.rt = atoi(rt);
    u.R.oper = 0x0;
    u.R.funct = 0x23;
  }
  else if (str == "sw")
  {
    if (label)
    {
      sscanf(LineVector[i].c_str(), "%*s %s $%[^,],%[^(]($%[^)]", oper, rd, rs, rt);
    }
    else
      sscanf(LineVector[i].c_str(), "%s $%[^,],%[^(]($%[^)]", oper, rd, rs, rt);

    std::string rstemp(rt);
    std::string rdtemp(rd);
    u.I.rd = RegisterCompare(rstemp);
    u.I.rs = RegisterCompare(rdtemp);
    
    ////////////////////////////////
    u.I.immed = 0;
    u.I.oper = 0x2b;
  }
  else if (str == "syscall")
  {
    u.R.rd = 0x0;
    u.R.rs = 0x0;
    u.R.rt = 0x0;
    u.R.oper = 0x0;
    u.R.shamt = 0x0;
    u.R.funct = 0xc;
    // Activate Certain Register
    // Fill with 0's and C
  }
  else if (str == ".text")
  {
    NotDir = 0;
    // std::cout << ".text" << std::endl;
  }
  else if (str == ".size")
  {
    NotDir = 0;
    std::cout << ".size" << std::endl;
  }
  else if (str == ".word")
  {
    sscanf(LineVector[i].c_str(), "%*s %*s %s", oper);
    //std::cout << oper << std::endl;
    word = atoi(oper);
    for (int j = 0; j < word; ++j)
    {
      u.x = word; //adds whatever value the wordcount is to hex
    }
    // std::cout << ".word" << std::endl;
    WordCount += 1; //when it notices a .word the WordCount increments
  }
  else if (str == ".data")
  {
    NotDir = 0;
    u.x = 0;
    // std::cout << ".data" << std::endl;
  }
  else
    std::cout << "Missing One" << std::endl;
    
};

void LineScan()
{
  for (int i = 0; i < LineVector.size(); ++i)
  {
    if (LineVector[i].find(':') != std::string::npos)
    {
      sscanf(LineVector[i].c_str(), "%[^:]s", ScanChar); //Scanning for the string before a colon
      std::string ScanCharConvert(ScanChar); //Converting character array to a string
      ScanMap[ScanCharConvert] = i; //inputting the string and location into a map
      //std::cout << ScanMap[ScanCharConvert] << "," << ScanCharConvert << std::endl;
    }
  }
};

void LineEncode()
{
  for (int i = 0; i < LineVector.size(); ++i)
  {
    label = 0;
    NotDir = 1;
    if (LineVector[i].find(':') != std::string::npos)
    {
      label = 1;
      //std::cout << "find label" << LineVector[i];
      sscanf(LineVector[i].c_str(), "%*s %s", ScanChar2); //scan for the string after the label
      std::string Mneumonic(ScanChar2); //make that label a string
      Compare(Mneumonic, i); //compare it
      if (NotDir)
      {
      FinalVector.push_back(u.x); //push it to the vector
      }
    }
    else
    {
      sscanf(LineVector[i].c_str(), "%s", ScanChar2); //scan for the first string
      std::string Mneumonic(ScanChar2); //make the char array a string
      Compare(Mneumonic, i); //comp
      if (NotDir)
      {
      FinalVector.push_back(u.x); //push onto the final vector
      }
    }
   // std::cout << ScanChar2 << std::endl;
  }
};





int main()
{
  char line[MAXLINE]={0};
  

  // while (fgets(line, MAXLINE, stdin)) {
  //   /* check if a 3-address R format instruction */
  //   if (sscanf(line, "%s $%[^,],$%[^,],$%s", oper, rd, rs, rt) == 4) {
  //     printf("input line: %s\n", line);
  //     printf("parsed line: op:%10s rd:%5s rs:%5s rt:%5s\n",
	//      oper, rd, rs, rt);
  //   }
  //   /* you need to add other patterns for R,I,and J encoding */    
  //   else {
  //     printf("input line: %s\n", line);      
  //     printf("you need to add sscanf format\n");
  //   }
  // }
  // return 0;

  // Output Testy Boi
  // for(int i = 0; i < LineVector.size(); ++i)
  // {
  // std::cout << LineVector[i];
  // };

  while (fgets(line, MAXLINE, stdin)) 
  {
    std::string StringConvert(line); //Converting Cstring to string names StringConvert
    LineVector.push_back(StringConvert); //Pushing each line that gets fed in to the back of the vector
  }
  LineScan(); //First pass, stored labels in a map
  LineEncode(); //Second Pass
  std::cout << FinalVector.size() - WordCount << " " << WordCount << std::endl;
  for (int i = 0; i < FinalVector.size(); ++i)
  {
  printf("%08x\n",FinalVector[i]);
  };
  // New comment
  return 0;
}
