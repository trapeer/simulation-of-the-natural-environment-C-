#include<stdio.h>
#include<stdlib.h>
#include<cstdlib>
#include<cstdio>
#include<time.h>
#include<math.h>
#define food_appearing_rate 0.3
#define map_size 100


bool get_animals(class animalss* animals);
void simulate(class animalss* animals);
bool save_animals(class animalss* animals);

class rabbit
{
	public:
	unsigned int position_x;
	unsigned int position_y; //position on map at that moment
	int age; //age in turns, age = -1 means death
	int sex; //0-female, 1-male
	int food_capacity; //maximum food that can be magazine in animal body
	int sight_range; //range in squares in which animal can recognize objects
	int speed; //number of squares that animal can travel in one turn
	int agility; //number used to count which animal win fight or escape
	float reproduction_probability; //probability for successful reproduction;
	int pregnant; // number of turns needed to get birth -1 - not pregnant
	int owned_food; //food that animal have in it body at that moment
	int movements; //number of squares that animal can move yet
	
	rabbit(unsigned int position_x, unsigned int position_y, int age, int sex, int food_capacity, int sight_range, int speed, int agility,  float reproduction_probability, int pregnant, int owned_food)
	{
		this->position_x = position_x;
		this->position_y = position_y;
		this->age = age;
		this->sex = sex;
		this->food_capacity = food_capacity;
		this->sight_range = sight_range;
		this->speed = speed;
		this->movements = speed;
		this->agility = agility;
		this->reproduction_probability = reproduction_probability;
		this->pregnant = pregnant;
		this->owned_food = owned_food;
	}
	
	rabbit(void)
	{
		this->position_x = 0;
		this->position_y = 0;
		this->age = -1;
		this->sex = 0;
		this->food_capacity = 0;
		this->sight_range = 0;
		this->speed = 0;
		this->movements = 0;
		this->agility = 0;
		this->reproduction_probability = 0;
		this->pregnant = -1;
		this->owned_food = 0;
	}
	
	void eat(bool map[map_size][map_size])
	{
		if(map[this->position_x][this->position_y] == 1)
		{
			if(this->owned_food < this->food_capacity)
			{
				map[this->position_x][this->position_y] = 0;
				this->owned_food++;
			}
		}
	}
	
	bool survive() //return = 0 - death 1 - survive
	{
		if(this->pregnant >= 0) this->pregnant--;
		if(this->owned_food == 0 && rand()%2 == 0) return 0;
		if(this->owned_food > 0)this->owned_food--;
		this->age++;
		if(this->age > 20 && rand()%5 == 0) return 0;
		return 1;
	}
	
};


class animalss
{
	public:
	rabbit *rabbits; //array where object rabbit will be store
	unsigned int rabbits_counter; // number of objects rabbit in rabbits
	unsigned int movements_max; //the largest number of moves for an animal
	animalss(void)
	{
		this->rabbits = NULL;
		this->rabbits_counter = 0;
		this->movements_max = 0;
	}
	
	void rabbits_new(unsigned int position_x, unsigned int position_y, int age, int sex, int food_capacity, int sight_range, int speed, int agility,  float reproduction_probability, int pregnant, int owned_food)
	{
		int random;
		this->rabbits_counter++;
		this->rabbits = (rabbit*) realloc(this->rabbits,sizeof(rabbit)*this->rabbits_counter);
		if(this->rabbits == NULL) printf("error when try to allocate memory");
		if(position_x > map_size - 1) position_x = map_size - 1; //needed when we make smaller map 
		if(position_y > map_size - 1) position_y = map_size - 1;
		if(food_capacity < 0) food_capacity = 0;
		if(sight_range < 0) sight_range = 0;
		if(speed < 0) speed = 0;
		if(agility < 0) agility = 0;
		if(reproduction_probability < 0) reproduction_probability = 0.0;
		if(reproduction_probability > 1) reproduction_probability = 1.0;
		while(food_capacity + sight_range + speed + agility > 15) //ograniczenie maksymalnych statystyk
		{
			random = rand()%4;
			if(random == 0 && food_capacity > 0) food_capacity--;
			if(random == 1 && sight_range > 0) sight_range--;
			if(random == 2 && speed > 0) speed--;
			if(random == 3 && agility > 0) agility--;
		}
		this->rabbits[this->rabbits_counter - 1] =  rabbit(position_x, position_y, age, sex, food_capacity, sight_range, speed, agility,  reproduction_probability, pregnant, owned_food);
		if(speed > this->movements_max) this->movements_max = speed;
	}
	
