#include "cepollclient.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

CEpollClient::CEpollClient(int iUserCount, const char* pIP, int iPort)
{
    strcpy(m_ip, pIP);
    m_iPort = iPort;
    m_iUserCount = iUserCount;
    m_iEpollFd = epoll_create(_MAX_SOCKFD_COUNT);
    m_pAllUserStatus = (struct UserStatus*)malloc(iUserCount*sizeof(struct UserStatus));
    for(int iuserid=0; iuserid<iUserCount ; iuserid++)
    {
        m_pAllUserStatus[iuserid].iUserStatus = FREE;
        sprintf(m_pAllUserStatus[iuserid].cSendbuff, "GET /2014/ch/style/nav.css HTTP/1.1\r\nHost: www.hzau.edu.cn\r\nConnection: keep-alive\r\nAccept: text/css,*/*;q=0.1\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/40.0.2214.111 Safari/537.36\r\nReferer: http://www.hzau.edu.cn/2014/ch/\r\nAccept-Encoding: gzip, deflate, sdch\r\nAccept-Language: zh-CN,zh;q=0.8,en;q=0.6\r\n\r\n", iuserid);
        m_pAllUserStatus[iuserid].iBuffLen = strlen(m_pAllUserStatus[iuserid].cSendbuff) + 1;
        m_pAllUserStatus[iuserid].iSockFd = -1;
    }
    memset(m_iSockFd_UserId, 0xFF, sizeof(m_iSockFd_UserId));
}

CEpollClient::~CEpollClient()
{
    free(m_pAllUserStatus);
}

int CEpollClient::ConnectToServer(int iUserId,const char *pServerIp,unsigned short uServerPort)
{
    if( (m_pAllUserStatus[iUserId].iSockFd = socket(AF_INET,SOCK_STREAM,0) ) < 0 )
    {
        //cout <<"[CEpollClient error]: init socket fail, reason is:"<<strerror(errno)<<",errno is:"<<errno<<endl;
        m_pAllUserStatus[iUserId].iSockFd = -1;
        return  m_pAllUserStatus[iUserId].iSockFd;
    }

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(uServerPort);
    addr.sin_addr.s_addr = inet_addr(pServerIp);

    int ireuseadd_on = 1;//
    setsockopt(m_pAllUserStatus[iUserId].iSockFd, SOL_SOCKET, SO_REUSEADDR, &ireuseadd_on, sizeof(ireuseadd_on));

    unsigned long ul = 1;
    ioctl(m_pAllUserStatus[iUserId].iSockFd, FIONBIO, &ul); //

    connect(m_pAllUserStatus[iUserId].iSockFd, (const sockaddr*)&addr, sizeof(addr));
    m_pAllUserStatus[iUserId].iUserStatus = CONNECT_OK;
    m_pAllUserStatus[iUserId].iSockFd = m_pAllUserStatus[iUserId].iSockFd;

    return m_pAllUserStatus[iUserId].iSockFd;
}
int CEpollClient::SendToServerData(int iUserId)
{
    //sleep(1);//
    int isendsize = -1;
    if( CONNECT_OK == m_pAllUserStatus[iUserId].iUserStatus || RECV_OK == m_pAllUserStatus[iUserId].iUserStatus)
    {
        isendsize = send(m_pAllUserStatus[iUserId].iSockFd, m_pAllUserStatus[iUserId].cSendbuff, m_pAllUserStatus[iUserId
                ].iBuffLen, MSG_NOSIGNAL);
        if(isendsize < 0)
        {
            //cout <<"[CEpollClient error]: SendToServerData, send fail, reason is:"<<strerror(errno)<<",errno is:"<<errno<<endl;
        }
        else
        {
            //printf("[CEpollClient info]: iUserId: %d Send Msg Content:%s\n", iUserId, m_pAllUserStatus[iUserId].cSendbuff);
            m_pAllUserStatus[iUserId].iUserStatus = SEND_OK;
        }
    }
    return isendsize;
}
int CEpollClient::RecvFromServer(int iUserId,char *pRecvBuff,int iBuffLen)
{
    int irecvsize = -1;
    if(SEND_OK == m_pAllUserStatus[iUserId].iUserStatus)
    {
        irecvsize = recv(m_pAllUserStatus[iUserId].iSockFd, pRecvBuff, iBuffLen, 0);
        if(0 > irecvsize)
        {
            //cout <<"[CEpollClient error]: iUserId: " << iUserId << "RecvFromServer, recv fail, reason is:"<<strerror(errno)<<",errno is:"<<errno<<endl;
        }
        else if(0 == irecvsize)
        {
            //cout <<"[warning:] iUserId: "<< iUserId << "RecvFromServer, STB0,irecvsize:"<<irecvsize<<",iSockFd:"<< m_pAllUserStatus[iUserId].iSockFd << endl;
        }
        else
        {
            //printf("Recv Server Msg Content:%s\n", pRecvBuff);
            m_pAllUserStatus[iUserId].iUserStatus = RECV_OK;
        }
    }
    return irecvsize;
}

