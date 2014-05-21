#ifndef SOLDECK_H
#define SOLDECK_H
#include "carddeck.h"
#include <fstream>
#include <string>
#include <iostream>

const char SOLDECKSIZE = 54;	// size of the solitaire deck

class SolDeck : public CardDeck	
{
public:

	SolDeck(char);
	SolDeck();

	void Encrypt(std::string p, 
				 std::string &k, 
				 std::string &c);
	/*****
	preconditions:
		p is plaintext string with no punctuation or whitespace
		c and k are empty strings
	postconditions:		
		k is generated keystream based on this deck
		c is generated ciphertext
	*****/

	void Decrypt(std::string &p, 
				 std::string &k, 
				 std::string c);
	/*****
	preconditions:
		c is ciphertext string with no punctuation or whitespace
		p and k are empty strings
	postconditions:		
		k is generated keystream based on this deck
		p is generated plaintext
	*****/

	std::string GetText(std::string);
	/*****
	Gets text from user and
		1. discards punctuation and whitespace
		2. discards all non-alpha characters
		3. converts all characters to uppercase
		4. returns the edited string
	*****/

	void PrintText(std::string,std::ofstream & out);
	/*****
	Prints a string in 5-letter groups
	*****/

	void SetPass(std::string);
	/*****
	Orders the deck using a key or pass phrase
	*****/
	
	int  U2N(char);
	/*****
	Translates an uppercase character (A..Z) to a number (1..26)
	*****/

	char N2U(int);
	/*****
	Translates a number (1..52 or beyond) to an uppercase character (A..Z)
	*****/

	void TripleCut();
	/*****
		preconditions:
			valid card deck with two jokers, e.g.,
				c1, c2, c3, ..., ci, Jok1 ... Jok2, cj, ... , cn
				note that JokA and JokB designations do not matter --
				just use the "first" joker and "second" joker
		postconditions:
			deck has been rearranged:
				cj, ..., cn, Jok1 ... Jok2, c1, ..., ci
	*****/

	void CountCut();
	void CountCut(int i);
	/*****
		preconditions:
			valid card deck, e.g.,
				c1, c2, ..., cn-1, cn
		postconditions:
			deck has been rearranged:
				1. count down i cards into the deck
				2. shift the first i cards to the position just
					before the bottom card (cn)
				3. note that the bottom card (cn) remains in place

		The parameterized version takes a numeric argument,
		and counts down that number of cards.

		The non-parameterized version passes the value of
		the bottom card to the parameterized version.
	*****/

	void MoveJoker(	/* in   */ CardType joker,
					/* in   */ int num_cards);
	/*****
		preconditions:
			valid card deck
			A or B designation for which joker to move
			x tells the number of positions to move the joker
				(typically, 1 or 2)
		postconditions:
			The joker has been found and has been moved "down"
			in the deck x positions.

			Special conditions:
				If x == 1 and joker is on the bottom of the deck,
					new joker position is after 1st card
				If x == 2 
					if joker is in bottom position
						new joker position is after 2nd card
					else if joker is in penultimate position
						new joker position is after 1st card
	*****/
	
	CardType OutputKey();
	/*****
	Produces an output keystream number.

	preconditions:  Deck is defined
	postconditions: Returns a number between 1..52 or
					0 if the output card is a Joker
					
	*****/

	char GenKeyLetter();
	/*****
	Produces a single keystream letter
	*****/

};
#endif
