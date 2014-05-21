#include <iostream>
#include <clocale>
#include <vector>
#include "soldeck.h"
#include "mtrand.h"

using namespace std;

void getcards(SolDeck&);
void shuffleWarning();
void getTxt(vector<string>&, ifstream&, int, int);
void analysis(vector<string> &, vector<string> &, vector<string> &, int *, int *, int *, int, double &);
void printAlpha(int *, int *, int *, int );
void analysis2(vector<string> &, vector<string> &, int);
bool letter(char);

int main() {
	setlocale(LC_CTYPE, "rus");

	/*ofstream outMes("messages.txt");//объявление выходных потоков   для генерации сообщений
	ofstream outCip("ciphermessages.txt"); // для создания шифросообщений*/

	SolDeck deck(0);
	SolDeck temp;
	MTRand_int32 forMes;
	srand(time(NULL));
	deck.Append(temp);//Начальная конфигурация колоды
	deck.Shuffle();
	int mesNum = 0;//Количество сообщений
	int expNum = 0;//Количество экспериментов
	int letNum = 100;//Количество символов в конкретном сообщении
	char key1, key2;
	char prevTopCard;
	
	cout<<"Введите количество экспериментов:"<<endl;
	cin>>expNum;
	cout << "Введите количество сообщений:"<<endl;
	cin >> mesNum;
	vector<int> topMatch(expNum), letMatch(expNum), topAndLetMatch(expNum), noTopAndLetMatch(expNum);
	double sumTopMatch=0, sumLetMatch=0, sumTopAndLetMatch=0, sumNoTopAndLetMatch=0;
	/*Сбор информации-статистики*/////////////////////////////////////////////////////////
	for(int j=0;j<expNum;j++)
	{
		topMatch[j]=0;
		letMatch[j]=0;
		topAndLetMatch[j]=0;
		noTopAndLetMatch[j]=0;
		for(int i=0;i<mesNum;i++)
		{	
			key1=deck.GenKeyLetter();
			prevTopCard = deck.TopCard();
			for(int k=1;k<letNum;k++)
			{
				key2 = deck.GenKeyLetter();
				if(prevTopCard == deck.TopCard())
				{
					topMatch[j]++;
					if(key1 == key2)
						topAndLetMatch[j]++;
				}
				else{
					if(key1 == key2)
						noTopAndLetMatch[j]++;
				}
				if(key1 == key2)
					letMatch[j]++;
				key1 = key2;
				prevTopCard = deck.TopCard();
			}
		}
		sumTopMatch+=topMatch[j];
		sumLetMatch+=letMatch[j];
		sumTopAndLetMatch+=topAndLetMatch[j];
		sumNoTopAndLetMatch+=noTopAndLetMatch[j];
	}
	
	/*контрольный тест*/ 
	int topMatch1=0,topAndLetMatch1=0,noTopAndLetMatch1=0,letMatch1=0;
	int mesNum1 = mesNum;
		//mesNum*10000;
	for(int i=0;i<mesNum1;i++)
		{	
			key1=deck.GenKeyLetter();
			prevTopCard = deck.TopCard();
			for(int k=1;k<letNum;k++)
			{
				key2 = deck.GenKeyLetter();
				if(prevTopCard == deck.TopCard())
				{
					topMatch1++;
					if(key1 == key2)
						topAndLetMatch1++;
				}
				else{
					if(key1 == key2)
						noTopAndLetMatch1++;
				}
				if(key1 == key2)
					letMatch1++;
				key1 = key2;
				prevTopCard = deck.TopCard();
			}
		}
	int allLet1 = mesNum1*(letNum-1);
	/*анализ информации*//////////////////////////////////////////////////////////
	
	int allLet = mesNum*(letNum-1)*expNum;

	double averTopMatch=sumTopMatch/expNum;
	double averLetMatch=sumLetMatch/expNum;
	double averTopAndLetMatch=sumTopAndLetMatch/expNum;
	double averNoTopAndLetMatch=sumNoTopAndLetMatch/expNum;

	double sigma1=0, sigma2=0,sigma3=0,sigma4=0;



/* Поиск сигм*///////////////////////////////////////////////////////////////
	for(int i=0;i<expNum;i++)
	{
		sigma1+=pow(topMatch[i]-averTopMatch,2);
		sigma2+=pow(letMatch[i]-averLetMatch,2);
		sigma3+=pow(topAndLetMatch[i]-averTopAndLetMatch,2);
		sigma4+=pow(noTopAndLetMatch[i]-averNoTopAndLetMatch,2);
	}
	sigma1=sqrt(sigma1/(expNum-1));
	sigma2=sqrt(sigma2/(expNum-1));
	sigma3=sqrt(sigma3/(expNum-1));
	sigma4=sqrt(sigma4/(expNum-1));

	cout<<endl<<"sigma1="<<sigma1<<endl
		<<"sigma2="<<sigma2<<endl
		<<"sigma3="<<sigma3<<endl
		<<"sigma4="<<sigma4<<endl;


	cout<<"проверка!!!"
		<<averTopMatch<<endl
		<<averLetMatch<<endl
		<<averTopAndLetMatch<<endl
		<<averNoTopAndLetMatch<<endl;
	/*преобразования*////////////////////////////////////////////
	double p1=0.0192044, p2=0.0384615;//Вероятности выпадения конкретной карты из колоды и конкретной буквы из алфавита
	double pTopMatch = (double)topMatch1/(double)allLet1;
	double pLetMatch = (double)letMatch1/(double)allLet1;
	double pTopAndLetMatch = (double)topAndLetMatch1/(double)topMatch1;
	double pNoTopAndLetMatch = (double)noTopAndLetMatch1/(double)(allLet1-topMatch1);
	double betweenTopMatch = pTopMatch - p1;
	double betweenLetMatch = pLetMatch - p2;
	double betweenTopAndLetMatch = pTopAndLetMatch - p2;
	double betweenNoTopAndLetMatch = pNoTopAndLetMatch - p2;
	/*основной выход*//////////////////////////////////////////////////
	cout<<"Совпадение верхних карт колоды:"<<topMatch1<<"/"<<allLet1<<"="<<pTopMatch<<endl
		<<"Увеличение на "<<betweenTopMatch/p1<<endl
		<<"Совпадений всего:"<<letMatch1<<"/"<<allLet1<<"="<<pLetMatch<<endl
		<<"Увеличение на "<<betweenLetMatch/p2<<endl
		<<"Совпадений при одинаковых верхних картах:"<<topAndLetMatch1<<"/"<<topMatch1<<"="<<pTopAndLetMatch<<endl
		<<"Увеличение на "<<betweenTopAndLetMatch/p2<<endl
		<<"Совпадений при разных верхних картах:"<<noTopAndLetMatch1<<"/"<<(allLet1-topMatch1)<<"="<<pNoTopAndLetMatch<<endl
		<<"Увеличение на "<<betweenNoTopAndLetMatch/p2<<endl;

	/*генерация mesNum сообщений
	/*for (int i = 0; i < mesNum; i++){
		outMes << i << " ";
		for (int k = 0; k < letNum; k++){
			rand = forMes.operator()()%26;
			outMes << deck.N2U(rand);
		}
		outMes << "(" << letNum << ") " << '\n';
	}
	/*char mesLet;
	for (int i = 0; i < mesNum; i++){
		outMes << int(i) << " B";
		for (int k = 0; k < letNum; k += 0){
			in >> mesLet;
			if (isalpha(mesLet))
			{
				k++;
				cout << mesLet;
				outMes << mesLet;
			}
		}
		outMes << "(" << letNum << ") " << '\n';
		cout << endl;
	}
	
	outMes << ".";
	outMes.close();
/////////////////////////////////////////////////////////////////////
	ifstream pin("in.txt");
	mes.resize(mesNum);
	cipher.resize(mesNum);
	keystream.resize(mesNum);
	getTxt(mes, pin, mesNum, letNum);
	pin.close();
	for (int i = 0; i < mesNum; i++)
	{
		deck.Encrypt(mes[i], keystream[i], cipher[i]);
		outCip << i << " ";
		deck.PrintText(cipher[i],outCip);
	}
	outCip.close();
////////////////////////////////////////////////////////////////////
	/*double numLetters = mesNum * letNum, equals=0;
	int alphabet1[26], alphabet2[26], alphabet3[26];
	analysis(mes, cipher, keystream, alphabet1, alphabet2, alphabet3, mesNum, equals);
	double a = (equals / numLetters) * 100.0;
	cout << "Количество совпадающих букв= " << equals << endl << "Количество букв во всех сообщениях= " << numLetters << endl << "Процент совпадающих букв= " << a << "%"<<endl;
	printAlpha(alphabet1, alphabet2, alphabet3, mesNum);
	analysis2(keystream, cipher, mesNum);*/


	cout << endl; 
	return 0;
}

