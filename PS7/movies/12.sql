SELECT title FROM movies JOIN stars AS s1, stars AS s2, people AS p1, people AS p2 ON movies.id = s1.movie_id AND s1.person_id =
 p1.id AND movies.id = s2.movie_id AND s2.person_id = p2.id WHERE p1.name = 'Bradley Cooper' AND p2.name = 'Jennifer Lawrence';
