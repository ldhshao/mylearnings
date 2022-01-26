#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <stdio.h>

using namespace std;

#define VALUE_MAX 10000
#define CHAN_MAX 20
#define COL_MAX  4000
struct sStatisticsInfo
{
    short *pwBuf;
    float fAvg[4];//min,max,avg,count
    float fVariance[4];
    float fAmp[4];

    void init(){
        fAvg[0] = VALUE_MAX;
        fAvg[1] = 0;
        fAvg[2] = 0;
        fAvg[3] = 0;
        fVariance[0] = VALUE_MAX;
        fVariance[1] = 0;
        fVariance[2] = 0;
        fVariance[3] = 0;
        fAmp[0] = VALUE_MAX;
        fAmp[1] = 0;
        fAmp[2] = 0;
        fAmp[3] = 0;
    }
};
void updataStatics(float iVal, float*pStatistics)
{
    if (iVal < pStatistics[0])
        pStatistics[0] = iVal;
    if (iVal > pStatistics[1])
        pStatistics[1] = iVal;
    pStatistics[3] += 1;
    pStatistics[2] = pStatistics[2] + (iVal - pStatistics[2])/pStatistics[3];
}
int Read_Txt(const char* filepath,short * buff)
{
	FILE * fp;
	int cnt = 0;
	int i;
	int tmp = 0;

	fp = fopen(filepath,"r");
	if(!fp)
	{
		printf("The file is not exist!");
		return 0;
	}

	while(1)
	{
		if (fscanf(fp,"%d",&tmp) == EOF)
		{
			break;
		}
		else
		{
			buff[cnt] = tmp;
			cnt++;
		}

	}	
	fclose(fp);

	return cnt;
}
void analyzeData(const string strFile, struct sStatisticsInfo* pStat, int iChanCnt, int iStartIdx, int iEndIdx);
int main(int argc, char* argv[])
{
    if (5 > argc)
    {
        cout<<"analyse directory filename channelcount startindex endindex\n";
        return 0;
    } 
    cout <<argv[0]<<" "<<argv[1]<<" "<<argv[3]<<"\n"; 
    string strDataPath = argv[1];
    char *end;
    int iChanCnt = strtol(argv[3], &end, 10); 
    int iStartIdx = strtol(argv[4], &end, 10); 
    int iEndIdx = strtol(argv[5], &end, 10); 
    struct sStatisticsInfo statistics[CHAN_MAX];
    for (int i=0; i < CHAN_MAX; i++)
    {
        statistics[i].init();
        statistics[i].pwBuf = (short *)malloc(sizeof(short)*CHAN_MAX*COL_MAX);
    }
    for (const auto &subDir : std::filesystem::directory_iterator(strDataPath))
    {
        string strFile = subDir.path().string();
        strFile.append("/").append(argv[2]);

        analyzeData(strFile, statistics, iChanCnt, iStartIdx, iEndIdx);
    }
    {
    }
	
	//输出总结果
	cout<<"data count "<<statistics[0].fAvg[3]<<"\n";
	//cout<<"data count "<<statistics[1].fAvg[3]<<"\n";
	//cout<<"data count "<<statistics[0].fAmp[3]<<"\n";
	//cout<<"data count "<<statistics[1].fAmp[3]<<"\n";
    for (int j = 0; j < iChanCnt; j++)
    {
    cout<<"channel "<<j<<"\n";
	cout<<"(max - min) range: ["<<statistics[j].fAmp[0]<<","<<statistics[j].fAmp[1]<<"] average "<<statistics[j].fAmp[2]<<"\n";
	cout<<"variance range: ["<<statistics[j].fVariance[0]<<","<<statistics[j].fVariance[1]<<"] average "<<statistics[j].fVariance[2]<<"\n";
    free(statistics[j].pwBuf);
    }
	return 0;
}
 
void analyzeData(const string strFile, struct sStatisticsInfo* pStat, int iChanCnt, int iStartIdx, int iEndIdx)
{
    if (!std::filesystem::exists(strFile))
        return;

    //cout<<strFile<<"\n";
    int iDataCnt = Read_Txt(strFile.c_str(),pStat->pwBuf);
    int i = 0, k = 0;
    int iMin[CHAN_MAX] = {5000};
    int iMax[CHAN_MAX] = {0};
    int iSum[CHAN_MAX] = {0};
    int iCnt[CHAN_MAX] = {0};
    float fAvg[CHAN_MAX] = {0};
    float fVar[CHAN_MAX] = {0};
    for (int j = 0; j < iChanCnt; j++)
    {
        iMin[j] = 5000;
        fVar[j] = 0;
    }

    while(i < iDataCnt)
    {
        for (int j = 0; j < iChanCnt; j++)
        {
            int iVal = pStat->pwBuf[i + j];
            if (k < iStartIdx || k > iEndIdx) continue;
            if (iVal < iMin[j]) iMin[j] = iVal;
            if (iVal > iMax[j]) iMax[j] = iVal;
            iSum[j] += iVal;
            iCnt[j]++;
        }
        i += iChanCnt;
        if (i + iChanCnt > iDataCnt)
            break;
        k++;
    }

    //count average 
    for (int j = 0; j < iChanCnt; j++)
    {
        fAvg[j] = (iSum[j] + 0.0) / iCnt[j];
        //cout<<fAvg[j]<<" ";
    }

    //count variance
    i = 0, k = 0;
    while(i < iDataCnt)
    {
        for (int j = 0; j < iChanCnt; j++)
        {
            float fVal = pStat->pwBuf[i + j];
            if (k < iStartIdx || k > iEndIdx) continue;
            //cout<<fVal<<" "<<fAvg[j]<<"\n";
            fVar[j] += ((fVal - fAvg[j]) * (fVal - fAvg[j])); 
            //fVar[j] = fVar[j] + ((fVal - fAvg[j]) * (fVal - fAvg[j])); 
            //cout<<fVar[j]<<"\n";
        }
        i += iChanCnt;
        if (i + iChanCnt > iDataCnt)
            break;
        k++;
    }
    
    for (int j = 0; j < iChanCnt; j++)
    {
        fVar[j] /= (iCnt[j]-1);
        //cout<<fVar[j]<<" ";
    }

    //fill statistics
    for (int j = 0; j < iChanCnt; j++)
    {
        updataStatics(fAvg[j], pStat[j].fAvg);
        updataStatics(iMax[j] - iMin[j], pStat[j].fAmp);
        updataStatics(fVar[j], pStat[j].fVariance);
    }
}
