#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <vector>

using namespace std;

class armedbandit
{
	int max_reward;
public:
	armedbandit(int maxreward) : max_reward(maxreward) {}  // Constructor sets a value to be used as the "max reward" possible

	double onepull()
	{
		double number;
		double odds_are = (double)rand() / RAND_MAX;
		if (odds_are > 0.5)
			number = (double)rand() / RAND_MAX * 100 + max_reward;  // 50% chance of pulling max_reward + random number between 0 and 100
		else
			number = (double)rand() / RAND_MAX * -100 + max_reward;  // 50% chance of pulling max_reward - random number between 0 and 100
		return number;  // Return a value to be used as the reward for the one pull
	}
};

class Qlearner
{
public:
	vector<double> q;
	int index_of_best = -1;

	int times = 5000;                //number of episodes
	int action = 0;
	int machines = 5;              //number of slot machines. If this number is changed, two other spots of code MUST be changed, and a third SHOULD be changed

	double alpha = 0.1;            //learning reate
	double reward = 0;             //R
	double gamma = 0.0;            //discount factor
	double epsilon = 0.1;          //chance of picking a random machine

	void init_q();
	int decide();
	int greedy_action();
	int rand_action();
	void update_q();
};

void Qlearner::init_q()
{
	for (int j = 0; j < machines; j++)
	{
		q.push_back(100);  // starting q-value for each slot machine
	}
}

int Qlearner::decide()
{
	for (int i = 0; i < times; i++)
	{
		double a = (double)rand() / RAND_MAX;
		if (a < epsilon)
			action = rand_action();
		else
			action = greedy_action();
	}
	return action;
}

int Qlearner::greedy_action()
{
	double current_best = -9999999999999;
	for (int i = 0; i < machines; i++)
	{
		if (q.at(i) > current_best)
		{
			index_of_best = i;
			current_best = q.at(i);
		}
	}
	return index_of_best;
}

int Qlearner::rand_action()
{
	int a = rand() % machines;
	return a;
}

void Qlearner::update_q()
{
	cout << "Qlearner chose bandit #   " << action+1 << endl;

	q.at(action) = q.at(action) + alpha*(reward + gamma - q.at(action));  // update the q-value for the machine picked this episode

	cout << q.at(0) << "    " << q.at(1) << "     " << q.at(2) << "     " << q.at(3) << "     " << q.at(4) << endl; // prints to screen the current q-value for each machine
}

int main()
{
	srand(time(NULL));

	armedbandit bandit1(100), bandit2(200), bandit3(100), bandit4(150), bandit5(220);  // must have the number of different bandits = "machines" variable

	Qlearner yup;

	vector<armedbandit> casino;  // vector to store our slot machines

	casino.push_back(bandit1);
	casino.push_back(bandit2);
	casino.push_back(bandit3);
	casino.push_back(bandit4);
	casino.push_back(bandit5);

	//for (int j = 0; j < yup.machines; j++)
	//{
	//	casino.push_back(0);
	//}

	yup.init_q();

	for (int i = 0; i < yup.times; i++)
	{
		int action = yup.decide();
		yup.reward = casino.at(action).onepull();
		yup.update_q();
	}

	return 0;
}