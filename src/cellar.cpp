#include "cellar.h"
#include "utils.h"

using namespace std;
using namespace UTILS;

CELLAR::CELLAR(int size, int bottles, int shelves, int crates)
:   Grid(size, size),
    Size(size),
    NumBottles(bottles),
	 NumShelves(shelves),
	 NumCrates(crates),
    SmartMoveProb(0.95),
    UncertaintyCount(0)
{
    
	 NumObjects = shelves + crates;	 
	 NumObjectTypes = 2;
    NumActions = NumBottles + NumObjects + E_SAMPLE+1;
	 E_OBJCHECK = NumBottles + E_SAMPLE + 1; //First object check action
    NumObservations = 3 + NumObjectTypes; //none,good,bad + each object type
    RewardRange = 50; //NOTE: exploration rate recommended to be H_max - H_min, max with c = 0, min with rollouts
    Discount = 0.99;  //TODO: proper discount horizon?
	
	 //RandomSeed(time(NULL));

    if (size == 7 && bottles == 8 && shelves == 7 && crates == 8)
        Init_7_8();
    else if (size == 11 && bottles == 11 && shelves == 15 && crates == 15)
        Init_11_11();
	 else if (size == 5 && bottles == 1 && shelves == 0 && crates == 4)
		  Init_5_1();
	 else if (size == 5 && bottles == 2 && shelves == 0 && crates == 4)
		  Init_5_2();
    else
        InitGeneral();
}

void CELLAR::Init_5_1(){
	cout << "Using special layout for cellar(5, 1)" << endl;
	
	COORD bottles[] =
	{
	  COORD(2, 2)	  
	};
	
	COORD objects[] =
	{ 
		//Crates
		COORD(1, 2),
		COORD(2, 1),
		COORD(2, 3),
		COORD(3, 2)	
    };
	 
	HalfEfficiencyDistance = 20;
	StartPos = COORD(0, 2);
	Grid.SetAllValues(-1);
	for (int i = 0; i < NumBottles; ++i)
	{
		Grid(bottles[i]) = i;
		BottlePos.push_back(bottles[i]);
	}
	for (int i = 0; i < NumObjects; ++i)
	{      
		if(i < NumShelves)
			Grid(objects[i]) = 100+E_SHELF;
		else
			Grid(objects[i]) = 100+E_CRATE;
		ObjectPos.push_back(objects[i]);
	}
}

void CELLAR::Init_5_2(){
	cout << "Using special layout for cellar(5, 2)" << endl;
	
	COORD bottles[] =
	{		
		COORD(2, 2),
		COORD(0, 0)
	};
	
	COORD objects[] =
	{ 
		//Crates
		COORD(1, 2),
		COORD(2, 1),
		COORD(2, 3),
		COORD(3, 2)	
    };
	 
	HalfEfficiencyDistance = 20;
	StartPos = COORD(0, 2);
	Grid.SetAllValues(-1);
	
	for (int i = 0; i < NumBottles; ++i)
	{
		Grid(bottles[i]) = i;
		BottlePos.push_back(bottles[i]);
	}
	
	for (int i = 0; i < NumObjects; ++i)
	{      
		if(i < NumShelves)
			Grid(objects[i]) = 100+E_SHELF;
		else
			Grid(objects[i]) = 100+E_CRATE;
		ObjectPos.push_back(objects[i]);
	}
}

void CELLAR::Init_7_8(){
	cout << "Using special layout similar to rocksample(7, 8)" << endl;
	
	COORD bottles[] =
	{
	  COORD(0, 4),
	  COORD(1, 1),
	  COORD(1, 6),
	  COORD(3, 0),
	  COORD(3, 6),
	  COORD(5, 0),
	  COORD(5, 6),
	  COORD(6, 5)
	};
	
	COORD objects[] =
	{
		//Shelves
		COORD(0, 3),
      COORD(0, 5),
      COORD(1, 0),
		COORD(2, 6),
		COORD(4, 0),
		COORD(4, 6),
		COORD(6, 0),
		 
		//Crates
		COORD(0, 1),
		COORD(1, 4),
		COORD(2, 1),
		COORD(3, 1),
		COORD(3, 5),
		COORD(4, 3),
		COORD(5, 1),
		COORD(5, 4)
    };
	 
	HalfEfficiencyDistance = 20;
	StartPos = COORD(0, 2);
	Grid.SetAllValues(-1);
	for (int i = 0; i < NumBottles; ++i)
	{
		Grid(bottles[i]) = i;
		BottlePos.push_back(bottles[i]);
	}
	for (int i = 0; i < NumObjects; ++i)
	{      
		if(i < NumShelves)
			Grid(objects[i]) = 100+E_SHELF;
		else
			Grid(objects[i]) = 100+E_CRATE;
		ObjectPos.push_back(objects[i]);
	}
}

void CELLAR::Init_11_11(){
	// Equivalent to RockSample_11_11.pomdp(x)
    cout << "Using special layout similar to rocksample(11, 11)" << endl;

	 /*NumShelves = 15;
	 NumCrates = 15;
	 NumObjects = NumShelves + NumCrates;*/
	
    COORD bottles[] =
    {
        COORD(0, 3),
        COORD(0, 7),
        COORD(1, 8),
        COORD(2, 4),
        COORD(3, 3),
        COORD(3, 8),
        COORD(4, 3),
        COORD(5, 8),
        COORD(6, 1),
        COORD(9, 3),
        COORD(9, 9)
    };
	 
	 COORD objects[] =
    {
		 //Shelves
		  COORD(0, 0),
        COORD(0, 10),
        COORD(1, 0),
		  COORD(1, 10),
		  COORD(2, 0),
		  COORD(2, 6),
 		  COORD(2, 10),        
        COORD(3, 6),        
        COORD(4, 5),
		  COORD(4, 6),
		  COORD(4, 7),
		  COORD(10, 0),
		  COORD(10, 1),
		  COORD(10, 2),
		  COORD(10, 3),
		  
		 
		 //Crates
		  COORD(0, 8),
		  COORD(1, 3),
		  COORD(1, 5),
		  COORD(1, 7),		  
		  COORD(4, 2),
		  COORD(4, 8),
		  COORD(5, 1),
		  COORD(5, 2),
		  COORD(5, 5),
		  COORD(6, 5),
		  COORD(7, 5),
		  COORD(7, 8),
		  COORD(8, 2),
		  COORD(8, 8),
		  COORD(9, 7)
    };

    HalfEfficiencyDistance = 20;
    StartPos = COORD(0, 5);
    Grid.SetAllValues(-1);
    for (int i = 0; i < NumBottles; ++i)
    {
        Grid(bottles[i]) = i;
        BottlePos.push_back(bottles[i]);
    }
	 for (int i = 0; i < NumObjects; ++i)
    {      
      if(i < NumShelves)
			Grid(objects[i]) = 100+E_SHELF;
		else
			Grid(objects[i]) = 100+E_CRATE;
		ObjectPos.push_back(objects[i]);
    }
}

