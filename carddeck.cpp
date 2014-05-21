#include <iostream>
#include "carddeck.h"
#include "mtrand.h"

using namespace std;

/*****
* C O N S T R U C T O R S
*****/
CardDeck::CardDeck()		// default initializer
/*****
pre:	none
post:	puts deck in bridge order(крести,буби,червы,пики), followed
		by the A and B jokers
		numcards set to the number of cards
*****/
{
	CardDeck(MAXDECK);			// call parameterized version
}

CardDeck::CardDeck(char n)		// initializer w/ num of cards
/*****
pre:	n <= MAXDECK
post:	puts deck in bridge order up to n cards
		numcards set to the number of cards (n)

Notes:
	The cards are stored in a linked list.
	Each node of the list contains a card value and
	a pointer to the next node.
	Card values are numeric as follows:
		 1..13	= A..K of Clubs
		14..26	= A..K of Diamonds
		27..39	= A..K of Hearts
		40..52	= A..K of Spades
		53		= Joker A
		54		= Joker B
*****/
{
	if (n > MAXDECK) {
		cout << int(n) << " is larger than the maximum size of "
			<< int(MAXDECK) << ". " << int(MAXDECK) << " assumed."
			<< "\n(Are you sure YOU are playing with a full deck?)" 
			<< endl;
		n = MAXDECK;
	}

	//  создаём первый узел и указываем его в качестве начала
	// также следующим элментом указываем сам первый узел	
	CardPtr currPtr;
	currPtr->next = currPtr = head = new CardNode;

	for (numcards=0; numcards<n; numcards++) {//заполняем колоду 
		currPtr = currPtr->next;		// move to next node
		currPtr->card = numcards+1;		// initialize card value
		currPtr->next = new CardNode;	// create next node
	}


	// был создан один дополнительный узел, поэтому
	delete currPtr->next;		// избавляемся от него

	if (numcards>0) {
		currPtr->next = 0;		// point last node to null
		tail = currPtr;			// point tail to last node
	}
	else
		head = tail = 0;
}	

/*****
* P r i n t
*****/
void CardDeck::Print() const
/*****
 Print out the entire card deck in a user-friendly way

 pre: numcards cards in the deck
 post: the deck has been printed
*****/
{
	cout << "Current deck contents:" << endl;
	
	if (!head) {
		cout << "\tThere are no cards in the deck." << endl;
		return;
	}
	
	CardPtr currPtr = head;

	int i=1;		// track number of cards printed

	do {
		PrintCard(currPtr);
		
		// output endl every 13 cards
		if ((i++) % 13 == 0)
			cout << endl;

	} while (currPtr = currPtr->next);	// tricky use of assignment

/* the following lines are for debugging only */
//	cout << "\nFirst card = ";
//	PrintCard(head);
//	cout << "\tLast card =  ";
//	PrintCard(tail);
//	cout << int(BottomCard());
//	cout << "\tnumcards = " << Size();
/* end debugging junk */

	cout << endl;

}

/*****
* P r i n t C a r d
*****/
void CardDeck::PrintCard(CardPtr currPtr) const
/*****
Prints a single card
Pre:	currPtr points to a card node
Post:	the card has been printed
*****/
{
	/*****
	The following constants work for MS Visual C++'s
	extended ASCII character set.  They may or may not
	work under other implementations.
	*****/
	const char CLUB		= '\005';//символ крести
	const char DIAMOND	= '\004';//символ буби
	const char HEART	= '\003';//символ червы
	const char SPADE	= '\006';//символ пики
	

	if (currPtr->card == JokA)
		cout << "JoA ";
	else if (currPtr->card == JokB)
		cout << "JoB ";
	else {
		int c = currPtr->card%13;
		switch (c) {
			case 1:		cout << "A"; break;
			case 11:	cout << "J"; break;
			case 12:	cout << "Q"; break;
			case 0:		cout << "K"; break;
			default:	cout << c; break;
		} // end switch

		if (currPtr->card <= 13)
			cout << CLUB;
		else if (currPtr->card <= 26)
			cout << DIAMOND;
		else if (currPtr->card <= 39)
			cout << HEART;
		else if (currPtr->card <= 52)
			cout << SPADE;
		
		// spacing routine
		if (c == 10)
			cout << " ";
		else
			cout << "  ";

	} // end else switch
}

