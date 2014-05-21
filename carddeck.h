#ifndef CARDDECK_H
#define CARDDECK_H
#include <string>			// for AddCard()

const char MAXDECK = 54;	// char used to save space

typedef char CardType;		// for ease in changing representation

const CardType JokA = 53;
const CardType JokB = 54;

class CardDeck 

{
public:

	CardDeck();						// prototype for default init
	CardDeck(char);

	void Print() const;
	int Size() const;
	CardDeck FindCard(CardType) const;
	CardDeck FindNthCard(int) const;
	CardType BottomCard() const;
	CardType TopCard() const;
	void	 Split(const CardDeck);
	CardDeck Append(CardDeck&);
	void	 Shuffle();
	void	 AddCard(std::string, char);

private:

	struct CardNode;			// forward declaration
	typedef CardNode *CardPtr;

	struct CardNode {
		CardType card;
		CardPtr next;
	};

	CardPtr head;
	CardPtr tail;

	char numcards;				// number of cards in deck
								// usually 52 or 54

	void PrintCard(CardPtr) const;	// prints a single card
};

#endif