bool letter(char c)
{
	if(c<'z'&& c>'a' || c<'Z' && c>'A')
		return true;
	else return false;
}

void getTxt(vector<string>& mes,
			ifstream & in,
			int N,int K)
/*****
*Считыввает сообщения из файла
******/
{
	char z;
	int i=0,k;
	string s = "";
	z = in.get();
	while (i<N && !in.eof()){
		k=0;
		while (k<K && !in.eof())
		{
			if (letter(z))
			{	
				k++;
				s += char(toupper(z));
			}
			z = in.get();
		}
		i++;
		mes[i - 1] = s;
		s = "";
	}
}

void analysis(vector<string> &mes,
			  vector<string> &cipher,
			  vector<string> &key,
			  int alphabet1[26],
			  int alphabet2[26],
			  int alphabet3[26],
			  int N,
			  double &equals)
/*****
*Считает количество совпадений текста с шифротекстом
*Считает количество посторений каждой буквы в тексте и шифротексте
******/
{	
	double equalsTemp, maxEq = 0, a;
	int tempi;
	for (int i = 0; i < 26; i++)
	{
		alphabet1[i] = 0;
		alphabet2[i] = 0;
		alphabet3[i] = 0;
	}
	for (int i = 0; i < N; i++)
	{
		equalsTemp = 0;
		for (int k = 0; k < mes[i].length(); k++)
		{
			if (mes[i][k] == cipher[i][k])//Сколько раз буква переходит в саму себя
				equalsTemp+=1;



			switch (mes[i][k])//Считаем количество каждой буквы в исходном тексте		
			{
			case 'A':alphabet1[0]++; break;
			case 'B':alphabet1[1]++; break;
			case 'C':alphabet1[2]++; break;
			case 'D':alphabet1[3]++; break;
			case 'E':alphabet1[4]++; break;
			case 'F':alphabet1[5]++; break;
			case 'G':alphabet1[6]++; break;
			case 'H':alphabet1[7]++; break;
			case 'I':alphabet1[8]++; break;
			case 'J':alphabet1[9]++; break;
			case 'K':alphabet1[10]++; break;
			case 'L':alphabet1[11]++; break;
			case 'M':alphabet1[12]++; break;
			case 'N':alphabet1[13]++; break;
			case 'O':alphabet1[14]++; break;
			case 'P':alphabet1[15]++; break;
			case 'Q':alphabet1[16]++; break;
			case 'R':alphabet1[17]++; break;
			case 'S':alphabet1[18]++; break;
			case 'T':alphabet1[19]++; break;
			case 'U':alphabet1[20]++; break;
			case 'V':alphabet1[21]++; break;
			case 'W':alphabet1[22]++; break;
			case 'X':alphabet1[23]++; break;
			case 'Y':alphabet1[24]++; break;
			case 'Z':alphabet1[25]++; break;
			}
			switch (cipher[i][k])//Считаем количество каждой буквы в шифротексте
			{
			case 'A':alphabet2[0]++; break;
			case 'B':alphabet2[1]++; break;
			case 'C':alphabet2[2]++; break;
			case 'D':alphabet2[3]++; break;
			case 'E':alphabet2[4]++; break;
			case 'F':alphabet2[5]++; break;
			case 'G':alphabet2[6]++; break;
			case 'H':alphabet2[7]++; break;
			case 'I':alphabet2[8]++; break;
			case 'J':alphabet2[9]++; break;
			case 'K':alphabet2[10]++; break;
			case 'L':alphabet2[11]++; break;
			case 'M':alphabet2[12]++; break;
			case 'N':alphabet2[13]++; break;
			case 'O':alphabet2[14]++; break;
			case 'P':alphabet2[15]++; break;
			case 'Q':alphabet2[16]++; break;
			case 'R':alphabet2[17]++; break;
			case 'S':alphabet2[18]++; break;
			case 'T':alphabet2[19]++; break;
			case 'U':alphabet2[20]++; break;
			case 'V':alphabet2[21]++; break;
			case 'W':alphabet2[22]++; break;
			case 'X':alphabet2[23]++; break;
			case 'Y':alphabet2[24]++; break;
			case 'Z':alphabet2[25]++; break;
			}
			switch (key[i][k])//Считаем количество каждой буквы в исходном тексте		
			{
			case 'A':alphabet3[0]++; break;
			case 'B':alphabet3[1]++; break;
			case 'C':alphabet3[2]++; break;
			case 'D':alphabet3[3]++; break;
			case 'E':alphabet3[4]++; break;
			case 'F':alphabet3[5]++; break;
			case 'G':alphabet3[6]++; break;
			case 'H':alphabet3[7]++; break;
			case 'I':alphabet3[8]++; break;
			case 'J':alphabet3[9]++; break;
			case 'K':alphabet3[10]++; break;
			case 'L':alphabet3[11]++; break;
			case 'M':alphabet3[12]++; break;
			case 'N':alphabet3[13]++; break;
			case 'O':alphabet3[14]++; break;
			case 'P':alphabet3[15]++; break;
			case 'Q':alphabet3[16]++; break;
			case 'R':alphabet3[17]++; break;
			case 'S':alphabet3[18]++; break;
			case 'T':alphabet3[19]++; break;
			case 'U':alphabet3[20]++; break;
			case 'V':alphabet3[21]++; break;
			case 'W':alphabet3[22]++; break;
			case 'X':alphabet3[23]++; break;
			case 'Y':alphabet3[24]++; break;
			case 'Z':alphabet3[25]++; break;
			}
		}
		a = equalsTemp / mes[i].length() * 100;
		//cout << "Процент совпедений в "<<i<<"-м сообщении = " << a << "%" << endl;
		equals += equalsTemp;
		if (equalsTemp > maxEq)
		{
			maxEq = equalsTemp;
			tempi = i;
		}
	}
	cout << "Максимальный процент совпадений в " << tempi << "-м сообщении " << maxEq << "%" << endl;
	cout << endl;
}

