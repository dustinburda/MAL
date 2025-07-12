
#ifndef MAL_REPFUNCS_H
#define MAL_REPFUNCS_H

#include <string>

#include "../include/environment.h"
#include "../include/reader.h"


MalNode ReadStr(std::string src);
MalNode READ(std::string line);
MalNode Apply(Environment& env, std::vector<MalNode>& children);
MalNode EVAL(MalNode ast, Environment& env);
std::string PRINT(MalNode ast);
std::string rep(std::string line, Environment& env);
void InitEnvironment(Environment& env);

#endif // MAL_REPFUNCS_H