	void rabbits_delete(unsigned int position)
	{
		if (position >= this->rabbits_counter) printf("error when try to delete object rabbit");
		for(int i = position; i + 1 < this->rabbits_counter; i++)
		{
			this->rabbits[i] = this->rabbits[i+1];
		}
		this->rabbits_counter--;
		this->rabbits = (rabbit*) realloc(this->rabbits,sizeof(rabbit)*this->rabbits_counter);
		if(this->rabbits == NULL) printf("error when try to allocate memory");
	}
};

int main(void)
{
	srand(time(NULL));
	animalss animals = animalss();
	if(get_animals(&animals) == 0) return 0;
	simulation:
	simulate(&animals);
	if(save_animals(&animals) == 0) return 0;
	goto simulation;
}

bool get_animals(class animalss* animals)
{
	char animal_name[20];
	int position_x;
	int position_y; //position on map at that moment
	int age; //age in turns, age = -1 means death
	int sex; //0-female, 1-male
	int food_capacity; //maximum food that can be magazine in animal body
	int sight_range; //range in squares in which animal can recognize objects
	int speed; //number of squares that animal can travel in one turn
	int agility; //number used to count which animal win fight or escape
	float reproduction_probability; //probability for successful reproduction;
	int pregnant; // number of turns needed to get birth -1 - not pregnant
	int owned_food; //food that animal have in it body at that moment
    FILE *f= fopen("animals.txt", "r");
    if(f == NULL)
    {
        printf("animals.txt not found");
        return 0;
    }
    fscanf(f, "%s%s%s%s%s%s%s%s%s%s%s%s",animal_name,animal_name,animal_name,animal_name,animal_name,animal_name,animal_name,animal_name,animal_name,animal_name,animal_name,animal_name);
    for(int i = 0;;i++)
    {
		fscanf(f, "%s\n",&animal_name);
        if(animal_name[0] == 'r' && animal_name[1] == 'a' && animal_name[2] == 'b' && animal_name[3] == 'b' && animal_name[4] == 'i' && animal_name[5] == 't')
		{
			fscanf(f, "%d %d %d %d %d %d %d %d %f %d %d", &position_x, &position_y, &age, &sex, &food_capacity, &sight_range, &speed, &agility,  &reproduction_probability, &pregnant, &owned_food);
			animals->rabbits_new(position_x, position_y, age, sex, food_capacity, sight_range, speed, agility, reproduction_probability, pregnant, owned_food);
		}
        else
        {
        	printf("error when try to get animals name from animals.txt");
        	return 0;
		}
        if(feof(f) != 0) break;
    }
    fclose(f);
	return 1;
}

bool save_animals(class animalss* animals)
{
	FILE *f= fopen("animals_temp.txt", "w");
    if(f == NULL)
    {
        printf("animals.txt not work");
        return 0;
    }
    fprintf(f, "animal_name pos_x pos_y age sex food_capacity sight_range speed agility reproduction_pro pregnant food_owned");
    for(int i = 0; i < animals->rabbits_counter;i++)
    {
    	fprintf(f, "\nrabbit     %5d %5d %3d %3d %13d %11d %5d %7d %16.2f %8d %10d",animals->rabbits[i].position_x, animals->rabbits[i].position_y, animals->rabbits[i].age, animals->rabbits[i].sex, animals->rabbits[i].food_capacity, animals->rabbits[i].sight_range, animals->rabbits[i].speed, animals->rabbits[i].agility, animals->rabbits[i].reproduction_probability, animals->rabbits[i].pregnant, animals->rabbits[i].owned_food);
	}
	fclose(f);
	remove("animals.txt");
	rename("animals_temp.txt","animals.txt");
	return 1;
}

