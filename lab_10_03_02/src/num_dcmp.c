#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "num_dcmp.h"

#define PRIMES_AMOUNT 500U
#define FIRST_PRIME 2U
static unsigned int primes[PRIMES_AMOUNT];

static bool imp__is_prime(unsigned int number)
{
    bool is_prime = true;

    for (unsigned int i = 0U; primes[i] <= number / 2; i++)
    {
        is_prime = number % primes[i] != 0;
        if (!is_prime)
            break;
    }

    return is_prime;
}

static void imp__recalc_primes()
{
    if (primes[0] == 0U)
    {
        primes[0] = FIRST_PRIME;
        for (unsigned short i = 1U; i < PRIMES_AMOUNT; i++)
        {
            primes[i] = primes[i - 1U];
            while (!imp__is_prime(++primes[i]));
        }
    }
}

static struct imp__power_node *create_node(unsigned int power)
{
    struct imp__power_node *node = calloc(1, sizeof(struct imp__power_node));
    if (node != NULL)
        node->power = power;
    return node;
}

static struct imp__power_node *push_back(struct num_dcmp *nd, unsigned int power, struct imp__power_node *tail)
{
    if (tail == NULL)
    {
        nd->head = create_node(power);
        tail = nd->head;
    }
    else
    {
        tail->next = create_node(power);
        tail = tail->next;
    }

    return tail;
}

struct num_dcmp nd_decompose(unsigned long number)
{
    struct num_dcmp nd;
    if (number <= 1U)
        nd.head = NULL;
    else
    {
        imp__recalc_primes();
        struct imp__power_node *tail = NULL;
        
        for (unsigned short i = 0U; i < PRIMES_AMOUNT && number > 1U; i++)
        {
            unsigned int power = 0U;
            while (number % primes[i] == 0U)
            {
                number /= primes[i];
                power++;
            }

            tail = push_back(&nd, power, tail);
        }
    }
    return nd;
}

void nd_destroy(struct num_dcmp *nd)
{
    for (struct imp__power_node *prev = nd->head, *node = NULL; prev; free(prev), prev = node)
        node = prev->next;
}

static void imp__compress(struct imp__power_node **node)
{
    if (*node != NULL)
    {
        imp__compress(&(*node)->next);
        if ((*node)->power == 0U && (*node)->next == NULL)
        {
            free(*node);
            *node = NULL;
        }
    }
}

void nd_compress(struct num_dcmp *nd)
{
    imp__compress(&nd->head);
}

struct num_dcmp nd_multiply(const struct num_dcmp *nd_1, const struct num_dcmp *nd_2)
{
    struct num_dcmp res = { .head = NULL };

    struct imp__power_node *node_1 = nd_1->head;
    struct imp__power_node *node_2 = nd_2->head;

    struct imp__power_node *tail = NULL;

    while (node_1 != NULL && node_2 != NULL)
    {
        unsigned int power = node_1->power + node_2->power;
        tail = push_back(&res, power, tail);

        node_1 = node_1->next;
        node_2 = node_2->next;
    }

    while (node_1 != NULL)
    {
        tail = push_back(&res, node_1->power, tail);
        node_1 = node_1->next;
    }

    while (node_2 != NULL)
    {
        tail = push_back(&res, node_2->power, tail);
        node_2 = node_2->next;
    }

    return res;
}

struct num_dcmp nd_divide(const struct num_dcmp *nd_1, const struct num_dcmp *nd_2)
{
    struct num_dcmp res = { .head = NULL };

    struct imp__power_node *node_1 = nd_1->head;
    struct imp__power_node *node_2 = nd_2->head;

    struct imp__power_node *tail = NULL;
    
    bool error = false;

    while (!error && node_1 != NULL && node_2 != NULL)
    {
        if (node_1->power < node_2->power)
            error = true;
        else
        {
            unsigned int power = node_1->power - node_2->power;
            tail = push_back(&res, power, tail);

            node_1 = node_1->next;
            node_2 = node_2->next;
        }
    }

    while (!error && node_1 != NULL)
    {
        tail = push_back(&res, node_1->power, tail);
        node_1 = node_1->next;
    }

    if (!error && node_2 != NULL)
        error = true;

    if (error)
    {
        nd_destroy(&res);
        res.head = NULL;
    }

    nd_compress(&res);
    return res;
}

struct num_dcmp nd_square(const struct num_dcmp *nd)
{
    struct num_dcmp res = { .head = NULL };

    for (struct imp__power_node *node = nd->head, *tail = NULL; node; node = node->next)
        tail = push_back(&res, node->power * 2U, tail);

    return res;
}

int nd_output(const struct num_dcmp *nd)
{
    int status = EXIT_FAILURE;

    if (nd->head != NULL)
    {
        for (const struct imp__power_node *node = nd->head; node; node = node->next)
            printf("%u ", node->power);
        printf("L\n");
        status = EXIT_SUCCESS;
    }

    return status;
}
