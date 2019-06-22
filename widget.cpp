#include "widget.h"
#include "ui_widget.h"
#include <QIcon>
#include <QPalette>
#include <QtWidgets>
#include <QString>
#include <QtWidgets>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QStandardItemModel>
#include <QString>
#include <QAction>
#include<QMenu>
#include <QString>
#include  <QModelIndex>
#include <QMessageBox>
//#include <QVideoWidget>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    urlList.clear();

    QPixmap pix;
    pix.load(":/new/image/666.png"); // 加载图片
    setWindowIcon(QIcon(":/new/image/ico.png"));
    setWindowTitle(tr("音乐播放器"));
    resize(pix.size()); // 设置窗口大小为图片大小
    setMask(pix.mask()); // 为窗口设置遮罩

    //setWindowOpacity(0.4); //设置透明
     //设置搜索的按钮图标
     selectBtn=new QPushButton(this);
     selectBtn->setToolTip("搜索");
     selectBtn->move(10,20);
     selectBtn->setFixedSize(32,32);//设置按钮大小
     selectBtn->setFlat(true); //窗口透明不显示边框
     selectBtn->setStyleSheet("QPushButton{background-image: url(:/new/image/sss.png);}");
     connect(selectBtn,&QPushButton::clicked,this,[=]()
     {
         QFileInfoList DirInfoList=QDir::drives();
         QStringList filterName;
         filterName<<"*.mp3";  //筛选器
         foreach(QFileInfo fileInfo,DirInfoList)
         {
             QString qstr=fileInfo.path();
             if(qstr=="E:/")
             findSpecliaFile(qstr,filterName);
            // QMessageBox::information(this,"information",qstr);

         }
         if(!g_sfnp.isEmpty())
         {
             foreach(sfileNamePath str1,g_sfnp)
             {
                 bool falg =false;
                 if(urlList.count())
                 {
                     foreach(QString strur1,urlList)
                         if(strur1==str1.strfilePath)
                         {
                             falg=true;
                             break;

                         }
                 }

             if(!falg)
             {
                 urlList.append(str1.strfilePath);
                 playlist->addMedia(QUrl::fromLocalFile(str1.strfilePath));
                 int first= str1.strfilePath.lastIndexOf("/");
                 QString title=str1.strfilePath.right(str1.strfilePath.length()-first-1);
                 int index=title.lastIndexOf(".");    //通过路径来获取文件名
                 title.truncate(index);
                 QStandardItem *item=new QStandardItem(title);
                 model->appendRow(item);

             }
           }
           listView->setUpdatesEnabled(true);
           player->setPlaylist(playlist);
         }
         else

             QMessageBox::information(this,"information","not find the mp3 file");
             g_sfnp.clear();

     });





     ///创建一个水平方向的滑动条QSlider控件。进度控制
     seekSlider1 = new QSlider(Qt::Horizontal,this);
     seekSlider1 -> setGeometry(10,420,300,10);
     seekSlider1->setToolTip("播放进度条");
     seekSlider1->setObjectName(tr("seekSlider1"));
     seekSlider1 -> setCursor(QCursor(Qt::PointingHandCursor));
     connect(seekSlider1,&QSlider::sliderMoved,this,[=](int position) //手动拖进度条
     {
         player->setPosition(position *1000); //设置播放进度
     });


     ///创建一个水平方向的滑动条QSlider控件。音量控制
     seekSlider = new QSlider(Qt::Horizontal,this);
     seekSlider -> setGeometry(200,461,100,10);
     seekSlider->setToolTip("音量控制进度条");
     seekSlider->setObjectName(tr("seekSlider"));
     seekSlider -> setCursor(QCursor(Qt::PointingHandCursor));
    // seekSlider->setValue(player->volume());//第一首歌曲音量
     //由于不涉及刷新，所以move和click两个信号进行处理可以共有一个槽函数
     connect(seekSlider,&QSlider::sliderPressed,this,&Widget::slider_vilume_changed);
     connect(seekSlider,&QSlider::sliderMoved,this,&Widget::slider_vilume_changed);

//     //打开QSS文件
//     QFile file(":/new/image/1.qss");
//     // 只读方式打开该文件
//     file.open(QFile::ReadOnly);
//     // 读取文件全部内容
//     QString styleSheet = QString(file.readAll());
//     // 为QApplication设置样式表
//     qApp->setStyleSheet(styleSheet);
//     file.close();

     player=new QMediaPlayer(this);       //播放音乐类
     connect(player,&QMediaPlayer::positionChanged,this,&Widget::updatePosition);
     //palyer->setMedia(QUrl("E:/QTt/MS/musicpalyer/水星记.mp3"));

     playlist=new  QMediaPlaylist;  //播放列表
     playlist->setCurrentIndex(0);  //设置播放第几个
     playlist->setPlaybackMode(QMediaPlaylist::Random);//设置播放类型