/*****
* F i n d C a r d
*****/
CardDeck CardDeck::FindCard(/* in */ CardType c) const
/*****
Pre:	c is a card to find in the deck
Post:	if c exists in the deck
			return new subdeck with 
				head -> c's node
				tail -> deck's tail
				correct numcards
		else
			return empty subdeck

Written: 20jun00 dlr
*****/
{
	CardDeck subDeck(0);	// create an empty deck
	subDeck.head = head;
	subDeck.tail = tail;
	subDeck.numcards = numcards;

	while (subDeck.numcards > 0) {
		if (c == subDeck.head->card)
			break;						// found it!
		subDeck.head = subDeck.head->next;
		subDeck.numcards--;
	}

	if (subDeck.numcards == 0)
		subDeck.head = subDeck.tail = 0;	// if not found, subDeck is empty

	return subDeck;
}

/*****
* S p l i t
*****/
void CardDeck::Split(/* in */ const CardDeck sub)
/*****
pre:	'sub' points to a subdeck within the deck
		sub.head MUST point to a node in the deck, or must be null.

post:	deck = deck@entry without subdeck 'sub'
		sub = sub@entry

Written:  20jun00 dlr.
*****/
{
	if (head == sub.head) {		// this deck will have no cards left
		head = tail = 0;
		numcards = 0;
		return;
	}

	if (sub.head == 0)			// sub has no cards in it
		return;

	numcards = 1;				// initialize counter for deck
	tail = head;				// init tail to point to head

	while (tail->next != sub.head) {	// search for ptr to sub
		tail = tail->next;
		numcards++;
	}

	tail->next = 0;				// point last node to null
	return;
}

/*****
* F i n d N t h C a r d
*****/
CardDeck CardDeck::FindNthCard(int n) const
/*****
Pre:	n is the position of a card in the deck
		n >= 1
		(the first card in the deck is #1)

Post:	if n <= numcards
			return new subdeck with head -> c's node
		else
			return empty subdeck

Written:  21jun00 by CIS217 class
*****/
{
	CardDeck subDeck(0);	// create an empty deck

	if (n < 1 || n > numcards)		// n==0 might be valid in some (strange) circumstances 
									// n>numcards is invalid input
		return subDeck;				// In either case, return an empty deck

	subDeck.head = head;
	subDeck.tail = tail;
	subDeck.numcards = numcards;

	for (int count = 1; count < n; count++) {
		subDeck.head = subDeck.head->next;
		subDeck.numcards--;
	}

	return subDeck;
}

/*****
* A p p e n d
*****/
CardDeck CardDeck::Append(/* inout */ CardDeck &sub)
/*****
pre:	'sub' points to a (possibly empty) subdeck which will be
		appended to the end of the current deck
		
post:	deck = deck@entry + sub@entry
		sub points to an empty deck

Written:  21jun00 dlr.
*****/
{
	if (sub.numcards == 0) {
		return *this;					// nothing to do
	}

	if (numcards == 0) {		// no cards in main deck
		head = sub.head;
		tail = sub.tail;
		numcards = sub.numcards;
	}
	else {
		tail->next = sub.head;		// point tail of current deck to 
									// head of sub deck
		tail = sub.tail;			// point tail to sub's tail
		numcards += sub.numcards;	// update numcards
	}

	sub.head = sub.tail = 0;
	sub.numcards = 0;

	return *this;
}

/*****
* S i z e
*****/
int CardDeck::Size() const
/*****
Post:	size of current deck is returned
*****/
{
	return int(numcards);
}

/*****
* B o t t o m C a r d
*****/
CardType CardDeck::BottomCard() const
/*****
Post: returns the value of the bottom card
	of the deck
*****/
{
	return tail->card;
}