/*
	The Grid exists for consistency across start states.  This way multiple (starting) instances of the same problem (ie. object location)
can be created.  This is used when creating state samples for belief state.
*/
void CELLAR::InitGeneral()
{
    HalfEfficiencyDistance = 20;
    StartPos = COORD(0, Size / 2);
    RandomSeed(0);
    Grid.SetAllValues(-1);
	//Place Bottles
    for (int i = 0; i < NumBottles; ++i)
    {
        COORD pos;
        do
        {
            pos = COORD(Random(Size), Random(Size));
        }
        while (Grid(pos) >= 0);
        Grid(pos) = i;
        BottlePos.push_back(pos);
    }
	 	 
	 //Place Objects
	 
	 for (int i = 0; i < NumObjects; ++i)
    {
      COORD pos;
      do
      {
          pos = COORD(Random(Size), Random(Size));
      }
      while (Grid(pos) != -1 || pos == StartPos);
      if(i < NumShelves)
			Grid(pos) = 100+E_SHELF;
		else
			Grid(pos) = 100+E_CRATE;
		ObjectPos.push_back(pos);
    }
	 
	 /*
	 for (int i = 0; i < NumCrates; ++i)
    {
      COORD pos;
      do
      {
          pos = COORD(Random(Size), Random(Size));
      }
      while (Grid(pos) != -1);
      //Grid(pos) = 100 + NumShelves +i; //Start counting at 100 for objects
		ObjectPos.push_back(pos);
    }*/
}

STATE* CELLAR::Copy(const STATE& state) const
{
    const CELLAR_STATE& cellarstate = safe_cast<const CELLAR_STATE&>(state);
    CELLAR_STATE* newstate = MemoryPool.Allocate();
    *newstate = cellarstate;
    return newstate;
}

void CELLAR::Validate(const STATE& state) const
{
    const CELLAR_STATE& cellarstate = safe_cast<const CELLAR_STATE&>(state);
    assert(Grid.Inside(cellarstate.AgentPos));
}

STATE* CELLAR::CreateStartState() const
{
    CELLAR_STATE* cellarstate = MemoryPool.Allocate();
    cellarstate->AgentPos = StartPos;
	 cellarstate->CollectedBottles = 0;
    cellarstate->Bottles.clear();
	 cellarstate->Objects.clear();
	
    for (int i = 0; i < NumBottles; i++)
    {
        CELLAR_STATE::ENTRY entry;
        entry.Collected = false;
		  if(NumBottles <= 2){
			  //For the special cases with 1 and 2 bottles, make sure the first one is always good and the second one bad
				entry.Valuable = (i == 0)? 1 : 0;				
		  }
		  else
				entry.Valuable = Bernoulli(0.5);
		  
        entry.Count = 0;
        entry.Measured = 0;
        entry.ProbValuable = 0.5;
        entry.LikelihoodValuable = 1.0;
        entry.LikelihoodWorthless = 1.0;
        cellarstate->Bottles.push_back(entry);
    }
	 
	 /*
		TODO: This array must be managed online when objects are:
			1) Discovered
			2) Considered irrelevant
	 
		In the Step function, when action is "search" (or w/e) and observation is an object,
	 it should be added with some probability.
	 
		It could either:
			a) Detect the object accurately and estimate its usefulness (discard or not)
			b) Try to detect its type AND usefulness
	 */	 
	 //COORD pos;
	 /*std::vector<int> types;	 
	 for(int i=0;i<NumShelves;i++) types.push_back(E_SHELF);
	 for(int i=0;i<NumCrates;i++) types.push_back(E_CRATE);	 
	 std::random_shuffle(types.begin(), types.end());*/
	 for (int i = 0; i < NumObjects; i++)
    {
        CELLAR_STATE::OBJ_ENTRY entry;
        entry.Type = Grid(ObjectPos[i])-100; //types[i];
		  /*do{
				pos = COORD(Random(Size), Random(Size));
		  }while(!EmptyTile(*cellarstate, pos));*/
        entry.ObjPos = ObjectPos[i];
        entry.Count = 0;
        entry.Measured = 0;
        entry.ProbCrate = 0.5;
        entry.LikelihoodCrate = 1.0;
        entry.LikelihoodShelf = 1.0;
		  entry.AssumedType = E_NONE;
        cellarstate->Objects.push_back(entry);
    }
	 /*
	 for (int i = NumShelves; i < NumObjects; i++)
    {
        CELLAR_STATE::OBJ_ENTRY entry;
        entry.Type = E_CRATE;
		  do{
				pos = COORD(Random(Size), Random(Size));
		  }while(!EmptyTile(*cellarstate, pos));
        entry.ObjPos = ObjectPos[i];
        entry.Count = 0;
        entry.Measured = 0;
        entry.ProbCrate = 0.5;
        entry.LikelihoodCrate = 1.0;
        entry.LikelihoodShelf = 1.0;
        cellarstate->Objects.push_back(entry);
    }*/
	 
	 assert(cellarstate->Objects.size() == NumObjects);
    //cellarstate->Target = SelectTarget(*cellarstate);
    
	 
	 return cellarstate;
}