void printAlpha(int alphabet1[26],
				int alphabet2[26],
				int alphabet3[26],
				int N)
/*****
*Печатает количество каждой буквы
*****/
{
	cout << endl << "|Распределение букв"<<endl
		<<"    в сообщениях:    " <<
		"в шифротексте:       " <<
		"в гамме:   "  <<endl;
	for (int i = 0; i < 26; i++)
	{
		cout << endl << char(int('A') + i) << "            "
			<< alphabet1[i]
			<< "          	" << alphabet2[i]
			<< "            " << alphabet3[i] << " - это " << double((double)alphabet3[i] / ((double)N * 100) * 100) << "%";
		
	}
	cout << endl;
}

void analysis2(vector<string> &key,
			   vector<string> &cipher,
				int N)
/*****
*Процент совпадений 2-х подряд идущих букв в гамме и в шифротексте
*****/
{
	double equalInKey, equalInCipher, inKey = 0, inCipher = 0;
	for (int i = 0; i < N; i++)
	{	
		equalInKey = 0, equalInCipher = 0;
		for (int k = 0; k < key[i].length()-1; k++)
		{
			if (key[i][k] == key[i][k + 1])
				equalInKey += 1;
			if (cipher[i][k] == cipher[i][k + 1])
				equalInCipher += 1;
		}
		inKey += (equalInKey / (double)(key[i].length() - 1)) * 100;
		inCipher += (equalInCipher / (double)(cipher[i].length() - 1)) * 100;
		cout << "Проценты в гамме:" << (equalInKey / (double)(key[i].length() - 1)) * 100 <<
	"   Проценты в шифротексте:" << (equalInCipher / (double)(cipher[i].length() - 1)) * 100 << endl;
	}
	cout << "Процент совпадений 2-х подряд идущих букв в гамме=" << inKey/N  << endl;
	cout << "Процент совпадений 2-х подряд идущих букв в шифротексте=" << inCipher/N  << endl;
}


