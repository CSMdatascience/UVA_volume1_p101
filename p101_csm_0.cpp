/*	This problem is from the UVA Online Judge problem set volume 1 (100-199)
URL: https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=3&page=show_problem&problem=37


From the assignment sheet:

"The valid commands for the robot arm that manipulates blocks are:
• move a onto b
where a and b are block numbers, puts block a onto block b after returning any blocks that are
stacked on top of blocks a and b to their initial positions.
• move a over b
where a and b are block numbers, puts block a onto the top of the stack containing block b, after
returning any blocks that are stacked on top of block a to their initial positions.
• pile a onto b
where a and b are block numbers, moves the pile of blocks consisting of block a, and any blocks
that are stacked above block a, onto block b. All blocks on top of block b are moved to their
initial positions prior to the pile taking place. The blocks stacked above block a retain their order
when moved.
• pile a over b
where a and b are block numbers, puts the pile of blocks consisting of block a, and any blocks
that are stacked above block a, onto the top of the stack containing block b. The blocks stacked
above block a retain their original order when moved.
• quit
terminates manipulations in the block world.
Any command in which a = b or in which a and b are in the same stack of blocks is an illegal
command. All illegal commands should be ignored and should have no affect on the configuration of
blocks.
Input
The input begins with an integer n on a line by itself representing the number of blocks in the block
world. You may assume that 0 < n < 25.
The number of blocks is followed by a sequence of block commands, one command per line. Your
program should process all commands until the quit command is encountered.
You may assume that all commands will be of the form specified above. There will be no syntactically
incorrect commands.
Output
The output should consist of the final state of the blocks world. Each original block position numbered
i (0  i < n where n is the number of blocks) should appear followed immediately by a colon. If there
is at least a block on it, the colon must be followed by one space, followed by a list of blocks that appear
stacked in that position with each block number separated from other block numbers by a space. Don’t
put any trailing spaces on a line.
There should be one line of output for each block position (i.e., n lines of output where n is the
integer on the first line of input).
Sample Input
10
move 9 onto 1
move 8 over 1
move 7 over 1
move 6 over 1
pile 8 over 6
pile 8 over 5
move 2 over 1
move 4 over 9
quit
Sample Output
0: 0
1: 1 9 2 4
2:
3: 3
4:
5: 5 8 7 6
6:
7:
8:
9:
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
#include <string>

using namespace std;

//Finds block containing a
int findBlock(vector<vector<int>> blocks, int a)
{
	int aBlock = -1;
	for (unsigned int i = 0; i < blocks.size();)
		if (find(blocks.at(i).begin(), blocks.at(i).end(), a) != blocks.at(i).end())
			return i;
		else
			i++;
}

//Finds position of a in aBlock
int findPos(vector<vector<int>> blocks, int ablock, int a)
{

	for (unsigned int i = 0; i < blocks.at(ablock).size(); )
		if (blocks.at(ablock).at(i) == a)
			return i;
		else
			i++;
}

//Checks if a and b are in same pile
bool samePile(vector<vector<int>> blocks, int a, int b)
{
	if (findBlock(blocks, a) == findBlock(blocks, b))
		return true;
	else
		return false;
}

//Returns blocks stacked on top of a to their intial positions
void returnBlocks(vector<vector<int>> &blocks, int aBlock, int a)
{
	while (blocks.at(aBlock).back() != a)
	{
		int temp = blocks.at(aBlock).back();
		blocks.at(temp).push_back(temp);
		blocks.at(aBlock).pop_back();
	}
}

//Returns blocks stacked on top of a and b to their initial positions, then stacks a on top of b
void moveAontoB(vector<vector<int>> &blocks, int a, int b)
{
	int aBlock = findBlock(blocks, a);
	int bBlock = findBlock(blocks, b);
	
	//return blocks stacked on top of a and b to their initial positions
	returnBlocks(blocks, aBlock, a);
	returnBlocks(blocks, bBlock, b);

	//move a onto b
	blocks.at(bBlock).push_back(a);
	blocks.at(aBlock).pop_back();
}

//Returns blocks stacked on top of a to their initial positions, then stacks a on top of stack containing b
void moveAoverB(vector<vector<int>> &blocks, int a, int b)
{
	int aBlock = findBlock(blocks, a);
	int bBlock = findBlock(blocks, b);

	returnBlocks(blocks, aBlock, a);		

	blocks.at(bBlock).push_back(a);
	blocks.at(aBlock).pop_back();
}

//Returns blocks stacked on top of b to their initial positions, then stacks a and any values stacked above a onto b
void pileAontoB(vector<vector<int>> &blocks, int a, int b)
{
	int aBlock = findBlock(blocks, a);
	int bBlock = findBlock(blocks, b);
	int aPos = findPos(blocks, aBlock, a);
	

	//return blocks stacked on top of b to their intial positions
	returnBlocks(blocks, bBlock, b);
	
	//stack will hold a and any values stacked above it
	stack<int> temp;

	//load up the stack 
	while (blocks.at(aBlock).size() > aPos)
	{
		temp.push(blocks.at(aBlock).back());
		blocks.at(aBlock).pop_back();
	} 

	//deposit stack LIFO on top of b
	while (!temp.empty())
	{
		blocks.at(bBlock).push_back(temp.top());
		temp.pop();
	}
}

//Stacks a and any values stacked above a on top of the stack containing b
void pileAoverB(vector<vector<int>> &blocks, int a, int b)
{
	int aBlock = findBlock(blocks, a);
	int bBlock = findBlock(blocks, b);
	int aPos = findPos(blocks, aBlock, a);
	//stack will hold a and any values stacked above it
	stack<int> temp;

	//load up the stack 
	while (blocks.at(aBlock).size() > aPos)
	{
		temp.push(blocks.at(aBlock).back());
		blocks.at(aBlock).pop_back();
	}

	//deposit stack LIFO on top of b
	while (!temp.empty())
	{
		blocks.at(bBlock).push_back(temp.top());
		temp.pop();
	}
}

//Holds a row of stacks of blocks
vector<vector<int>> blocks;
char ans;

int main()
{
	
	//num_ is number of blocks
	//a and b are operands
	int num_, a, b;

	do {
		//num is string form of number input - we want to check for errors in user input / type conversion to int
		//action is the operation to perform
		//prep is preposition that refines action
		string num, action, prep;

		//get whole line of input for exception handling
		cout << "Enter number of blocks (0 < n < 25):\n";
		getline(cin, num);

		//convert num to the first substring in user input
		//after splitting string, make sure it's an integer
		try
		{
			//holds position of first ' ' character, if there is one
			int pos;
			//find position of first ' ' character
			pos = num.find(' ', 0);

			//if a ' ' character is found, take substring before the ' '
			if (pos != string::npos)
			{
				num = num.substr(0, pos);
			}
			//convert the substring to integer
			num_ = stoi(num);
		}
		//if stoi fails (i.e. returns const invalid_argument&, enter new num
		catch (const invalid_argument&)
		{
			cout << "Enter number of blocks (0 < n < 25):\n";
			getline(cin, num);
		}
		
		//initialize blocks vector with as many stacks of blocks as num
		for (int i = 0; i < num_; i++)
		{
			//each block will be its own stack
			vector<int> block;
			block.push_back(i);
			blocks.push_back(block);

		}
		cout << "Enter a series of commands ('quit' to terminate):\n";
		cin >> action;

		do
		{
			cin >> a >> prep >> b;

			//if a and be are identical, do nothing
			//if a and b are on the same pile, do nothing
			if (a != b && !samePile(blocks, a, b))
			{
				if ((action == "move") && (prep == "onto"))
					moveAontoB(blocks, a, b);
				else if ((action == "move") && (prep == "over"))
					moveAoverB(blocks, a, b);
				else if ((action == "pile") && (prep == "onto"))
					pileAontoB(blocks, a, b);
				else if ((action == "pile") && (prep == "over"))
					pileAoverB(blocks, a, b);
			}

			cin >> action;
		} while (action != "quit");

		cout << "Current state:\n";
		//print results
		for (int i = 0; i < num_; i++)
		{
			cout << i << ": ";

			for (unsigned int j = 0; j < blocks.at(i).size(); j++)
				cout << blocks.at(i).at(j) << " ";

			cout << endl;
		}
		blocks.clear();
		cout << "Again? ('Y' or 'y')\n";
		cin >> ans;
		ans = tolower(ans);
		cin.ignore();
	} while (ans == 'y');

	return 0;
}

/*Sample Output:

//Cases of moveAontoB:

Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 9 onto 1
quit
Current state:
0: 0
1: 1 9
2: 2
3: 3
4: 4
5: 5
6: 6
7: 7
8: 8
9:
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 9 onto 1
move 1 onto 2
quit
Current state:
0: 0
1:
2: 2 1
3: 3
4: 4
5: 5
6: 6
7: 7
8: 8
9: 9
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 8 onto 2
move 1 onto 2
quit
Current state:
0: 0
1:
2: 2 1
3: 3
4: 4
5: 5
6: 6
7: 7
8: 8
9: 9
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
move 9 onto 1
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 9 onto 1
move 8 onto 2
move 1 onto 2
quit
Current state:
0: 0
1:
2: 2 1
3: 3
4: 4
5: 5
6: 6
7: 7
8: 8
9: 9
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
move 9 onto 1
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 9 onto 1
move 9 onto 2
quit
Current state:
0: 0
1: 1
2: 2 9
3: 3
4: 4
5: 5
6: 6
7: 7
8: 8
9:
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 8 onto 2
move 1 onto 8
quit
Current state:
0: 0
1:
2: 2 8 1
3: 3
4: 4
5: 5
6: 6
7: 7
8:
9: 9
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 9 onto 1
move 8 over 1
move 9 onto 2
quit
Current state:
0: 0
1: 1
2: 2 9
3: 3
4: 4
5: 5
6: 6
7: 7
8: 8
9:
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 9 onto 2
move 8 over 2
move 1 onto 9
quit
Current state:
0: 0
1:
2: 2 9 1
3: 3
4: 4
5: 5
6: 6
7: 7
8: 8
9:
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 9 onto 1
move 8 over 1
move 7 onto 2
move 9 onto 2
quit
Current state:
0: 0
1: 1
2: 2 9
3: 3
4: 4
5: 5
6: 6
7: 7
8: 8
9:
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 9 onto 1
move 8 onto 2
move 7 over 2
move 1 onto 8
quit
Current state:
0: 0
1:
2: 2 8 1
3: 3
4: 4
5: 5
6: 6
7: 7
8:
9: 9
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 9 onto 1
move 8 over 1
move 7 onto 2
move 9 onto 7
quit
Current state:
0: 0
1: 1
2: 2 7 9
3: 3
4: 4
5: 5
6: 6
7:
8: 8
9:
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 9 onto 1
move 8 onto 2
move 7 over 2
move 9 onto 8
quit
Current state:
0: 0
1: 1
2: 2 8 9
3: 3
4: 4
5: 5
6: 6
7: 7
8:
9:
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 9 onto 1
move 8 over 1
move 7 onto 2
move 6 over 2
move 9 onto 7
quit
Current state:
0: 0
1: 1
2: 2 7 9
3: 3
4: 4
5: 5
6: 6
7:
8: 8
9:
Again? ('Y' or 'y')
n
Press any key to continue . . .

//Cases of moveAoverB
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 9 over 1
quit
Current state:
0: 0
1: 1 9
2: 2
3: 3
4: 4
5: 5
6: 6
7: 7
8: 8
9:
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 9 over 1
move 1 over 2
quit
Current state:
0: 0
1:
2: 2 1
3: 3
4: 4
5: 5
6: 6
7: 7
8: 8
9: 9
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
move 9 over 2
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 9 over 2
move 1 over 2
quit
Current state:
0: 0
1:
2: 2 9 1
3: 3
4: 4
5: 5
6: 6
7: 7
8: 8
9:
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
mover 9 over 1
move 8 over 2
move 9 over 1
quit
Current state:
0: 0
1: 1 9
2: 2 8
3: 3
4: 4
5: 5
6: 6
7: 7
8:
9:
Again? ('Y' or 'y')

//Cases of pileAontoB


Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
pile 9 onto 1
quit
Current state:
0: 0
1: 1 9
2: 2
3: 3
4: 4
5: 5
6: 6
7: 7
8: 8
9:
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 9 onto 1
pile 1 onto 2
quit
Current state:
0: 0
1:
2: 2 1 9
3: 3
4: 4
5: 5
6: 6
7: 7
8: 8
9:
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 8 onto 2
pile 1 onto 2
quit
Current state:
0: 0
1:
2: 2 1
3: 3
4: 4
5: 5
6: 6
7: 7
8: 8
9: 9
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 9 onto 1
move 8 onto 2
pile 1 onto 2
quit
Current state:
0: 0
1:
2: 2 1 9
3: 3
4: 4
5: 5
6: 6
7: 7
8: 8
9:
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 9 onto 1
pile 9 onto 2
quit
Current state:
0: 0
1: 1
2: 2 9
3: 3
4: 4
5: 5
6: 6
7: 7
8: 8
9:
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 8 onto 2
pile 1 onto 8
quit
Current state:
0: 0
1:
2: 2 8 1
3: 3
4: 4
5: 5
6: 6
7: 7
8:
9: 9
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 9 onto 1
move 8 over 1
pile 9 onto 2
quit
Current state:
0: 0
1: 1
2: 2 9 8
3: 3
4: 4
5: 5
6: 6
7: 7
8:
9:
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 9 onto 2
move 8 over 2
pile 1 onto 9
quit
Current state:
0: 0
1:
2: 2 9 1
3: 3
4: 4
5: 5
6: 6
7: 7
8: 8
9:
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 9 onto 1
move 8 over 1
move 7 onto 2
pile 9 onto 2
quit
Current state:
0: 0
1: 1
2: 2 9 8
3: 3
4: 4
5: 5
6: 6
7: 7
8:
9:
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 9 onto 1
move 8 onto 2
move 7 over 2
pile 1 onto 8
quit
Current state:
0: 0
1:
2: 2 8 1 9
3: 3
4: 4
5: 5
6: 6
7: 7
8:
9:
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 9 onto 1
move 8 over 1
move 7 onto 2
pile 9 onto 7
quit
Current state:
0: 0
1: 1
2: 2 7 9 8
3: 3
4: 4
5: 5
6: 6
7:
8:
9:
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 9 onto 1
move 8 onto 2
move 7 over 2
pile 9 onto 8
quit
Current state:
0: 0
1: 1
2: 2 8 9
3: 3
4: 4
5: 5
6: 6
7: 7
8:
9:
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
move 9 onto 1
move 8 over 1
move 7 onto 2
move 6 over 2
pile 9 onto 7
quit
Current state:
0: 0
1: 1
2: 2 7 9 8
3: 3
4: 4
5: 5
6: 6
7:
8:
9:
Again? ('Y' or 'y')
n
Press any key to continue . . .

//Cases of pileAoverB


Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
pile 9 over 1
quit
Current state:
0: 0
1: 1 9
2: 2
3: 3
4: 4
5: 5
6: 6
7: 7
8: 8
9:
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
pile 9 over 1
pile 1 over 2
quit
Current state:
0: 0
1:
2: 2 1 9
3: 3
4: 4
5: 5
6: 6
7: 7
8: 8
9:
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
pile 9 over 2
pile 1 over 9
quit
Current state:
0: 0
1:
2: 2 9 1
3: 3
4: 4
5: 5
6: 6
7: 7
8: 8
9:
Again? ('Y' or 'y')
y
Enter number of blocks (0 < n < 25):
10
Enter a series of commands ('quit' to terminate):
pile 9 over 1
pile 8 over 1
pile 7 over 2
pile 6 over 2
pile 9 over 7
quit
Current state:
0: 0
1: 1
2: 2 7 6 9 8
3: 3
4: 4
5: 5
6:
7:
8:
9:
Again? ('Y' or 'y')
*/