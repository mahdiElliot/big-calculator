// Tamrine_2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>

using namespace std;

class Pair
{
public:
	int CarryOut;
	string Sum;

	Pair(int cout, string sum)
	{
		CarryOut = cout;
		Sum = sum;
	}
};

class BigCalc
{
private:
	int CHUNK = 2;
	string OutPath;
	string In1Path;
	string In2Path;
	
	Pair PartialSum(string str1, string str2, int carry)
	{
		string str = "";
		int n1 = str1.length(), n2 = str2.length();

		for (int i = 0; i < n1; i++)
		{
			int sum = ((str1[i] - '0') +
				(str2[i] - '0') +
				carry);
			str.push_back(sum % 10 + '0');
			carry = sum / 10;
		}

		Pair result(carry, str);
		return result;
	}

	void Randomize(string input, int maxDigits, int salt)
	{
		ifstream inFile(input);
		ofstream outFile;

		cout << "Generating random digits..." << endl;

		/*bool isFile1Empty = inFile.peek() == ifstream::traits_type::eof();
		if (isFile1Empty)
		{*/
			outFile.open(input, ios::out);
			for (int i = 0; i < maxDigits; i++)
			{
				srand(time(NULL) + rand() % (rand() % ((i % salt) + 1 + salt) + 1));
				int digit = rand() % 10;
				outFile << digit;
			}

			outFile.close();
		/*}
		else
			cout << "Existing file [" << input << "] has a big number\n" << endl;*/
		inFile.close();
	}
	
	bool IsSmaller(string str1, string str2)
	{
		// Calculate lengths of both string 
		int n1 = str1.length(), n2 = str2.length();

		if (n1 < n2)
			return true;
		if (n2 < n1)
			return false;

		//for (int i = 0; i < n1; i++)
		for (int i = n1 - 1; i >= 0; i--)
			if (str1[i] < str2[i])
				return true;
			else if (str1[i] > str2[i])
				return false;

		return false;
	}

	Pair PartialSub(string str1, string str2, int carry)
	{
		/*if (IsSmaller(str1, str2))
			swap(str1, str2);*/

		string str = "";
		int n1 = str1.length(), n2 = str2.length();

		for (int i = 0; i < n2; i++)
		{
			int sub = ((str1[i] - '0') - (str2[i] - '0') - carry);
			if (sub < 0)
			{
				sub += 10;
				carry = 1;
			}
			else
				carry = 0;

			str.push_back(sub + '0');
		}

		Pair result(carry, str);
		return result;
	}

public:
	BigCalc(string inPath, string outPath)
	{
		In1Path = inPath;
		OutPath = outPath;
	}

	BigCalc(string in1Path, string in2Path, string outPath)
	{
		In1Path = in1Path;
		In2Path = in2Path;
		OutPath = outPath;
	}

	void SetChunk(int _chunk)
	{
		if (_chunk > 0)
			CHUNK = _chunk;
	}

	void Sum()
	{
		ifstream inFile1(In1Path);
		ifstream inFile2(In2Path);

		ofstream outFile;
		outFile.open(OutPath, ios::out);
		outFile.close();

		int num1Digits = -1;
		int num2Digits = -1;
		int temp;
		char eachDigit;

		while (inFile1)
		{
			inFile1.get(eachDigit);
			temp = eachDigit;

			if (temp > 47 && temp < 58)
				num1Digits++;
		}

		while (inFile2)
		{
			inFile2.get(eachDigit);
			temp = eachDigit;

			if (temp > 47 && temp < 58)
				num2Digits++;
		}

		if (num1Digits == num2Digits)
		{
			string num1;
			string num2;
			string sum = "";
			int carry = 0;

			cout << "Calculating summation..." << endl;

			inFile1.clear();
			inFile1.seekg(0, ios::beg);
			inFile2.clear();
			inFile2.seekg(0, ios::beg);

			for (int i = 0, step; i < num1Digits; i += step)
			{
				char temp1;
				char temp2;
				step = (num1Digits - i < CHUNK) ? num1Digits - i : CHUNK;

				for (int j = i + 1; j < i + step + 1; j++)
				{
					inFile1.seekg(-j, ios::end);
					inFile1.get(temp1);
					num1 += temp1;

					inFile2.seekg(-j, ios::end);
					inFile2.get(temp2);
					num2 += temp2;
				}

				Pair result = PartialSum(num1, num2, carry);
				sum += result.Sum;
				carry = result.CarryOut;

				num1 = "";
				num2 = "";
			}

			sum += (carry > 0) ? to_string(carry) : "";
			reverse(sum.begin(), sum.end());
			//string carryStr = (carry > 0) ? to_string(carry) : "";
			//cout << "Sum: " << carryStr << sum << endl;

			ofstream outFile;
			outFile.open(OutPath, ios::out);
			outFile << sum;
			outFile.close();
		}
		else
			cout << "* Lengths are not equal. *" << endl;
	}

