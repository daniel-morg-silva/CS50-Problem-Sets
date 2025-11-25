#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

int pair_count;
int candidate_count;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
bool help_lock(int loser, int winner);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int n = 0; n < candidate_count; n++)
        if (strcmp(name, candidates[n]) == 0)
        {
            ranks[rank] = n;
            return true;
        }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int m = 0; m < candidate_count; m++)
        for (int n = 0; n < candidate_count; n++)
            if (m < n)
                preferences[ranks[m]][ranks[n]]++;

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int m = 0; m < candidate_count; m++)
        for (int n = 0; n < candidate_count; n++)
            if (preferences[n][m] < preferences[m][n])
            {
                pairs[pair_count].winner = m;
                pairs[pair_count].loser = n;
                pair_count++;
            }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int m = 0; m < pair_count - 1; m++)
        for (int n = 0; n < pair_count; n++)
            if (preferences[pairs[m].winner][pairs[n].loser] - preferences[pairs[n].loser][pairs[m].winner] <
                preferences[pairs[m + 1].winner][pairs[n + 1].loser] - preferences[pairs[n + 1].loser][pairs[m + 1].winner])
            {
                pair temp_pair = pairs[m];
                pairs[m] = pairs[m + 1];
                pairs[m + 1] = temp_pair;
            }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int n = 0; n < pair_count; n++)
        if (!help_lock(pairs[n].loser, pairs[n].winner))
            locked[pairs[n].winner][pairs[n].loser] = true;

    return;
}

bool help_lock(int loser, int winner)
{
    for (int m = 0; m < pair_count; m++)
        if (locked[loser][winner] == 1)
            return true;
        else if (locked[loser][m] == 1)
            if (help_lock(m, winner))
                creturn true;
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    for (int n = 0, candidate = 0; n < candidate_count; n++)
        if (locked[n][candidate] == 1)
        {
            candidate = n;
            n = 0;
        }
        else if (n == candidate_count - 1 && locked[n][candidate] == 0)
        {
            printf("%s\n", candidates[candidate]);
            return;
        }
    return;
}
