/*
Nile University
Dr. Soha Labib
ECEN204: Text Editor Project
*/
#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <thread>
#include <chrono>
using namespace std;
using namespace std::this_thread;
using namespace std::chrono;

struct undoorder{
	int lineNumber;
	string text;
	int commandNumber;
	int xLine;
	int yLine;
};

struct node{
	string data;
	struct node *next;
};

class linked_list
{
private:
	node *head;
	node *tail;
	int numOfLines = 0;
	int next = 1;
	stack<undoorder> undoStack;
public:
	std::ofstream outfile;
	linked_list(){
		int choice = -1;
		head = NULL;
		tail = NULL;
		int lastprintedPage = 1;
		while(choice != 0){
            again:
			cout<<"-------------------------\n"<<"(ECEN204 Text Editor)\n"<<endl;
			cout<<"Please choose what you want to do:\n"<<"1. Open a .txt file"<<endl;
			cout<<"2. Save into a .txt file\n"<<"3. Insert text into Line N"<<endl;
			cout<<"4. Delete line N\n"<<"5. Move line Y into line X"<<endl;
			cout<<"6. Replace text in Line N\n"<<"7. Undo"<<endl;
			cout<<"8. Print all\n"<<"9. Print the next page"<<endl;
			cout<<"10. Print the previous page\n"<<"11. About\n"<<"12. Exit\n"<<endl;
			cin>>choice;
			cout<<endl;
			if (choice == 1)            //Open .txt file from your local device
			{
				node* current = head;
				node* next;
				while (current != NULL)
				{
				    next = current->next;
					  free(current);
				    current = next;
				}
				head = NULL;
				openFile();
			}
			else if (choice == 2)		//Saving the lines of text into a .txt file
			{
				saveFile();
			}
			else if (choice == 3)	    //Insert a new line in (the first - between - the end)
			{
				insertretry:
				int lineGiven;
				string dataGiven;
				cout<<"Enter the number of the line you want to put text in: ";
				cin >> lineGiven;
				if (std::cin.fail()) {
                    std::cout << "Input is not an integer." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    goto insertretry;
				}
				else if (lineGiven == 0)
                {
                    cout<<"There's no line 0, did you mean 1 instead?\n"<<endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    goto insertretry;
                }
				cout<<"Enter text : ";
				cin.ignore(1);
				getline(cin,dataGiven);
				dataGiven+="\n";
				if (lineGiven == 1)
				{
					addinBegining(dataGiven);
				}
				else if (lineGiven > numOfLines)
				{
					insertinspecificLine(dataGiven,lineGiven);
				}
				else if (lineGiven < numOfLines)
				{
					insertinBetween(dataGiven,lineGiven);
				}
				else if (lineGiven == numOfLines)
				{
					selectionretry:
					int selection;
					cout<<"You want to replace last line?\n(1 to replace the last line/2 to insert a new line)";
					cin>>selection;
					if (selection == 1)
					{
						replaceLine(dataGiven,lineGiven);
					}
					else if (selection == 2)
					{
						addinEnding(dataGiven);
					}else {
                        std::cout << "Invalid choice!" << std::endl;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        goto selectionretry;
					}
				}
			}
			else if (choice == 4)		//Delete a line by entering its number
			{
				deleteretry:
				int lineGiven;
				cout<<"Enter the line you want to delete: ";
				cin>>lineGiven;
				if (std::cin.fail()) {
                    std::cout << "Input is not an integer." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    goto deleteretry;
				}
				deleteLine(lineGiven);
			}
			else if (choice == 5)		//Exchange line Y and line X together
			{
				exchange1retry:
				int lineGiven1;
				int lineGiven2;
				cout<<"Enter line 1 you want to swap: ";
				cin>>lineGiven1;
				if (std::cin.fail()) {
                    std::cout << "Input is not an integer." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    goto exchange1retry;
				}else if (lineGiven1 > numOfLines){
				    std::cout << "This line doesn't exist" << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    goto exchange1retry;
				}
				exchange2retry:
				cout<<"Enter line 2 you want to swap: ";
				cin>>lineGiven2;
				if (std::cin.fail()) {
                    std::cout << "Input is not an integer." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    goto exchange2retry;
				}else if (lineGiven2 > numOfLines){
				    std::cout << "This line doesn't exist" << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    goto exchange2retry;
				}
				moveYtoX(lineGiven1, lineGiven2);
			}
			else if (choice == 6)       //Replace text in line number N
			{
			    replaceretry:
				int lineGiven;
				string dataGiven;
				cout<<"Enter line you want to change the content of : ";
				cin>>lineGiven;
				if (lineGiven > numOfLines)
				{
					cout<<"This line doesn't exist"<<endl;
				} else if (std::cin.fail()) {
                    std::cout << "Input is not an integer." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    goto replaceretry;
				}
				else{
					cout<<"Enter the new text : ";
					cin>>dataGiven;
					dataGiven+="\n";
					replaceLine(dataGiven, lineGiven);
				}
			}
			else if (choice == 7)       //Undo last step
			{
				if (undoStack.empty())
				{
					cout<<"No moves available"<<endl;
					sleep_for(nanoseconds(1000));
					sleep_until(system_clock::now() + 1s);
				}
				else{
					undo();
					sleep_for(nanoseconds(1000));
					sleep_until(system_clock::now() + 1s);
				}
			}
			else if (choice == 8)       //Print all lines in the .txt file
			{
				printall();
				sleep_for(nanoseconds(1000));
				sleep_until(system_clock::now() + 1s);
			}
			else if (choice == 9)       //Printing the next page of lines (10 lines)
			{
				if (lastprintedPage*10 > numOfLines)
				{
					printPage(lastprintedPage);
					sleep_for(nanoseconds(1000));
					sleep_until(system_clock::now() + 1s);
				}
				else if (lastprintedPage == 1)
				{
					printPage(1);
					lastprintedPage++;
					sleep_for(nanoseconds(1000));
					sleep_until(system_clock::now() + 1s);
				}
				else{
					printPage(lastprintedPage);
					lastprintedPage++;
					sleep_for(nanoseconds(1000));
					sleep_until(system_clock::now() + 1s);
				}
			}
			else if (choice == 10)		//Printing the previous page of lines (10 lines)
			{
				if (lastprintedPage <= 0)
				{
					lastprintedPage = 1;
					printPage(1);
					sleep_for(nanoseconds(1000));
					sleep_until(system_clock::now() + 1s);
				}
				else if (lastprintedPage == 1)
				{
					lastprintedPage--;
					printPage(1);
					sleep_for(nanoseconds(1000));
					sleep_until(system_clock::now() + 1s);
				}
				else{
					lastprintedPage--;
					printPage(lastprintedPage);
					sleep_for(nanoseconds(1000));
					sleep_until(system_clock::now() + 1s);
				}
			}
			else if (choice == 11)      //Print credit for our team
            {
                cout<<"Coded and optimized by: \n";
                cout<<"Marcelino Emad Ibrahim 211001330 \n";
                cout<<"Mohaned Raafat Asker 211000845 \n";
                cout<<"Amro Hussam Elshinawy 211001434 \n";
                cout<<"Abdelrahman Gamal Abd-El Fattah 211000869 \n";
                cout<<"Marwan Mohamed Shahwan 211000705 \n"<<endl;
                system("pause");
                cout<<endl;
			}
			else if (choice == 12)      //Exit the program
			{
                break;
			}
			else
			{
                cout<< "Please enter a valid number"<<endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                goto again;

			}
		}
	}

//Functions of each choice:
    //Functions used in lines insertion:
	void addinBegining(string dataGiven){                           //This function work when the user insert new line in the very begining
		if (head == NULL)
		{
			node *temp;
			temp = new node;
			temp->data = dataGiven;
			temp->next = NULL;
			head = temp;
			tail = head;
			numOfLines++;
		}
		else
		{
			node *temp;
			temp = new node;
			temp->data = dataGiven;
			temp->next = NULL;
			temp->next = head;
			head = temp;
			numOfLines++;
		}
		undoorder adddedtoBegining;
		adddedtoBegining.lineNumber = 1;
		adddedtoBegining.commandNumber = 1;
		undoStack.push(adddedtoBegining);
	}

