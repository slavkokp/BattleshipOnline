#ifndef PCH
#define PCH
#define CONNECTION_STRING "dbname = BattleshipBase user = postgres password = 555 hostaddr = 127.0.0.1 port = 5432"
//sfml
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

//postgres inteface
#include <pqxx/pqxx>

#include <iostream>
#include <sstream>
#include <fstream>

#include <cctype>
#include <map>
#include <unordered_map>
#include <stack>
#include <queue>
#include <vector>
#include <string>
#include <regex>

#endif // !PCH