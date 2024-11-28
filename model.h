#ifndef MODEL_H
#define MODEL_H

using namespace System::Data::SqlClient;

int testModel();
SqlConnection^ initSqlConnect();
int getResultQuery(long long*);
int inputData(void *data);
#endif
