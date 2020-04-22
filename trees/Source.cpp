#include <iostream>
#include <string>
#include <locale>

using namespace std;

class MyString : public string
{
public:
	void ltrim(const std::string chars = "\t\n\v\f\r ")
	{
		erase(0, find_first_not_of(chars));
	}
	void rtrim(const std::string chars = "\t\n\v\f\r ")
	{
		erase(find_last_not_of(chars) + 1);
	}
	void trim(const std::string chars = "\t\n\v\f\r ")
	{
		ltrim();
		rtrim();
	}
	void symmetrical_trim(const std::string left = "\t\n\v\f\r ", const std::string right = "\t\n\v\f\r ")
	{
		while ((find_first_of(left) == 0) && (find_last_of(right) == length() - 1)) {
			erase(0, 1);
			erase(length() - 1, 1);
		}
	}
	//Expressions in brackets will be processed later, after everything around them is processed 
	int find_last_of_ignore_brackets(string ptr)
	{
		int depth = 0;
		for (int i = length() - 1; i >= 0; i--) {
			if (this->c_str()[i] == ')')
				depth++;
			else if (this->c_str()[i] == '(')
				depth--;
			else if (depth == 0)
				if (ptr.find(this->c_str()[i]) != string::npos)
					return i;
		}
		return string::npos;
	}
	
};

class Data {
public:
	double num;
	bool isSign;
	string sign;
	Data* left;
	Data* right;
public:

	Data(MyString& eq)
	{
		eq.trim();
		eq.symmetrical_trim("(", ")");
		eq.trim();

		int sign = 0;
		sign = eq.find_last_of_ignore_brackets("+-");

		if (sign == string::npos)
			sign = eq.find_last_of_ignore_brackets("*/");

		if (sign == string::npos) {
			this->isSign = false;
			this->num = atof(eq.c_str());
			left = nullptr;
			right = nullptr;
		}
		else {
			this->isSign = true;
			this->sign = eq[sign];
			string leftStr = eq.substr(0, sign);
			string rightStr = eq.substr(sign + 1, eq.length() - 1);
			left = new Data(static_cast<MyString&>(leftStr));
			right = new Data(static_cast<MyString&>(rightStr));
		}
	}
	string Get() {
		if (isSign)
			return sign;
		else
			return to_string(num);
	}
};

void LRN(Data** node)
{
	if (*node != nullptr)
	{
		LRN(&((*node)->left));
		LRN(&((*node)->right));

		if (((*node)->isSign) && (!(*node)->left->isSign) && (!(*node)->right->isSign))
		{
			switch ((*node)->sign[0])
			{
			case '+':
				(*node)->num = (*node)->left->num + (*node)->right->num;
				break;
			case '-':
				(*node)->num = (*node)->left->num - (*node)->right->num;
				break;
			case '*':
				(*node)->num = (*node)->left->num * (*node)->right->num;
				break;
			case '/':
				(*node)->num = (*node)->left->num / (*node)->right->num;
				break;
			}
			(*node)->isSign = false;

		}

	}
}


int main() {
	MyString input;
	cin >> input;
	input.trim();

	Data* tree = new Data(input);
	int a = 0;
	LRN(&tree);
	
	cout << tree->Get() << endl;

}