//     playlist->addMedia(QUrl("E:/QTt/MS/musicpalyer/水星记.mp3"));
//     playlist->addMedia(QUrl("E:/QTt/MS/musicpalyer/郊游.mp3"));
//     playlist->addMedia(QUrl("E:/QTt/MS/musicpalyer/放学歌.mp3"));


     player->setPlaylist(playlist);//设置播放列表
     //player->play();  //音乐播放

     //上一曲按钮
     LastMusic=new QPushButton(this);
     LastMusic->setToolTip("上一曲");
     LastMusic->move(10,450);
     LastMusic->setFixedSize(38,38);//设置按钮大小
     LastMusic->setFlat(true); //窗口透明不显示边框
     LastMusic->setStyleSheet("QPushButton{background-image: url(:/new/image/last.png);}");
     connect(LastMusic,&QPushButton::clicked,this,[=]()
     {
         playlist->previous();
     });

     //播放暂停按钮按钮
     isPaly=false;
     PlayorpauseMusic=new QPushButton(this);
     PlayorpauseMusic->setToolTip("播放/暂停");
     PlayorpauseMusic->move(50,440);
     PlayorpauseMusic->setFixedSize(55,55);//设置按钮大小
     PlayorpauseMusic->setFlat(true); //窗口透明不显示边框
     PlayorpauseMusic->setStyleSheet("QPushButton{background-image: url(:/new/image/paly.png);}");
     connect(PlayorpauseMusic,&QPushButton::clicked,this,[=]()
     {
         isPaly=!isPaly;
         if(isPaly)
         {
             player->play();
             PlayorpauseMusic->setStyleSheet("QPushButton{background-image: url(:/new/image/pause.png);}");
         }
         else
         {
             player->pause();
             PlayorpauseMusic->setStyleSheet("QPushButton{background-image: url(:/new/image/paly.png);}");
         }
     });


     //下一曲按钮
     LastMusic=new QPushButton(this);
     LastMusic->setToolTip("下一曲");
     LastMusic->move(110,450);
     LastMusic->setFixedSize(38,38);//设置按钮大小
     LastMusic->setFlat(true); //窗口透明不显示边框
     LastMusic->setStyleSheet("QPushButton{background-image: url(:/new/image/next.png);}");
     connect(LastMusic,&QPushButton::clicked,this,[=]()
     {
         playlist->next();
     });


      //播放列表
      listView=new QListView(this);
      listView->setFixedSize(312,133);
      listView->setEditTriggers(QAbstractItemView::NoEditTriggers); //控制它不能它输入
      listView->move(3,277);
      listView->setContextMenuPolicy(Qt::CustomContextMenu);//没有就右键点击事件不会产生
      //双击播放
      connect(listView,&QListView::doubleClicked,this,&Widget::on_doubleClicked_list);
      connect(listView,&QListView::customContextMenuRequested,this,[=](const QPoint&)
      {
          QMenu *cmenu=new QMenu(listView);
          QAction *AddMusic=cmenu->addAction("添加歌曲");
          connect(AddMusic,&QAction::triggered,this,&Widget::openFile);

          QAction *DirMusic=cmenu->addAction("添加目录歌曲");
          connect(DirMusic,&QAction::triggered,this,&Widget::openDir);

          if(urlList.count())
          {
              QAction *PlayMusic=cmenu->addAction("播放歌曲");
              connect(PlayMusic,&QAction::triggered,this,[=]()
              {
                  playlist->setCurrentIndex((listView->currentIndex().row()));
                  player->play();
                  PlayorpauseMusic->setStyleSheet("QPushButton{background-image: url(:/new/image/pause.png);}");
                  isPaly=true;

              });

              QAction *DelMusic=cmenu->addAction("删除歌曲");
              connect(DelMusic,&QAction::triggered,this,[=]()
              {
                 int row=listView->currentIndex().row();
                 playlist->removeMedia(row);
                 model->removeRow(row);
                 urlList.removeAt(row);
                 listView->setUpdatesEnabled(true);

              });

          }
           cmenu->exec(QCursor::pos()) ;
          delete cmenu;

      });

      model=new QStandardItemModel(this);
      listView->setModel(model);




}
void Widget::paintEvent(QPaintEvent *)  ///窗口重绘
{
    QPainter painter(this);
    // 从窗口左上角开始绘制图片
    painter.drawPixmap(0,0,QPixmap(":/new/image/666.png"));

}