void CELLAR::FreeState(STATE* state) const
{
    CELLAR_STATE* cellarstate = safe_cast<CELLAR_STATE*>(state);
    MemoryPool.Free(cellarstate);
}

bool CELLAR::Step(STATE& state, int action,
    int& observation, double& reward) const
{
	 //cout << "Rolloutlevel: " << Knowledge.RolloutLevel << endl;
	 if(Knowledge.RolloutLevel >= KNOWLEDGE::PGS){
		 //cout << "Calling Step PGS" << endl;
		 return StepPGS(state, action, observation, reward);
	 }
	 else
		 return StepNormal(state, action, observation, reward);
}

/*** Added by JCS

		Step function with new reward distribution using PGS action biases
***/
bool CELLAR::StepPGS(STATE& state, int action,
    int& observation, double& reward) const
{
	double scale = 10.0;
	double r = 0.0;
	double r2 = 0.0;
	STATE* oldstate = Copy(state);	
	
	bool result = StepNormal(state, action, observation, reward);
	 // Potential-based reward bonus
	
	if(reward != -100){//Not terminal or out of bounds
		r2 = PGS(*oldstate);
		r = PGS_RO(*oldstate, state, action, r2);
		
		
		//cout << "reward = " << reward << ", r1 = " << r << ", r2 = " << r2 << endl;
				
		reward += scale*r - scale*r2;
	}
	FreeState(oldstate);
	
	return result;
}