void simulate(class animalss* animals)
{
	bool map[map_size][map_size]; // 0 - no food, 1 - there is a food
	unsigned int number_of_rounds;
	unsigned int number_of_rabbits_start = animals->rabbits_counter; //how many rabbits where on the start of round
	unsigned int random;
	unsigned int nearest_food = 1000;
	unsigned int nearest_animal_for_breeding = 1000;
	////////variables used to reproduct/////////////////////////////////////////
	int sex; //0-female, 1-male
	int food_capacity; //maximum food that can be magazine in animal body
	int sight_range; //range in squares in which animal can recognize objects
	int speed; //number of squares that animal can travel in one turn
	int agility; //number used to count which animal win fight or escape
	float reproduction_probability; //probability for successful reproduction;
	printf("how many rounds do you want to simulate?\n");
	scanf("%d",&number_of_rounds);
	for(unsigned int i = 0; i < number_of_rounds; i++)
	{
		for(unsigned int l = 0; l < map_size; l++)
		{
			for(unsigned int m = 0; m < map_size; m++)
			{
				if(rand()%101 <= food_appearing_rate*100) map[l][m] = 1;
				else map[l][m] = 0;
			}
		}
		for(unsigned int k = 0; k < animals->rabbits_counter; k++)
		{
			animals->rabbits[k].movements = animals->rabbits[k].speed;
		}
		for(unsigned int n = 0; n < animals->movements_max; n++)
		{
			for(unsigned int k = 0; k < animals->rabbits_counter; k++)
			{
				nearest_food = 1000;
				nearest_animal_for_breeding = 1000;
	////////////////animals moves///////////////////////////////////////////////////////////////////
				if(animals->rabbits[k].movements != 0 )
				{
					animals->rabbits[k].movements--;
					if(animals->rabbits[k].owned_food >= 2 && animals->rabbits[k].pregnant == -1)
					{
						for(unsigned int l = 0; l < animals->rabbits_counter; l++) //looking for animal  to breed
						{
							if(abs(animals->rabbits[l].position_x - animals->rabbits[k].position_x) <= animals->rabbits[k].sight_range && abs(animals->rabbits[l].position_y - animals->rabbits[k].position_y) <= animals->rabbits[k].sight_range && animals->rabbits[l].pregnant == -1 && animals->rabbits[l].owned_food >= 2 && animals->rabbits[k].sex != animals->rabbits[l].sex)
							{
								if(abs(animals->rabbits[l].position_x - animals->rabbits[k].position_x) + abs(animals->rabbits[l].position_y - animals->rabbits[k].position_y) < nearest_animal_for_breeding) nearest_animal_for_breeding = abs(animals->rabbits[l].position_x - animals->rabbits[k].position_x) + abs(animals->rabbits[l].position_y - animals->rabbits[k].position_y);
							}
						}
						if(nearest_animal_for_breeding != 1000)
						{
							for(unsigned int l = 0; l < animals->rabbits_counter; l++)
							{
								if(l == k) continue;
								if(abs(animals->rabbits[l].position_x - animals->rabbits[k].position_x) <= animals->rabbits[k].sight_range && abs(animals->rabbits[l].position_y - animals->rabbits[k].position_y) <= animals->rabbits[k].sight_range && animals->rabbits[l].pregnant == -1 && animals->rabbits[l].owned_food >= 2 && animals->rabbits[k].sex != animals->rabbits[l].sex)
								{
									if(abs(animals->rabbits[l].position_x - animals->rabbits[k].position_x) + abs(animals->rabbits[l].position_y - animals->rabbits[k].position_y) == nearest_animal_for_breeding)
									{
										if(animals->rabbits[k].position_x > animals->rabbits[l].position_x)
										{
											animals->rabbits[k].position_x--;
											goto rabbits_eat;
										}
										else if(animals->rabbits[k].position_x < animals->rabbits[l].position_x)
										{
											animals->rabbits[k].position_x++;
											goto rabbits_eat;
										}
										else if(animals->rabbits[k].position_y > animals->rabbits[l].position_y)
										{
											animals->rabbits[k].position_y--;
											goto rabbits_eat;
										}
										else if(animals->rabbits[k].position_y < animals->rabbits[l].position_y)
										{
											animals->rabbits[k].position_y++;
											goto rabbits_eat;
										}
										goto rabbits_eat;
									}
								}
							}
						}
					}
					if(animals->rabbits[k].owned_food != animals->rabbits[k].food_capacity) //looking for food
					{
						for(int l = animals->rabbits[k].sight_range * -1; l <= animals->rabbits[k].sight_range; l++)
						{
							for(int m = animals->rabbits[k].sight_range * -1; m <= animals->rabbits[k].sight_range; m++)
							{
								if(animals->rabbits[k].position_x + m < map_size && animals->rabbits[k].position_x + m >= 0 && animals->rabbits[k].position_y + l < map_size && animals->rabbits[k].position_y + l >= 0)
								{
									if(map[animals->rabbits[k].position_x + m][animals->rabbits[k].position_y + l] == 1 && nearest_food > abs(m) + abs(l)) nearest_food = abs(m) + abs(l);	
								}
							}
						}
						if(nearest_food != 1000)
						{
							for(int l = animals->rabbits[k].sight_range * -1; l <= animals->rabbits[k].sight_range; l++)
							{
								for(int m = animals->rabbits[k].sight_range * -1; m <= animals->rabbits[k].sight_range; m++)
								{
									if(animals->rabbits[k].position_x + m < map_size && animals->rabbits[k].position_x + m >= 0 && animals->rabbits[k].position_y + l < map_size && animals->rabbits[k].position_y + l >= 0)
									{
										if(map[animals->rabbits[k].position_x + m][animals->rabbits[k].position_y + l] == 1 && nearest_food == abs(m) + abs(l))
										{
											if(m > 0)
											{
												animals->rabbits[k].position_x++;
												goto rabbits_eat;
											}
											else if(m < 0)
											{
												animals->rabbits[k].position_x--;
												goto rabbits_eat;
											}
											else if(l > 0)
											{
												animals->rabbits[k].position_y++;
												goto rabbits_eat;
											}
											else if(l < 0)
											{
												animals->rabbits[k].position_y--;
												goto rabbits_eat;
											}
											else goto rabbits_eat;
										}
									}
								}
							}
						}
					}
					random = rand()%4; //when we didnt found other rabbit nor food we are moving in random way
					if(random == 0)
					{
						if(animals->rabbits[k].position_x + 1 < map_size) animals->rabbits[k].position_x++;
						else animals->rabbits[k].position_x--;
					}
					if(random == 1) 
					{
						if(animals->rabbits[k].position_x >= 1) animals->rabbits[k].position_x--;
						else animals->rabbits[k].position_x++;
					}
					if(random == 2) 
					{
						if(animals->rabbits[k].position_y + 1 < map_size) animals->rabbits[k].position_y++;
						else animals->rabbits[k].position_y--;
					}
					if(random == 3)
					{
						if(animals->rabbits[k].position_y >= 1) animals->rabbits[k].position_y--;
						else animals->rabbits[k].position_y++;
					}
				}
				rabbits_eat:
				animals->rabbits[k].eat(map);
	////////////////animals reproduction////////////////////////////////////////////////////////////			
				if(animals->rabbits[k].sex == 0 && animals->rabbits[k].owned_food >= 2 && animals->rabbits[k].pregnant == -1)
				{
					for(int z = 0; z < animals->rabbits_counter; z++)
					{
						if(animals->rabbits[z].sex == 1 && animals->rabbits[z].owned_food >=2 && animals->rabbits[z].position_x == animals->rabbits[k].position_x && animals->rabbits[z].position_y == animals->rabbits[k].position_y)
						{
							if(rand()%100 <= (animals->rabbits[k].reproduction_probability + animals->rabbits[z].reproduction_probability)*50)
							{
								animals->rabbits[k].pregnant = 1;
								animals->rabbits[k].owned_food--;
								if(rand()%2 == 0) sex = 0;
								else sex = 1;
								if(rand()%2 == 0) food_capacity = animals->rabbits[k].food_capacity;
								else food_capacity = animals->rabbits[z].food_capacity;
								if(rand()%10 == 0) food_capacity++;
								else if(rand()%10 == 0) food_capacity--;
								if(rand()%2 == 0) sight_range = animals->rabbits[k].sight_range;
								else sight_range = animals->rabbits[z].sight_range;
								if(rand()%10 == 0) sight_range++;
								else if(rand()%10 == 0) sight_range--;
								if(rand()%2 == 0) speed = animals->rabbits[k].speed;
								else speed = animals->rabbits[z].speed;
								if(rand()%10 == 0) speed++;
								else if(rand()%10 == 0) speed--;
								if(rand()%2 == 0) agility = animals->rabbits[k].agility;
								else agility = animals->rabbits[z].agility;
								if(rand()%10 == 0) agility++;
								else if(rand()%10 == 0) agility--;
								if(rand()%2 == 0) reproduction_probability = animals->rabbits[k].reproduction_probability;
								else reproduction_probability = animals->rabbits[z].reproduction_probability;
								if(rand()%10 == 0) reproduction_probability+=0.02;
								else if(rand()%10 == 0) reproduction_probability-=0.02;
								animals->rabbits_new(animals->rabbits[k].position_x, animals->rabbits[k].position_y, 0, sex, food_capacity, sight_range, speed, agility, reproduction_probability, -1, 1);
								break;
							}
						}
					}
				}
				else if(animals->rabbits[k].sex == 1 && animals->rabbits[k].owned_food >= 2)
				{
					for(int z = 0; z < animals->rabbits_counter; z++)
					{
						if(animals->rabbits[z].sex == 0 && animals->rabbits[z].owned_food >= 2 && animals->rabbits[z].pregnant == -1 && animals->rabbits[z].position_x == animals->rabbits[k].position_x && animals->rabbits[z].position_y == animals->rabbits[k].position_y)
						{
							if(rand()%100 <= (animals->rabbits[k].reproduction_probability + animals->rabbits[z].reproduction_probability)*50)
							{
								animals->rabbits[z].pregnant = 1;
								animals->rabbits[z].owned_food--;
								if(rand()%2 == 0) sex = 0;
								else sex = 1;
								if(rand()%2 == 0) food_capacity = animals->rabbits[k].food_capacity;
								else food_capacity = animals->rabbits[z].food_capacity;
								if(rand()%10 == 0) food_capacity++;
								else if(rand()%10 == 0) food_capacity--;
								if(rand()%2 == 0) sight_range = animals->rabbits[k].sight_range;
								else sight_range = animals->rabbits[z].sight_range;
								if(rand()%10 == 0) sight_range++;
								else if(rand()%10 == 0) sight_range--;
								if(rand()%2 == 0) speed = animals->rabbits[k].speed;
								else speed = animals->rabbits[z].speed;
								if(rand()%10 == 0) speed++;
								else if(rand()%10 == 0) speed--;
								if(rand()%2 == 0) agility = animals->rabbits[k].agility;
								else agility = animals->rabbits[z].agility;
								if(rand()%10 == 0) agility++;
								else if(rand()%10 == 0) agility--;
								if(rand()%2 == 0) reproduction_probability = animals->rabbits[k].reproduction_probability;
								else reproduction_probability = animals->rabbits[z].reproduction_probability;
								if(rand()%10 == 0) reproduction_probability+=0.02;
								else if(rand()%10 == 0) reproduction_probability-=0.02;
								animals->rabbits_new(animals->rabbits[k].position_x, animals->rabbits[k].position_y, 0, sex, food_capacity, sight_range, speed, agility, reproduction_probability, -1, 1);
								break;
							}
						}
					}
				}	
			}
		}
////////////////animals survive////////////////////////////////////////////////////////////
		for(unsigned int k = 0; k < animals->rabbits_counter; k++)
		{
			if(animals->rabbits[k].survive() == 0)
			{
				animals->rabbits_delete(k);
				k--; //needed becouse function rabbits_delete() is changing order of rabbits in array
			}
		}
		printf("round %d:  start: rabbits = %d  finish: rabbits = %d\n",i + 1,number_of_rabbits_start, animals->rabbits_counter);
		number_of_rabbits_start = animals->rabbits_counter;
	}
}