	void Sub()
	{
		bool isNegative = false;
		ifstream inFile1(In1Path);
		ifstream inFile2(In2Path);

		int num1Digits = -1;
		int num2Digits = -1;
		int temp;
		char eachDigit;

		while (inFile1)
		{
			inFile1.get(eachDigit);
			temp = eachDigit;

			if (temp > 47 && temp < 58)
				num1Digits++;
		}

		while (inFile2)
		{
			inFile2.get(eachDigit);
			temp = eachDigit;

			if (temp > 47 && temp < 58)
				num2Digits++;
		}

		if (num1Digits == num2Digits)
		{
			string num1;
			string num2;
			string sub = "";
			int carry = 0;

			inFile1.clear();
			inFile1.seekg(0, ios::beg);
			char firstDigit1;
			inFile1.get(firstDigit1);

			inFile2.clear();
			inFile2.seekg(0, ios::beg);
			char firstDigit2;
			inFile2.get(firstDigit2);

			if ((firstDigit1 - '0') < (firstDigit2 - '0'))
				isNegative = true;

			cout << "Calculating substraction..." << endl;

			inFile1.clear();
			inFile1.seekg(0, ios::beg);
			inFile2.clear();
			inFile2.seekg(0, ios::beg);

			for (int i = 0, step; i < num1Digits; i += step)
			{
				char temp1;
				char temp2;
				step = (num1Digits - i < CHUNK) ? num1Digits - i : CHUNK;

				for (int j = i + 1; j < i + step + 1; j++)
				{
					inFile1.seekg(-j, ios::end);
					inFile1.get(temp1);
					num1 += temp1;

					inFile2.seekg(-j, ios::end);
					inFile2.get(temp2);
					num2 += temp2;
				}

				if (isNegative)
					swap(num1, num2);

				Pair result = PartialSub(num1, num2, carry);
				sub += result.Sum;
				carry = result.CarryOut;

				num1 = "";
				num2 = "";

				/*reverse(sub.begin(), sub.end());

				ofstream outFile;
				outFile.open(OutPath, ios::app);
				outFile.seekp(0, ios::beg);
				outFile << sub;
				outFile.close();*/
			}

			/*ofstream outFile;
			outFile.open(OutPath, ios::app);
			outFile.seekp(0, ios::beg);*/

			if (isNegative)
				sub += "-";

			reverse(sub.begin(), sub.end());

			ofstream outFile;
			outFile.open(OutPath, ios::out);
			outFile << sub;
			outFile.close();
		}
		else
			cout << "* Lengths are not equal. *" << endl;
	}

	void FillFile(int maxDigits)
	{
		Randomize(In1Path, maxDigits, 16512);
		Randomize(In2Path, maxDigits, 32426);
	}

	BigCalc operator + (BigCalc const &other)
	{
		string in1Path = this->In1Path;
		string in2Path = other.In1Path;
		BigCalc result(in1Path, in2Path, other.OutPath);
		result.Sum();

		return result;
	}

	BigCalc operator - (BigCalc const &other)
	{
		string in1Path = this->In1Path;
		string in2Path = other.In1Path;
		BigCalc result(in1Path, in2Path, other.OutPath);
		result.Sub();

		return result;
	}

	~BigCalc() {}
};

int main()
{
	BigCalc aa("number1.txt", "number2.txt", "output.txt");
	aa.SetChunk(300);
	aa.FillFile(100000);
	//aa.Sum();
	aa.Sub();

	BigCalc num1("number1a.txt", "outputA.txt");
	num1.SetChunk(4);
	BigCalc num2("number1b.txt", "outputB.txt");
	num1.FillFile(10);
	num2.FillFile(10);
	BigCalc num3 = num1 + num2;
	BigCalc num4 = num1 - num2;
}