bool CEpollClient::CloseUser(int iUserId)
{
    close(m_pAllUserStatus[iUserId].iSockFd);
    m_pAllUserStatus[iUserId].iUserStatus = FREE;
    m_pAllUserStatus[iUserId].iSockFd = -1;
    return true;
}

int CEpollClient::RunFun()
{
    int isocketfd = -1;
    for(int iuserid=0; iuserid<m_iUserCount; iuserid++)
    {
        struct epoll_event event;
        isocketfd = ConnectToServer(iuserid, m_ip, m_iPort);
        if(isocketfd < 0){
            //cout <<"[CEpollClient error]: RunFun, connect fail" <<endl;
        }
        m_iSockFd_UserId[isocketfd] = iuserid;//IDsocketid

        event.data.fd = isocketfd;
        event.events = EPOLLIN|EPOLLOUT|EPOLLERR|EPOLLHUP;

        m_pAllUserStatus[iuserid].uEpollEvents = event.events;
        epoll_ctl(m_iEpollFd, EPOLL_CTL_ADD, event.data.fd, &event);
    }
    while(1)
    {
        struct epoll_event events[_MAX_SOCKFD_COUNT];
        char buffer[1024];
        memset(buffer,0,1024);
        int nfds = epoll_wait(m_iEpollFd, events, _MAX_SOCKFD_COUNT, 100 );//epoll
        for (int ifd=0; ifd<nfds; ifd++)//
        {
            struct epoll_event event_nfds;
            int iclientsockfd = events[ifd].data.fd;
            //cout << "events[ifd].data.fd: " << events[ifd].data.fd << endl;
            int iuserid = m_iSockFd_UserId[iclientsockfd];//socketfdID
            if( events[ifd].events & EPOLLOUT)
            {
                int iret = SendToServerData(iuserid);
                if( 0 < iret )
                {
                    event_nfds.events = EPOLLIN|EPOLLERR|EPOLLHUP;
                    event_nfds.data.fd = iclientsockfd;
                    epoll_ctl(m_iEpollFd, EPOLL_CTL_MOD, event_nfds.data.fd, &event_nfds);
                }
                else
                {
                    //cout <<"[CEpollClient error:] EpollWait, SendToServerData fail, send iret:"<<iret<<",iuserid:"<<iuserid<<",fd:"<<events[ifd].data.fd<<endl;
                    //DelEpoll(events[ifd].data.fd);
                    //CloseUser(iuserid);
                }
            }
            else if( events[ifd].events & EPOLLIN )//
            {
                int ilen = RecvFromServer(iuserid, buffer, 1024);
                if(0 > ilen)
                {
                    cout <<"[CEpollClient error]: RunFun, recv fail, reason is:"<<strerror(errno)<<",errno is:"<<errno<<endl;
                    //DelEpoll(events[ifd].data.fd);
                    //CloseUser(iuserid);
                }
                else if(0 == ilen)
                {
                    //cout <<"[CEpollClient warning:] server disconnect,ilen:"<<ilen<<",iuserid:"<<iuserid<<",fd:"<<events[ifd].data.fd<<endl; 
                    DelEpoll(events[ifd].data.fd);
                    CloseUser(iuserid);

                    struct epoll_event event;
                    isocketfd = ConnectToServer(iuserid, m_ip, m_iPort);
                    if(isocketfd < 0){
                        cout <<"[CEpollClient error]: RunFun, connect fail" <<endl;
                    }else{
                        static int count = 0;
                        count ++;
                        cout<<"Connect to server successed!  "<<count<<endl;
		            }
                    m_iSockFd_UserId[isocketfd] = iuserid;//IDsocketid

                    event.data.fd = isocketfd;
                    event.events = EPOLLIN|EPOLLOUT|EPOLLERR|EPOLLHUP;

                    m_pAllUserStatus[iuserid].uEpollEvents = event.events;
                    epoll_ctl(m_iEpollFd, EPOLL_CTL_ADD, event.data.fd, &event);

                }
                else
                {
                    m_iSockFd_UserId[iclientsockfd] = iuserid;//socketfdID
                    event_nfds.data.fd = iclientsockfd;
                    event_nfds.events = EPOLLOUT|EPOLLERR|EPOLLHUP;
                    epoll_ctl(m_iEpollFd, EPOLL_CTL_MOD, event_nfds.data.fd, &event_nfds);
                }
            }
            else
            {
                //cout <<"[CEpollClient error:] other epoll error"<<endl;
                //DelEpoll(events[ifd].data.fd);
                //CloseUser(iuserid);
            }
        }
    }
}

bool CEpollClient::DelEpoll(int iSockFd)
{
    bool bret = false;
    struct epoll_event event_del;
    if(0 < iSockFd)
    {
        event_del.data.fd = iSockFd;
        event_del.events = 0;
        if( 0 == epoll_ctl(m_iEpollFd, EPOLL_CTL_DEL, event_del.data.fd, &event_del) )
        {
            bret = true;
        }
        else
        {
           // cout <<"[SimulateStb error:] DelEpoll,epoll_ctl error,iSockFd:"<<iSockFd<<endl;
        }
        m_iSockFd_UserId[iSockFd] = -1;
    }
    else
    {
        bret = true;
    }
    return bret;
}

