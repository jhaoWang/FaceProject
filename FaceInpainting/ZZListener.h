#ifndef ZZ_LISTENER_H
#define ZZ_LISTENER_H
/**************************************************
 * Listener为监听者类，在设计模式中，有监听者就有观察者类，
 * 在重明中，因为我们软件功能并没有太过复杂，没有过多的观察者监听者组合。
 * 所以我们只需要一个观察者来统一管理所有的监听事件就可以了。因此我们将观察者
 * Observer改成了ListenerManger，该类是一个单例。
 *
 * 在重明中监听者只有三个：
 * 		# ControlWidget控制界面类
 *		# ParamWidget参数界面类
 *		# ViewWidget视觉窗口类
 * 而事件也只有相机枚举、连接/断连、拉流/停止拉流、相机切换这几种。
 * 因此这个监听者模式规模比较小。通过监听者模式可以让我们避免使用大量的信号槽去处理我们
 * 不同监听者对不同事件的处理。
 *
 **************************************************/

#include <QMap>
#include <QVector>

enum MESSAGE {
    ZHUZHAO_UPDATE_SRCIMAGE	= 0x00000001,//刷新输入图像列表
    ZHUZHAO_UPDATE_RESULTI  = 0x00000002,//刷新算法运行结果
    ZHUZHAO_RESET           = 0x00000003,//重置界面参数
    ZHUZHAO_RUNONCE         = 0x00000004,//单次运行一次
};

class ZZListener
{
public:
    ZZListener() {};
    virtual ~ZZListener() {};
    virtual void RespondMessage(int message) = 0;
};

class ListenerManger
{
    typedef QMap<int, QVector<ZZListener*>> mmap; //类型重定义
public:
    //获取单例类对象指针
    static ListenerManger* Instance();
    //事件到来，通知监听者
    void notify(int message);
    //注册事件
    void registerMessage(int message, ZZListener* listener);
private:
    ListenerManger() {};
    ~ListenerManger() {};
    static ListenerManger* m_listenerManger;//单例类对象指针
    QMap<int, QVector<ZZListener*>> m_messageToLister;//事件-监听者列表
};

#endif // ZZ_LISTENER_H
