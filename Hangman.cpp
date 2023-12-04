#include<iostream>
#include<ctime>
#include<fstream>
#include<cstdlib>
#include<string>
using namespace std;

void welcomescreen();
int guessLetters(const string[], char [], int &);
void takeword(const int, ifstream &, string []);
int hangmanFigure(int, const string[], int);
void wrfile(ofstream &, const int [], const string [], int);
int wordLetters(const string [], char [], const int);

int main()
{
    const int maxlist = 20; // max limit of 20 words
    const int maxsize = 20; // max limit of 20 blanks

    string words[maxsize]; // A word cannot have more than 20 letters

    unsigned seed = time(0); //To generate words randomly
    srand(seed);

    welcomescreen();

    ifstream inputFile;
    inputFile.open("puzzleoptions.txt");

    if (!inputFile) //input validation
        cout << "Error! Couldn't open the word list! Game Crashed!!\n";
    else
    {
        int word_index, gamescore;
        int try_index = 0;
        char blank_for_word[maxsize];
        int score_list[maxlist];
        string words_list[maxlist];
        char option;

        takeword(maxlist, inputFile, words);

        do
        {
            word_index = wordLetters(words, blank_for_word, maxlist);
            gamescore = guessLetters(words, blank_for_word, word_index);

            cout << "\nDo you want to play again(press y or Y)?"<<endl;
            cout<<"Or you want to quit(press 'n')";
            cin >> option;

            system("cls");

            words_list[try_index] = words[word_index];
            score_list[try_index] = gamescore;

            try_index++;

        } while (option == 'y' || option == 'Y');

        inputFile.close();

        ofstream outputFile;
        outputFile.open("scores.txt");

        wrfile(outputFile, score_list, words_list, try_index);

        outputFile.close();

        if(option == 'n' || option == 'N')
        {
          char ch;
	        const char *fileName="scores.txt";
	        ifstream file;
	        file.open(fileName,ios::in);
	        if(!file)
	        {
		        cout<<"Error in opening file!!!"<<endl;
		        return -1;
	        }
	        while (!file.eof()) 
	        {
		       file >> noskipws >> ch;	
		       cout << ch;	
	        }
	        file.close();
        }
    
    }

    return 0;
}



/**for printing welcome screen**/
void welcomescreen()
{
    char ch;
    cout << "~~~~~~WELCOME TO HANGMAN!!!~~~~~~"<<endl;
    cout << "... Good Luck!!..."<<endl;
    cout << "Press Y to continue to game: ";
    cin >> ch;

    if (ch == 'Y')
        system("cls");
}

/**generates blanks and take input **/
int wordLetters(const string words[], char blank_for_word[], const int maxlist)
{
    int random_word = rand() % 12;

    for (int blanks = 0; blanks < words[random_word].length(); blanks++)
    {
        blank_for_word[blanks] = '_';
        cout << blank_for_word[blanks] << " ";
    }
    return random_word;
}

/**read each word from word list file**/
void takeword(const int maxlist, ifstream &inputFile, string words[])
{
    int i = 0;

        while (i < maxlist && getline(inputFile, words[i]))
        {
            i++;
        }
}

/**guess the letter and place blank corresponding to it and keep record of score**/
int guessLetters(const string words[], char blank_for_word[], int &word_index)
{
    char letter;
    int i = 0;
    int wrong_choice = 0;
    int score = 0, rounded_score = 0;
    const int MAX_GUESS = 11;

    while (i < MAX_GUESS)
    {
        cout << "\n\nEnter a letter to guess: ";
        cin >> letter;

        int search_letter = 0;
        int count_letter = 0;
        bool found_letter = false;

        //loop to search if the letter matches the word
        while (search_letter < words[word_index].length())
        {
            if (blank_for_word[search_letter] == letter)
            {
                cout << "You already guessed that letter!";
                score--;
            }
            if (words[word_index][search_letter] == letter)
            {
                blank_for_word[search_letter] = letter;
                found_letter = true;
                score++;
            }
            search_letter++;
        }
        i++;

        int j = 0;
        //loop to check if all the blanks are filled and count the letters found
        while (j < words[word_index].length())
        {
            if (blank_for_word[j] != '_')
                count_letter++;
            j++;
        }

        system("cls");

        int k = 0;
        //loop to generate the no.of blanks according to the length of the word
        while (k < words[word_index].length())
        {
            cout << blank_for_word[k] << " ";
            k++;
        }

        if (!found_letter)
        {
            cout << "\n\nWrong letter!\n";
            wrong_choice++;
            score--;
        }

        cout << "\n\n\n\n\nHANGMAN FIGURE DISPLAY\n\n\n\n";
        int check = hangmanFigure(wrong_choice, words, word_index);

        rounded_score = static_cast<double>(score) / words[word_index].length() * 10;

        if (count_letter == words[word_index].length() && check == 0)
        {
            cout << "\nYou guessed the word!";
            rounded_score++;

            if (wrong_choice == 0)
            {
                cout << "\nCONGRATS ON FIRST TRY!!";
                rounded_score++;
            }

            break;
        }
        else if (check == -1)
        {
            if (rounded_score < 0)
                rounded_score = 0;
            break;
        }
    }
    return rounded_score;
}

/**write score in the score.txt file**/
void wrfile(ofstream &outputFile, const int score_list[], const string words_list[], int try_index)
{
    int i = 0;

    while (i < try_index)
    {
        outputFile << words_list[i] << ": " << score_list[i] << endl;
        i++;
    }

}

/**for hungman figure**/
int hangmanFigure(int wrong_choice, const string words[], int word_index)
{
    char head = 'O', left_body = '/', right_body = '\\', body = '|';

    if (wrong_choice > 0)
        cout << "\t\t " << head << endl;
    if (wrong_choice == 2)
        cout << "\t\t " << body;
    else if (wrong_choice > 2)
        cout << "\t\t" << left_body << body;
    if (wrong_choice > 3)
        cout << right_body << endl;
    if (wrong_choice > 4)
        cout << "\t\t" << left_body;
    if (wrong_choice > 5)
        cout << " " << right_body;
    if (wrong_choice > 6)
    {
        cout << "\n\nGAME OVER!!";
        cout << "\nYour word was " << words[word_index];

        return -1;
    }
    return 0;
}
