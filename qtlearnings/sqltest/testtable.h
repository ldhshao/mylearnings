#ifndef TESTTABLE_H
#define TESTTABLE_H

#define CORE_STATUS_SIZE 16
#define ERROR_NO_SQLITE  -1
#define ERROR_OPEN_DB    -2
#define ERROR_CREATE_TABLE -3

class CoreTest{
public:
    CoreTest(){
        uiBeginTime = 0; uiEndTime = 0;
        bResult = 0; bErrCode = 0;
    }
    unsigned uiBeginTime;
    unsigned uiEndTime;
    unsigned char bResult;
    unsigned char bErrCode;
    unsigned char bCoreStatus[CORE_STATUS_SIZE];
};

int createTestTable();
bool initTestTable();
int GetData(CoreTest* pTest, unsigned uiTestCount);
#endif // TESTTABLE_H
