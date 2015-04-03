#include "cepollclient.h"
//cat ans | tr -s '\n' | tr '\n' 'X' | sed 's/X/\\r\\n/g'
int main(int argc, char *argv[])
{
        CEpollClient *pCEpollClient = new CEpollClient(1000, "211.69.143.10", 80);
        if(NULL == pCEpollClient)
        {
                cout<<"[epollclient error]:main init"<<"Init CEpollClient fail"<<endl;
        }
        pCEpollClient->RunFun();
        if(NULL != pCEpollClient)
        {
                delete pCEpollClient;
                pCEpollClient = NULL;
        }
        return 0;
}

