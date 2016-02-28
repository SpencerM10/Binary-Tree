#include <iostream>
#include <string>
#include <sstream>
#include<climits>
#include<stdlib.h>
#include <stack>
using namespace std;

//checks a string for an operator
bool IsMathOperator(string word)
{
	//of string is any one of the math operators return true
	if (word.compare("+") == 0 || word.compare("-") == 0 || word.compare("*") == 0 || word.compare("/") == 0 || word.compare("^") == 0)
		return true;
	else
		return false;
}


//checks a string for an operator
bool IsOperator(string word)
{
	//of string is any one of the math operators return true
	if (word.compare("+") == 0 || word.compare("-") == 0 || word.compare("*") == 0 || word.compare("/") == 0 || word.compare("^") == 0 || word.compare("(") == 0 || word.compare(")") == 0)
		return true;
	else
		return false;
}

//checks for string to be an operand
bool IsOperand(string str)
{
	//looping through the string
	for (unsigned int i = 0; i<str.length(); i++)
	{
		//if character of string is not between 0 - 9, return false
		if (str[i] < '0' || str[i] > '9')
			return false;
	}
	//return true if all characters are numbers
	return true;
}

//structure for the binary tree node
typedef struct Node{
	// store operator or operand
	string data;
	// only valid for operator
	int precedence;
	struct Node* parent;
	struct Node* left;
	struct Node* right;
}CNode, *PNode;

//function to create a tree node
PNode CreateNode(const string& x)
{
	//creates an empty node
	PNode p = new CNode;
	//assigning  right and parent to null
	p->parent = p->left = p->right = NULL;
	//assigning data of the node
	p->data = x;
	return p;
}

//function to check if the string is a left braces
bool IsLeftParenthesis(const string& x)
{
	return x == "(";
}
//function to check if the string is a right braces
bool IsRightParenthesis(const string& x)
{
	return x == ")";
}
//function to get the mathematical precedence of the operators
//* and / are the highest - return 2
//+ and - come below them - return 1
int GetPrecedence(const string& x)
{
	if (x[0] == '*' || x[0] == '/') return 2;
	else return 1;
}

//function to create inifix tree from a valid infix string
PNode CreateInfixTree(const string& exp)
{
	// create a dummy root with minimal precedence
	// its content is trivial
	PNode root = CreateNode("0");
	root->precedence = INT_MIN;

	// the previous operand of current operator
	PNode preOperand = NULL;
	// the previous operator of current operator
	PNode preOperator = root;
	// the impact of preceding parenthesis, if any
	int correction = 0;

	//variable to hold each word of the infix string
	string token;
	//stream to read the infix string word by word
	stringstream ss(exp);

	//while there are words in the stream
	while (ss >> token)
	{
		//if the word is an operand
		if (IsOperand(token))
		{
			//create a node
			preOperand = CreateNode(token);
		}
		//if the word is + or - or * or /
		else if (IsMathOperator(token))
		{
			//create node and set the precedence
			PNode p = CreateNode(token);
			p->precedence = GetPrecedence(token) + correction;
			if (p->precedence > preOperator->precedence)
			{
				p->left = preOperand;
				preOperator->right = p;
				p->parent = preOperator;
			}
			else
			{
				preOperator->right = preOperand;
				PNode q = preOperator->parent;
				while (p->precedence <= q->precedence) q = q->parent;

				p->left = q->right;
				q->right = p;
				p->parent = q;
			}
			preOperand = NULL;
			preOperator = p;

		}//else if (IsOperator(token)
		//if the word is a left brace
		else if (IsLeftParenthesis(token))
		{
			correction += 2;
		}
		//if the word is a right brace
		else if (IsRightParenthesis(token))
		{
			correction -= 2;
		}
		//just a error check
		//we are already providing a valid infix string
		else
		{
			cout << "illegal token found: " << token << endl;
			break;
		}
	}//while

	//if there is a problem while creating infix tree
	//printing out the problem
	if (preOperand == NULL)
		cout << "illegal expression: cannot end with operator: "
		<< preOperator->data << endl;
	else preOperator->right = preOperand;

	// delete dummy root
	PNode realRoot = root->right;
	delete root;
	if (realRoot) realRoot->parent = NULL;

	//if there is no problem realRoot will be the root
	//else it will be null
	return realRoot;
}


//overload method of IsOperator
bool IsOperator(char ch)
{
	//returns true if the character is a Math operator
	if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
		return true;
	else
		return false;
}


