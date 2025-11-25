SELECT DISTINCT(name) FROM people JOIN directors, movies, ratings ON people.id = directors.person_id AND directors.movie_id = movies.id AND movies.id = ratings.movie_id WHERE ratings.rating >= 9;