/*****
* T o p C a r d
*****/
CardType CardDeck::TopCard() const
/*****
Post: returns the value of the top card
	of the deck
*****/
{
	return head->card;
}

/*****
* S h u f f l e
*****/
void CardDeck::Shuffle()
/*****
Shuffles the card deck.

Pseudocode:
	The cards are copied to an array; 
	Knuth's shuffle algorithm 3.4.2P is applied to the array; 
	the cards are copied back to the linked list.

Limitations:  This routine depends upon the C++ standard
library rand() function.  The shuffled deck is only as
good as the rand() function -- for game-playing, rand()
is probably good enough, but if used for encryption,
there may be vulnerabilities.

"Any one who considers arithmetical methods of producing
random digits is, of course, in a state of sin."
	-- John Von Neumann (1951)

Pre: The deck is defined and has cards in it.
	 numcards <= MAXCARDS
	 numcards is accurate

Post: The deck has been shuffled to a randomized
	 permutation.

*****/
{
	CardType tempdeck[MAXDECK];	// deck array
	CardType tempcard;			// single card value
	int i,j,n;					// array indices
	CardPtr curr = head;		// ptr to current card

	// loop thru deck list and create deck array
	// NOTE: This assumes that 'numcards' is valid
	for (n=0; n<numcards; n++) {
		tempdeck[n] = curr->card;
		curr = curr->next;
	}
	
	// at this point, n = array size
	
	// seed the random number generator -- one time only
	static bool seeded = false;
	MTRand_int32 mt;
	if (!seeded) {
		seeded = true;
		srand(time(NULL));
	}

	// apply Knuth's shuffle algorithm 3.4.2P
	for (i=n-1; i>0; i--) {
		j = mt.operator()() % (i+1);		// 0 <= j <= i
		tempcard = tempdeck[i];
		tempdeck[i] = tempdeck[j];
		tempdeck[j] = tempcard;
	}

	// put 'em back in the list
	curr = head;
	for (i=0; i<n; i++) {
		curr->card = tempdeck[i];
		curr = curr->next;
	}

	return;
}

/*****
* A d d C a r d
*****/
void CardDeck::AddCard(string face, char suit)
/*****
Support routine to add a single card to an existing deck.

  pre:  face and suit have been set as follows:

		face = A, 2, 3, ... , 10, J, Q, K, Jo (joker)
		suit = C, D, H, S, or A, B (joker A or B)

  post: a card has been added to the current deck
		the (modified) current deck is returned

		if the passed card is invalid, an error msg is
		output.
*****/
{
	int n = 0;		// accumulator

	switch (char(toupper(face[0]))) {
		case 'A':	n = 1; break;
		case '2':	n = 2; break;
		case '3':	n = 3; break;
		case '4':	n = 4; break;
		case '5':	n = 5; break;
		case '6':	n = 6; break;
		case '7':	n = 7; break;
		case '8':	n = 8; break;
		case '9':	n = 9; break;
		case '1':	n = 10; break;
		case 'J':	n = 11; break;
		case 'Q':	n = 12; break;
		case 'K':	n = 13; break;
		default:	std::cout << "Unknown face = " 
							  << face
							  << std::endl;
					return;
	}

	// note that jokers are temporarily assigned
	// the wrong value -- it's corrected below

	switch (char(toupper(suit))) {
		case 'C':	break;
		case 'D':	n += 13; break;
		case 'H':	n += 26; break;
		case 'S':	n += 39; break;
		case 'A':	n = JokA; break;
		case 'B':	n = JokB; break;
		default:	std::cout << "Unknown suit = "
						<< suit
						<< std::endl;
					return;
	}

	// create new node at tail
	if (numcards == 0) 
		head = tail = new CardNode;
	else {
		tail->next = new CardNode;
		tail = tail->next;
	}
	tail->card = CardType(n);
	tail->next = 0;
	numcards++;

	return;
}