	void keepPushtoTail(string dataGiven){                          //This function work to keep pushing the insereted line away from tail till reach the meant place by user
		if (head == NULL)
		{
			node *temp;
			temp = new node;
			temp->data = dataGiven;
			temp->next = NULL;
			head = temp;
			tail = head;
			numOfLines++;
		}
		else
		{
			node *temp;
			temp = new node;
			temp->data = dataGiven;
			temp->next = NULL;
			tail->next = temp;
			tail = temp;
			numOfLines++;
		}
	}

	void addinEnding(string dataGiven){                             //This function to insert the inputted line after the last existing line
		if (head == NULL)
		{
			node *temp;
			temp = new node;
			temp->data = dataGiven;
			temp->next = NULL;
			head = temp;
			tail = head;
			numOfLines++;
		}
		else
		{
			node *temp;
			temp = new node;
			temp->data = dataGiven;
			temp->next = NULL;
			tail->next = temp;
			tail = temp;
			numOfLines++;
		}
		undoorder addedtoEnding;
		addedtoEnding.lineNumber = 1;
		addedtoEnding.commandNumber = 7;
		undoStack.push(addedtoEnding);
	}

	void insertinBetween(string dataGiven, int lineGiven){          //This function used to insert line between 2 existing lines
        node *prevNode = head;
        node *nextNode = head;
        node *temp = new node();
        temp->data = dataGiven;
        temp->next = NULL;
        int iterator = 2;
        while(iterator < lineGiven)
        {
            prevNode = prevNode->next;
            nextNode = nextNode->next;
            iterator++;
        }
        nextNode = nextNode->next;
        prevNode->next = temp;
        temp->next = nextNode;
        numOfLines++;
        undoorder insertedinBetween;
        insertedinBetween.lineNumber = lineGiven;
        insertedinBetween.commandNumber = 5;
        undoStack.push(insertedinBetween);
	}