/*
	TODO: punishment for all non-rewarded actions? Eg. checking, etc.  The idea is they take time.
*/
bool CELLAR::StepNormal(STATE& state, int action,
    int& observation, double& reward) const
{
    CELLAR_STATE& cellarstate = safe_cast<CELLAR_STATE&>(state);
    reward = 0;
    observation = E_NONE;
	
	 double reward_check = -0.5;
	 int reward_move = -1;
	 int reward_push = -2;
	
	 int reward_sample = 10;
	 int reward_terminal = 10;
	 int punishment = -10;
	 int outOfBounds = -100;

    if (action < E_PUSHNORTH) // move
    {
		  COORD pos;
		  reward = reward_move; //NOTE: movement punishment
        switch (action)
        {
            case COORD::E_EAST:
					 pos = COORD(cellarstate.AgentPos.X+1, cellarstate.AgentPos.Y);
                if (cellarstate.AgentPos.X + 1 < Size){
						 if(FreeTile(cellarstate, pos)) cellarstate.AgentPos.X++;
                   break;
					 }
                else
                {// Maze exit
						if(cellarstate.CollectedBottles){
                    reward = reward_terminal;
                    return true;
						}
						break;
                }
					 
            case COORD::E_NORTH:
					 pos = COORD(cellarstate.AgentPos.X, cellarstate.AgentPos.Y+1);
                if (cellarstate.AgentPos.Y + 1 < Size){
						  if(FreeTile(cellarstate, pos)) cellarstate.AgentPos.Y++;
					 }
                else
                    reward = outOfBounds;
                break;

            case COORD::E_SOUTH:
					 pos = COORD(cellarstate.AgentPos.X, cellarstate.AgentPos.Y-1);
                if (cellarstate.AgentPos.Y - 1 >= 0){
						 if(FreeTile(cellarstate, pos)) cellarstate.AgentPos.Y--;
					 }
                else
                    reward = outOfBounds;
                break;

            case COORD::E_WEST:
					 pos = COORD(cellarstate.AgentPos.X-1, cellarstate.AgentPos.Y);
                if (cellarstate.AgentPos.X - 1 >= 0){
						if(FreeTile(cellarstate, pos)) cellarstate.AgentPos.X--;
					 }
                else
                    reward = outOfBounds;
                break;
        }
    }

    if (action == E_SAMPLE) // sample
    {
        int bottle = Grid(cellarstate.AgentPos);
        if (bottle >= 0 && bottle < NumBottles && !cellarstate.Bottles[bottle].Collected)
        {
            cellarstate.Bottles[bottle].Collected = true;
            if (cellarstate.Bottles[bottle].Valuable){
					reward = reward_sample;
					cellarstate.CollectedBottles++;
				}
            else
                reward = punishment;
        }
        else
        {
            reward = outOfBounds;
        }
    }
	 
	 /* Push actions:
			- Crates cannot be pushed outside of the grid
			- Shelves cannot be budged and yield -10 reward
			- Pushing a crate moves it in the desired direction unless it's blocked by another object				
	 */
	if(action >= E_PUSHNORTH && action < E_SAMPLE){
		COORD pos1;
		COORD pos2;
		int offsetX = 0;
		int offsetY = 0;
		switch(action){
			case E_PUSHNORTH:
				pos1 = COORD(cellarstate.AgentPos.X, cellarstate.AgentPos.Y+1);
				pos2 = COORD(cellarstate.AgentPos.X, cellarstate.AgentPos.Y+2);
				offsetX = 0;
				offsetY = 1;
				break;
			case E_PUSHSOUTH:
				pos1 = COORD(cellarstate.AgentPos.X, cellarstate.AgentPos.Y-1);
				pos2 = COORD(cellarstate.AgentPos.X, cellarstate.AgentPos.Y-2);
				offsetX = 0;
				offsetY = -1;
				break;
			case E_PUSHWEST:
				pos1 = COORD(cellarstate.AgentPos.X-1, cellarstate.AgentPos.Y);
				pos2 = COORD(cellarstate.AgentPos.X-2, cellarstate.AgentPos.Y);
				offsetX = -1;
				offsetY = 0;
				break;
			case E_PUSHEAST:
				pos1 = COORD(cellarstate.AgentPos.X+1, cellarstate.AgentPos.Y);
				pos2 = COORD(cellarstate.AgentPos.X+2, cellarstate.AgentPos.Y);
				offsetX = 1;
				offsetY = 0;
				break;
		}
	
	 /* Push crate and move agent IF: 
		 - there is a crate is the request direction
		 - the crate will end up inside the grid
		 - the crate will end up on an empty cell
		*/
		if (Grid.Inside(pos1)){
			if(CrateAt(cellarstate, pos1) && Grid.Inside(pos2) && EmptyTile(cellarstate, pos2)){
				int objNum = ObjectNumber(cellarstate, pos1);				
								
				// Update the agent
				cellarstate.AgentPos.X += offsetX;
				cellarstate.AgentPos.Y += offsetY;
				
				// Update the objects known position
				cellarstate.Objects[objNum].ObjPos.X += offsetX;
				cellarstate.Objects[objNum].ObjPos.Y += offsetY;
				
				// TODO: Reward pushing CRATES?
				reward = reward_push; //NOTE: Same as move, to prevent pushing crates around for no reason
			}
			else// if(ShelfAt(cellarstate, pos1)) //Pushing a shelf
				reward = punishment;
			//TODO: pushing breaks bottles? (ie not valuable)
		}
	}

    if (action > E_SAMPLE && action < E_OBJCHECK) // Bottle check
    {
        int bottle = action - E_SAMPLE - 1;
        assert(bottle < NumBottles);
        observation = GetObservation(cellarstate, bottle, 1);
        cellarstate.Bottles[bottle].Measured++;

        double distance = COORD::EuclideanDistance(cellarstate.AgentPos, BottlePos[bottle]);
    	  double efficiency = (1 + pow(2, -distance / HalfEfficiencyDistance)) * 0.5;

        if (observation == E_GOOD)
        {
            cellarstate.Bottles[bottle].Count++;
            cellarstate.Bottles[bottle].LikelihoodValuable *= efficiency;
            cellarstate.Bottles[bottle].LikelihoodWorthless *= 1.0 - efficiency;

        }
        else
        {
            cellarstate.Bottles[bottle].Count--;
            cellarstate.Bottles[bottle].LikelihoodWorthless *= efficiency;
            cellarstate.Bottles[bottle].LikelihoodValuable *= 1.0 - efficiency;
		  }
		double denom = (0.5 * cellarstate.Bottles[bottle].LikelihoodValuable) +
			(0.5 * cellarstate.Bottles[bottle].LikelihoodWorthless);
		cellarstate.Bottles[bottle].ProbValuable = (0.5 * cellarstate.Bottles[bottle].LikelihoodValuable) / denom;
		  
		  //NOTE: Check action punishment
		  reward = reward_check;
    }
	 
	 // Check object returns a noisy reading of the type
	 // Approx. probability of object being a crate
	 if (action >= E_OBJCHECK) // Object check
    {
        int obj = action - E_OBJCHECK;
        assert(obj < NumObjects);
		 
        observation = GetObservation(cellarstate, obj, 2);
        cellarstate.Objects[obj].Measured++;

        double distance = COORD::EuclideanDistance(cellarstate.AgentPos, cellarstate.Objects[obj].ObjPos);
    	  double efficiency = (1 + pow(2, -distance / HalfEfficiencyDistance)) * 0.5;

        if (observation == E_CRATE)
        {
            cellarstate.Objects[obj].Count++;
            cellarstate.Objects[obj].LikelihoodCrate *= efficiency;
            cellarstate.Objects[obj].LikelihoodShelf *= 1.0 - efficiency;

        }
        else
        {
            cellarstate.Objects[obj].Count--;
            cellarstate.Objects[obj].LikelihoodShelf *= efficiency;
            cellarstate.Objects[obj].LikelihoodCrate *= 1.0 - efficiency;
		}
		double denom = (0.5 * cellarstate.Objects[obj].LikelihoodCrate) +
			(0.5 * cellarstate.Objects[obj].LikelihoodShelf);
		cellarstate.Objects[obj].ProbCrate = (0.5 * cellarstate.Objects[obj].LikelihoodCrate) / denom;
		
		//As soon as entropy is reduced, assume the closest type
		if(cellarstate.Objects[obj].AssumedType == E_NONE){
			double p = cellarstate.Objects[obj].ProbCrate;
			double binaryEntropy = -1*p*log2(p) - (1-p)*log2(1-p);
		
			if(binaryEntropy <= 0.5){
				//cout << "For object " << obj << " ";
				if(round(cellarstate.Objects[obj].ProbCrate)){
					cellarstate.Objects[obj].AssumedType = E_CRATE;
					//cout << "with P(crate) = " << cellarstate.Objects[obj].ProbCrate << " assumed CRATE" << endl;
				}
				else{
					cellarstate.Objects[obj].AssumedType = E_SHELF;
					//cout << "with P(crate) = " << cellarstate.Objects[obj].ProbCrate << " assumed SHELF" << endl;
				}
			}
		}
		//Note: Check action punishment
		reward = reward_check;
    }
	 
	 
/*
    if (cellarstate.Target < 0 || cellarstate.AgentPos == BottlePos[cellarstate.Target])
        cellarstate.Target = SelectTarget(cellarstate);*/

    //assert(reward != -100);
	 if(reward == -100){
			DisplayState(cellarstate, cout);
		   DisplayAction(action, cout);
		   int x = Size - cellarstate.AgentPos.Y;
			int y = cellarstate.AgentPos.X + 1;
		   cout << "AgentPos: " << cellarstate.AgentPos << endl;
		   cout << "Grid(" << x << "," << y <<") : "<< Grid(cellarstate.AgentPos) << endl;
		   if(Grid(cellarstate.AgentPos) >= 0 && Grid(cellarstate.AgentPos) < NumBottles) cout << "Bottle collected? " << cellarstate.Bottles[Grid(cellarstate.AgentPos)].Collected << endl;
		 assert(reward != -100);
	 }
    return false;
}


