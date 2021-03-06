#ifndef PARSER_H
#define PARSER_H

#include "tree.h"

node* parser();
node* program();
node* block();
node* vars();
node* mvars();
node* expr();
node* M();
node* F();
node* R();
node* stats();
node* mStat();
node* stat();
node* in();
node* out();
node* if_stmt();
node* loop();
node* assign();
node* RO();

#endif
