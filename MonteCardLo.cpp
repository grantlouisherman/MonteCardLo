#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <random>
#include<tuple>
#include <unordered_map>
#include <cassert>
#include <map>
#include <format>

class Card {
public:
  std::string card_id;
  std::string card_categroy;
  std::string card_value;
  std::string card_name;
  std::string card_side;

  Card(std::string id, std::string categroy, std::string value, std::string name, std::string side) {
    this->card_id = id;
    this->card_categroy = categroy;
    this->card_value = value;
    this->card_name = name;
    this->card_side = side;
  }

  void print(){
    std::cout << "Card ID: " << card_id
    << std::endl <<
    "Card Categroy: " << card_categroy
    << std::endl
    << "Card Value: " << card_value
    << std::endl <<
    "Card Name: " << card_name;
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

void writeCSV(std::vector<std::map<std::string, int>> maps){
  std::cout << "Writing CSV...";
  std::ofstream simulation_ouput("simulation.csv");
  std::cout << "Map Size: " << maps.size() << std::endl;
  for(int i=0;i<maps.size();i++){
    // switch(i){
    //   case 0:
    //     simulation_ouput << "Categories";
    //     // break;
    //   case 1:
    //     simulation_ouput << "Sides";
    //     // break;
    //   case 2:
    //     simulation_ouput << "Names";
    //     // break;
    //   case 3:
    //     simulation_ouput << "Values";
    //     // break;
    //   // default:
    //   //   // break;
    // }
    for (const auto& [key, value] : maps[i]) {
      simulation_ouput << key;
      simulation_ouput << value;
    }
  }
  simulation_ouput.close();
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
   constructor_vars[3],
   constructor_vars[4]);
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
  std::shuffle(deck.begin(), deck.end(), g);
  return deck;
}


void printCardStrings(std::vector<Card> randomizedDeck){
  // sanity check on deck creation
  for(auto i: randomizedDeck){
    std::cout << i.card_id << std::endl;
  }
}

std::tuple<
std::vector<std::vector<Card>>,
std::vector<std::vector<Card>>>
monteCarloSimulation(std::vector<Card> deck, int N){
  std::cout << "Monte Carlo Started" << std::endl;
  std::vector<std::vector<Card>> player1;
  std::vector<std::vector<Card>> player2;

  for(int i=0;i<=N;i++){
    //std::cout << "Current Interation: " << i << std::endl;
    std::vector<Card> randomizedDeck = randomShuffle(deck);
    // printCardStrings(randomizedDeck);
    std::vector<Card> player_1_hand;
    std::vector<Card> player_2_hand;
    for(int i=0;i<14;i++){
      // player1 gets cards on even and 0
      if(i % 2 == 0 || i == 0){
        player_1_hand.push_back(randomizedDeck[i]);
      } else {
        player_2_hand.push_back(randomizedDeck[i]);
      }
    }
    player1.push_back(player_1_hand);
    player2.push_back(player_2_hand);
  }
  std::cout << "Monte Carlo Ended" << std::endl;
  return {player1, player2};
}

std::map<std::string, int> mapInsert(std::map<std::string, int> map, std::string key){
  if(map.find(key) == map.end()){
    map[key] = 1;
  } else {
    auto value = map[key];
    map[key]=value+1;
  }
  return map;
}
// {categroy, sides, names, values}
std::vector<std::map<std::string, int>> analyzeHand(std::vector<std::vector<Card>> player_hands){
  std::cout << "Analyzing";
  std::map<std::string, int> categories;
  std::map<std::string, int> sides;
  std::map<std::string, int> names;
  std::map<std::string, int> values;
  for(auto player_hand: player_hands){
    for(auto card: player_hand){
      auto category = card.card_categroy;
      auto value  = card.card_value;
      auto name = card.card_name;
      auto side = card.card_side;
      categories = mapInsert(std::move(categories), category);
      sides = mapInsert(std::move(sides), side);
      names = mapInsert(std::move(names), name);
      values = mapInsert(std::move(values), value);
    }
  }
  std::vector maps = {categories, sides, names, values};
  return maps;
}

//void printMapKeysValues()
int main(int argc, char const *argv[]) {
  std::cout << "Starting Simulation" << std::endl;
  std::vector<std::string> cards = readCSV("./FTP.csv");
  std::vector<Card> deck = createCardObjects(cards);
  auto simulations = monteCarloSimulation(deck, 100000);
  auto player_1 = std::get<0>(simulations);
  auto player_2 = std::get<1>(simulations);
  auto player_1_hand_stats = analyzeHand(player_1);
  writeCSV(player_1_hand_stats);
  std::cout << "Simulation Ended ;)";
  return 0;
}