/*
void getcards(SolDeck &deck)
/*****
Пользователь устанавливает порядок карт в колоде, указанный пользователем

предусловия:
колода пуста

результат:
колода содержит карты в порядке, указанном пользователем
*****
{
cout << "\n\t>>==> Внимание! <==<<\n"
<< "Вы ответственны за постановку всех 54 карт.\n"
<< "Эта подрограмма не проверяет, "
<< "корректно ли Вы определили колоду."
<< "\nИспользуйте эту опцию на Ваш риск\n"
<< endl;

cout << "Enter\n\ta face value (A, 2, 3, ..., 10, J, Q, K),"
<< "\n\ta space,\n\tand a suit (C, D, H, S)."
<< "\nFor the jokers, enter \"Jo A\" or \"Jo B\"."
<< "\nEach card may be separated from the next "
<< "by a space or a return."
<< "\nContinue entering until all 54 cards have "
<< "been specified.\n"
<< endl;

string	face;
char	suit;

while (deck.Size() < SOLDECKSIZE) {
cin >> face >> suit;
deck.AddCard(face, suit);
}
}

void shuffleWarning()
/*****
Печатает предосторежении о псевдослучайности колоды
****
{
cout << "\n\t>>==> Внимание! <==<<\n"
<< "Эта перетасовка основана на способности\n"
<< "Вашего компьтера производить псевдослучайные числа.\n"
<< "Могут быть слабые места "
<< "при использовании этой перетасовки.\n"
<< endl;
}
*/