//function to display the menu
//and prompt input
void displayMenu()
{
	cout << "***************************************************************************" << endl;
	cout << "SELECTIONS:" << endl;
	cout << "1. Read an expression" << endl;
	cout << "2. Display the prefix expression" << endl;
	cout << "3. Display the infix expression" << endl;
	cout << "4. Display the postfix expression" << endl;
	cout << "5. Evaluate the expression using postfix notation." << endl;
	cout << "6. Exit" << endl;
	cout << "***************************************************************************" << endl;
	cout << "Select: ";
}

//function to validate the infix expression entered by the user
//returns true if it is valid
bool validateInfix(string exp)
{
	//variables to keep track of
	//#error, #leftbraces, $rightbraces
	int error = 0;
	int lb = 0;
	int rb = 0;

	//converting string to stringsteam so that we can do word by word evaluation
	istringstream iss(exp);
	//variable to store the current word
	string word;

	//variable to tell if the expression has ended with incorrect operator
	bool endIsOp = false;

	//while there are words in the stream
	while (iss >> word)
	{
		if (rb > lb)
		{
			//cout<<"Error braces"<<endl;
			error++;
			return false;
		}
		//cout<<"Evaluating: "<<word<<endl;
		if (IsOperand(word))
		{
			endIsOp = false;
			//cout<<"Word: "<<word<<" is an operand"<<endl;
		}
		else if (IsOperator(word))
		{
			//cout<<"Word: "<<word<<" is an operator"<<endl;
			//if the word is left braces
			if (word.compare(")") == 0)
			{
				rb++;
				//checking the next word 
				word = iss.peek();
				//if next word is an operator but not left braces
				//it is an error
				if (IsOperator(word) && (word.compare("+") == 0 || word.compare("-") == 0 || word.compare("*") == 0 || word.compare("/") == 0 || word.compare("^") == 0 || word.compare(")") == 0))
				{
					//if there is a word in the stream
					if (iss >> word)
					{
						if (word.compare(")") == 0)
							rb++;
					}
					else
					{
						//cout<<"Abrupt end 1"<<endl;
						error++;
						return false;
					}
				}
				//no word left in the stream 
				//incomplete expression
				else if (IsOperator(word))
				{
					error++;
					return false;
				}
			}
			else if (word.compare("(") == 0)
			{
				endIsOp = true;
				lb++;
				word = iss.peek();
				if (IsOperator(word) && word.compare("(") == 0)
				{
					//if there is a word in the stream
					if (iss >> word)
						lb++;
					else
					{
						error++;
						return false;
					}
				}
				//no word left in the stream 
				//incomplete expression
				else if (IsOperator(word))
				{
					error++;
					return false;
				}
			}
			else
			{
				endIsOp = true;
				word = iss.peek();
				if (IsOperator(word) && word.compare("(") == 0)
				{
					//if there is a word in the stream
					if (iss >> word)
						lb++;
					else
					{
						error++;
						return false;
					}
				}
				//no word left in the stream 
				//incomplete expression
				else if (IsOperator(word))
				{
					error++;
					return false;
				}
			}
		}
		else
			//anything other than numbers and operators
			//should be an error
		{
			error++;
			return false;
		}

	}

	if (error == 0 && lb == rb && !endIsOp)
	{
		return true;
	}
	else
		return false;
}

//function to perform the math operations
int performOperation(int op1, int op2, char op)
{
	//variable to store the answer
	int ans;
	//switch depending up on operator
	switch (op){
	case '+':
		ans = op2 + op1;
		break;
	case '-':
		ans = op2 - op1;
		break;
	case '*':
		ans = op2 * op1;
		break;
	case '/':
		ans = op2 / op1;
		break;
	}
	return ans;
}

//function to evaluate the postfix string using a stack
int evaluatePostfix(string exp)
{
	//variables to store the operator, length of string and stack
	char buffer[15];
	int i, op1, op2, len, j, x;
	stack<int> s;
	len = exp.length();
	j = 0;

	//while there is a number 
	//read the char to buffer
	//and finally convert it to int
	//and push to the stack
	for (i = 0; i<len; i++){
		if (exp[i] >= '0' && exp[i] <= '9'){
			buffer[j++] = exp[i];
		}
		else if (exp[i] == ' '){
			if (j>0){
				buffer[j] = '\0';
				x = atoi(buffer);
				s.push(x);
				j = 0;
			}
		}
		//if it is an operator pop the top two and perform the operation
		//again push the result to the stack
		else if (IsOperator(exp[i])){
			op1 = s.top();
			s.pop();
			op2 = s.top();
			s.pop();
			s.push(performOperation(op1, op2, exp[i]));
		}
	}
	//return the final value
	return s.top();
}

