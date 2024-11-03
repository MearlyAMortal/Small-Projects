#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
#include <vector>
#include <unistd.h>
#include <thread>
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
  int un_used_count;
  int used_count;
};

struct Hand{
  Card cards[11]; //Max is 4 aces 4 twos and 3 threes
  int size;
  int score;
};

struct Player{
  int id;
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
	  std::cout << "card_number: " << card_number << " i: " << i << " j: " << j << std::endl;
	 */
      }
    }
  }    
  deck.un_used_count = DECK_SIZE;
  deck.used_count = 0;
}




// Move all used cards back to unused
void ResetDeck(Deck &deck) {
  // Checking if cards still add up to total
  if ( deck.used_count + deck.un_used_count != DECK_SIZE ){
    std::cout << "ERROR in ResetDeck: Card count is off!" << std::endl;
    exit(1);
  }
  // Checking if there are cards to move
  if ( deck.used_count == 0 ){
    std::cout << "ERROR in ResetDeck: No cards to move!" << std::endl;
  }
  // Moving the cards
  for (int i = 0; i < deck.used_count; ++i) {
    deck.un_used[deck.un_used_count + i] = deck.used[i];
  }
  // Updating deck counts
  deck.un_used_count += deck.used_count;
  deck.used_count = 0;

  std::cout << "Deck successfully reset." << std::endl;
}




// Moves all cards to un_used and shuffles
void ShuffleDeck(Deck &deck) {
  // Have any cards been used?
  if ( deck.un_used_count != DECK_SIZE ){
    ResetDeck(deck); 
  }
  
  // Initialize random number generator
  std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));

  // Fisher-Yates shuffle algorithm
  for (int i = DECK_SIZE - 1; i > 0; --i) {
    // Generate a random index from 0 to i
    std::uniform_int_distribution<int> dist(0, i);
    int j = dist(rng); // Random index

    // Swap the elements at indices i and j
    std::swap(deck.un_used[i], deck.un_used[j]);
  }
  std::cout << "Deck successfully shuffled." << std::endl;
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
  for (int i = 0; i < DECK_SIZE; ++i) {
    printCard(deck.un_used[i]);
    std::cout << std::endl;
  }
}
// Function to print current players
void printPlayers(const std::vector<Player> &players){
  std::cout << "Players: " << std::endl;
  for ( int i = 0; i < players.size(); i++ ){
    std::cout << "Player " << players[i].id << ","<< std::endl;
  }
}




// Returns a vector with the desired number of players.                                                                                                              
void MakePlayers(std::vector<Player> &players){
  int num_players_gen;
  std::cout << "And how many of you will be joining us today? (table sits 5): ";
  std::cin >> num_players_gen;
  if ( num_players_gen < 1 || num_players_gen > 5 ){
    std::cout << "ERROR in MakePlayers: Unable to add " << num_players_gen << " players to the game." << std::endl;
    exit(1);
  }

  for ( int i = 0; i < num_players_gen; i++ ){
    Player player;
    player.id = i;
    players.push_back(player);
  }
  std::cout << "Added " << players.size() << " players to the game." << std::endl;
}




// Functions for placing cards in hands.
void PlaceCardInDealerHand(Deck& deck, Dealer& dealer, int deck_marker){
  if ( deck.un_used_count == 0 ){
    ResetDeck(deck);
    ShuffleDeck(deck);
    deck_marker = (deck.un_used_count)-1;
  }
  dealer.hand.cards[dealer.hand.size] = deck.un_used[deck_marker];
  dealer.hand.score += deck.un_used[deck_marker].value;
  dealer.hand.size++;
  
  deck.used[deck.used_count] = deck.un_used[deck_marker];
  deck.used_count++;

  Card card;
  deck.un_used[deck_marker] = card;
  deck.un_used_count--;
  std::cout << "...";
  std::cout.flush();
  sleep(2);
  if ( dealer.hand.size == 1 ){
    std::cout << "slide";
  }
  if ( dealer.hand.size == 2 ) {
    std::cout << "slide...";
    std::cout.flush();
    sleep(2);
    std::cout << "dealer revealed " << deck.used[deck.used_count-1].value << "|" << deck.used[deck.used_count-1].suit;
  }
  if ( dealer.hand.size > 2 ){
    std::cout << "flipped " << deck.used[deck.used_count-1].value << "|" << deck.used[deck.used_count-1].suit << " for the dealer";
  }
}
void PlaceCardInPlayerHand(Deck& deck, Player& player, int deck_marker){
  if ( deck.un_used_count == 0 ){
    ResetDeck(deck);
    ShuffleDeck(deck);
    deck_marker = (deck.un_used_count)-1;
  }
  player.hand.cards[player.hand.size] = deck.un_used[deck_marker];
  player.hand.score += deck.un_used[deck_marker].value;
  player.hand.size++;
  
  deck.used[deck.used_count] = deck.un_used[deck_marker];
  deck.used_count++;

  Card card;
  deck.un_used[deck_marker] = card;
  deck.un_used_count--;
  std::cout << "...";
  std::cout.flush();
  sleep(1);
  std::cout << "flipped " << deck.used[deck.used_count-1].value << "|" << deck.used[deck.used_count-1].suit << " for player " << (player.id)+1;
}