//TODO: figure out local object transformations for cellar domain
bool CELLAR::LocalMove(STATE& state, const HISTORY& history,
    int stepObs, const STATUS& status) const
{
    CELLAR_STATE& cellarstate = safe_cast<CELLAR_STATE&>(state);
    int bottle, obj;
	
	//Modify bottle
		bottle = Random(NumBottles);
		cellarstate.Bottles[bottle].Valuable = !cellarstate.Bottles[bottle].Valuable;	
	 /*if(Random(2)){
	
	 }
	 else{
		//Modify a random object
		obj = Random(NumObjects);
		if(cellarstate.Objects[obj].Type == E_CRATE)
			cellarstate.Objects[obj].Type = E_SHELF;
		else
			cellarstate.Objects[obj].Type = E_CRATE;
	 }*/
	 
    if (history.Back().Action > E_SAMPLE) // check
    {
		if(history.Back().Action < E_OBJCHECK){		  
        bottle = history.Back().Action - E_SAMPLE - 1;
        int realObs = history.Back().Observation;

        // Condition new state on real observation
        int newObs = GetObservation(cellarstate, bottle, 1);
        if (newObs != realObs)
            return false;

        // Update counts to be consistent with real observation
        if (realObs == E_GOOD && stepObs == E_BAD)
            cellarstate.Bottles[bottle].Count += 2;
        if (realObs == E_BAD && stepObs == E_GOOD)
            cellarstate.Bottles[bottle].Count -= 2;		  
	   }
		else{				
			//Compare with last observation for consistency with history
		  obj = history.Back().Action - E_OBJCHECK;
        int realObs = history.Back().Observation;

        // Condition new state on real observation
        int newObs = GetObservation(cellarstate, obj, 2);
        if (newObs != realObs)
            return false;

        // Update counts to be consistent with real observation
        if (realObs == E_CRATE && stepObs == E_SHELF)
            cellarstate.Objects[obj].Count += 2;
        if (realObs == E_SHELF && stepObs == E_CRATE)
            cellarstate.Objects[obj].Count -= 2;	
		}
    }
	 
    return true;
}

/*** PGS Rollout policy ***/
double CELLAR::PGS_RO(STATE& oldstate, STATE& state, int action, double oldpgs) const
{
	double points = 0.0;
	double oldpoints = 0.0;
	
	//1. Cast to cellarstate
	CELLAR_STATE& cellarstate = safe_cast<CELLAR_STATE&>(state);
	CELLAR_STATE& oldcellarstate = safe_cast<CELLAR_STATE&>(oldstate);
	
	int bottle = -1;
	if(action == E_SAMPLE){
		bottle = Grid(cellarstate.AgentPos);
		if (cellarstate.Bottles[bottle].Valuable){
			if(cellarstate.Bottles[bottle].Count)
				points++; //+1 for sampling rocks w/ good observations
		}
		else points--;
		
	}
	else if (action > E_SAMPLE && action < E_OBJCHECK){ //Bottle check
		bottle = action - E_SAMPLE - 1;
		double p = cellarstate.Bottles[bottle].ProbValuable;
		double binaryEntropy = -1*p*log2(p) - (1-p)*log2(1-p);
		if(binaryEntropy > 0.5) points--;
		
		p = oldcellarstate.Bottles[bottle].ProbValuable;
		double oldBinaryEntropy = -1*p*log2(p) - (1-p)*log2(1-p);
		if(oldBinaryEntropy > 0.5) oldpoints--;
		//else points -= 0.5;
	}
/*	else if (action >= E_OBJCHECK){ //Object check
		int obj = action - E_OBJCHECK;
		if(cellarstate.Objects[obj].AssumedType == E_NONE) points--;
		if(oldcellarstate.Objects[obj].AssumedType == E_NONE) oldpoints--;
		//else points--;
	}
	*/
	// TODO: verify this thing.
	// Bonus points for proximity to promising bottles
	//for(int bottle=0; bottle<NumBottles; ++bottle)
	//	if(COORD::EuclideanDistance(BottlePos[bottle], cellarstate.AgentPos) <= 1 && cellarstate.Bottles[bottle].Count) points += 0.25;
	
	//Update difference for current bottle
	double result = oldpgs - oldpoints + points;

	return result;
}


// PGS score
// TODO: find a better way to encourage good checking
/*
	THINK: is it an error to deduct points if(entropy)? Should it be entropy OR not measured?
*/
double CELLAR::PGS(STATE& state) const
{
	double points = 0.0;
	double heuristicProbLimit = 0.4;
	
	//1. Cast to cellarstate
	CELLAR_STATE& cellarstate = safe_cast<CELLAR_STATE&>(state);
	
	//2. Sample	
	for(int bottle=0; bottle < NumBottles; ++bottle){
		if(cellarstate.Bottles[bottle].Collected){
			if (cellarstate.Bottles[bottle].Valuable){
				if(cellarstate.Bottles[bottle].Count)
					points++; //+1 for sampling rocks w/ good observations
			}
			else points--;
		}
		else{
			double p = cellarstate.Bottles[bottle].ProbValuable;
			double binaryEntropy = -1*p*log2(p) - (1-p)*log2(1-p);
			if(binaryEntropy > 0.5) points--;
			
			// TODO: verify this thing.
			// Bonus points for proximity to promising bottles
			//if(COORD::EuclideanDistance(BottlePos[bottle], cellarstate.AgentPos) <= 1 &&	cellarstate.Bottles[bottle].Count) points += 0.25;
		}
	}
	
	//3. Punishment for uncertainty of objects
	//TODO: maybe punishing ALL objects is counter productive because the agent will try to check ALL of them.
	// In large problems like 11x11, this might be causing performance to drag.
/*
	for(int obj=0; obj < NumObjects; ++obj){
		if(cellarstate.Objects[obj].AssumedType == E_NONE) points--;
	}
	*/
	return points;
}

