#ifndef WIDGET_H
#define WIDGET_H
#include<QtWidgets>
#include <QWidget>
#include <QSlider>
#include <QPushButton>
#include  <QListView>
#include  <QMediaPlayer>
#include  <QMediaPlaylist>
#include <QStandardItemModel>
#include  <QStringList>
#include  <QModelIndex>
#include  <QString>
typedef struct fileNamePath
{
    QString strfileName; //文件名称
    QString strfilePath; //文件路径
}sfileNamePath;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void  paintEvent(QPaintEvent *);
    void findSpecliaFile(QString strDir,QStringList filter);//搜索功能










private slots:
    //void on_pushButton_clicked();
     void  updatePosition(qint64 position);//播放音乐更新滑块
     void  slider_vilume_changed();  //音量控制
     void  openFile();  //打开文件添加歌曲
     void  openDir();  //打开目录添加歌曲
     void  on_doubleClicked_list(QModelIndex index); //双击播放


private:
    Ui::Widget *ui;

    QPushButton * selectBtn; //搜索按钮
    QSlider *seekSlider;//音量控制进度条
    QSlider *seekSlider1;//进度控制进度条
    QPushButton *PlayorpauseMusic;//播放暂停按钮
    QPushButton *NextMusic;//下一曲播放按钮
    QPushButton *LastMusic;//上一曲
    QListView *listView ;   //播放列表
    QMediaPlaylist *playlist;  //播放列表
    QMediaPlayer *player;       //播放音乐类

    bool isPaly;       //是否播放
    QStandardItemModel *model;//列表模型
    QStringList urlList; //路径链表

    QList<sfileNamePath>g_sfnp;//搜索歌曲链表


};

#endif // WIDGET_H
