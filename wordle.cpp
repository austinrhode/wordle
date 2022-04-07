#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_set>
#include <cstdlib>
#include <map>

#define TRUE 1
#define FALSE 0

#define NUM_GUESSES 6

using namespace std;

class Wordle {
	private:
		unordered_set<string> words;
		vector<string> word_vector;
		string answer;
		int guesses_left = NUM_GUESSES;
		string guesses[NUM_GUESSES];

	public:
		Wordle(){
			string filename("words.txt");
			string line;

			ifstream input_file(filename);
			if(!input_file.is_open()){
				cerr << "Could not open file" << endl;
			}

			while(getline(input_file,  line)){
				this->words.insert(line);
				//this->word_vector.push_back(line);
			}

			input_file.close();

			ifstream answer_file("answers.txt");
			if(!answer_file.is_open()){
				cerr << "Could not open answers" << endl;
			}

			while(getline(answer_file, line)){
				this->word_vector.push_back(line);
			}

			answer_file.close();

			this->choose_answer();
		}

		void choose_answer(){
			int length = this->word_vector.size();
			srand(time(0));
			int index = rand() % length;

			answer = this->word_vector[index];
			//cout << answer << endl;
		}

		unordered_set<string> getWords(){
			return this->words;
		}

		int validateGuess(string guess){
			if(this->words.find(guess) == this->words.end()){
				return FALSE;
			}else{
				return TRUE;
			}
		}

		int isWord(string guess){
			if(guess == this->answer){
				return TRUE;
			}else{
				return FALSE;
			}
		}

		map<char, int> letterCount(string guess){
			map<char, int> letter_count;
			for(int i = 0; i < guess.size(); i++){
				if(letter_count.find(guess[i]) == letter_count.end()){
					letter_count[guess[i]] = 1;
				}else{
					letter_count[guess[i]] += 1;
				}
			}

			return letter_count;
		}

		void print_guesses(){
			for(int i = 0; i < sizeof(this->guesses) / sizeof(string); i++){
				if(this->guesses[i] == ""){
					cout << "_ _ _ _ _" << endl;
				}else{

					map<char, int> letter_counts = this->letterCount(this->answer);
					map<char, int> guess_counts = this->letterCount(this->guesses[i]);
					int colors[5] = {37, 37, 37, 37, 37}; // all white

					for(int k = 0; k < answer.size(); k++){  //cycle
						for(int j = this->guesses[i].size() - 1; j >= 0;  j--){ //leave
							if(answer[k] == this->guesses[i][j]){
								if(k == j){
									colors[j] = 32; //green
									letter_counts[this->guesses[i][j]] -= 1;
								}else if(guess_counts[this->guesses[i][j]] == 1 && letter_counts[this->guesses[i][j]] != 0){
									colors[j] = 33; //yellow
									letter_counts[this->guesses[i][j]] -= 1;
								}
								guess_counts[this->guesses[i][j]] -= 1;

							}
						}
					}
							

					for(int j = 0; j < this->guesses[i].size(); j++){
						char character = toupper(this->guesses[i][j]);
						cout << "\033[0;" << colors[j] << "m" << character << "\033[0m ";
					}
					cout << endl;
				}
			}
		}

		int guess(){
			string guess;
			cout << ">> ";
			cin >> guess;
			while(!validateGuess(guess)){
				cout << "Invalid guess" << endl;
				cout << ">> ";
				cin >> guess;
			}
			
				
			this->guesses[NUM_GUESSES - this->guesses_left] = guess;

			this->guesses_left--;

			this->print_guesses();

			if(isWord(guess)){
				cout << "Correct!" << endl;
				return TRUE;
			}
			return FALSE;	
		}

		void reset_game(){
			this->guesses_left = NUM_GUESSES;
			
			for(int i = 0; i < NUM_GUESSES; i++){
				this->guesses[i] = "";
			}
			
			this->choose_answer();
		}

		int getRemainingGuesses(){
			return this->guesses_left;
		}

		string getAnswer(){
			return this->answer;
		}		
};


int main(){
	Wordle wordle;
	unordered_set<string> words(wordle.getWords()); 

	string play_again = "y";

	while(play_again == "y"){
		wordle.print_guesses();

		while(!wordle.guess() && wordle.getRemainingGuesses() > 0){
		
		}

		cout << "The word was: " << wordle.getAnswer() << endl;

		wordle.reset_game();

		cout << "Play again [y/n]? ";
		cin >> play_again;
		cout << endl;
	}

	return 0;
}
