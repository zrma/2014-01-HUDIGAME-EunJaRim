#pragma once

const char* SQL_InsertTest = "INSERT INTO players VALUES(?, ?, ?, ?, ?, ?)" ;
const char* SQL_SelectTest = "SELECT name, win, lose, error FROM players WHERE pid=?" ;
const char* SQL_UpdateTest = "UPDATE players SET win=?, lose=?, error=?, comment=? WHERE pid=?" ;
const char* SQL_DeleteTest = "DELETE FROM players WHERE pid=?" ;

