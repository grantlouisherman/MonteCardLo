#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <cstdlib>

enum CardCategory {
  FOO,
  BAR
};

class Card {
public:
  std::string card_id;
  CardCategory card_categroy;
  std::string card_value;
  std::string card_name;

  Card(std::string id, std::string categroy, std::string value, std::string name) {
    this->card_id = id;
    this->card_categroy = CardCategory::FOO;
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
      string_stream >> card;
      std::cout << "/* message */" << card << '\n';
      cards.push_back(card);
    }
  }
  CSV.close();
  return cards;
}

Card createCardObject(std::string card) {
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
  std::vector<Card> deck;
  for(std::string card_string: cards){
    deck.push_back(createCardObject(card_string));
  }
  return deck;
}

std::vector<Card> randomShuffle(std::vector<Card> deck){
  std::cout << "Random Shuffle";
  std::vector<Card> randomDeck;
  std::vector<int> chosenNumbers;

  while(randomDeck.size() != deck.size()){
    if(randomDeck.size() > deck.size()){
      std::cout << "Something went wrong with randomization";
      throw;
    }
    int random_number = std::rand() % deck.size()-1;
    std::cout << random_number;
    if(
      std::find(chosenNumbers.begin(), chosenNumbers.end(), random_number)
      != chosenNumbers.end()){
      continue;
    }
    randomDeck[random_number] = deck[random_number];
    chosenNumbers.push_back(random_number);
  }
  return randomDeck;
}

int main(int argc, char const *argv[]) {
  std::cout << "Starting Simulation";
  std::vector<std::string> cards = readCSV("./TestCSV.csv");
  std::vector<Card> deck = randomShuffle(createCardObjects(cards));
  // sanity check on deck creation
  for(auto i: deck){
    std::cout << i.card_id << std::endl;
  }
  std::cout << "Simulation Ended ;)";
  return 0;
}
