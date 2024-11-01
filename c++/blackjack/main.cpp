#include <iostream>
//#include <ifstream>

/* Author: Logan Puntous
   Date: Apr/9/2024
   Program: Double hand Blackjack

   Goal: The player is to reach a total of 21 before the dealer.
   Payout is 2 to 1.

 */


const int DECK_SIZE = 104;


struct Card{
  int value;
  char suit;
  char color;
  char face;
};

struct Deck{
  Card un_used[104];
  Card used[104];
};

struct Hand{
  Card hand[11]; //Max is 4 aces 4 twos and 3 threes
  int score;
};

struct Player{
  int wins;
  int balance;
  Hand hand;
  //Chips chips;
};

struct Dealer{
  int wins;
  Hand hand;
  //Chips chips;
};


// Fills un_used card array with two decks of cards.
void GenerateDeck(Deck &deck){
  char current_suit;
  char current_color;

  int card_number=0; // Current card placement in deck
  
  // Two decks so must repeat process twice ugh.
  //0-1
  for ( int deck_num=0; deck_num<=1; deck_num++ ){
    // 1-4
    for ( int i=1; i<5; i++ ){
      if ( i == 0 ) { current_suit = 'D'; current_color = 'R'; } // Diamonds
      if ( i == 1 ) { current_suit = 'C'; current_color = 'B'; } // Clubs
      if ( i == 2 ) { current_suit = 'H'; current_color = 'R'; } // Hearts
      if ( i == 3 ) { current_suit = 'S'; current_color = 'B'; } // Spades

      // 1-13
      for ( int j=1; j<14; j++ ){
	Card current_card;
      
	if ( j >= 1 && j <= 10 ) { current_card.value = j; } // value 1-10
	else { current_card.value = 10;} // value jack/queen/king = 10
      
	current_card.suit = current_suit;
	current_card.color = current_color;
      
	// ACE
	if ( j == 1 ){
	  current_card.face = 'A';
	}
	// 2 - 9
	if ( j >= 2 && j <= 9 ){
	  current_card.face = ('0' + j); // int -> char 
	}
	// 10
	if ( j == 10 ) { current_card.face = 'X'; }
	// 11 - 13
	if ( j == 11 ) { current_card.face = 'J'; }
	if ( j == 12 ) { current_card.face = 'Q'; }
	if ( j == 13 ) { current_card.face = 'K'; }


	deck.un_used[card_number] = current_card; // Placing card
        card_number++; // Incrementing card number placement


	
	// **DEBUG**
	/*
	std::cout << "value: " << current_card.value << " suit: "
		<< current_card.suit << " color: " << current_card.color
		<< " face: " << current_card.face << std::endl;
		
	std::cout << card_number << " i: " << i << " j: " << j << " j<< std::endl;
	*/
	
	// Placing card into un_used deck
      }
    }
  }
}




// Shuffles the deck provided allowing user to cut the deck.
void ShuffleDeck(Deck &deck){
  
  Deck split_one;
  Deck split_two;

  // Cut deck
  for ( int i = 0; i < DECK_SIZE; i++ ){
    // First half
    if ( i >= 0 && i < DECK_SIZE/2 ){
      split_one.un_used[i] = deck.un_used[i];
    }
    // Second half
    if ( i >= DECK_SIZE/2 && i < DECK_SIZE ){
      split_two.un_used[i-DECK_SIZE/2] = deck.un_used[i];
    }
  }
  // Zipper halfs together
  for ( int j = 0; j < DECK_SIZE; j++ ){
    if ( j % 2 == 0) {
      deck.un_used[j] = split_two[];
    }
    if ( j % 2 == 1 ){
      deck.un_used[j] = split_one[];
    }
  }
  

  

}






// Function to print the contents of a Card
void printCard(const Card& card) {
    std::cout << "Value: " << card.value << std::endl;
    std::cout << "Suit: " << card.suit << std::endl;
    std::cout << "Color: " << card.color << std::endl;
    std::cout << "Face: " << card.face << std::endl;
}

// Function to print the contents of the un_used array in a Deck
void printDeck(const Deck& deck) {
    std::cout << "Printing un_used cards:" << std::endl;
    for (int i = 0; i < 104; ++i) {
        printCard(deck.un_used[i]);
        std::cout << std::endl;
    }
}




int main(){
  Deck deck;
  GenerateDeck(deck); // Generate the default stack of two decks of cards.
  ShuffleDeck(deck);
  
  //printDeck(deck);
  
  return 0;
}
