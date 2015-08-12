#ifndef HLT_H
#define HLT_H

#include <list>
#include <vector>
#include <random>

namespace hlt
{
	struct Location { unsigned short x, y;  };
	static bool operator<(const Location& l1, const Location& l2)
	{
		return ((l1.x + l1.y)*(unsigned int(l1.x) + l1.y + 1) / 2) + l1.y < ((l2.x + l2.y)*(unsigned int(l2.x) + l2.y + 1) / 2) + l2.y;
	}

	struct Site { unsigned char owner, age; };
	class Map
	{
	public:
		std::vector< std::vector<Site> > contents;
		unsigned short map_width, map_height; //Number of rows & columns, NOT maximum index.

		Map()
		{
			map_width = 0;
			map_height = 0;
			contents = std::vector< std::vector<Site> >(map_height, std::vector<Site>(map_width, { 0, 0 }));
		}
		Map(short width, short height, unsigned char numberOfPlayers)
		{
			contents = std::vector< std::vector<Site> >(map_height, std::vector<Site>(map_width, { 0, 0 }));
			map_width = width;
			map_height = height;

			std::list<Location> takenSpots;
			float minDistance = sqrt(map_height*map_width) / 2;
			for(int a = 1; a <= numberOfPlayers; a++)
			{
				bool bad = true;
				int counter = 0;
				Location l;
				while(bad)
				{
					bad = false;
					l = { rand() % map_width, rand() % map_height };
					for(auto b = takenSpots.begin(); b != takenSpots.end(); b++)
					{
						if(getDistance(l, *b) <= minDistance)
						{
							bad = true;
							break;
						}
					}
					counter++;
					if(counter > 150)
					{
						counter = 0;
						minDistance *= 0.85;
					}
				}
				contents[l.y][l.x] = { a, 0 };
				takenSpots.push_back(l);
			}
		}

		bool inBounds(Location l)
		{
			return l.x < map_width && l.y < map_height;
		}
		float getDistance(Location l1, Location l2)
		{
			short dx = abs(l1.x - l2.x), dy = abs(l1.y - l2.y);
			if(dx > map_width / 2) dx = map_width - dx;
			if(dy > map_height / 2) dy = map_height - dy;
			return sqrt((dx*dx) + (dy*dy));
		}
		float getAngle(Location l1, Location l2)
		{
			short dx = l2.x - l1.x, dy = l2.y - l1.y;
			if(dx > map_width - dx) dx -= map_width;
			else if(-dx > map_width + dx) dx += map_width;
			if(dy > map_height - dy) dy -= map_height;
			else if(-dy > map_height + dy) dy += map_height;
			return atan2(dy, dx);
		}
		Site& getSite(Location l)
		{
			if(l.y < 0) l.y += map_height;
			if(l.y > map_height - 1) l.y -= map_height;

			if(l.x < 0) l.x += map_width;
			if(l.x > map_width - 1) l.x -= map_width;

			return contents[l.y][l.x];
		}
		Site& getNorthernSite(Location l)
		{
			if(l.y != 0) l.y--;
			else l.y = map_height - 1;
			return contents[l.y][l.x];
		}
		Site& getEasternSite(Location l)
		{
			if(l.x != map_width - 1) l.x++;
			else l.x = 0;
			return contents[l.y][l.x];
		}
		Site& getSouthernSite(Location l)
		{
			if(l.y != map_height - 1) l.y++;
			else l.y = 0;
			return contents[l.y][l.x];
		}
		Site& getWesternSite(Location l)
		{
			if(l.x != 0) l.x--;
			else l.x = map_width - 1;
			return contents[l.y][l.x];
		}
		Location getNorthern(Location l)
		{
			if(l.y != 0) l.y--;
			else l.y = map_height - 1;
			return l;
		}
		Location getEastern(Location l)
		{
			if(l.x != map_width - 1) l.x++;
			else l.x = 0;
			return l;
		}
		Location getSouthern(Location l)
		{
			if(l.y != map_height - 1) l.y++;
			else l.y = 0;
			return l;
		}
		Location getWestern(Location l)
		{
			if(l.x != 0) l.x--;
			else l.x = map_width - 1;
			return l;
		}
	};

	struct Move { Location l; unsigned char d; };
	static bool operator<(const Move& m1, const Move& m2)
	{
		unsigned int l1Prod = ((m1.l.x + m1.l.y)*(unsigned int(m1.l.x) + m1.l.y + 1) / 2) + m1.l.y, l2Prod = ((m2.l.x + m2.l.y)*(unsigned int(m2.l.x) + m2.l.y + 1) / 2) + m2.l.y;
		return ((l1Prod + m1.d)*(l1Prod + m1.d + 1) / 2) + m1.d < ((l2Prod + m2.d)*(l2Prod + m2.d + 1) / 2) + m2.d;
	}
}

#define STILL 0
#define NORTH 1
#define EAST 2
#define SOUTH 3
#define WEST 4

#endif