#include "ZZListener.h"

//定义单例模式类指针
ListenerManger* ListenerManger::m_listenerManger = new ListenerManger();

ListenerManger* ListenerManger::Instance()
{
    return m_listenerManger;
}

void ListenerManger::notify(int message)
{
    //通过键值在map容器中搜索该事件是否在容器中注册
    mmap::iterator iter = m_messageToLister.find(message);
    //事件存在，通知对应的监听者做出反应（调用该监听者接口）
    if (iter != m_messageToLister.end())
    {
        QVector<ZZListener*>::iterator listener = iter.value().begin();
        while (listener != iter.value().end())
        {
            (*listener)->RespondMessage(message);//调用监听者的函数接口
            listener++;
        }
    }
    else  //未注册过该事件
    {
        //cout << "no Listener has insterested this meeage" << endl;
    }

}

void ListenerManger::registerMessage(int message, ZZListener* listener)
{
    auto Register = [&](int singleMessage)->void {
        mmap::iterator iter = m_messageToLister.find(singleMessage);
        //没有注册该事件
        if (iter == m_messageToLister.end())
        {
            QVector<ZZListener*> listeners;
            listeners.push_back(listener);
            m_messageToLister[singleMessage] = listeners;
        }
        else//有该事件
        {
            iter.value().push_back(listener);
        }
    };

    if ((message & MESSAGE::ZHUZHAO_UPDATE_SRCIMAGE) == MESSAGE::ZHUZHAO_UPDATE_SRCIMAGE)
    {
        Register(MESSAGE::ZHUZHAO_UPDATE_SRCIMAGE);
    }
    if ((message & MESSAGE::ZHUZHAO_UPDATE_RESULTI) == MESSAGE::ZHUZHAO_UPDATE_RESULTI)
    {
        Register(MESSAGE::ZHUZHAO_UPDATE_RESULTI);
    }
    if ((message & MESSAGE::ZHUZHAO_RESET) == MESSAGE::ZHUZHAO_RESET)
    {
        Register(MESSAGE::ZHUZHAO_RESET);
    }
    if ((message & MESSAGE::ZHUZHAO_RUNONCE) == MESSAGE::ZHUZHAO_RUNONCE)
    {
        Register(MESSAGE::ZHUZHAO_RUNONCE);
    }
}
