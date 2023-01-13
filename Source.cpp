#include <iostream>
#include <string>
#include <stack>
using namespace std;

bool isOperator(char c);
bool isOperand(char c);
int precendence(char c);
int computeOperations(int operandOne, int operandTwo, char operation);
string convert(string expression);
//two exception classes: semantic error class, syntax error class
class SemanticError : public logic_error 
{
public:
	SemanticError() : logic_error("Semantic Error Detected") {}
	void initializeExpression(string exp)
	{
		expression = exp;
	}
	void errorCheck() const //if an opperand is being divided by zero, it is a logic error
	{
		for (int i = 0; i < expression.length(); i++)
		{
			if (expression[i] == '/' && expression[i + 1] == '0') {
				throw SemanticError();
			}
		}
	}

private:
	string expression;
};
class SyntaxError : public runtime_error
{
public:
	SyntaxError() : runtime_error("Syntax Error Detected") {}
	void initializeExpression(string exp)
	{
		expression = exp;
	}
	void errorCheck() const
	{
		stack<char> checker;
		for (int i = 0; i < expression.length(); i++) //checks for imbalance of parrenthesis, use of stack
		{
			if (expression[i] == '(')
			{
				checker.push(expression[i]);
			}
			else if(expression[i]==')')
			{
				if (checker.empty() || checker.top() != '(')
				{
					throw SyntaxError();
				}
				checker.pop();
			}
		}
		if (!checker.empty()) { 
			throw SyntaxError();
		}
		for (int i = 0; i < expression.length(); i++)
		{
			if (isOperator(expression[i]) && i == 0) //checks for dangling operators that are not attached to any operands
			{
				throw SyntaxError();
			}
			else if (isOperator(expression[i]) && i == expression.length() - 1)
			{
				throw SyntaxError();
			}
			else if (isOperator(expression[i]) && isOperator(expression[i + 1]))//checks if there are operators next to eachother instead of opperands
			{
				throw SyntaxError();
			}
		}
	}
private:
	string expression;
};
//Needs a character
//determines if character is an operator
//returns a boolean T/F if character is an operator
bool isOperator(char c)
{
	if ((c == '*') || (c == '/') || (c == '+') || (c == '-'))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//Needs a character
//determines if character is an opperand: digit between 0 and 9 inclusive.
//returns a boolean if character is an operand
bool isOperand(char c) 
{
	if (c >= '0' && c <= '9')
	{
		return true;
	}
	else
	{
		return false;
	}
}
//Needs a character
//determines the precendence, on a scale of 2 to 1, and -1 if no operator is found
//returns an int value that will be used to organize the set of operations to take place
int precendence(char c)
{
	if ((c == '*') || (c == '/'))
	{
		return 2;
	}
	else if (c == '+' || c == '-')
	{
		return 1;
	}
	else
		return -1;
}
//tests the precedence, whether char c and char d presedence lessthan or equal to move on to next operations
bool precendenceCalc(char c, char d)
{
	int a;
	int b;
	a = precendence(c);
	b = precendence(d);
	if (a <= b)
		return true;
	else
		return false;
}
//calculates the result of given operands and an operation, used in the evaluate function
//returns an int value
int computeOperations(int operandOne, int operandTwo, char operation)
{
	if (operation == '+') return operandOne + operandTwo;
	else if (operation == '-') 
		return operandOne - operandTwo;
	else if (operation == '*') 
		return operandOne * operandTwo;
	else if (operation == '/') 
		return operandOne / operandTwo;

	else
	{
		return -1;
	}
}
//the convert from infix to postfix function
string convert(string expression) { 
	stack<char> theStack;
	string PostFixResult;

	for (int i = 0; i < expression.length(); i++)//goes through the entire string character by character from left to right
	{
		if (isOperand(expression[i]))//determines if character at given index is an operand
		{							//If it is an operand, output it
			PostFixResult += expression[i]; //adds to the result which is a string
		}
		else if (expression[i] == '(') //If it is opening parenthesis, push it on the stack.
		{
			theStack.push('(');
		}
		else if (expression[i] == ')')//If it is a closing parenthesis, pop operators from the stack and output them until an opening parenthesis is encountered. Pop and discard the opening parenthesis.
		{
			while (theStack.top() != '(')
			{
				PostFixResult += theStack.top();
				theStack.pop();
			}
			theStack.pop();
		}
		else//If it is an operator, then
		{
			while (!theStack.empty() && precendenceCalc(expression[i],theStack.top()))
			{
				PostFixResult += theStack.top();//If the top of the stack is opening parenthesis, push the operator on the stack
				theStack.pop();
			}
			PostFixResult += ' ';
			theStack.push(expression[i]);// If the stack is empty, push the operator on the stack
		}

	}
	while (!theStack.empty()) 
	{
		PostFixResult += theStack.top();
		theStack.pop();
	}
	return PostFixResult;

}

int evaluatePostfix(string postfixExpression)
{
	stack<int> thePostStack;
	for (int i = 0; i < postfixExpression.length(); i++)//goes through the entire string character by character from left to right. 
	{
		if (postfixExpression[i] == ' ')//if the character is a space, move onto next character of the string
		{
			continue;//continue exits out of the for loop and goes 
		}
		else if (isOperand(postfixExpression[i]))//if the character at position i is operand, we convert it to an int
		{
			int operand = 0;
			while (isOperand(postfixExpression[i]) && (i < postfixExpression.length()))//while the operands are still digits  and the i does not exceed the string length itself
			{
				operand = (operand * 10) + (postfixExpression[i] - '0'); //subtract - '0' from char or string to turn it into an int. Everytime the while loop runs, i gets incremented and the operand gets multiplied by ten because a new decimal value is added to the end of the new int.
				i++; //increment i till the digits are no more on that index or the length is reached.
			}//after while loop is finished, the entire operand is present and ready to be pushed in the stack.
			i--; //decrement i by one because if i is an operator, it will skip it because incremented i inside the while loop at the end..
			thePostStack.push(operand); //since it is an operand, it's value gets pushed to the stack
		}
		else if (isOperator(postfixExpression[i])) //if the token is an operator
		{
			int operand2 = thePostStack.top();//operand2 = the element popped from the top of the stack
			thePostStack.pop();
			int operand1 = thePostStack.top();//operand1 = the element popped from the top of the stack
			thePostStack.pop();

			int result = computeOperations(operand1, operand2, postfixExpression[i]);//compute operand1 operator operand2
			thePostStack.push(result); //push result onto the stack
		}
	}
	return thePostStack.top();//return top of the stack as result
}

int main() {
	string theExpression;
	cout << "Please type the expression in the format of Infix Notation :";
	cin >> theExpression;

	//Errors check:
	SemanticError SemanticErrorsOfExpression;
	SyntaxError SyntaxErrorsOfExpression;
	SemanticErrorsOfExpression.initializeExpression(theExpression);
	SyntaxErrorsOfExpression.initializeExpression(theExpression);
	SemanticErrorsOfExpression.errorCheck();
	SyntaxErrorsOfExpression.errorCheck();

	string postfixExpression = convert(theExpression);
	cout << "The postfix notation :";
	cout << postfixExpression << endl;
	int solution;
	solution = evaluatePostfix(postfixExpression);
	cout << "The result :";
	cout << solution;


	return 0;
}