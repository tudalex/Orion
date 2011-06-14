#include "mainwindow.h"
#include <attica/provider.h>
#include <attica/providermanager.h>
#include <attica/person.h>
#include <attica/itemjob.h>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QCommandLinkButton>
#include "categorybutton.h"
#include <attica/category.h>
#include <attica/content.h>
#include <QListWidget>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    this->setMinimumWidth(600);
    QHBoxLayout *hbox = new QHBoxLayout;
    m_categories = new QVBoxLayout;
    hbox->addLayout(m_categories);
    m_SoftwareList = new QListWidget;

    hbox->addWidget(m_SoftwareList);
    QWidget *center = new QWidget;
    center->setLayout(hbox);
    this->setCentralWidget(center);
    QCommandLinkButton *test = new QCommandLinkButton(QString::fromLatin1("buton de test"));
    //categories->addWidget(test);

    connect(&m_manager, SIGNAL(defaultProvidersLoaded()), SLOT(providersChanged()));
    // tell it to get the default Providers
    m_manager.addProviderFromXml(QString::fromLatin1("<provider>"
                                 "<id>opendesktop</id>"
                                 "<location>https://api.opendesktop.org/v1/</location>"
                                 "<name>openDesktop.org</name>"
                                 "<icon></icon>"
                                 "<termsofuse>https://opendesktop.org/terms/</termsofuse>"
                                 "<register>https://opendesktop.org/usermanager/new.php</register>"
                                 "<services>"
                                 "  <person ocsversion=\"1.3\" />"
                                 "  <friend ocsversion=\"1.3\" />"
                                 "  <message ocsversion=\"1.3\" />"
                                 "  <activity ocsversion=\"1.3\" />"
                                 "  <content ocsversion=\"1.3\" />"
                                 "  <fan ocsversion=\"1.3\" />"
                                 "  <knowledgebase ocsversion=\"1.3\" />"
                                 "  <event ocsversion=\"1.3\" />"
                                 "</services>"
                                 "</provider>"));

}


void MainWindow::providersChanged()
{
    qDebug("provider has changed");
    if (!m_manager.providers().isEmpty()) {
        qDebug("provider list is not empty");
        m_provider = m_manager.providerByUrl(QUrl(QString::fromAscii("https://api.opendesktop.org/v1/")));
        if (!m_provider.isValid()) {
            qDebug("provider is not valid");
            return;
        }
        Attica::ListJob<Attica::Category>* job = m_provider.requestCategories();
        connect(job, SIGNAL(finished(Attica::BaseJob*)), SLOT(onContentRecieved(Attica::BaseJob*)));
        job->start();
    }

}

void MainWindow::onContentRecieved(Attica::BaseJob *job)
{
    qDebug("Job finished");
    Attica::ListJob<Attica::Category> *personJob = static_cast< Attica::ListJob<Attica::Category> * >( job );
    if (personJob->metadata().error() == Attica::Metadata::NoError)
    {
        int i = 0;
        Attica::Category::List l(personJob->itemList());
        for (QList <Attica::Category>::iterator it = l.begin(); it!=l.end(); ++ it)
        {
            Attica::Category *cat = new Attica::Category(*it);
            CategoryButton *category_button = new CategoryButton(this,cat);
            category_button->category=cat;

            connect(category_button,SIGNAL(clicked()),this,SLOT(category_selected()));
            m_categories->addWidget(category_button);
            if (i>10)
                break;
            ++i;
        }

    }
    else
        qDebug("job didn't go well");
}

void MainWindow::category_selected()
{
    CategoryButton *test =static_cast < CategoryButton *>(QObject::sender());

    QList <Attica::Category> list;
    list.append(*(test->category));
    Attica::ListJob<Attica::Content>* job = m_provider.searchContents(list);
    m_SoftwareList->clear();
    connect(job, SIGNAL(finished(Attica::BaseJob*)), SLOT(onContentListRecieved(Attica::BaseJob*)));
    job->start();
}

void MainWindow::onContentListRecieved(Attica::BaseJob *job)
{
    Attica::ListJob<Attica::Content> *ContentListJob = static_cast< Attica::ListJob<Attica::Content> * >( job );
    if (ContentListJob->metadata().error() == Attica::Metadata::NoError)
    {
        QString clist;
        Attica::Content::List l(ContentListJob->itemList());
        for (Attica::Content::List::iterator it = l.begin(); it!=l.end(); ++ it)
        {
            m_SoftwareList->addItem((static_cast < Attica::Content> (*it)).name());
        }

    }
}