// Returns state 'w' = win21, 'c' = current = in-play(can return > 21), 's' = stand at 17, 'b' = bust
char DealerAction(Deck& deck, Dealer& dealer){
  int deck_marker = (deck.un_used_count)-1;
  // Winner?
  if ( dealer.hand.score == 21 ){
    return 'w';
  }
  // Bust?
  if ( dealer.hand.score > 21 ){
    return 'b';
  }
  // Not 17 yet?
  if ( dealer.hand.score < 17 ){
    // Then hit, warning this can return 21, must check somewhere to apply x to state
    deck_marker = (deck.un_used_count)-1;
    PlaceCardInDealerHand(deck, dealer, deck_marker);
    return 'c';
  }
  
  // Else return stand
  return 's';
}

// Returns state 'w' = win, 'c' = current = in-play(can return > 21), 's' = stand
char PlayerAction(Deck& deck, Player& player){
  std::cout << std::endl;
  std::cout << "Player " << (player.id)+1 << " has " << player.hand.score << ". " << std::endl;

  // Winner winner chicken feet                                                                                                                                      
  if ( player.hand.score == 21 ) {
    std::cout << "Player wins 2.5x bet!" << std::endl;
    player.wins++;
    return 'w';
  }

  char player_action;
  int deck_marker = (deck.un_used_count)-1;
  std::cout << "| (D)Double  | (h)Hit | (s)Stand |  (S)Split |" << std::endl;
  std::cout << "Player " << (player.id)+1 << "'s action: ";
  std::cin >> player_action;
  // Loop for checking correct input
  while ( player_action == 'D' || player_action == 'h' || player_action == 'S' || player_action == 's' ){
    // Reset deck marker
    deck_marker = (deck.un_used_count)-1;
    // Hit
    if ( player_action == 'h' ){
      PlaceCardInPlayerHand(deck, player, deck_marker);
      return 'c';
    }
    // Stand
    if ( player_action == 's' ){
      return 's';
    }
    // TODO double
    if ( player_action == 'D' ){
      std::cout << "Best of luck to you sir, doubling the bet." << std::endl;
      PlaceCardInPlayerHand(deck, player, deck_marker);
      // Double bet here..
      return 's';
    }
    // TODO split
    if ( player_action == 'S' ){
      PlaceCardInPlayerHand(deck, player, deck_marker);
      return 'c';
    }
    // ATP player has input incorrect action
    std::cout << "ERROR in PlayerAction: " << player_action << " is not a valid action." << std::endl;
    std::cout << "Action: ";
    std::cin >> player_action;
  }
  return 'c';
}




// Deals cards to players and dealer
void DealCards(Deck& deck, Dealer& dealer, std::vector<Player> &players){
  int deck_marker;
  // Making sure hand size is initialized
  dealer.hand.size = 0;
  dealer.hand.score = 0;
  for ( int p = 0; p < players.size(); p++ ){
    players[p].hand.size = 0;
    players[p].hand.score = 0;
  }
  
  // For each card to be delt
  for ( int i = 0; i < (players.size()+1)*2; i++ ){
    // End of shoe so reset the deck and shuffle to continue
    if ( deck.un_used_count == 0 ){
      ResetDeck(deck);
      ShuffleDeck(deck);
    }
    // Grab current location in deck.un_used                                                                                                                         
    deck_marker = (deck.un_used_count)-1;
    
    // If dealer
    if ( i == players.size() || i == ((players.size()+1)*2)-1){
      PlaceCardInDealerHand(deck, dealer, deck_marker);
    } else {  // else player
      PlaceCardInPlayerHand(deck, players[i%(players.size()+1)], deck_marker);
    }
  }
  std::cout << std::endl;
}