	void insertinspecificLine(string dataGiven, int lineGiven){     //This function uses (keepPushtoTail) function to keep push the line, it works to put the inserted line far after last existing line
		undoorder insertedinSpecific;
		insertedinSpecific.lineNumber = 0;
		insertedinSpecific.commandNumber = 8;
		if (head == NULL)
		{
			while(numOfLines < lineGiven-1)
			{
				keepPushtoTail("\n");
				insertedinSpecific.lineNumber++;
			}
			keepPushtoTail(dataGiven);
		}
		else{
			while(numOfLines < lineGiven-1)
			{
				keepPushtoTail("\n");
				insertedinSpecific.lineNumber++;
			}
			keepPushtoTail(dataGiven);
		}
		undoStack.push(insertedinSpecific);
	}

    //Functions used in lines deletion:
	void keeppopTail(){                                             //This function used in undo stack as it's used to undo move after using (keepPushtoTail) function
		node *temp = head;
		if (head == NULL)
		{
			cout<<"Nothing to be deleted."<<endl;
		}
		else if (head == tail)
		{
			temp = head;
			string backup = temp->data;
			delete(temp);
			head = NULL;
			tail = NULL;
			numOfLines--;
		}
		else
		{
			while (temp->next != NULL && temp->next->next != NULL)
			{
				temp = temp->next;
			}
			tail = temp;
			delete temp->next;
			temp->next = NULL;
			numOfLines--;
		}
	}

	void deleteBegining(){                                          //This function used in undo stack as it's used to undo move after using (addinBegining) function
		string backup = head->data;
		node *temp = head;
		node *nextNode = head->next;
		head = nextNode;
		delete(temp);
		numOfLines--;
		undoorder deletedBegin;
		deletedBegin.text = backup;
		deletedBegin.lineNumber = 1;
		deletedBegin.commandNumber = 3;
		undoStack.push(deletedBegin);
	}

	void deleteEnding(){                                            //This function used in undo stack as it's used to undo move after using (addinEnding) function
		node *temp = head;
		if (head == NULL)
		{
			cout<<"Nothing to be deleted."<<endl;
		}
		else if (head == tail)
		{
			temp = head;
			string backup = temp->data;
			delete(temp);
			head = NULL;
			tail = NULL;
			numOfLines--;
			undoorder deletedEnd;
			deletedEnd.text = backup;
			deletedEnd.lineNumber = 1;
			deletedEnd.commandNumber = 6;
			undoStack.push(deletedEnd);
		}
		else
		{
			while (temp->next != NULL && temp->next->next != NULL)
			{
				temp = temp->next;
			}
			tail = temp;
			string backup = temp->next->data;
			delete temp->next;
			temp->next = NULL;
			numOfLines--;
			undoorder deletedEnd;
			deletedEnd.text = backup;
			deletedEnd.lineNumber = 1;
			deletedEnd.commandNumber = 7;
			undoStack.push(deletedEnd);
		}
	}

