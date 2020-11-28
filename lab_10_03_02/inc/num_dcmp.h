#ifndef __NUM_DCMP_H_
#define __NUM_DCMP_H_

struct imp__power_node
{
    unsigned int power;
    struct imp__power_node *next;
};

// number decomposition struct (linked-list-like)
struct num_dcmp
{
    struct imp__power_node *head;
};

struct num_dcmp nd_decompose(unsigned long number);
void nd_destroy(struct num_dcmp *nd);

void nd_compress(struct num_dcmp *nd);

struct num_dcmp nd_multiply(const struct num_dcmp *nd_1, const struct num_dcmp *nd_2);
struct num_dcmp nd_divide(const struct num_dcmp *nd_1, const struct num_dcmp *nd_2);

struct num_dcmp nd_square(const struct num_dcmp *nd);

int nd_output(const struct num_dcmp *nd);

#endif // __NUM_DCMP_H_