// PGS Rollout policy
// Computes PGS only for non Checking actions
void CELLAR::GeneratePGS(const STATE& state, const HISTORY& history,
    vector<int>& legal, const STATUS& status) const
{
	static vector<int> acts;
	acts.clear();
	STATE * newstate;
	STATE * oldstate = Copy(state);
	PGSLegal(state, history, acts, status);
	int numLegal = acts.size();
	//STATE * newstate = new STATE*[numLegal];
	//for(int i=0; i<numLegal; i++) newstates[i] = Copy(state);
	
	double pgs_values[numLegal]; //pgs_values[i] <-- legalMove[i]
	double pgs_state = PGS(*oldstate);	
		
	int max_p = -1;
	double max_v = -Infinity;	
	
	int observation;
	double reward;
	
	//cout << "Generating PGS values..." << endl;
	//cout << "Found " << numLegal << " legal actions." << endl;
		
	for(unsigned int i=0; i<numLegal; i++){		
		newstate = Copy(state);
		
		StepNormal(*newstate, acts[i], observation, reward); //Simulate transition with action a
		
		// Using regular PGS (slow)
		//pgs_values[i] = PGS(*newstate);
		
		// Adding only PGS differences (fast)
		pgs_values[i] = PGS_RO(*oldstate, *newstate, acts[i], pgs_state); //add differences

		FreeState(newstate);		
	}
	
	FreeState(oldstate);
	max_p = std::distance(pgs_values, max_element(pgs_values, pgs_values+numLegal));
	max_v = pgs_values[max_p];
	assert(max_p > -1);
	
	legal.push_back(acts[max_p]); //Add best action to return vector
	// Add other maxima
	for(int i=0; i<numLegal; i++){
		if(i != max_p && pgs_values[i] == max_v)
		//if(pgs_values[i] >= 0.5)
			legal.push_back(acts[i]);
	}

	//cout << "found " << legal.size() << " rollout actions " << endl;
}

void CELLAR::PGSLegal(const STATE& state, const HISTORY& history,
    vector<int>& legal, const STATUS& status) const
{

    const CELLAR_STATE& cellarstate =
        safe_cast<const CELLAR_STATE&>(state);

	 COORD pos = COORD(cellarstate.AgentPos.X, cellarstate.AgentPos.Y);
	 COORD posN = COORD(cellarstate.AgentPos.X, cellarstate.AgentPos.Y+1);
	 COORD posS = COORD(cellarstate.AgentPos.X, cellarstate.AgentPos.Y-1);
	 COORD posW = COORD(cellarstate.AgentPos.X-1, cellarstate.AgentPos.Y);
	 COORD posE = COORD(cellarstate.AgentPos.X+1, cellarstate.AgentPos.Y);		
	
	//Move is only possible when there are no obstacles
    if (cellarstate.AgentPos.Y + 1 < Size && FreeTile(cellarstate, posN))
        legal.push_back(COORD::E_NORTH);

	 if(FreeTile(cellarstate, posE))
		  legal.push_back(COORD::E_EAST);

    if (cellarstate.AgentPos.Y - 1 >= 0 && FreeTile(cellarstate, posS))
        legal.push_back(COORD::E_SOUTH);

    if (cellarstate.AgentPos.X - 1 >= 0 && FreeTile(cellarstate, posW))
        legal.push_back(COORD::E_WEST);

	 // If standing over a non sampled bottle, allow sample
    int bottle = Grid(cellarstate.AgentPos);
    if (bottle >= 0 && bottle < NumBottles && !cellarstate.Bottles[bottle].Collected){
        legal.push_back(E_SAMPLE);
	 }

	 // 'Check' possible for non sampled bottles
    for (bottle = 0; bottle < NumBottles; ++bottle)
        if (!cellarstate.Bottles[bottle].Collected)
            legal.push_back(bottle + 1 + E_SAMPLE);
	
	 /*TODO: Add to PGS action selection in LNAI paper:
				arg max_a \in A pgs(s,a)
		  
				where A are uncertainty reducing actions
		  */
	 // 'Check' possible for objects without an assumed type		  
	 for (int obj = 0; obj < NumObjects; ++obj){
		if(cellarstate.Objects[obj].AssumedType == E_NONE)
			legal.push_back(obj + E_OBJCHECK);
	 }
		  
	 //Additional actions for adjacent objects
	 if (cellarstate.AgentPos.Y + 1 < Size && !EmptyTile(cellarstate, posN))
        legal.push_back(E_PUSHNORTH);

	 if(cellarstate.AgentPos.X + 1 < Size && !EmptyTile(cellarstate, posE))
		  legal.push_back(E_PUSHEAST);

    if (cellarstate.AgentPos.Y - 1 >= 0 && !EmptyTile(cellarstate, posS))
        legal.push_back(E_PUSHSOUTH);

    if (cellarstate.AgentPos.X - 1 >= 0 && !EmptyTile(cellarstate, posW))
        legal.push_back(E_PUSHWEST);
}

void CELLAR::GenerateLegal(const STATE& state, const HISTORY& history,
    vector<int>& legal, const STATUS& status) const
{

    const CELLAR_STATE& cellarstate =
        safe_cast<const CELLAR_STATE&>(state);

	 COORD pos = COORD(cellarstate.AgentPos.X, cellarstate.AgentPos.Y);
	 COORD posN = COORD(cellarstate.AgentPos.X, cellarstate.AgentPos.Y+1);
	 COORD posS = COORD(cellarstate.AgentPos.X, cellarstate.AgentPos.Y-1);
	 COORD posW = COORD(cellarstate.AgentPos.X-1, cellarstate.AgentPos.Y);
	 COORD posE = COORD(cellarstate.AgentPos.X+1, cellarstate.AgentPos.Y);		
	
	//Move is only possible when there are no obstacles
    if (cellarstate.AgentPos.Y + 1 < Size && FreeTile(cellarstate, posN))
        legal.push_back(COORD::E_NORTH);

	 if(FreeTile(cellarstate, posE))
		  legal.push_back(COORD::E_EAST);

    if (cellarstate.AgentPos.Y - 1 >= 0 && FreeTile(cellarstate, posS))
        legal.push_back(COORD::E_SOUTH);

    if (cellarstate.AgentPos.X - 1 >= 0 && FreeTile(cellarstate, posW))
        legal.push_back(COORD::E_WEST);

	 // If standing over a non sampled bottle, allow sample
    int bottle = Grid(cellarstate.AgentPos);
    if (bottle >= 0 && bottle < NumBottles && !cellarstate.Bottles[bottle].Collected){
        legal.push_back(E_SAMPLE);
	 }

	 // 'Check' possible for non sampled bottles
    for (bottle = 0; bottle < NumBottles; ++bottle)
        if (!cellarstate.Bottles[bottle].Collected)
            legal.push_back(bottle + 1 + E_SAMPLE);
	
	 // 'Check' possible for all other objects, always
	 for (int obj = 0; obj < NumObjects; ++obj)
      legal.push_back(obj + E_OBJCHECK);
		  
	 //Additional actions for adjacent objects
	 if (cellarstate.AgentPos.Y + 1 < Size && !EmptyTile(cellarstate, posN))
        legal.push_back(E_PUSHNORTH);

	 if(cellarstate.AgentPos.X + 1 < Size && !EmptyTile(cellarstate, posE))
		  legal.push_back(E_PUSHEAST);

    if (cellarstate.AgentPos.Y - 1 >= 0 && !EmptyTile(cellarstate, posS))
        legal.push_back(E_PUSHSOUTH);

    if (cellarstate.AgentPos.X - 1 >= 0 && !EmptyTile(cellarstate, posW))
        legal.push_back(E_PUSHWEST);
}

