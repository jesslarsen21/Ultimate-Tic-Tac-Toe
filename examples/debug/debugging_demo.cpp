//
// This program is simply built to use in
// demonstrating the basics of debugging tools.
//
// This program uses basic arithmetic to change
// any positive integer into the number 7.
//
//
// DEBUGGING TASK I
//#################
//
// Using the debugger, run this program.  Input the
// number 15 at the prompt, then answer these questions:
//
// 1- What is the value of your_number in the function g(),
//    the first time it is called?  Answer that question
//    before each of the lines A,B,C,D are executed.
// 2- Before the function g() returns, what is the value
//    of your_number in the function f(), at line E?
// 3- What is the value of your_number in function f(), after
//    g() returns the first time?
// 4- What is the value of your_number in function f()
//    before the line F executes?
//
//
//
// DEBUGGING TASK II
//##################
//
// Using the debugger, run this program.  Input the
// number 15 at the prompt.
//
// 1- Record the sequence of values returned from
//    the function g().
// 2- It's probably best to set a breakpoint on the
//    line in question.
//
#include <iostream>

int g(int your_number)                //
{
  your_number = 3 * your_number;      // A
  your_number = your_number / 6;      // B
  your_number = your_number;          // C
  return your_number;                 // D
}


int f(int your_number)                //
{
  while(your_number > 1)              //
    {
      your_number = g(your_number);   // E
    }
  int my_number = 7 * your_number;    // F
  return my_number;                   // G
}

int main()
{
  int your_number;
  std::cout << "This is a magic program.  It will take any positive" << std::endl;
  std::cout << "integer, and covert it the number 7." << std::endl;
  std::cout << "Number? ";
  std::cin >> your_number;
  if(your_number < 1)
    {
      std::cout << "Positive numbers only please." << std::endl;
      return 1;
    }
  int my_number = f(your_number);
  std::cout <<  "f(" << your_number << ") = " << my_number << std::endl;
  return 0;
}
