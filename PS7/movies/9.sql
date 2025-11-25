SELECT DISTINCT(name) FROM people JOIN stars, movies ON people.id = stars.person_id AND stars.movie_id = movies.id WHERE movies.year = 2004 ORDER BY people.birth;