/*
  Preferred actions RO policy from Rocksample.
*/
void CELLAR::GeneratePreferred(const STATE& state, const HISTORY& history,
    vector<int>& actions, const STATUS& status) const
{	
	if(Knowledge.RolloutLevel >= KNOWLEDGE::PGS)
	{
		//Added alternative rollout policy for PGS
		GeneratePGS(state, history, actions, status);
		//GeneratePGS_fake(state, history, actions, status);
		//Legal moves may also be used.
		//GenerateLegal(state, history, actions, status);
	}
	else
	{
		static const bool UseBlindPolicy = false;

		if (UseBlindPolicy)
		{
			actions.push_back(COORD::E_EAST);
			return;
		}

		const CELLAR_STATE& cellarstate =
				  safe_cast<const CELLAR_STATE&>(state);

		// Sample rocks with more +ve than -ve observations
		int rock = Grid(cellarstate.AgentPos);
		if (rock >= 0 && rock < NumBottles && !cellarstate.Bottles[rock].Collected)
		{
			int total = 0;
			for (int t = 0; t < history.Size(); ++t)
			{
				if (history[t].Action == rock + 1 + E_SAMPLE)
				{
					if (history[t].Observation == E_GOOD)
						total++;
					if (history[t].Observation == E_BAD)
						total--;
				}
			}
			if (total > 0)
			{
				actions.push_back(E_SAMPLE);
				return;
			}

		}

		// processes the bottles
		bool all_bad = true;
		bool north_interesting = false;
		bool south_interesting = false;
		bool west_interesting  = false;
		bool east_interesting  = false;

		for (int rock = 0; rock < NumBottles; ++rock)
		{
			const CELLAR_STATE::ENTRY& entry = cellarstate.Bottles[rock];
			if (!entry.Collected)
			{
				int total = 0;
				for (int t = 0; t < history.Size(); ++t)
				{
					if (history[t].Action == rock + 1 + E_SAMPLE)
					{
						if (history[t].Observation == E_GOOD)
							total++;
						if (history[t].Observation == E_BAD)
							total--;
					}
				}

				if (total >= 0)
				{
					all_bad = false;

					if (BottlePos[rock].Y > cellarstate.AgentPos.Y)
						north_interesting = true;
					if (BottlePos[rock].Y < cellarstate.AgentPos.Y)
						south_interesting = true;
					if (BottlePos[rock].X < cellarstate.AgentPos.X)
						west_interesting = true;
					if (BottlePos[rock].X > cellarstate.AgentPos.X)
						east_interesting = true;
				}
			}
		}

		// if all remaining rocks seem bad, then head east
		if (all_bad)
		{
			actions.push_back(COORD::E_EAST);
			return;
		}

		// generate a random legal move, with the exceptions that:
		//   a) there is no point measuring a rock that is already collected
		//   b) there is no point measuring a rock too often
		//   c) there is no point measuring a rock which is clearly bad or good
		//   d) we never sample a rock (since we need to be sure)
		//   e) we never move in a direction that doesn't take us closer to
		//      either the edge of the map or an interesting rock
		if (cellarstate.AgentPos.Y + 1 < Size && north_interesting)
				actions.push_back(COORD::E_NORTH);

		if (east_interesting)
			actions.push_back(COORD::E_EAST);

		if (cellarstate.AgentPos.Y - 1 >= 0 && south_interesting)
			actions.push_back(COORD::E_SOUTH);

		if (cellarstate.AgentPos.X - 1 >= 0 && west_interesting)
			actions.push_back(COORD::E_WEST);


		for (rock = 0; rock < NumBottles; ++rock)
		{
			if (!cellarstate.Bottles[rock].Collected    &&
				cellarstate.Bottles[rock].ProbValuable != 0.0 &&
				cellarstate.Bottles[rock].ProbValuable != 1.0 &&
				cellarstate.Bottles[rock].Measured < 5  &&
				std::abs(cellarstate.Bottles[rock].Count) < 2)
			{
				actions.push_back(rock + 1 + E_SAMPLE);
			}
		}
		
		//Use similar rules for object checks
		for (int obj = 0; obj < NumObjects; ++obj)
		{
			if (cellarstate.Objects[obj].ProbCrate != 0.0 &&
				cellarstate.Objects[obj].ProbCrate != 1.0 &&
				cellarstate.Objects[obj].Measured < 5  &&
				std::abs(cellarstate.Objects[obj].Count) < 2)
			{
				actions.push_back(obj + E_OBJCHECK);
			}
		}
	}
}

/////
bool CELLAR::CrateAt(const CELLAR_STATE& cellarstate, const COORD& coord) const{
	for(int i=0; i<cellarstate.Objects.size(); i++){
		if(cellarstate.Objects[i].ObjPos == coord && cellarstate.Objects[i].Type == E_CRATE)
			return true;
	}
	return false;
}

bool CELLAR::ShelfAt(const CELLAR_STATE& cellarstate, const COORD& coord) const{
	for(int i=0; i<cellarstate.Objects.size(); i++){
		if(cellarstate.Objects[i].ObjPos == coord && cellarstate.Objects[i].Type == E_SHELF)
			return true;
	}
	return false;	
}

