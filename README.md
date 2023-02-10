# BattleshipOnline
Online Battleship game written on C++ using [SFML](https://www.sfml-dev.org/). Using basic multimedia library functions, I implemented a simple game engine and created a server to client networking system using TCP sockets. Also created authentication system and match history using [pqxx](https://github.com/jtv/libpqxx) library to control PostgreSQL.  

Initial setup<br /> 
1)Create database BattleshipBase in postgres (edit connection string in [pch.h](https://github.com/slavkokp/BattleshipOnline/blob/master/Battleship/Battleship/pch.h)). Run SQL code from [database_creation_script.txt](https://github.com/slavkokp/BattleshipOnline/blob/master/database_creation_script.txt) in the database. <br /> 
2)Download and install [libpqxx](https://github.com/jtv/libpqxx) for your postgres version and location. Connect it to the Battleship project.<br /> 