//function to make the postfix string from the binary expression tree
void GetPostfix(PNode node, string *str)
{
	//if the node is not null
	if (node)
	{
		//recursively call the leftNode, rightNode and root
		GetPostfix(node->left, str);
		GetPostfix(node->right, str);
		(*str).append(node->data);
		(*str).append(" ");
	}
}

//function to pring the Postfix of the tree
void PostOrderPrintTree(PNode node)
{
	//if the node is not null
	if (node)
	{
		//recursively call the leftSubtree, rightSubtree
		PostOrderPrintTree(node->left);
		PostOrderPrintTree(node->right);
		//print the root
		cout << node->data << " ";
	}
}

//function to pring the Prefix of the tree
void PreOrderPrintTree(PNode node)
{
	//if the node is not null
	if (node)
	{
		//print the root    
		cout << node->data << " ";
		//recursively call the leftSubtree, rightSubtree
		PreOrderPrintTree(node->left);
		PreOrderPrintTree(node->right);
	}
}

//function to pring the Prefix of the tree
void InOrderPrintTree(PNode node)
{
	//if the node is not null
	if (node)
	{
		//if the node is not a leafNode, print the paranthesis
		if (node->left != NULL)
			cout << "( ";
		//recursively call the leftSubtree
		InOrderPrintTree(node->left);
		//print the root
		cout << " " << node->data << " ";
		//recursively call the rightSubtree
		InOrderPrintTree(node->right);
		if (node->right != NULL)
			cout << " )";
	}
}
int main()
{
	//variable to store the user option
	int option = -1;
	//string to store the infix expression entered by the user
	string infix;
	//boolean which determines whether there is a valid infix in the memory
	bool hasExp = false;
	//root of the binary tree
	PNode root = NULL;
	while (option != 6)
	{
		//displaying the menu and prompting user for input
		displayMenu();
		cin >> option;
		//skipping the \n at the end of option
		cin.ignore();
		switch (option)
		{
		case 1:
			//promot the user for infix input
			cout << "Enter an infix expression: " << endl;
			//getting the input
			getline(cin, infix);
			//validating the infix string
			hasExp = validateInfix(infix);
			//if it is not a valid string
			//print error statement
			if (!hasExp)
			{
				cout << "Invalid expression!!" << endl;
			}
			//else build the expression tree
			else
			{
				root = CreateInfixTree(infix);
				cout << endl;
			}
			break;
		case 2:
			//printing the PREFIX notation
			if (hasExp)
			{
				cout << "\nExpression from user input: " << infix << endl;
				cout << "PREFIX Expression: ";
				PreOrderPrintTree(root);
				cout << "\n" << endl;
			}
			else
			{
				cout << "Expression from user input is empty!\n" << endl;
			}
			break;
		case 3:
			//printing the INFIX notation
			if (hasExp)
			{
				cout << "\nExpression from user input: " << infix << endl;
				cout << "INFIX Expression: ";
				InOrderPrintTree(root);
				cout << "\n" << endl;
			}
			else
			{
				cout << "Expression from user input is empty!\n" << endl;
			}
			break;
		case 4:
			//printing the POSTFIX notation
			if (hasExp)
			{
				cout << "\nExpression from user input: " << infix << endl;
				cout << "POSTFIX Expression: ";
				PostOrderPrintTree(root);
				cout << "\n" << endl;
			}
			else
			{
				cout << "Expression from user input is empty!\n" << endl;
			}
			break;
		case 5:
			if (hasExp)
			{
				cout << "\nExpression from user input: " << infix << endl;
				string exp;
				//getting the POSTFIX string 
				GetPostfix(root, &exp);
				//evaluating the postfix string
				cout << "Evaluation of this Expression: " << evaluatePostfix(exp) << endl;
			}
			else
			{
				cout << "Expression from user input is empty!\n" << endl;
			}
			break;
		case 6:
			cout << "Thanks for using my program" << endl;
			break;
		default:
			cout << "Invalid option!" << endl;
		}
	}
	return 0;
}