//Empty if there is no crate, shelf or bottle.
bool CELLAR::EmptyTile(const CELLAR_STATE& cellarstate, const COORD& coord) const{
	bool empty = true;
	
	// If there is a bottle, not empty
	if(Grid.Inside(coord) && Grid(coord) >= 0 && Grid(coord) < NumBottles)
		empty = false;
	//If no bottles but objects, not empty
	if(empty)
		for(int i=0; i<cellarstate.Objects.size() && empty; i++){
			if(cellarstate.Objects[i].ObjPos == coord)
				empty = false;
		}
		
	return empty;
}

// Tiles are free if there are no obstacles (bottles are OK)
bool CELLAR::FreeTile(const CELLAR_STATE& cellarstate, const COORD& coord) const{
	return ObjectNumber(cellarstate, coord) < 0;
}

int CELLAR::ObjectNumber(const CELLAR_STATE& cellarstate, const COORD& coord) const{
	for(int i=0; i<cellarstate.Objects.size(); i++){
		if(cellarstate.Objects[i].ObjPos == coord)
			return i;
	}
	return -1;
}
/////

int CELLAR::GetObservation(const CELLAR_STATE& cellarstate, int pos, int type) const
{	 
    double distance;
    double efficiency;
	 int obs;
	
	 if(type == 1){
		distance = COORD::EuclideanDistance(cellarstate.AgentPos, BottlePos[pos]);
		efficiency = (1 + pow(2, -distance / HalfEfficiencyDistance)) * 0.5;
		 
		if (Bernoulli(efficiency))
        obs = cellarstate.Bottles[pos].Valuable ? E_GOOD : E_BAD;
      else
        obs = cellarstate.Bottles[pos].Valuable ? E_BAD : E_GOOD;
	 }
	 else{
		distance = COORD::EuclideanDistance(cellarstate.AgentPos, cellarstate.Objects[pos].ObjPos);
      efficiency = (1 + pow(2, -distance / HalfEfficiencyDistance)) * 0.5;
		 
		if (Bernoulli(efficiency))
        obs = cellarstate.Objects[pos].Type == E_CRATE ? E_CRATE : E_SHELF;
      else
        obs = cellarstate.Objects[pos].Type == E_CRATE ? E_SHELF : E_CRATE;
	 }
	
   return obs; 
}

int CELLAR::SelectTarget(const CELLAR_STATE& cellarstate) const
{
/*    int bestDist = Size * 2;
    int bestRock = -1;
    for (int rock = 0; rock < NumRocks; ++rock)
    {
        if (!cellarstate.Rocks[rock].Collected
            && cellarstate.Rocks[rock].Count >= UncertaintyCount)
        {
            int dist = COORD::ManhattanDistance(cellarstate.AgentPos, RockPos[rock]);
            if (dist < bestDist)
                bestDist = dist;
        }
    }
    return bestRock;*/
		return 0;
}

void CELLAR::DisplayBeliefs(const BELIEF_STATE& beliefState,
    std::ostream& ostr) const
{
	int numSamples = beliefState.GetNumSamples();
	
	ostr << numSamples << " belief samples: ";
	for (int i=0; i<numSamples; i++)
    {
		const STATE* s = beliefState.GetSample(i);
		cout << "Sample " << i+1 << ":" << endl;
		DisplayState(*s, cout);
    }
	ostr << endl;
}

void CELLAR::DisplayState(const STATE& state, std::ostream& ostr) const
{
    const CELLAR_STATE& cellarstate = safe_cast<const CELLAR_STATE&>(state);
    ostr << endl;
    for (int x = 0; x < Size + 2; x++)
        ostr << "# ";
    ostr << endl;
	
	 int thing = 0;
	 int obj = 0;
		
    for (int y = Size - 1; y >= 0; y--)
    {
        ostr << "# ";
        for (int x = 0; x < Size; x++)
        {
            COORD pos(x, y);
            thing = Grid(pos);
			  
            if (cellarstate.AgentPos == COORD(x, y))
                ostr << "* ";
            else
					if (thing >= 0 && thing < NumBottles){ //Display non collected bottles
					 const CELLAR_STATE::ENTRY& entry = cellarstate.Bottles[thing];
                //if(!entry.Collected)
					 ostr << thing << (entry.Valuable ? "$" : "X");
					}
					else{
						int obj = ObjectNumber(cellarstate, pos);						
						if(obj >= 0){
							const CELLAR_STATE::OBJ_ENTRY& entry = cellarstate.Objects[obj];
							ostr << (entry.Type == E_SHELF ? "S " : "C ");
						}
					   else
						  	ostr << ". ";
					}
        }
        ostr << "#" << endl;
    }
    for (int x = 0; x < Size + 2; x++)
        ostr << "# ";
    ostr << endl;
}

void CELLAR::DisplayObservation(const STATE& state, int observation, std::ostream& ostr) const
{
    switch (observation)
    {
    case E_NONE:
        break;
    case E_GOOD:
        ostr << "Observed good" << endl;
        break;
    case E_BAD:
        ostr << "Observed bad" << endl;
        break;
	 case E_CRATE:
		  ostr << "Observed a crate" << endl;
	     break;
	 case E_SHELF:
		  ostr << "Observed a shelf" << endl;
	     break;
    }
}

void CELLAR::DisplayAction(int action, std::ostream& ostr) const
{
	 ostr << "A: ";
    if (action < E_PUSHNORTH)
        ostr << "Move " << COORD::CompassString[action] << endl;
	 if (action == E_PUSHNORTH)
        ostr << "Push N" << endl;
	 if (action == E_PUSHSOUTH)
        ostr << "Push S" << endl;
	 if (action == E_PUSHEAST)
        ostr << "Push E" << endl;
	 if (action == E_PUSHWEST)
        ostr << "Push W" << endl;
    if (action == E_SAMPLE)
        ostr << "Sample" << endl;
    if (action > E_SAMPLE && action < E_OBJCHECK)
        ostr << "Check bottle " << action - E_SAMPLE << endl;
	 if (action >= E_OBJCHECK)
        ostr << "Check object " << action - E_OBJCHECK << endl;
}
