#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <random>

class Card {
public:
  std::string card_id;
  std::string card_categroy;
  std::string card_value;
  std::string card_name;

  Card(std::string id, std::string categroy, std::string value, std::string name) {
    this->card_id = id;
    this->card_categroy = categroy;
    this->card_value = value;
    this->card_name = name;
  }
};

std::vector<std::string> readCSV(std::string file_name) {
  std::ifstream CSV(file_name);
  std::vector<std::string> cards;
  if(CSV.good()){
    std::string current_line;
    while(std::getline(CSV, current_line)){
      std::string card;
      std::stringstream string_stream(current_line);
      std::string token;
      while(std::getline(string_stream, token)) {
          cards.push_back(token);
      }
    }
  }
  CSV.close();
  return cards;
}

Card createCardObject(std::string card) {
//std::cout << "Creating Card ...." << std::endl;
 std::vector<std::string> constructor_vars;
 int pos = 0;
 while(pos<card.size()){
   pos = card.find(",");
   constructor_vars.push_back(card.substr(0, pos));
   card.erase(0, pos+1);
 }
 Card cardObject(
   constructor_vars[0],
   constructor_vars[1],
   constructor_vars[2],
   constructor_vars[3]);
 return cardObject;
}

std::vector<Card> createCardObjects(std::vector<std::string> cards){
  std::cout << "Creating Card Objects ...." << std::endl;
  std::vector<Card> deck;
  for(std::string card_string: cards){
    deck.push_back(createCardObject(card_string));
  }
  return deck;
}

std::vector<Card> randomShuffle(std::vector<Card> deck){
  std::random_device rd;
  std::mt19937 g(rd());
  std::cout << "Random Shuffle" << std::endl;
  std::shuffle(deck.begin(), deck.end(), g);
  return deck;
}
void printCardStrings(std::vector<Card> randomizedDeck){
  // sanity check on deck creation
  for(auto i: randomizedDeck){
    std::cout << i.card_id << std::endl;
  }
}

void monteCarLoSimulation(std::vector<Card> deck){
  for(int i=0;i<=3;i++){
    std::vector<Card> randomizedDeck = randomShuffle(deck);
    printCardStrings(randomizedDeck);
  }
}

int main(int argc, char const *argv[]) {
  std::cout << "Starting Simulation" << std::endl;
  std::vector<std::string> cards = readCSV("./FTP.csv");
  std::vector<Card> deck = createCardObjects(cards);
  monteCarLoSimulation(deck);
  std::cout << "Simulation Ended ;)";
  return 0;
}
