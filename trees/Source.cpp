#include <iostream>
#include <string>
#include <locale>

using namespace std;

class MyString : public string
{
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
		erase(0);
		erase(length() - 1);
		}
	}
	int find_last_of_ignore_brackets(string ptr)
	{
		int depth = 0;
		for (int i = length() - 1; i >= 0; i--) {
			if (this[i] == ")")
				depth++;
			else if (this[i] == "(")
				depth--;
			else if (depth == 0)
				if (ptr.find(this[i]) != string::npos)
					return i;
		}
		return string::npos;
	}
};

std::string ltrim(std::string str, const std::string chars = "\t\n\v\f\r ")
{
	str.erase(0, str.find_first_not_of(chars));
	return str;
}

std::string rtrim(std::string str, const std::string chars = "\t\n\v\f\r ")
{
	str.erase(str.find_last_not_of(chars) + 1);
	return str;
}

std::string trim(std::string str, const std::string chars = "\t\n\v\f\r ")
{
	return ltrim(rtrim(str, chars), chars);
}
std::string symmetrical_trim(std::string str, const std::string left = "\t\n\v\f\r ", const std::string right = "\t\n\v\f\r ")
{
	while ((str.find_first_of(left) == 0) && (str.find_last_of(right) == str.length() - 1))
		str = str.substr(1, str.length() - 2);
	//cout << str << endl;
	return str;
}

int find_last_of_ignore_brackets(string str, string ptr)
{
	int depth = 0;
	for (int i = str.length() - 1; i >= 0; i--) {
		if (str[i] == ')')
			depth++;
		else if (str[i] == '(')
			depth--;
		else if (depth == 0)
			if (ptr.find(str[i]) != string::npos)
				return i;
	}
	return string::npos;
}




class Data {
public:
	double num;
	bool isSign;
	string sign;
	Data* left;
	Data* right;
public:

	Data(string eq)
	{
		eq = symmetrical_trim(eq, "(", ")");
			
		int sign = 0;
		sign = find_last_of_ignore_brackets(eq, "+-");

		if (sign == string::npos)
			sign = find_last_of_ignore_brackets(eq, "*/");

		if (sign == string::npos) {
			this->isSign = false;
			this->num = atof(eq.c_str());
			left = nullptr;
			right = nullptr;
		}
		else {
			this->isSign = true;
			this->sign = eq[sign];
			left = new Data(trim(eq.substr(0, sign )));
			right = new Data(trim(eq.substr(sign + 1, eq.length() - 1)));
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
	string input;
	cin >> input;
	
	Data* tree = new Data(trim(input));
	LRN(&tree);
	
	cout << tree->num << endl;

}

