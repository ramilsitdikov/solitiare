
#include <cctype>
#include "soldeck.h"

/*****
* C O N S T R U C T O R S
*****/
SolDeck::SolDeck(char n)
	: CardDeck(n)
{
}

SolDeck::SolDeck()
	: CardDeck(SOLDECKSIZE)
{
}

/*****
* T r i p l e C u t
*****/
void SolDeck::TripleCut()
/*****
	preconditions:
		valid card deck with two jokers, e.g.,
			c1, c2, c3, ..., ci, Jok1 ... Jok2, cj, ... , cn
			note that JokA and JokB designations do not matter --
			just use the "first" joker and "second" joker
	postconditions:
		deck has been rearranged:
			cj, ..., cn, Jok1 ... Jok2, c1, ..., ci

    Pseudocode:
		Original deck is called "A"
		Find JokA
		Find JokB

		Determine which is first joker
			(larger deck size will be first joker)

		B = Split A deck at first joker
		C = Split B at second joker

		Find 2nd card of C
		D = Split C at second card

		Append D, B, C, A
*****/

{
	CardDeck subFirstJok(0), subSecondJok(0),
		 subEndCards(0);
	subFirstJok = FindCard(JokA);
	subSecondJok = FindCard(JokB);
	
	//This block compares the joker subdecks and
	//swaps decks if the SecondJok > FirstJok.
	if (subFirstJok.Size() < subSecondJok.Size())
	{
		CardDeck temp = subFirstJok;
		subFirstJok = subSecondJok;
		subSecondJok = temp;
	}

	//This block creates four separate decks.
	Split(subFirstJok);
	subFirstJok.Split(subSecondJok);
	subEndCards = subSecondJok.FindNthCard(2);
	subSecondJok.Split(subEndCards);

	//This block appends the decks in triple cut
	//order.

	Append(subEndCards.Append(subFirstJok.Append(subSecondJok.Append(*this))));
}

/*****
* C o u n t C u t
*****/
void SolDeck::CountCut()
{
	CountCut(BottomCard());
}

void SolDeck::CountCut(int n)
/*****
	preconditions:
		valid card deck, e.g.,
			c1, c2, ..., cn-1, cn
	postconditions:
		deck has been rearranged:
			1. check value (i) of bottom card (cn)
				note that either joker counts as 53
			2. shift the first i cards to the position just
				before the bottom card (cn)
			3. note that the bottom card (cn) remains in place				

*****/
{
	CardDeck a(0), b(0), c(0);

	// copy entire deck to a
	a = FindNthCard(1);
	Split(a);
	
	// split off bottom card
	c=a.FindNthCard(a.Size());
	a.Split(c);

	// cut the main deck based on count
	b=a.FindNthCard(n+1);
	a.Split(b);
	
	Append(b.Append(a.Append(c)));
	
	return;
}	

/*****
* M o v e J o k e r
*****/
void SolDeck::MoveJoker(/* in */ CardType joker,
						/* in */ int num_moves)
/*****
	preconditions:
		valid card deck
		joker designation for which joker to move
		num_moves tells the number of positions to move the joker
			(typically, 1 or 2)
	postconditions:
		The joker has been found and has been moved "down"
		in the deck x positions.

		Special conditions:
			If num_moves == 1 and joker is on the bottom of the deck,
				new joker position is after 1st card
			If num_moves == 2 
				if joker is in bottom position
					new joker position is after 2nd card
				else if joker is in penultimate position
					new joker position is after 1st card
*****/
{
	CardDeck b(0);
	CardDeck c(0);
	CardDeck d(0);
	int counter;

	for (counter = 1; counter <= num_moves; counter++)
	{								// A IS THE CLASS INSTANCE

		b = FindCard(joker);		//in a find jokA
		Split(b);					//split a and b
		
	
		if (b.Size()<=1)				//if at bottom of the deck
		{	
			c = FindNthCard(1);	//		in a find 1st card
			Split(c);				//		split a and c
		}
		else						//otherwise
		{
			c = b.FindNthCard(2);	//		in b find 2nd card
			b.Split(c);				//		split b and c
		}

		d = c.FindNthCard(2);		//in c find 2nd card
		c.Split(d);					//split c and d

		Append(c.Append(b.Append(d)));

	}

}

/*****
* O u t p u t K e y
*****/
CardType SolDeck::OutputKey()
/****
Produces an output keystream number.

preconditions:  Deck is defined
postconditions: Returns a number between 1..52 or
				0 if the output card is a Joker

*****/
{
	CardType t;
	CardType n;

	t = TopCard();
	t = (t>JokA) ? JokA : t;

	n = FindNthCard(t+1).TopCard();
	n = (n<JokA) ? n : 0;

	/* debug */
//	std::cout << int(n) << ' ';
	/* gubed */

	return n;
}