	void deleteLine(int lineGiven){                                 //This function used in deleting line of user choice by number
		if (head == NULL)
		{
			cout<<"There is no line to be deleted\n"<<endl;
		}
		else if(lineGiven == 0){
			cout<<"There's no line 0, did you mean 1 instead?\n"<<endl;
		}
		else if(head == tail){
			node *temp = head;
			delete(temp);
			head = NULL;
			tail = NULL;
			numOfLines--;
		}
		else if(lineGiven == 1){
			string backup = head->data;
			node *temp = head;
			node *nextNode = head->next;
			head = nextNode;
			delete(temp);
			numOfLines--;
			undoorder headRemoved;
			headRemoved.text = backup;
			headRemoved.lineNumber = 1;
			headRemoved.commandNumber = 11;
			undoStack.push(headRemoved);
		}
		else if(lineGiven == numOfLines){
			node *temp = head;
			undoorder deletedLine;
			deletedLine.commandNumber = 10;
			while (temp->next != NULL && temp->next->next != NULL)
			{
				temp = temp->next;
			}
			tail = temp;
			string backup = temp->next->data;
			delete temp->next;
			temp->next = NULL;
			numOfLines--;
			deletedLine.text = backup;
			deletedLine.lineNumber = lineGiven;
			undoStack.push(deletedLine);

		}
		else if (lineGiven > numOfLines)
		{
			cout<<"No line exists by this number\n"<<endl;
		}
		else if (lineGiven < numOfLines)
		{
			undoorder deletedLine;
			deletedLine.commandNumber = 9;
			node *prevNode = head;
			node *nextNode = head;
			node *temp = head;
			int iterator = 2;
			while(iterator < lineGiven)
			{
				prevNode = prevNode->next;
				nextNode = nextNode->next;
				iterator++;
			}
			nextNode = nextNode->next;
			temp = nextNode;
			nextNode = nextNode->next;
			prevNode->next = nextNode;
			string backup = temp->data;
			delete(temp);
			numOfLines--;
			deletedLine.text = backup;
			deletedLine.lineNumber = lineGiven;
			undoStack.push(deletedLine);
		}
	}

    //Functions used in replace
	void replaceLine(string dataGiven,int lineGiven){               //This function used to replace the text in the line chosen by number by user
		undoorder replacedLine;
		if (numOfLines < lineGiven)
		{
			cout<<"This line doesn't exist!"<<endl;
		}
		else if (lineGiven == 0)
		{
			cout<<"There's no line 0, did you mean 1 instead?\n"<<endl;
		}
		else if (numOfLines >= lineGiven )
		{
			node *temp = head;
			int goToLine = 1;
			while(goToLine < lineGiven)
			{
				temp = temp->next;
				goToLine++;
			}
			string backup = temp->data;
			temp->data = dataGiven;
			replacedLine.lineNumber = lineGiven;
			replacedLine.text = backup;
			replacedLine.commandNumber = 4;
			undoStack.push(replacedLine);
		}
	}

	void moveYtoX(int ylineGiven, int xlineGiven){                  //This function used to exchange n and m lines together
		if (ylineGiven == 1)
		{
			string headText = head->data;
			deleteBegining();
			insertinBetween(headText,xlineGiven);
		}
		else
		{
			node *temp = head;
			int iterator = 1;
			while(iterator < ylineGiven)
			{
				temp = temp -> next;
				iterator++;
			}
			string dataSaved = temp->data;
			deleteLine(ylineGiven);
			insertinBetween(dataSaved,xlineGiven);
		}
		undoorder moveHeadToX;
		moveHeadToX.commandNumber = 2;
		moveHeadToX.yLine = ylineGiven;
		moveHeadToX.xLine= xlineGiven;
		undoStack.push(moveHeadToX);
	}

    //Standalone Functions:
	void printPage(int pageGiven){                                  //Printing function used to print coming and previous page
		node *temp = head;
		if (numOfLines < pageGiven*10)
		{
			int iterator = 1;
			while(iterator < (pageGiven*10)-9){
				temp = temp->next;
				iterator++;
			}
			for (int start = (pageGiven*10)-9 ; start <= numOfLines; start++)
			{
				cout<<start<<") "<<temp->data<<endl;
				temp = temp->next;
			}
			cout<<"<<<<<<<Page "<<pageGiven<<">>>>>>>\n";
		}
		else if (numOfLines >= pageGiven * 10)
		{
			int iterator = 1;
			while(iterator < (pageGiven*10)-9){
				temp = temp->next;
				iterator++;
			}
			for (int start = (pageGiven*10)-9 ; start <= pageGiven*10; start++)
			{
				cout<<start<<") "<<temp->data<<endl;
				temp = temp->next;
			}
			cout<<"<<<<<<<Page "<<pageGiven<<">>>>>>>\n";
		}
		else if (pageGiven * 10 > numOfLines)
		{
			cout<<"No more pages"<<endl;
		}
	}

