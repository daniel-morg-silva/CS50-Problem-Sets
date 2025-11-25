-- Keep a log of any SQL queries you execute as you solve the mystery.

--log from the theft
SELECT description FROM crime_scene_reports WHERE street = 'Humphrey Street' AND month = 7 AND day = 28;
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|                                                                                                       description                                                                                                        |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery. |
| Littering took place at 16:36. No known witnesses.                                                                                                                                                                       |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

--transcripts of the interviews
sqlite> SELECT name, transcript FROM interviews WHERE month = 7 AND day = 28;
+---------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|  name   |                                                                                                                                                     transcript                                                                                                                                                      |
+---------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Ruth    | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
| Eugene  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
| Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |'
+---------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

--license plate of every car that left the bakery within ten minutes of the theft
SELECT hour, minute, license_plate, activity FROM bakery_security_logs WHERE day = 28 AND month = 7 AND hour =  10  AND minute >= 15 AND minute <= 25 AND activity =  'exit' ORDER BY minute;
+------+--------+---------------+----------+
| hour | minute | license_plate | activity |
+------+--------+---------------+----------+
| 10   | 16     | 5P2BI95       | exit     |
| 10   | 18     | 94KL13X       | exit     |
| 10   | 18     | 6P58WS2       | exit     |
| 10   | 19     | 4328GD8       | exit     |
| 10   | 20     | G412CB7       | exit     |
| 10   | 21     | L93JTIZ       | exit     |
| 10   | 23     | 322W7JE       | exit     |
| 10   | 23     | 0NTHK55       | exit     |
+------+--------+---------------+----------+

--account numbers of all withdrawals at the atm in Leggett Street
sqlite> SELECT account_number, amount FROM atm_transactions WHERE day = 28 AND month = 7 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw';
+----------------+--------+
| account_number | amount |
+----------------+--------+
| 28500762       | 48     |
| 28296815       | 20     |
| 76054385       | 60     |
| 49610011       | 50     |
| 16153065       | 80     |
| 25506511       | 20     |
| 81061156       | 30     |
| 26013199       | 35     |
+----------------+--------+

--people that withdrew money at the atm
SELECT name FROM people JOIN bank_accounts ON people.id = bank_accounts.person_id
   ...>  WHERE bank_accounts.account_number IN  (SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND atm_location = 'Leggett Street' AND transaction_typ
e = 'withdraw');
+---------+
|  name   |
+---------+
| Bruce   |
| Diana   |
| Brooke  |
| Kenny   |
| Iman    |
| Luca    |
| Taylor  |
| Benista |
+---------+

--flight id and destination_airport_id from the earliest flight on the 29th of july
SELECT id AS flight_id, origin_airport_id, destination_airport_id, hour, minute FROM flights  WHERE year = 2023 AND month = 7 AND day = 29;
+-----------+-------------------+------------------------+------+--------+
| flight_id | origin_airport_id | destination_airport_id | hour | minute |
+-----------+-------------------+------------------------+------+--------+
| 18        | 8                 | 6                      | 16   | 0      |
| 23        | 8                 | 11                     | 12   | 15     |
| 36        | 8                 | 4                      | 8    | 20     | -->thief took this flight
| 43        | 8                 | 1                      | 9    | 30     |
| 53        | 8                 | 9                      | 15   | 20     |
+-----------+-------------------+------------------------+------+--------+

--list of passengers of flight number 36
SELECT name FROM people
 WHERE passport_number IN
 (
    SELECT passport_number FROM passengers
    WHERE flight_id IN
    (
        SELECT id FROM flights
        WHERE month = 7 AND day = 29 AND id = 36
    )
 );
+--------+
|  name  |
+--------+
| Kenny  |
| Sofia  |
| Taylor |
| Luca   |
| Kelsey |
| Edward |
| Bruce  |
| Doris  |
+--------+

--people that withdrew money from the atm in Leggett Street and left the bakery's parking lot within 10 minutes of the theft and had a call with less than a minute
SELECT name FROM people
    JOIN bank_accounts ON people.id = bank_accounts.person_id
        WHERE bank_accounts.account_number IN (
            SELECT account_number FROM atm_transactions
                WHERE month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw')
        AND people.license_plate IN (
            SELECT license_plate FROM bakery_security_logs
                WHERE day = 28 AND month = 7 AND hour =  10  AND minute >= 15 AND minute <= 25 AND activity =  'exit' ORDER BY minute)
        AND people.phone_number in (
            SELECT caller FROM phone_calls
                WHERE month = 7 AND day = 28 AND duration < 60);
+-------+
| name  |
+-------+
| Bruce | -->thief
| Diana |
+-------+

--tentar seguir o cumplice pela chamada
SELECT name FROM people
WHERE phone_number IN (
   SELECT receiver FROM phone_calls WHERE month = 7 AND day = 28 AND duration < 60 AND caller = '(367) 555-5533');
+------------+
|    name    |
+------------+
| Robin      | --> cumplice
+------------+

--destination of the thief's flight
SELECT city, hour, minute FROM airports JOIN flights ON airports.id = destination_airport_id WHERE year = 2023 AND month = 7 AND day = 29 AND flights.id = 36;
+---------------+------+--------+
|     city      | hour | minute |
+---------------+------+--------+
| New York City | 8    | 20     | --> thief escaped to NYC
+---------------+------+--------+