/*****
* G e t T e x t
*****/
std::string SolDeck::GetText(std::string kind_of_text)
/*****
Get plaintext or ciphertext from user

  pre:	kind_of_text is set (e.g., plain, key, ciper)
  post:	text received from user is returned as a C++ string
		all punctuation, whitespace, and numbers are stripped
		all lower case letters are translated to upper case
		result may be padded with Xs to make 5-character
			groups

  Note:  The string will only be padded with Xs if the
		first 5 characters of kind_of_text are 'plain'.
*****/
{
	std::cout << "\nEnter "
		<< kind_of_text
		<< " below.  Whitespace and punctuation"
		<< " will be ignored.\n"
		<< "Press <enter> when done."
		<< std::endl;

	char c;
	std::string s = "";

	while ((c = std::cin.get()) != '\n' && c != EOF) {
		if (isalpha(c)) {
			s += char(toupper(c));
		}
	}

	if (kind_of_text.substr(0,5) == "plain")
		while (s.length()%5) 
			s += 'X';
		

	return s;
}

/*****
* P r i n t T e x t
*****/
void SolDeck::PrintText(std::string s,std::ofstream & out)
/*****
Print plaintext or ciphertext in 5-letter groups

  pre:	s is set to an uppercase string w/o whitespace
  post:	s is output in 5-letter groups, 10 groups per line

*****/
{
	for (int n=0; n<s.length(); n++) {
		out << s[n];
		/*if (0 == (n + 1) % 5)
			out << ' ';
		if (0 == (n + 1) % 50) 
			out << "\n\t\t";*/
	}
	out << std::endl;
}


/*****
* E n c r y p t
*****/
void SolDeck::Encrypt(/* in  */ std::string p, 
			 /* out */ std::string &k, 
			 /* out */ std::string &c)
/*****
preconditions:
	p is plaintext string with no punctuation or whitespace
	c and k are empty strings
postconditions:		
	k is generated keystream based on this deck
	c is generated ciphertext
*****/
{
	while (k.length() < p.length()) {
		k += GenKeyLetter();
	}

	for (int i=0; i < p.length(); i++) {
		c += N2U((U2N(p[i]) + U2N(k[i])));
	}
}

/*****
* D e c r y p t
*****/
void SolDeck::Decrypt(/* out */ std::string &p, 
			 /* out */ std::string &k, 
			 /* in  */ std::string c)
/*****
preconditions:
	c is ciphertext string with no punctuation or whitespace
	p and k are empty strings
postconditions:		
	k is generated keystream based on this deck
	p is generated plaintext
*****/
{
	while (k.length() < c.length()) {
		k += GenKeyLetter();
	}

	for (int i=0; i < c.length(); i++) {
		p += N2U((U2N(c[i]) + 26 - U2N(k[i])));
	}
}

/*****
* U 2 N
*****/
int SolDeck::U2N(char u)
/*****
Converts an uppercase character u to an integer 1..26
Presumes an ASCII-like character set where all 
uppercase characters are contiguous
*****/
{
	return int(u) - int('A') + 1;
}

/*****
* N 2 U
*****/
char SolDeck::N2U(int n)
/*****
Converts an integer 1..52 to a character A..Z
*****/
{

	int m = n%26;
	m = (m == 0 ? 26 : m);
	return char(m + int('A') - 1);
}

/*****
* G e n K e y L e t t e r
*****/
char SolDeck::GenKeyLetter()
/*****
Generates one keystream letter using the Solitaire algorithm

  preconditions:
	the deck exists, and is in its previous state

  postconditions:
	one character is output
*****/
{

	int out;
	do{
		MoveJoker(JokA, 1);
		MoveJoker(JokB, 2);
		TripleCut();
		CountCut();
		out = OutputKey();
	}
	while(out==0);
	/*int out;
	CardType top;
	top = TopCard();
	MoveJoker(JokA, 1);
	MoveJoker(JokB, 2);
	TripleCut();
	CountCut();
	out = OutputKey();
	while (top == TopCard())
	{
		MoveJoker(JokA, 1);
		MoveJoker(JokB, 2);
		TripleCut();
		CountCut();
		out = OutputKey();
	}
	*/
	return N2U(out);
}

/*****
* S e t P a s s
*****/
void SolDeck::SetPass(std::string pass)
/*****
Order the deck using a passphrase

  preconditions:
	pass = the passphrase, all uppercase, w/o whitespace

  postconditions:
	the deck has been reordered

  Note:	This implementation does not include the optional
		step for ordering the jokers based on the last
		two letters of the passphrase.
*****/
{
	for (int i=0; i<pass.length(); i++) {
		MoveJoker(JokA, 1);
		MoveJoker(JokB, 2);
		TripleCut();
		CountCut();
		CountCut(U2N(pass[i]));

		/* debug */
//		std::cout << "after " << pass[i] << ":\n";
//		Print();
//		std::cin.get();
		/* gubed */
	}
}