	void openFile(){					                            //Function used to open that located in the same project file
		infileagain:
		string fileName;
		cout<<"Enter the file name : ";
		cin>>fileName;
		fileName+=".txt";
		ifstream myfile;
		myfile.open(fileName);
		if (myfile){
                string s;
                while(getline(myfile,s))
                {
                    addinEnding(s);
                }
                myfile.close();

		}else {
            cout<< "Cannot find the file\n"<<endl;
		}
	}

	void undo(){                                                    //Function used to undo last move
		undoorder temp = undoStack.top();
		if (temp.commandNumber == 1)
		{
			cout<<"Undo inserting line"<<endl;
			deleteBegining();
			undoStack.pop();
		}
		else if (temp.commandNumber == 2)
		{
			cout<<"Undo moving M to N"<<endl;
			moveYtoX(temp.xLine, temp.yLine);
			undoStack.pop();
		}
		else if (temp.commandNumber == 3)
		{
			cout<<"Undo deleting line"<<endl;;
			addinBegining(temp.text);
			undoStack.pop();
		}
		else if (temp.commandNumber == 4)
		{
			cout<<"Undo replacing line"<<endl;
			replaceLine(temp.text,temp.lineNumber);
			undoStack.pop();
		}
		else if (temp.commandNumber == 5)
		{
			cout<<"Undo inserting line"<<endl;
			deleteLine(temp.lineNumber);
			undoStack.pop();
		}
		else if (temp.commandNumber == 6)
		{
			cout<<"Undo deleting line"<<endl;
			addinEnding(temp.text);
			undoStack.pop();
		}
		else if (temp.commandNumber == 7)
		{
			cout<<"Undo inserting line"<<endl;
			deleteEnding();
			undoStack.pop();
		}
		else if (temp.commandNumber == 8)
		{
			int semicounter = temp.lineNumber;
			while(semicounter >= 0){
				keeppopTail();
				semicounter--;
			}
			undoStack.pop();
		}
		else if (temp.commandNumber == 9)
		{
			cout<<"Undo deleting line"<<endl;
			insertinBetween(temp.text, temp.lineNumber);
			undoStack.pop();
		}
		else if (temp.commandNumber == 10)
		{
			cout<<"Undo deleting line"<<endl;
			addinEnding(temp.text);
			undoStack.pop();
		}
		else if (temp.commandNumber == 11)
		{
			cout<<"Undo deleting line"<<endl;
			addinBegining(temp.text);
			undoStack.pop();
		}
	}

	void printall(){                                                //Function used to print everything in .txt file
		node *temp = head;
		int linePrinted = 1;
		int pagePrinted = 2;
		int choice;
		if (head == NULL)
		{
			cout<<"No lines to print"<<endl;
		}
		else{
			while(temp!=NULL)
			{
				if (linePrinted == 1)
				{
					cout<<"<<<<<<<Page 1>>>>>>>\n";
				}
				else if ((linePrinted-1) % 10 == 0)
				{
					cout<<"<<<<<<<Page "<<pagePrinted<<">>>>>>>\n";
					pagePrinted++;

				}
				cout<<linePrinted<<") "<<temp->data<<endl;
				temp = temp->next;
				linePrinted++;
			}
		}
	}

	void saveFile(){                                                //Function used to save all text into .txt file
		node *temp = head;
		int linePrinted = 1;
		int pagePrinted = 2;
		string fileName;
		cout<<"Enter the file name : ";
		cin>>fileName;
		fileName+=".txt";
		outfile.open(fileName, ios_base::app);
		while(temp!=NULL)
		{
			outfile<<temp->data;
			temp = temp->next;
			linePrinted++;
		}
		outfile.flush();
		outfile.close();
	}

};

int main(int argc, char const *argv[])
{
	linked_list ourList;
	return 0;
}