Widget::~Widget()
{
    delete ui;
}
//
 void  Widget::updatePosition(qint64 position)
 {
     seekSlider1->setMaximum(player->duration()/1000);
     seekSlider1->setValue(position/1000);
 }
 //音量控制
 void  Widget::slider_vilume_changed()
 {
    player->setVolume(seekSlider->value());
 }
 //打开文件添加歌曲
 void  Widget::openFile()
 {
     QFileInfo fi;
     QString fileName=QFileDialog::getOpenFileName(
                this,
                 "open Music",
                 QDir::currentPath(),
                 "music files(*.mp3)::All files(*.*)");
     if(fileName.isEmpty())
          return;
     bool flag=false;
     int i=0;
     if(urlList.count())
     {
         foreach(QString strurl,urlList)
         {
             if(strurl ==fileName)
             {
                 i++;
                 flag=true;
                 break;
             }
         }
     }
     if(!flag)
     {
         playlist->addMedia(QUrl::fromLocalFile(fileName));
         urlList.append(fileName);
         fi=QFileInfo(fileName);
         QString str=fi.fileName();//获取文件名
         int index;
         index=str.lastIndexOf(".");
         str.truncate(index);//去掉文件扩展名
         QStandardItem *item=new QStandardItem(str);
         model->appendRow(item);
         listView->setUpdatesEnabled(true);
         playlist->setCurrentIndex(model->rowCount()-1);

     }
     else
     {
         playlist->setCurrentIndex(i-1);
     }
     player->setPlaylist(playlist);
     player->play();
     PlayorpauseMusic->setStyleSheet("QPushButton{background-image: url(:/new/image/pause.png);}");
     isPaly=true;

 }

//打开目录添加歌曲
 void Widget::openDir()
 {
      QString dirname=QFileDialog::getExistingDirectory(this,tr("open Directory"),".");
      if(dirname.isEmpty())
          return;

      QDir dir(dirname);
      dir.setNameFilters(QStringList("*.mp3"));
      QStringList strListFileName=dir.entryList();//获取文件名列表
      QString strPathName=dir.absolutePath();//获取目录路径
      int count =strListFileName.count();


      for(int i=0;i<count;i++)
      {
          bool flag=false;
          if(urlList.count())
          {
              foreach(QString strurl,urlList)
              {
                  QString path=strPathName+'/'+strListFileName.at(i);
                  if(strurl ==path)
                  {
                      i++;
                      flag=true;
                      break;
                  }
              }
          }
          if(!flag)
          {
              playlist->addMedia(QUrl::fromLocalFile(strPathName+'/'+strListFileName.at(i)));
              urlList.append(strPathName+'/'+strListFileName.at(i));
              QString str=strPathName+'/'+strListFileName.at(i);

              int index;
              index=str.lastIndexOf(".");
              str.truncate(index);//去掉文件扩展名
              QStandardItem *item=new QStandardItem(str);
              model->appendRow(item);


          }



      }
      listView->setUpdatesEnabled(true);
      listView->setUpdatesEnabled(playlist);

 }
//双击播放
void  Widget::on_doubleClicked_list(QModelIndex index)
{
    playlist->setCurrentIndex(index.row());
    player->play();
    PlayorpauseMusic->setStyleSheet("QPushButton{background-image: url(:/new/image/pause.png);}");
    isPaly=true;


}

void Widget::findSpecliaFile(QString strDir,QStringList filter)//搜索功能
{

    QDir curdir(strDir);
    curdir.setFilter(QDir::AllEntries|QDir::NoDotAndDotDot);
    QFileInfoList curdirlist=curdir.entryInfoList();// 获取所有文件信息
    foreach(QFileInfo sfile,curdirlist)
    {
        QString sfilesname=sfile.fileName();//获取文件名字
        if(sfile.isDir()==true)
        {
         /*.当前目录 ..前一个目录 文件夹 文件*/
            QString strFileName=sfile.filePath();
            findSpecliaFile(strFileName,filter);
        }
        if(sfile.isFile()==true)
        {
            QString ext="*."+sfile.suffix(); //获取文件后缀名
            if(filter.contains(ext,Qt::CaseInsensitive))     //筛选器内容和ext相匹配
            {
                sfileNamePath snew;
                snew.strfileName=sfile.fileName();
                snew.strfilePath=sfile.filePath();
                g_sfnp.append(snew);  //把文件路径和文件名字加载到结构体中
            }
        }
    }
}




