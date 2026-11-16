#ifndef TORCHLINE_H
#define TORCHLINE_H


typedef struct TlNetwork TlNetwork;

const TlNetwork *tl_load_network(void);

int  tl_input_size(const TlNetwork *net);
int  tl_output_size(const TlNetwork *net);
void tl_forward(const TlNetwork *net, const float *inputs, float *outputs);


#endif // TORCHLINE_H