// Checks the scores and outputs some text and gives chips to players or dealer
void CheckScoresOutcome(Deck& deck, std::vector<Player> &players, Dealer& dealer, char player_states[], char dealer_state){
  for ( int p = 0; p < players.size(); p++ ){
    // Check for auto win 
    if ( player_states[p] == 'w' ){
      //Auto win
    }
    if ( player_states[p] == 'x' ){
      //Players Lose                                                                                                                                                 
    }
    if ( dealer_state == 'x' ){
      //Dealer lose, auto player win that didnt bust
    }
    // Push
    if ( players[p].hand.score == dealer.hand.score ){
      //Tie
    }
    if ( players[p].hand.score > dealer.hand.score ){
      //Player win
    }
    if ( players[p].hand.score < dealer.hand.score ){
      //Dealer win
    }   
  }
}




// Main game loop will go until there are no cards remaining or all players consedes
void GameLoop(Deck& deck, Dealer& dealer, std::vector<Player> &players){
  std::cout << "Your bets please...(coming soon)" << std::endl;
  // TODO bets ting bruv
  std::cout << "Best of luck!" << std::endl;
  DealCards(deck, dealer, players);
  
  char player_states[players.size()];
  char dealer_state;  
  int player_scores[players.size()];
  int dealer_score;
  
  std::cout << "Dealer has " << dealer.hand.cards[0].value << ". " << std::endl;

  // For each player
  for ( int i = 0; i < players.size(); i++ ){
    // Set player status by calling player action
    player_states[i] = PlayerAction(deck, players[i]);
    // While the player has actions remaining
    while ( true ) {
      // Set player score
      player_scores[i] = players[i].hand.score;
      // Check if player busted
      if ( player_states[i] == 'c' && player_scores[i] > 21 || player_states[i] == 'x'){
	player_states[i] = 'x';
	break;
      }
      // Check if player got 21 on first 2 cards
      if ( player_states[i] == 'w' ){
	player_states[i] = 'w';
	break;
      }
      // Check if player stood
      if ( player_states[i] == 's' ){
	break;
      }
      // Next player action for loop.
      player_states[i] = PlayerAction(deck, players[i]);
    } // This specific player has concluded actions
  } // All players have concluded actions

  // ----------------
  // Dealer turn now
  char dealer_action = DealerAction(deck, dealer);
  while ( true ){
    // Did he bust? pause
    if ( dealer_action == 'b' ){
      dealer_state = 'b';
      std::cout << std::endl;
      std::cout << "Dealer busted with " << dealer.hand.score << std::endl;
      break;
    }
    // Does he have 21?
    if ( dealer.hand.score == 21 || dealer_action == 'w' ){
      dealer_state = 'w';
      std::cout << std::endl;
      std::cout << "Dealer has 21. " << std::endl;
      break;
    }
    // Dealer stood
    if ( dealer_action == 's' ){
      dealer_state = 's';
      std::cout << std::endl;
      std::cout << "Dealer stands with " << dealer.hand.score << ". " << std::endl;
      break;
    }
    // In game
    dealer_action = DealerAction(deck, dealer);
  }
  
  // ------------
  // Check scores
  CheckScoresOutcome(deck, players, dealer, player_states, dealer_state);
  
  // --------------------------
  // Recursive call to GameLoop.
  //GameLoop(deck, dealer, players);
}




int main(){
  Deck deck;
  GenerateDeck(deck); // Generate the default stack of two decks of cards.
  ShuffleDeck(deck); // Randomly shuffle

  Dealer MrStealYourMoney;
  std::vector<Player> players;
  MakePlayers(players); // Generate all Player structs
  


  GameLoop(deck, MrStealYourMoney, players); // Goes until all players leave? 
  return 0;
}
