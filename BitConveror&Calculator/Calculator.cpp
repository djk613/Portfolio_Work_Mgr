#include "Calculator.h"

string Calculator::expr = "";
int Calculator::n_token_count = 0;

Calculator::Calculator(string expr)
{
	this->expr = expr;
	this->expr.erase(remove_if(this->expr.begin(), this->expr.end(), [](const char ch) {
		return ch == '(' || ch == ')' || ch == ' ';
	}), this->expr.end());

	CheckNegativeUnary();

	n_token_count = 0;
}

Calculator::~Calculator()
{
	delete ps;

	for (int i = 0; i < 300; i++) 
	{
		if (tokens[i])
		{
			delete tokens[i];
		}
	}
}

string Calculator::Calculate()
{
	if (Lexical())
	{
		if (Syntax())
		{
			Parsing();
			PostOrderView();

			string answer = Processing();

			if (answer.at(0) == '-') 
			{
				return '(' + answer + ')';
			}
			else
			{
				return answer;
			}
		}
		else 
		{
			cout << "Wrong Syntax";
			return "";
		}
	}
	else
	{
		cout << "Wrong Lexical" << endl;
		return "";
	}
}

int Calculator::MakeOperator(int i)
{
	tokens[n_token_count] = new Operator(expr.at(i));

	/*unary negative check*/
	if (expr.at(i + 1) == '-') 
	{
		expr.erase(i + 1, 1);
		static_cast<Operator*>(tokens[n_token_count])->m_bRightValueNegative = true;
	}

	n_token_count++;

	return i + 1;
}

int Calculator::MakeOperand(int i)
{
	int startIdx, endIdx;  
	startIdx = i;

	while (IsDigit(expr.at(i)) || IsDifferentNumeralSystem(expr.at(i)))
	{
		i++;

		if (expr.length() <= i) 
		{
			break;
		}
	}

	endIdx = i;

	string str = expr.substr(startIdx, (endIdx - startIdx));

	tokens[n_token_count++] = new Operand(str);

	return endIdx;
}

int Calculator::Lexical()
{
	int i = 0;
	while (expr.at(i))
	{
		if (IsOperator(expr.at(i)))
		{
			i = MakeOperator(i);
		}
		else
		{
			if (IsDigit(expr.at(i)) || IsDifferentNumeralSystem(expr.at(i)))
			{
				i = MakeOperand(i);
			}
			else
			{
				return false;
			}
		}

		if (expr.length() <= i) 
		{
			break;
		}
	}

	return true;
}

bool Calculator::Syntax()
{
	if (n_token_count % 2 == 0)
	{
		return false;
	}
	if (tokens[0]->priority != 3)
	{
		return false;
	}
	for (int i = 1; i < n_token_count; i += 2)
	{
		if (tokens[i]->priority == 3)
		{
			return false;
		}
		if (tokens[i + 1]->priority != 3)
		{
			return false;
		}
	}
	return true;
}

void Calculator::Parsing()
{
	ps = new ParserTree(tokens[0]);

	for (int i = 1; i < n_token_count; i++) 
	{
		ps->Add(tokens[i]);
	}
}

void Calculator::PostOrderView()
{
	ps->View();
}

string Calculator::Processing()
{
	return ps->Calculate();
}

void Calculator::CheckNegativeUnary()
{ 
	if (expr.at(0) == '-' && isdigit(expr.at(1)))
	{
		expr = "0" + expr;
	}
}

bool Calculator::IsOperator(char ch)
{
	return (ch == '+') | (ch == '-') | (ch == '*') | (ch == '/');
}

bool Calculator::IsDigit(char ch)
{
	return (ch >= '0') && (ch <= '9');
}

bool Calculator::IsDifferentNumeralSystem(char ch)
{
	return (ch == 'b') || (ch == 'x');
}
